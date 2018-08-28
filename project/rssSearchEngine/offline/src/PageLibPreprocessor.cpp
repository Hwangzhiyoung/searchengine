 ///
 /// @file    PageLibPreprocessor.cpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2016-01-19 11:35:20
 ///


#include "PageLibPreprocessor.hpp"
#include "GlobalDefine.hpp"

#include <stdio.h>
#include <time.h>
#include <fstream>
#include <sstream>

using std::ifstream;
using std::ofstream;
using std::stringstream;

namespace wd
{

PageLibPreprocessor::PageLibPreprocessor(Configuration & conf)
: _conf(conf)
{}



void PageLibPreprocessor::doProcess()
{
	readInfoFromFile();
	time_t t1 = time(NULL);
	cutRedundantPages();
	buildInvertIndexTable();
	time_t t2 = time(NULL);
	printf("preprocess time: %d min\n", (t2 - t1)/60);

	storeOnDisk();
	time_t t3 = time(NULL);
	printf("store time: %d min\n", (t3 - t2)/60);
}

void PageLibPreprocessor::readInfoFromFile()
{
	map<string, string> &configMap = _conf.getConfigMap();
	string pageLibPath = configMap[RIPEPAGELIB_KEY];
	string offsetLibPath = configMap[OFFSETLIB_KEY];

	ifstream pageIfs(pageLibPath.c_str());
	ifstream offsetIfs(offsetLibPath.c_str());

	if((!pageIfs.good()) || (!offsetIfs.good()))
	{
		cout << "page or offset lib open error" << endl;
	}

	string line;
	int docId, docOffset, docLen;

	while(getline(offsetIfs, line))
	{
		stringstream ss(line);
		ss >> docId >> docOffset >> docLen;
		
		string doc;
		doc.resize(docLen, ' ');
		pageIfs.seekg(docOffset, pageIfs.beg);
		pageIfs.read(&*doc.begin(), docLen);

		WebPage webPage(doc, _conf, _jieba);
		_pageLib.push_back(webPage);

		_offsetLib.insert(std::make_pair(docId, std::make_pair(docOffset, docLen)));
	}
#if 0
	for(auto mit : _offsetLib)
	{
		cout << mit.first << "\t" << mit.second.first << "\t" << mit.second.second << endl;
	}
#endif
}

void PageLibPreprocessor::cutRedundantPages()
{
	cout << "PageLibPreprocessor::cutRedundantPages() _pageLib.size()=" << _pageLib.size() << endl;
	for(size_t i = 0; i != _pageLib.size() - 1; ++i)
	{
		for(size_t j = i + 1; j != _pageLib.size(); ++j)
		{
			if(_pageLib[i] == _pageLib[j])
			{
				_pageLib[j] = _pageLib[_pageLib.size() - 1];
				_pageLib.pop_back();
				--j;
			}
		}
	}
}

void PageLibPreprocessor::buildInvertIndexTable()
{
	for(auto page : _pageLib)
	{
		map<string, int> & wordsMap = page.getWordsMap();
		for(auto wordFreq : wordsMap)
		{
			cout << "wordFreq.first=" << wordFreq.first  <<" page.getDocId()=" << page.getDocId() << " wordFreq.second=" <<wordFreq.second << endl;
			_invertIndexTable[wordFreq.first].push_back(std::make_pair(
					page.getDocId(), wordFreq.second));
		}
	}
	
	//计算每篇文档中的词的权重,并归一化
	map<int, double> weightSum;// 保存每一篇文档中所有词的权重平方和. int 代表docid

	int totalPageNum = _pageLib.size();
	cout << "totalPageNum=" << totalPageNum << endl;
	for(auto & item : _invertIndexTable)
	{	
		//df 出现该词的文档总数
		int df = item.second.size();
		//求关键词item.first的逆文档频率
		double idf = log(static_cast<double>(totalPageNum)/ df + 0.05) / log(2);
		
		for(auto & sitem : item.second)
		{
			double weight = sitem.second * idf;

			weightSum[sitem.first] += pow(weight, 2);
			sitem.second = weight;
		}
	}

	for(auto & item : _invertIndexTable)
	{	//归一化处理
		for(auto & sitem : item.second)
		{
			sitem.second = sitem.second / sqrt(weightSum[sitem.first]);
		}
	}


#if 0 // for debug
	for(auto item : _invertIndexTable)
	{
		cout << item.first << "\t";
		for(auto sitem : item.second)
		{
			cout << sitem.first << "\t" << sitem.second <<  "\t";
		}
		cout << endl;
	}
#endif
}

void PageLibPreprocessor::storeOnDisk()
{
	sort(_pageLib.begin(), _pageLib.end());	

	ofstream ofsPageLib(_conf.getConfigMap()[NEWPAGELIB_KEY].c_str());
	ofstream ofsOffsetLib(_conf.getConfigMap()[NEWOFFSETLIB_KEY].c_str());

	if( !ofsPageLib.good() || !ofsOffsetLib.good())
	{	
		cout << "new page or offset lib ofstream open error!" << endl;
	}

	for(auto & page : _pageLib)
	{
		int id = page.getDocId();
		int length = page.getDoc().size();
		ofstream::pos_type offset = ofsPageLib.tellp();
		ofsPageLib << page.getDoc();

		ofsOffsetLib << id << '\t' << offset << '\t' << length << '\n';
	}

	ofsPageLib.close();
	ofsOffsetLib.close();


	// invertIndexTable
	ofstream ofsInvertIndexTable(_conf.getConfigMap()[INVERTINDEX_KEY].c_str());
	if(!ofsInvertIndexTable.good())
	{
		cout << "invert index table ofstream open error!" << endl;
	}
	for(auto item : _invertIndexTable)
	{
		ofsInvertIndexTable << item.first << "\t";
		for(auto sitem : item.second)
		{
			ofsInvertIndexTable << sitem.first << "\t" << sitem.second <<  "\t";
		}
		ofsInvertIndexTable << endl;
	}
	ofsInvertIndexTable.close();
}

}// end of namespace wd
