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
#define _LOG_CLOCK  << "(" << std::setw(4) << (float(std::clock() - startTime) / CLOCKS_PER_SEC) << ") "

#ifndef NDEBUG
#define _LOG_EXTRA  << "{" << std::setw(19) << __func__ << "} "
#else
#define _LOG_EXTRA
#endif

#define LOG         std::clog << "[INFO] "    _LOG_CLOCK _LOG_EXTRA

#define LOG_WARNING std::cerr << "[WARNING] " _LOG_CLOCK _LOG_EXTRA

#define LOG_ERROR   std::cerr << "[ERROR] "   _LOG_CLOCK _LOG_EXTRA

#define LOG_FATAL   std::cerr << "[FATAL] "   _LOG_CLOCK _LOG_EXTRA

/**
 * Use this for logging only in debug mode.
 */
#ifndef NDEBUG
  #define DOUT( x )  x
#else
  #define DOUT( x )
#endif

#endif // LOGGER_H
