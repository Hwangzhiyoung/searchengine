 ///
 /// @file    PageLib.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2015-11-10 15:57:22
 ///


#ifndef _WD_PAGELIB_H_
#define _WD_PAGELIB_H_

#include <string>
#include <vector>


namespace wd
{

class Configuration;
class DirScanner;

class PageLib
{
public:
	PageLib(Configuration & conf,
			DirScanner & dirScanner);

	void create();
	void store();

private:
	Configuration &          _conf;
	DirScanner &             _dirScanner;
	std::vector<std::string> _vecPages;
};

}//end of namespace wd

#endif
