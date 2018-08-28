 ///
 /// @file    RssReader.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2016-08-30 14:09:14
 ///

#include "RssReader.hpp"
 
#include <iostream>
#include <sstream>
#include <fstream>
#include <boost/regex.hpp>
using std::cout;
using std::endl;
using std::ostringstream;
using std::ofstream;


RssReader::RssReader(vector<string> & files)
: _files(files)
{
	cout << "RssReader()" << endl;
}
RssReader::~RssReader()
{
	cout << "~RssReader()" << endl;
}
	
void RssReader::loadFeedFiles()
{
	for(size_t idx = 0; idx != _files.size(); ++idx)
	{
		loadFeedFile(_files[idx]);
		cout << "has load " << idx + 1 << " rss files" << endl;
	}
}

//处理一个rss文件
void RssReader::loadFeedFile(const string & filename)
{
	XMLDocument doc;
	doc.LoadFile(filename.c_str());
	cout << "loadFile: " << filename << endl;
	XMLError errorID = doc.ErrorID();
	if(errorID)
	{
		cout << "XMLDocument LoadFile error!" << endl;
		return ;
	}
	else
	{
		parseRss(doc);
	}
}


void RssReader::makePages(vector<string> & pages)
{
	cout << "pages's size = " << _items.size() << endl;
	for(size_t idx = 0; idx != _items.size(); ++idx)
	{
		ostringstream oss;
		oss << "<doc>\n"
			<< "  <docid>" << (idx + 1) << "</docid>\n"
			<< "  <title>" << _items[idx]->_title << "</title>\n"
			<< "  <link>"  << _items[idx]->_link << "</link>\n" 
			<< "  <content>" << _items[idx]->_content << "</content>\n"
			<< "/<doc>\n";
		
		string page = oss.str();
		pages.push_back(page);
	}
}

void RssReader::parseRss(XMLDocument & doc)
{
	cout << "parsRss(XMLDocument & doc)" << endl;
	XMLElement * root = doc.FirstChildElement();
	//<channel>
	XMLElement * channel = root->FirstChildElement("channel");

	//  <item>
	XMLElement * item = channel->FirstChildElement("item");
	for(; item; item = item->NextSiblingElement())
	{
		shared_ptr<RssItem> rssItemPtr(new RssItem);
		const char * itemTitle = item->FirstChildElement("title")->GetText();
		const char * itemLink = item->FirstChildElement("link")->GetText();
		const char * itemDescription = item->FirstChildElement("description")->GetText();

		const char * iContent = nullptr;
		XMLElement * contentEncoded = item->FirstChildElement("content:encoded");
		if(contentEncoded)
		{	iContent = contentEncoded->GetText();	}
		else
		{	iContent = itemDescription; }

		//string szReg = "<(\S*?)[^>]*>";//可行
		string szReg = "<.*?>";//可行
		
		boost::regex fmt(szReg);
		string content = boost::regex_replace(string(iContent), fmt, string(""));
		
		rssItemPtr->_title = itemTitle ? itemTitle : string("");
		rssItemPtr->_link = itemLink ? itemLink : string("");
		rssItemPtr->_content = content;

		_items.push_back(rssItemPtr);
	}
}

#if 0
string RssReader::debug()
{
	ostringstream oss;
#if 1
	oss << "--- <RssData> ---\n"
		<< "Version: " << _rssDataPtr->_version << "\n"
		<< "Language: " << _rssDataPtr->_language << "\n"
		<< "Link: " << _rssDataPtr->_link << "\n"
		<< "Title: " << _rssDataPtr->_title << "\n"
		<< "Description: " << _rssDataPtr->_description << "\n"
		<< "LastBuildDate: " << _rssDataPtr->_lastBuildDate << "\n"
		<< "  <items>  " << "\n";
#endif
	
	for(size_t idx = 0; idx != _rssDataPtr->_items.size(); ++idx)
	{
		oss << "  ---items[" << idx << "]---\n" 
			<< "  items[" << idx << "].title: " << _rssDataPtr->_items[idx]->_title << "\n"
			<< "  items[" << idx << "].link: " << _rssDataPtr->_items[idx]->_link << "\n"
		//	<< "  items[" << idx << "].guid: " << _rssDataPtr->_items[idx]->_guid << "\n"
		//	<< "  items[" << idx << "].pubDate: " << _rssDataPtr->_items[idx]->_pubDate << "\n"
		//	<< "  items[" << idx << "].description: " << _rssDataPtr->_items[idx]->_description << "\n"
			<< "  items[" << idx << "].content: " << "\n" 
			<< _rssDataPtr->_items[idx]->_content << "\n";
	}

	oss	<< "--- </RssData> ---\n";

	return oss.str();
}	

void RssReader::dump(const string & filename)
{
	ofstream ofs(filename.c_str());
	if(!ofs.good())
	{	cout << "ofstream open error!" << endl;	}

	ofs << debug();
	ofs.close();
}
#endif 
