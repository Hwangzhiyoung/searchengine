 ///
 /// @file    WordSegmentation.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2016-01-18 15:43:06
 ///

#ifndef _WD_WORD_SEGMENTATION_H_
#define _WD_WORD_SEGMENTATION_H_


#include "cppjieba/Jieba.hpp"

#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::vector;

namespace wd
{

const char * const DICT_PATH = "/data/wwwroot/SearchEngine/project/cppjieba/dict/jieba.dict.utf8";
const char * const HMM_PATH = "/data/wwwroot/SearchEngine/project/cppjieba/dict/hmm_model.utf8";
const char * const USER_DICT_PATH = "/data/wwwroot/SearchEngine/project/cppjieba/dict/user.dict.utf8";


class WordSegmentation
{
public:
	WordSegmentation()
		: _jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH)
	{
		cout << "cppjieba init!" << endl;
	}


	vector<string> operator()(const char * src)
	{
		vector<string> words;
		_jieba.Cut(src, words, true);//cutAll
		return words;
	}

private:
	cppjieba::Jieba _jieba;
};

}// end of namespace wd


#endif
