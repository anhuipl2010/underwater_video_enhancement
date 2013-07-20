#ifndef FILESYS_H_
#define FILESYS_H_

#include <string>
#include <vector>

void getFiles(const std::string& directory, const std::string& pattern,
              std::vector<std::string>& files);

void getFiles(const std::vector<std::string>& directories, const std::string& pattern,
              std::vector<std::string>& files);

bool matchWildcard(const char* pattern, const char* str);

#endif // FILESYS_H_
