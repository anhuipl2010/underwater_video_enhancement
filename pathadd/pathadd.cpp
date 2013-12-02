#include <iostream>
#include <set>
#include <stdlib.h>
#include <sstream>

#ifdef __linux__
const char delim = ':';
#endif

#ifdef _WIN32
const char delim = ';';
#endif

int main(int argc, char** argv) {
    if (argc == 1) {
        std::cout << "usage: " << argv[0];
        std::cout << " env_var [directories ...]\n";
        std::cout << std::flush;
        //TODO: write informative help
        return 0;
    }

    std::set<std::string> paths;
    for (int i = 2; i < argc; i++)
        paths.insert(argv[i]);

    const char* name = argv[1];
    char* val = getenv(name);
    if (val) {
        std::stringstream iss(val);
        std::string item;
        while (std::getline(iss, item, delim))
            paths.insert(item);
    }

    bool sep = false;
    for (std::set<std::string>::iterator it = paths.begin(), end = paths.end(); it != end; ++it) {
        if (sep)
            std::cout << delim;
        std::cout << *it;
        sep = true;
    }
    std::cout << std::flush;

    return 0;
}
