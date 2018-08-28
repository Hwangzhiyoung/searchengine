 ///
 /// @file    Socket.h
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2015-11-04 17:25:32
 ///

#ifndef __WD_SOCKET_H
#define __WD_SOCKET_H

#include "Noncopyable.h"

namespace wd
{
class InetAddress;
class Socket : Noncopyable
{
public:
	Socket(int sockfd);
	Socket();
	~Socket();

	void ready(const InetAddress & addr);

	int accept();
	void shutdownWrite();
	int fd(){	return sockfd_;	}

	static InetAddress getLocalAddr(int sockfd);
	static InetAddress getPeerAddr(int sockfd);

private:
	void bindAddress(const InetAddress & addr);
	void listen();
	void setReuseAddr(bool flag);
	void setReusePort(bool flag);
private:
	int sockfd_;
};
}// end of namespace wd

#endif
