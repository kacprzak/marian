#include "ListenNetSocket.h"

#if PLATFORM == PLATFORM_WINDOWS
  #include <winsock2.h>
#else
  #include <unistd.h> // close
#endif

#include "Logger.h"

using namespace net;

ListenNetSocket::ListenNetSocket()
{
}

//------------------------------------------------------------------------------

void ListenNetSocket::init(int portNum)
{
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket == -1) {
        PLOG << "socket";
        exit(-1);
    }

    int value = 1;
    if (setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, (const char *)&value, sizeof(value)) == -1) {
        PLOG << "setsockopt";
    }

    struct sockaddr_in sa;
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = INADDR_ANY;
    sa.sin_port = htons(portNum);

    if (bind(m_socket, (struct sockaddr*)&sa, sizeof(sa)) == -1) {
        PLOG << "bind";
#if PLATFORM == PLATFORM_WINDOWS
        closesocket(m_socket);
#else
        close(m_socket);
#endif
        m_socket = -1;
    }

    setBlocking(false);

    if (listen(m_socket, 256) == -1) {
        PLOG << "listen";
#if PLATFORM == PLATFORM_WINDOWS
        closesocket(m_socket);
#else
        close(m_socket);
#endif
        m_socket = -1;
    }

    port = portNum;
}

//------------------------------------------------------------------------------

int ListenNetSocket::acceptConnection(unsigned int *addr)
{
#if PLATFORM == PLATFORM_WINDOWS
    typedef int socklen_t;
#endif

    int new_socket;
    struct sockaddr_in socka;
    socklen_t size = sizeof(socka);

    new_socket = accept(m_socket, (struct sockaddr*)&socka, &size);
    if (new_socket == -1) {
        PLOG << "accept";
        return -1;
    }

    if (getpeername(new_socket, (struct sockaddr*)&socka, &size) == -1) {
        PLOG << "getpeername";
#if PLATFORM == PLATFORM_WINDOWS
        closesocket(m_socket);
#else
        close(m_socket);
#endif
        return -1;
    }

    *addr = ntohl(socka.sin_addr.s_addr);

    return new_socket;
}
