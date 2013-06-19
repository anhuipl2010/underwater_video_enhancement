#include <iostream>
#include <string>
#include <vector>
#include <getopt.h>
#include <ctime>
#include <stdio.h>

#include "filesys.h"

int main(int argc, char** argv)
{
	std::string script;
	std::string pattern; // wildcard pattern for input files
	std::vector<std::string> dirs; // directories to be processed

	while (true)
	{
		int opt = getopt(argc, argv, "-hs:p:");
		if (opt == -1)
			break;
		switch (opt)
		{
		case 'h': // help
			std::cout << "usage: " << argv[0] << " -s script -p pattern [directory ...]\n";
			std::cout << "\tscript: path to program that will be run for each file found\n";
			std::cout << "\tpattern: wildcard search pattern for files to be processed\n";
			std::cout << "\tdirectory: directory that should be scanned for files\n";
			std::cout << std::flush;
			return 0;
		case 's': // script
			script = optarg;
			break;
		case 'p': // pattern
			pattern = optarg;
			break;
		case '\1': // no option
			dirs.push_back(optarg);
			break;
		default:
			std::cerr << "wrong option" << std::endl;
			return 1;
		}
	}

	if (!fileExists(script))
	{
		std::cerr << "script file does not exist" << std::endl;
		return 1;
	}

	std::cout << "start batch processing" << std::endl;

	std::vector<std::string> files;
	getFiles(dirs, pattern, files);
	std::cout << files.size() << " files found" << std::endl;
	for (std::vector<std::string>::iterator it = files.begin(), end = files.end(); it != end; ++it)
	{
		std::string fn = *it;

		// command to launch
		char cmd[1024];
		sprintf(cmd, "%s \"%s\"", script.c_str(), fn.c_str());

		std::string logFileName = fn + ".log";
		FILE* logFile = fopen(logFileName.c_str(), "wt");

		std::cout << "run: " << fn << std::flush;
		time_t startTime = std::time(NULL);

		FILE* proc = popen(cmd, "r"); // starting the process

		// wait while finish, logging output messages from script
		char outputBuffer[4096];
		size_t readLen;
		while ((readLen = fread(outputBuffer, 1, sizeof(outputBuffer), proc)) != 0)
			fwrite(outputBuffer, 1, readLen, logFile);
		fclose(logFile);

		int retCode = pclose(proc); // stopping the process

		time_t endTime = std::time(NULL);
		if (retCode == 0)
			std::cout << " complete";
		else
			std::cout << " fail (error code " << retCode << ")";
		std::cout << " " << (endTime - startTime) << " sec";
		std::cout << std::endl;
	}

	std::cout << "batch processing is finished" << std::endl;
	return 0;
}
