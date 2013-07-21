#include "filesys.h"

#include <iostream>
#include <deque>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

void getFiles(const std::string& directory, const std::string& pattern,
              std::vector<std::string>& files)
{
	std::vector<std::string> dirs;
	dirs.push_back(directory);
	getFiles(dirs, pattern, files);
}

void getFiles(const std::vector<std::string>& directories, const std::string& pattern,
              std::vector<std::string>& files)
{
	const std::string pathSep = "/"; // OK for Windows too

	std::deque<std::string> dirs(directories.begin(), directories.end());
	while (!dirs.empty())
	{
		std::string curDir = dirs.front();
		dirs.pop_front();

		DIR* handle = opendir(curDir.c_str());
		if (handle == NULL)
		{
			std::cerr << "cannot open directory: " << curDir << std::endl;
			continue;
		}

		dirent* entry;
		while ((entry = readdir(handle)) != NULL)
		{
			std::string path = curDir + pathSep + entry->d_name;

			// check is it a directory or a regular file
			struct stat fileStat;
			stat(path.c_str(), &fileStat);
			if (S_ISDIR(fileStat.st_mode)) // there is no entry->d_type on MinGW
			{
				if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
					dirs.push_back(path);
			}
			else
			{
				if (matchWildcard(pattern.c_str(), entry->d_name))
					files.push_back(path);
			}
		}

		closedir(handle);
	}
}

bool matchWildcard(const char* pattern, const char* str)
{
	while (true)
	{
		const char c = *pattern++;
		switch (c)
		{
		case '\0':
			return (*str == '\0');
			break;
		case '\\':
			if (*pattern == '\0' || *str++ != *pattern++)
				return false;
			break;
		case '?':
			if (*str++ == '\0')
				return false;
			break;
		case '*':
			while (*pattern == '*')
				++pattern;
			if (*pattern == '\0')
				return true;
			while (*str != '\0')
				if (matchWildcard(pattern, str++))
					return true;
			return false;
			break;
		default:
			if (*str++ != c)
				return false;
			break;
		}
	}
	return false;
}
