 ///
 /// @file    Configuration.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2015-11-10 17:21:12
 ///

#ifndef _WD_CONFIGURATION_H_
#define _WD_CONFIGURATION_H_

#include <set>
#include <map>
#include <string>

namespace wd
{

class Configuration
{
public:
	Configuration(const std::string & filepath);

	std::map<std::string, std::string> & getConfigMap();
	std::set<std::string> & getStopWordList();
private:

	void readConfiguration();
private:
	std::string filepath_;
	std::map<std::string, std::string> configMap_;
	std::set<std::string> stopWordList_;
};

}//end of namespace wd

#endif
