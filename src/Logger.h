#ifndef LOGGER_H
#define LOGGER_H

#include <ctime>
#include <iomanip>
#include <iostream>

/*!
 * \brief Super simple log library.
 */

static std::clock_t startTime = std::clock();

static const char* severMsg[] = {"[DEBUG] ", "[INFO]  ", "[WARN]  ", "[ERROR] ",
                                 "[FATAL] "};

void logHelper(int sockId, const char* data, unsigned int size,
               const char* msg);

// This will show CPU time! It is different from the actual time.
#define _LOG_CLOCK                                                             \
    << std::setw(4) << std::setprecision(2) << std::fixed                      \
    << (float(std::clock() - startTime) / CLOCKS_PER_SEC) << " "

#if !(defined NDEBUG) && !(defined _MSC_VER)
#define _LOG_EXTRA(f) << "{" << std::setw(19) << f << "} "
#else
#define _LOG_EXTRA(f)
#endif

#define _LOG_DEBUG std::cerr << severMsg[0] _LOG_CLOCK _LOG_EXTRA(__func__)

#define LOG std::clog << severMsg[1] _LOG_CLOCK _LOG_EXTRA(__func__)

#define LOG_WARNING std::cerr << severMsg[2] _LOG_CLOCK _LOG_EXTRA(__func__)

#define LOG_ERROR std::cerr << severMsg[3] _LOG_CLOCK _LOG_EXTRA(__func__)

#define LOG_FATAL std::cerr << severMsg[4] _LOG_CLOCK _LOG_EXTRA(__func__)

#define LOG_PACKET(sockId, data, size, msg)                                    \
    do {                                                                       \
        std::clog << "[PACKET] " _LOG_CLOCK;                                   \
        logHelper(sockId, data, size, msg);                                    \
    } while (0)

class PerrorLogger
{
  public:
    explicit PerrorLogger(const char* func)
    {
        if (func)
            std::cerr << severMsg[3] _LOG_CLOCK _LOG_EXTRA(func);
        else
            std::cerr << severMsg[3] _LOG_CLOCK;
    }

    void operator<<(const char* msg) { std::perror(msg); }
};

#if !(defined NDEBUG) && !(defined _MSC_VER)
#define PLOG PerrorLogger(__func__)
#else
#define PLOG PerrorLogger(nullptr)
#endif

/*!
 * Use this for logging only in debug mode.
 */
#ifndef NDEBUG
#define DBG(x) _LOG_DEBUG << x << std::endl
#else
#define DBG(x)
#endif

#endif // LOGGER_H
