#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <ctime>
#include <iomanip>

/**
 * @brief Super simple log library.
 */

static std::clock_t startTime = std::clock();

// This will show CPU time! It is different from the actual time.
#define _LOG_CLOCK  << std::setw(4) << (float(std::clock() - startTime) / CLOCKS_PER_SEC) << " "

#ifndef NDEBUG
#define _LOG_EXTRA(f)  << "{" << std::setw(19) << f << "} "
#else
#define _LOG_EXTRA(f)
#endif

#define LOG         std::clog << "[INFO]  "  _LOG_CLOCK _LOG_EXTRA(__func__)

#define LOG_WARNING std::cerr << "[WARN]  "  _LOG_CLOCK _LOG_EXTRA(__func__)

#define LOG_ERROR   std::cerr << "[ERROR] "  _LOG_CLOCK _LOG_EXTRA(__func__)

#define LOG_FATAL   std::cerr << "[FATAL] "  _LOG_CLOCK _LOG_EXTRA(__func__)

#define _LOG_DEBUG  std::cerr << "[DEBUG] "  _LOG_CLOCK _LOG_EXTRA(__func__)

class PerrorLogger {
 public:
    PerrorLogger(const char *func) {
        if (func)
            std::cerr << "[ERROR] " _LOG_CLOCK _LOG_EXTRA(func);
        else
            std::cerr << "[ERROR] " _LOG_CLOCK;
    }

    void operator<<(const char *msg) {
        std::perror(msg);
    }
};

#ifndef NDEBUG
#define PLOG        PerrorLogger(__func__)
#else
#define PLOG        PerrorLogger(nullptr)
#endif

/**
 * Use this for logging only in debug mode.
 */
#ifndef NDEBUG
  #define DBG( x ) _LOG_DEBUG << x << std::endl
#else
  #define DBG( x )
#endif

#endif // LOGGER_H
