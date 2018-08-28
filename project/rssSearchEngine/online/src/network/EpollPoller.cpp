 ///
 /// @file    EpollPoller.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2015-11-06 16:18:29
 ///

#include "EpollPoller.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <sys/eventfd.h>


namespace wd
{

int createEpollFd()
{
	int efd = ::epoll_create1(0);
	if(-1 == efd)
	{
		perror("epoll_create1 error");
		exit(EXIT_FAILURE);
	}
	return efd;
}

int createEventFd()
{
	int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
	if(-1 == evtfd)
	{
		perror("eventfd create error");
	}
	return evtfd;
}

void addEpollFdRead(int efd, int fd)
{
	struct epoll_event ev;
	ev.data.fd = fd;
	ev.events = EPOLLIN;
	int ret = epoll_ctl(efd, EPOLL_CTL_ADD, fd, &ev);
	if(-1 == ret)
	{
		perror("epoll_ctl add error");
		exit(EXIT_FAILURE);
	}
}

void delEpollReadFd(int efd, int fd)
{
	struct epoll_event ev;
	ev.data.fd = fd;
	int ret = epoll_ctl(efd, EPOLL_CTL_DEL, fd, &ev);
	if(-1 == ret)
	{
		perror("epoll_ctl del error");
		exit(EXIT_FAILURE);
	}
}

int acceptConnFd(int listenfd)
{
	int peerfd = ::accept(listenfd, NULL, NULL);
	if(peerfd == -1)
	{
		perror("accept error");
		exit(EXIT_FAILURE);
	}
	return peerfd;
}


//预览数据
size_t recvPeek(int sockfd, void * buf, size_t len)
{
	int nread;
	do
	{
		nread = ::recv(sockfd, buf, len, MSG_PEEK);
	}while(nread == -1 && errno == EINTR);
	return nread;
}

//通过预览数据，判断conn是否关闭
bool isConnectionClosed(int sockfd)
{
	char buf[1024];
	int nread = recvPeek(sockfd, buf, sizeof(buf));
	if(-1 == nread)
	{
		perror("recvPeek--- ");
		return true;
		//exit(EXIT_FAILURE);//若peer端已关闭连接，会导致server端崩溃
	}
	return (0 == nread);
}

//==========================================


EpollPoller::EpollPoller(int listenfd)
: epollfd_(createEpollFd())
, listenfd_(listenfd)
, wakeupfd_(createEventFd())
, isLooping_(false)
, eventsList_(1024)
{
	addEpollFdRead(epollfd_, listenfd_);
	addEpollFdRead(epollfd_, wakeupfd_);
}


EpollPoller::~EpollPoller()
{
	::close(epollfd_);
}

void EpollPoller::loop()
{
	isLooping_ = true;
	while(isLooping_)
	{
		waitEpollfd();
	}
}

void EpollPoller::unloop()
{
	if(isLooping_)
		isLooping_ = false;
}


void EpollPoller::runInLoop(const Functor & cb)
{
	{
	MutexLockGuard mlg(mutex_);
	pendingFunctors_.push_back(cb);
	}
	wakeup();
}

void EpollPoller::doPendingFunctors()
{
	printf("doPendingFunctors()\n");
	std::vector<Functor> functors;
	{
	MutexLockGuard mlg(mutex_);
	functors.swap(pendingFunctors_);
	}
	
	for(size_t i = 0; i < functors.size(); ++i)
	{
		functors[i]();
	}
}


void EpollPoller::wakeup()
{
	uint64_t one = 1;
	ssize_t n = ::write(wakeupfd_, &one, sizeof(one));
	if(n != sizeof(one))
	{
		perror("EpollPoller::wakeup() n != 8");
	}
}

void EpollPoller::handleRead()
{
	uint64_t one = 1;
	ssize_t n = ::read(wakeupfd_, &one, sizeof(one));
	if(n != sizeof(one))
	{
		perror("EpollPoller::handleRead() n != 8");
	}
}


void EpollPoller::setConnectionCallback(EpollCallback cb)
{
	onConnectionCb_ = cb;
}

void EpollPoller::setMessageCallback(EpollCallback cb)
{
	onMessageCb_ = cb;
}

void EpollPoller::setCloseCallback(EpollCallback cb)
{
	onCloseCb_ = cb;
}

void EpollPoller::waitEpollfd()
{
	int nready;
	do
	{
		nready = ::epoll_wait(epollfd_, 
							  &(*eventsList_.begin()),
							  eventsList_.size(),
							  5000);
	}while(nready == -1 && errno == EINTR);

	if(nready == -1)
	{
		perror("epoll_wait error");
		exit(EXIT_FAILURE);
	}
	else if(nready == 0)
	{
		printf("epoll_wait timeout\n");	
	}
	else
	{
		//做一个扩容的操作
		if(nready == static_cast<int>(eventsList_.size()))
		{
			eventsList_.resize(eventsList_.size() * 2);
		}
		
		//遍历每一个激活的文件描述符
		for(int idx = 0; idx != nready; ++idx)
		{
			if(eventsList_[idx].data.fd == listenfd_)
			{
				if(eventsList_[idx].events & EPOLLIN)
				{
					handleConnection();
				}
			}
			else if(eventsList_[idx].data.fd == wakeupfd_)
			{
				printf("wakeupfd light\n");
				if(eventsList_[idx].events & EPOLLIN)
				{
					handleRead();
					doPendingFunctors();
				}
			}
			else //accept之后的peerfd被注册到eventList中，以后都靠它来传递客户端/服务端的信息（socket编程）
			{
				if(eventsList_[idx].events & EPOLLIN)//如果读到有
				{
					handleMessage(eventsList_[idx].data.fd);
				}
			}
		}//end for
	}//end else
}

void EpollPoller::handleConnection()
{
	int peerfd = acceptConnFd(listenfd_);
	addEpollFdRead(epollfd_, peerfd);

	TcpConnectionPtr conn(new TcpConnection(peerfd, this));
	//...给客户端发一个欢迎信息==> 挖一个空: 等...
	//conn->send("welcome to server.\n");
	conn->setConnectionCallback(onConnectionCb_);
	conn->setMessageCallback(onMessageCb_);
	conn->setCloseCallback(onCloseCb_);

	std::pair<ConnectionMap::iterator, bool> ret;
	ret = connMap_.insert(std::make_pair(peerfd, conn));
	assert(ret.second == true);
	(void)ret;
	//connMap_[peerfd] = conn;

	conn->handleConnectionCallback();
}

void EpollPoller::handleMessage(int peerfd)
{
	bool isClosed = isConnectionClosed(peerfd);
	ConnectionMap::iterator it = connMap_.find(peerfd);
	assert(it != connMap_.end());

	if(isClosed)
	{
		it->second->handleCloseCallback();
		delEpollReadFd(epollfd_, peerfd);
		connMap_.erase(it);
	}
	else
	{
		it->second->handleMessageCallback();
	}
}

}// end of namespace wd
