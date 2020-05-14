#ifndef UTIL_H
#define UTIL_H

#define LOG_MSG(...)                            \
    do {                                        \
        fprintf(stderr, __VA_ARGS__);           \
        fflush(stderr);                         \
    } while (0)
#define LOG_WARN(...)                           \
    do {                                        \
        fprintf(stderr, "[WARN] " __VA_ARGS__); \
        fflush(stderr);                         \
    } while(0)
#define LOG_ERR(...)                                \
    do {                                            \
        fprintf(stderr, "[ERROR] "  __VA_ARGS__);   \
        exit(EXIT_FAILURE);                         \
    } while (0)

#ifdef DEBUG
#define LOG_DEBUG(...)                          \
    do {                                        \
        fprintf(stderr, "[DEBUG]" __VA_ARGS__); \
        fflush(stderr);                         \
    } while (0)
#else
#define LOG_DEBUG(...)
#endif

#endif
