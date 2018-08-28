 ///
 /// @file    WordQuery.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2016-01-21 09:42:00
 ///

#ifndef _WD_WORD_QUERY_H_
#define _WD_WORD_QUERY_H_

#include "WebPage.hpp"
#include "Configuration.hpp"
#include "WordSegmentation.hpp"

#include <vector>
#include <map>
#include <set>
#include <string>

using std::vector;
using std::map;
using std::unordered_map;
using std::set;
using std::string;
using std::pair;
using std::make_pair;
using namespace wd;

namespace wd
{

class WordQuery
{
public:
	WordQuery(Configuration & conf);

	string doQuery(const string & str);

private:
	void loadLibrary();

	vector<double> getQueryWordsWeightVector(vector<string> & queryWords);
	
	bool executeQuery(const vector<string> & queryWords, vector<pair<int, vector<double> > > & resultVec);
	
	string createJson(vector<int> & docIdVec, const vector<string> & queryWords);

	string returnNoAnswer();

private:
	WordSegmentation                                _jieba;
	Configuration &									_conf;
	unordered_map<int, WebPage>                     _pageLib;
	unordered_map<int, pair<int, int> >             _offsetLib;
	unordered_map<string, set<pair<int, double> > > _invertIndexTable;
};

} // end of namespce wd
#endif
