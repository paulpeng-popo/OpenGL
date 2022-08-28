#include "../myheaders/Error_Debug.h"

void error(std::string error_format, ...)
{
    va_list args;
    va_start(args, error_format);
    vfprintf(stderr, ("[ERROR] " + error_format + "\n").c_str(), args);
    va_end(args);
    exit(EXIT_FAILURE);
}

void debug(std::string format, ...)
{
    va_list args;
    va_start(args, format);
    vprintf(("[INFO] " + format + "\n").c_str(), args);
    va_end(args);
}

void raw_debug(std::string format, ...)
{
    va_list args;
    va_start(args, format);
    vprintf((format + "\n").c_str(), args);
    va_end(args);
}
