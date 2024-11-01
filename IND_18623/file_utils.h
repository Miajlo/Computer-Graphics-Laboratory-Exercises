#pragma once
#include <iostream>
#include <string>
#include <direct.h>  // For _getcwd on Windows
#include <limits.h>  // For MAX_PATH


class file_utils {
public:
	static std::string get_current_directory() {
        char buffer[_MAX_PATH];

        if (_getcwd(buffer, sizeof(buffer)) == nullptr) {
            perror("getcwd error");
            return {};
        }
        
        return std::string(buffer);
    }

    static std::string combine_paths(std::string& path1, std::string& path2) {
        return path1 + "\\" + path2;
    }
};