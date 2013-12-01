#include <ctime>
#include <fstream>
#include <getopt.h>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <vector>

#include "filesys.h"

#ifdef _WIN32
int _CRT_glob = 0; // disables globbing of command line arguments
#endif

void printHelp(char** argv) {
    std::cout << "usage: " << argv[0];
    std::cout << " -s script [-p pattern] [-l] [-c config] [directories ...]\n";
    std::cout << "\t-s script:  path to program that will be run for each file found\n";
    std::cout << "\t-p pattern:  wildcard search pattern for files to be processed\n";
    std::cout << "\t\t(\"*\" by default)\n";
    std::cout << "\t-l:  enable log\n";
    std::cout << "\t-c config:  configuration file\n";
    std::cout << "\tdirectories:  list of directories that should be scanned for files\n";
    std::cout << "\t\t(the current directory by default)\n";
    std::cout << std::flush;
}

int main(int argc, char** argv) {
    std::string script;
    std::string pattern = "*";      // wildcard pattern for input files
    std::vector<std::string> dirs;  // directories to be processed
    bool logEnabled = false;        // logs
    std::string config;             // path to configuration file

    while (true) {
        int opt = getopt(argc, argv, "-hs:p:lc:");
        if (opt == -1)
            break;
        switch (opt) {
        case 'h': // help
            printHelp(argv);
            return 0;
        case 's': // script
            script = optarg;
            break;
        case 'p': // pattern
            pattern = optarg;
            break;
        case 'l': // log
            logEnabled = true;
            break;
        case 'c': // config
            config = optarg;
            break;
        case '\1': // no option
            dirs.push_back(optarg);
            break;
        default:
            std::cerr << "wrong option" << std::endl;
            return 1;
        }
    }

    if (!config.empty()) {
        // config file parsing
        std::ifstream f(config.c_str());
        if (f) {
            std::string line;
            while (std::getline(f, line)) {
                std::stringstream ss(line);
                std::string descr;
                ss >> descr;

                if (descr == "script")
                    ss >> script;
                else if (descr == "pattern")
                    ss >> pattern;
                else if (descr == "log")
                    logEnabled = true;
                else if (descr == "directories") {
                    std::string dir;
                    while (ss >> dir) // spaces are not allowed
                        dirs.push_back(dir);
                }
            }
            f.close();
        } else
            std::cerr << "cannot open configuration file: " << config << std::endl;
    }

    if (script.empty()) {
        std::cerr << "The script argument cannot be empty." << std::endl;
        printHelp(argv);
        return 1;
    }

    if (dirs.empty())
        dirs.push_back("."); // add current directory by default

    std::cout << "start batch processing" << std::endl;

    std::vector<std::string> files;
    getFiles(dirs, pattern, files);
    std::cout << files.size() << " files found" << std::endl;

    int num = 1;
    for (std::vector<std::string>::iterator it = files.begin(), end = files.end(); it != end; ++it) {
        std::string fn = *it;

        // command to launch
        char cmd[1024];
        sprintf(cmd, "%s \"%s\" 2>&1", script.c_str(), fn.c_str());

        FILE* logFile = NULL;
        if (logEnabled) {
            std::string logFileName = fn + ".log";
            logFile = fopen(logFileName.c_str(), "wt");
            if (logFile != NULL) {
                fwrite(cmd, 1, strlen(cmd), logFile);
                fputc('\n', logFile);
            } else
                std::cerr << "cannot open log file: " << logFileName << std::endl;
        }

        std::cout << "#" << num << " run: " << cmd << std::endl;
        time_t startTime = std::time(NULL);
        FILE* proc = popen(cmd, "r"); // start the process

        // wait while finish, read output messages from the process
        char outputBuffer[4096];
        size_t readLen;
        while ((readLen = fread(outputBuffer, 1, sizeof(outputBuffer) - 1, proc)) != 0) {
            outputBuffer[readLen] = '\0';
            std::cout << outputBuffer << std::flush;
            if (logFile != NULL)
                fwrite(outputBuffer, 1, readLen, logFile);
        }

        if (logFile != NULL)
            fclose(logFile);

        int retCode = pclose(proc); // stop the process
        time_t endTime = std::time(NULL);

        std::cout << "#" << num;
        if (retCode == 0)
            std::cout << " complete";
        else
            std::cout << " fail (error code " << retCode << ")";
        std::cout << " " << (endTime - startTime) << " sec";
        std::cout << std::endl;
        num++;
    }

    std::cout << "batch processing is finished" << std::endl;
    return 0;
}
