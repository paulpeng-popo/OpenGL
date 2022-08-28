#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <iostream>
#include <string>

#define ERROR(x, ...) error(x, ##__VA_ARGS__)
#define DEBUG(x, ...) \
    if (DEBUG_MODE)   \
    debug(x, ##__VA_ARGS__)
#define RAW_DEBUG(x, ...) \
    if (DEBUG_MODE)       \
    raw_debug(x, ##__VA_ARGS__)

void error(std::string error_format, ...);
void debug(std::string format, ...);
void raw_debug(std::string format, ...);
