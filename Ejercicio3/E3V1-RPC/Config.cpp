#include "Config.h"
#include <algorithm>
#include <fstream>

Config::Config(std::string filename, unsigned maxLineLength)
{
    char * rawLine = (char*) malloc(sizeof (char) * maxLineLength);
    std::string line, key, value;
    std::ifstream file(filename.c_str());
    if (file.is_open())
    {
        while (!file.eof())
        {
            file.getline(rawLine, maxLineLength);
            line = trim(std::string(rawLine));
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
        free(rawLine);
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