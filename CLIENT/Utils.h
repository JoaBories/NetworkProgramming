#pragma once
#include <string>
#include <vector>

namespace Utils
{
    inline std::vector<std::string> breakString(const std::string& str, const char delimiter)
    {
        std::vector<std::string> strings;
        std::string temp = str;
        size_t pos = temp.find(delimiter);
        
        while (pos != std::string::npos)
        {
            strings.emplace_back(temp.substr(0, pos));
            temp = temp.substr(pos + 1);
            pos = temp.find(delimiter);
        }
        strings.push_back(temp);
        
        return strings;
    }    
};
