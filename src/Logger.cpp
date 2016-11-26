#include "Logger.h"

#include <algorithm>
#include <cstdio>
#include <cstring>

#define SHOW_PKT_SIZE false

void logHelper(int sockId, const char* data, unsigned int size, const char* msg)
{
    // debug data
    char buf[60];
    const unsigned int buf_size = sizeof(buf);

    memset(buf, '\0', buf_size);
    memcpy(buf, data, std::min(size, buf_size - 1));

    // Every byte is two nibbles plus ':' char
    char buf_hex[buf_size * 3 + 1];
    memset(buf_hex, '\0', sizeof(buf_hex));

    char* buf_hex_ptr = buf_hex;
    for (unsigned int i = 0; i < std::min(size, buf_size - 1); ++i) {
        buf_hex_ptr += sprintf(buf_hex_ptr, "%02x:", buf[i]);
    }
    // If something was inserted
    if (buf_hex_ptr > buf_hex)
        *(buf_hex_ptr - 1) = '\0';

    buf_hex_ptr                = buf_hex;
    char* buf_ptr              = buf;
    const size_t sizeof_uint32 = 4; // sizeof(u_int32_t);
    if (!SHOW_PKT_SIZE && (size > sizeof_uint32)) {
        buf_hex_ptr += sizeof_uint32 * 2 + 4; // plus 4 ':' chars
        buf_ptr += sizeof_uint32;
    }

    std::clog << "SockId=" << sockId << " " << msg << " " << std::setw(2)
              << size << " B: "
              << "|" << buf_hex_ptr << "||" << buf_ptr << "|" << std::endl;
}
