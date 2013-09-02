/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef PACKET_H
#define PACKET_H

#include <cassert>
#include <cstring>     // memcpy
#include <arpa/inet.h> // ntohl

class Packet
{
 public:
    Packet(const char * const data, unsigned long size);
    ~Packet();

    virtual const char *  getData() const { return m_data; }
    virtual unsigned long getSize() const;

 protected:
    char *m_data;

 private:

};

inline Packet::Packet(const char * const data, unsigned long size)
{
    // Allocate memory for size value and data
    m_data = new char[sizeof(size) + size];
    assert(m_data);
    // Copy size value into first 4 bytes
    *(unsigned long *)m_data = htonl(sizeof(size) + size);
    // Copy rest of the data
    memcpy(m_data + sizeof(size), data, size);
}

inline Packet::~Packet()
{
    delete m_data;
}

inline unsigned long Packet::getSize() const
{
    // Size is in first 4 bytes of data
    return ntohl(*(unsigned long *)m_data);
}

#endif // PACKET_H
