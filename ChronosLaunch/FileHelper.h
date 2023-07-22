#pragma once

#include <stdio.h>
#include <string>

class FileHelper {
public:
    static bool FileExist(const std::string& filePathStr);
    static bool DirectoryExist(const std::string& dirPath);
    static bool CreateDir(const std::string& path);
    static off_t getFileSize(FILE* pFile);
};

