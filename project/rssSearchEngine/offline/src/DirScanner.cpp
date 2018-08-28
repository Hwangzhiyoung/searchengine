 ///
 /// @file    DirScannerBak.cpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2015-11-10 09:34:28
 ///


#include "DirScanner.hpp"
#include "Configuration.hpp"
#include "GlobalDefine.hpp"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include <iostream>

using std::cout;
using std::endl;

namespace wd
{

DirScanner::DirScanner(Configuration & conf)
	: conf_(conf)
{
	vecFiles_.reserve(kFileNo);
}

void DirScanner::operator()()
{
	std::map<std::string, std::string> & confMap = conf_.getConfigMap();
	std::string dirName = confMap[YULIAO_KEY];
	traverse(dirName);
}

std::vector<std::string> & DirScanner::files()
{
	return vecFiles_;
}

void DirScanner::debug()
{
	for(auto & elem : vecFiles_)
	{
		cout << elem << endl;
	}
}
void DirScanner::traverse(const std::string & dirName)
{
	//打开指定的目录
	DIR * pdir = ::opendir(dirName.c_str());
	if(NULL == pdir)
	{
		printf("dir open error");	
		exit(EXIT_FAILURE);
	}

	//进入指定的目录
	::chdir(dirName.c_str());
	struct dirent * mydirent;
	struct stat mystat;
	//依次遍历该目录中的相关项
	while(NULL != (mydirent = ::readdir(pdir)))
	{
		//获取目录中项的属性
		::stat(mydirent->d_name, &mystat);
		//判断该项是不是目录。
		if(S_ISDIR(mystat.st_mode))
		{
		
			//如果该目录是'.'和‘..’（每个目录下都有这两项，如果不排除这两项程序会进入无限循环），则跳过该次循环继续下一次
			if(0 == ::strncmp(mydirent->d_name, ".", 1) || 
			   0 == ::strncmp(mydirent->d_name, "..", 2))
			{	continue;	}
			else//如果该目录不是前二者，则递归的遍历目录 
			{	traverse(mydirent->d_name);	}
		}
		else
		{
			//如果该项不是目录（是文件）,则保存该项的绝对路径
			std::string fileName;
			fileName.append(::getcwd(NULL, 0))
					.append("/")
					.append(mydirent->d_name);
			vecFiles_.push_back(fileName);
		}
	}
	::chdir("..");
	::closedir(pdir);
}

} // end of namespace wd
