 ///
 /// @file    DirScanner.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2015-11-10 09:26:00
 ///


#ifndef _WD_DIRSCAN_H_
#define _WD_DIRSCAN_H_

#include <string>
#include <vector>

namespace wd
{

const int kFileNo = 10000;

class Configuration;

class DirScanner
{
public:
	DirScanner(Configuration & conf);

	void operator()();

	std::vector<std::string> & files();

	void debug();
private:
	void traverse(const std::string & dirName);

private:
	Configuration & conf_;
	std::vector<std::string> vecFiles_;
};

} // end of namespace wd
#endif
