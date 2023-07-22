#include "FileHelper.h"

#include <filesystem>

bool FileHelper::FileExist(const std::string& filePathStr)
{
    FILE* f = nullptr;

    #ifdef _WIN32

    if(fopen_s(&f, filePathStr.c_str(), "r"))
        return false;

    #elif defined(LINUX)
    f = fopen(filePathStr.c_str(), "r");
    #endif
    
    if(f == nullptr)
        return false;

    fclose(f);
    
    return true;
}

namespace fs = std::filesystem;

bool FileHelper::DirectoryExist(const std::string& dirPath)
{
    return fs::exists(dirPath) && fs::is_directory(dirPath);
}

bool FileHelper::CreateDir(const std::string& path)
{
    try {
        fs::create_directory(path);
        return true;
    }
    catch (...) {
        return false;
    }
}

off_t FileHelper::getFileSize(FILE* pFile)
{
    off_t currOffset = ftell(pFile);

    // Going to the bottom
    fseek(pFile, 0, SEEK_END);
    off_t bottomOffset = ftell(pFile);

    // Restoring Stream Pointer
    fseek(pFile, currOffset, SEEK_SET);

    return bottomOffset;
}