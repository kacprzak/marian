#ifndef PACKET_H
#define PACKET_H

#include "config.h"

#if PLATFORM == PLATFORM_WINDOWS
#include <winsock2.h> // ntohl
#include <ws2tcpip.h>

typedef unsigned __int32 uint32;
#else
#include <arpa/inet.h> // ntohl

typedef u_int32_t uint32;
#endif

#include <cassert>
#include <cstring> // memcpy

namespace net {

class Packet final
{
  public:
    Packet(const char* const data, uint32 size);
    ~Packet();
    Packet(const Packet& other) = delete;
    Packet& operator=(const Packet& other) = delete;

    const char* getData() const { return m_data; }
    uint32 getSize() const;

  protected:
    char* m_data;

  private:
};

//------------------------------------------------------------------------------

inline Packet::Packet(const char* const data, uint32 size)
{
    // Allocate memory for size value and data
    m_data = new char[sizeof(size) + size];
    assert(m_data);
    // Copy size value into first 4 bytes
    *(uint32*)m_data = htonl(sizeof(size) + size);
    // Copy rest of the data
    memcpy(m_data + sizeof(size), data, size);
}

//------------------------------------------------------------------------------

inline Packet::~Packet() { delete[] m_data; }

//------------------------------------------------------------------------------

inline uint32 Packet::getSize() const
{
    // Size is in first 4 bytes of data
    return ntohl(*(uint32*)m_data);
}

} // namespace network

#endif // PACKET_H
