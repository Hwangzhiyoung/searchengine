 ///
 /// @file    WordQueryServer.cpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2016-01-25 19:46:04
 ///


#include "TcpServer.h"
#include "WordQuery.hpp"
#include "Threadpool.h"

#include <stdio.h>

#include <string>
#include <functional>


using namespace std;
using namespace wd;

class WordQueryServer
{
public:
	WordQueryServer(const string & configfile);

	void start();

private:
	void onConnection(const TcpConnectionPtr & conn);

	void onMessage(const TcpConnectionPtr & conn);

	void onClose(const TcpConnectionPtr & conn);

	void doTaskThread(const TcpConnectionPtr & conn, const string & msg);

private:
	Configuration _conf;
	WordQuery _wordQuery;
	TcpServer _tcpServer;
	Threadpool _pool;
};


WordQueryServer::WordQueryServer(const string & configfile)
: _conf(configfile)
, _wordQuery(_conf)
, _tcpServer(5080)
, _pool(4, 10)
{
	_tcpServer.setConnectionCallback(
			std::bind(&WordQueryServer::onConnection, this, placeholders::_1));
	_tcpServer.setMessageCallback(
			std::bind(&WordQueryServer::onMessage, this, placeholders::_1));
	_tcpServer.setCloseCallback(
			std::bind(&WordQueryServer::onClose, this, placeholders::_1));

}

void WordQueryServer::start()
{
	_pool.start();
	_tcpServer.start();
}


void WordQueryServer::onConnection(const TcpConnectionPtr & conn)
{
	printf("%s\n", conn->toString().c_str());
}


void WordQueryServer::onMessage(const TcpConnectionPtr & conn)
{
	string msg(conn->receive());
	size_t pos = msg.find('\n');
	msg = msg.substr(0, pos);
	cout << "client:" << msg << ",size:" << msg.size() << endl;

	//string ret = _wordQuery.doQuery(msg);
	//cout << "result's size:" << ret.size() << endl;
	//conn->send(ret);

	_pool.addTask(std::bind(&WordQueryServer::doTaskThread, this, conn, msg));
}


void WordQueryServer::onClose(const TcpConnectionPtr & conn)
{
	printf("%s close.\n", conn->toString().c_str());
}

void WordQueryServer::doTaskThread(const TcpConnectionPtr & conn, const string & msg)
{
	string ret = _wordQuery.doQuery(msg);

	int sz = ret.size();
	printf("result's size:%d\n",sz); 
	//printf("%s\n\n", ret.c_str());
	conn->sendInLoop(ret);
}


int main(void)
{
	WordQueryServer wordQueryServer("./conf/my.conf");
	wordQueryServer.start();
	
	return 0;
}
