#ifndef FILESYS_H_
#define FILESYS_H_

#include <string>
#include <vector>

bool fileExists(const std::string& path);

void getFiles(const std::string& dir, const std::string& pattern, std::vector<std::string>& files);

void getFiles(const std::vector<std::string>& dirs, const std::string& pattern, std::vector<std::string>& files);

#endif // FILESYS_H_
