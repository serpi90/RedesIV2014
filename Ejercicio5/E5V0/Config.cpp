#include "Config.h"
#include <algorithm>
#include <fstream>

#define MAX_LINE_SIZE 256

Config::Config(std::string filename)
{
    char raw_line[MAX_LINE_SIZE];
    std::string line, key, value;
    std::ifstream file(filename.c_str());
    while (!file.eof())
    {
        file.getline(raw_line, MAX_LINE_SIZE);
        line = trim(std::string(raw_line));
        if (line[0] != '#' && line[0] != ';' && !line.empty())
        {
            key = trim(line.substr(0, line.find('=')));
            value = trim(line.substr(line.find('=')).erase(0, 1));
            if (!key.empty())
            {
                properties.insert(std::pair<std::string, std::string>(key, value));
            }
        }
    }
}

std::string Config::getString(std::string key, std::string defaultValue)
{
    std::map<std::string, std::string>::iterator it = properties.find(key);
    if (it != properties.end())
    {
        return it->second;
    } else
    {
        return defaultValue;
    }
}

int Config::getInt(std::string key, int defaultValue)
{
    std::map<std::string, std::string>::iterator it = properties.find(key);
    if (it != properties.end())
    {
        return atoi(it->second.c_str());
    } else
    {
        return defaultValue;
    }
}

std::string Config::trim(std::string s)
{
    std::string::iterator lend, rbegin;
    std::string::reverse_iterator rrbegin;
    lend = std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace)));
    s.erase(s.begin(), lend);
    rrbegin = std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace)));
    rbegin = rrbegin.base();
    s.erase(rbegin, s.end());
    return s;
}