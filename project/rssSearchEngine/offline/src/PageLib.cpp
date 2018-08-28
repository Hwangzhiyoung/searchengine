 ///
 /// @file    PageLib.cpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2015-11-10 16:55:10
 ///


#include "Configuration.hpp"
#include "PageLib.hpp"
#include "DirScanner.hpp"
#include "RssReader.hpp"
#include "GlobalDefine.hpp"

#include <iostream>
#include <fstream>

namespace wd
{
PageLib::PageLib(Configuration & conf,
				 DirScanner & dirScanner)
	: _conf(conf),
	  _dirScanner(dirScanner)
{}


void PageLib::create()
{
	std::vector<std::string> & vecFiles = _dirScanner.files();//xml文件
	RssReader reader(vecFiles);
	reader.loadFeedFiles();
	reader.makePages(_vecPages);
}

void PageLib::store()
{
	//需要读取配置信息//???
	std::map<std::string, std::string> & confMap = _conf.getConfigMap();
	std::string pageLibPath = confMap[RIPEPAGELIB_KEY];
	std::string offsetLibPath = confMap[OFFSETLIB_KEY];

	std::ofstream ofsPage(pageLibPath.c_str());
	std::ofstream ofsOffset(offsetLibPath.c_str());
	if(!(ofsPage.good() && ofsOffset.good()))
	{
		std::cout << "ofstream open error!" << std::endl;
		return;
	}

	for(size_t idx = 0; idx != _vecPages.size(); ++idx)
	{
		int id = idx + 1;
		int length = _vecPages[idx].size();
		std::ofstream::pos_type offset = ofsPage.tellp();
		ofsPage << _vecPages[idx];

		ofsOffset << id << '\t' << offset << '\t' << length << '\n';
	}

	ofsPage.close();
	ofsOffset.close();
}

}//end of namespace wd
