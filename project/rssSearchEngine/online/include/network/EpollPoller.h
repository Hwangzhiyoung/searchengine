 ///
 /// @file    EpollPoller.h
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2015-11-06 16:12:11
 ///


#ifndef __WD_EPOLLPOLLER_H
#define __WD_EPOLLPOLLER_H

#include "Noncopyable.h"
#include "TcpConnection.h"
#include "MutexLock.h"
#include <sys/epoll.h>
#include <vector>
#include <map>
#include <functional>

namespace wd
{
class EpollPoller : Noncopyable
{
public:
	typedef TcpConnection::TcpConnectionCallback EpollCallback;
	typedef std::function<void()> Functor;

	EpollPoller(int listenfd);
	~EpollPoller();

	void loop();
	void unloop();
	void runInLoop(const Functor & cb);
	void doPendingFunctors();

	void wakeup();
	void handleRead();

	void setConnectionCallback(EpollCallback cb);
	void setMessageCallback(EpollCallback cb);
	void setCloseCallback(EpollCallback cb);

private:
	void waitEpollfd();
	void handleConnection();
	void handleMessage(int peerfd);
	
private:
	int epollfd_;
	int listenfd_;
	int wakeupfd_;
	bool isLooping_;

	MutexLock mutex_;
	std::vector<Functor> pendingFunctors_;

	typedef std::vector<struct epoll_event> EventList;
	EventList eventsList_;

	typedef std::map<int, TcpConnectionPtr> ConnectionMap;
	ConnectionMap connMap_;

	EpollCallback onConnectionCb_;
	EpollCallback onMessageCb_;
	EpollCallback onCloseCb_;
};


}//end of namespace wd

#endif
