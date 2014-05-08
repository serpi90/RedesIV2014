/* 
 * File:   Config.h
 * Author: lubuntu
 *
 * Created on May 6, 2014, 11:08 AM
 */

#ifndef CONFIG_H
#define	CONFIG_H
#include <map>
#include <string>

class Config {
public:
    Config(std::string filename);
    std::string getString(std::string key, std::string defaultValue);
    int getInt(std::string key, int defaultValue);
private:
    std::map<std::string, std::string> properties;
    std::string trim(std::string s);
};

#endif	/* CONFIG_H */

