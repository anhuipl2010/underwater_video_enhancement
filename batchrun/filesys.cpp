#include "filesys.h"

#include <fnmatch.h>
#include <fts.h>
#include <errno.h>
#include <string.h>
#include <iostream>

void getFiles(const std::string& dir, const std::string& pattern, std::vector<std::string>& files)
{
	std::vector<std::string> dirs;
	dirs.push_back(dir);
	getFiles(dirs, pattern, files);
}

void getFiles(const std::vector<std::string>& dirs, const std::string& pattern, std::vector<std::string>& files)
{
	if (dirs.empty())
		return;

	const size_t dirsCntMax = 8;
	char* paths[dirsCntMax + 1];

	size_t dirsCnt = dirs.size();
	if (dirsCnt > dirsCntMax)
	{
		std::cerr << "Too many directories passed to ";
		std::cerr << __FUNCTION__ << " function: " << dirsCnt;
		std::cerr << ". Only first " << dirsCntMax << " will be scanned.";
		std::cerr << std::endl;

		dirsCnt = dirsCntMax;
	}

	size_t ind = 0;
	for (; ind < dirsCnt; ind++)
		paths[ind] = const_cast<char*> (dirs[ind].c_str()); // should be ok with fts_open
	paths[ind] = NULL;

	FTS* tree = fts_open(paths, FTS_NOCHDIR, NULL);
	FTSENT* node;
	while ((node = fts_read(tree)) != NULL)
	{
		if (node->fts_errno != 0)
		{
			std::cerr << "fts_read \"" << node->fts_accpath << "\" error " << node->fts_errno;
			std::cerr << ": " << strerror(node->fts_errno);
			std::cerr << std::endl;
			continue;
		}
		if ((node->fts_info & FTS_F) != 0 && fnmatch(pattern.c_str(), node->fts_name, 0) == 0)
			files.push_back(node->fts_accpath);
	}
	fts_close(tree);
}
