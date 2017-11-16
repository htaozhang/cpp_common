// Copyright(c) 2017 macrozhang
// All rights reserved.
//
//

#ifndef __GENERIC_SOCKET_H__
#define __GENERIC_SOCKET_H__

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1 
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#ifdef errno
#undef errno
#endif
#define errno WSAGetLastError()

//#define strerror_r(errno, buf, len) strerror_s(buf, len, errno)

#define SO_NOSIGPIPE 0

typedef int socklen_t;
typedef long ssize_t;

inline int _getsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen) {
    return (optname == SO_NOSIGPIPE ? 0 : 
        (getsockopt(sockfd, level, optname, (char*)optval, optlen) == 0 ? 0 : -1));
}

inline int _setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen) {
    return (optname == SO_NOSIGPIPE ? 0 :
        (setsockopt(sockfd, level, optname, (const char*)optval, optlen) == 0 ? 0 : -1));
}

inline const char *_inet_ntop(int af, const void *src,
    char *dst, socklen_t size) {
    if (af == AF_INET) { // ipv4
        char cache[sizeof "255.255.255.255"];
        const unsigned char *p = static_cast<const unsigned char*>(src);
        int len = snprintf(cache, sizeof(cache), "%u.%u.%u.%u", p[0], p[1], p[2], p[3]);
        if (len >= size) {
            return NULL;
        } else {
            memcpy(dst, cache, len + 1);
            return dst;
        }
    } else if (af == AF_INET6) { // ipv6, don't support in windows
        return NULL;
    }

    return NULL;
}

inline int _inet_pton(int af, const char *src, void *dst) {
    if (af == AF_INET) {
        unsigned long addr = inet_addr(src);
        if (addr = INADDR_NONE) {
            return 0;
        }
        memcpy(dst, &addr, sizeof addr);
        return 1;
    } else if (af == AF_INET6) {
        return 0;
    }
    return 0;
}

inline void initsocket() {
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2), &wsaData);
}

#define setsockopt _setsockopt
#define getsockopt _getsockopt
#define inet_ntop _inet_ntop
#define inet_pton _inet_pton

#else
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h> // inet_pton()

inline void initsocket() {}

#endif

#endif /* __GENERIC_SOCKET_H__ */
