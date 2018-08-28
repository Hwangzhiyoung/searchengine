 ///
 /// @file    Configuration.cpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2015-11-10 17:30:39
 ///

#include "Configuration.hpp"
#include "GlobalDefine.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

namespace wd
{

Configuration::Configuration(const std::string & filepath)
	: filepath_(filepath)
{
	readConfiguration();
}

void Configuration::readConfiguration()
{
	std::ifstream ifs(filepath_.c_str());
	if(ifs.good())
	{
		std::string line;
		while(getline(ifs, line))
		{
			std::istringstream iss(line);
			std::string key;
			std::string value;
			iss >> key >> value;
			configMap_[key] = value;
		}
		std::cout << "read configuration is over!" << std::endl;
	}
	else
	{
		std::cout << "ifstream open error" << std::endl;
	}
}

std::map<std::string, std::string> & Configuration::getConfigMap()
{
	return configMap_;
}

std::set<std::string> & Configuration::getStopWordList()
{
	if(stopWordList_.size() > 0)
		return stopWordList_;

	std::string stopWordFilePath = configMap_[STOP_WORD_KEY];
	std::ifstream ifs(stopWordFilePath.c_str());
	if(!ifs)
	{
		std::cout << "StopWord ifstream open error!" << std::endl;
	}
	
	std::string word;
	while(getline(ifs, word))
	{
		stopWordList_.insert(word);
	}

// for debug	
#if 0
	for(auto &str : stopWordList_)
	{
		std::cout << str << "\t" << str.size() << std::endl;
	}
	std::cout << std::endl;
#endif

	return stopWordList_;
}

}// end of namespace wd
