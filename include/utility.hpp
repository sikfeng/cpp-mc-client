#ifndef __UTILITY_HPP__
#define __UTILITY_HPP__

#include <algorithm>
#include <string>
#include <chrono>
#include <vector>

#include <GL/glew.h>

using UUID = std::vector<unsigned char>;

template<class T>
void endswap(T *objp) {
    unsigned char *memp = reinterpret_cast<unsigned char *>(objp);
    std::reverse(memp, memp + sizeof(T));
}

template<class T>
void endswap(T &obj) {
    unsigned char *raw = reinterpret_cast<unsigned char *>(&obj);
    std::reverse(raw, raw + sizeof(T));
}

template<typename T>
T _min(T a, T b) {
    return (a > b) ? b : a;
}

template<typename T, typename... Args>
T _min(T a, T b, Args... args) {
    return _min(a > b ? b : a, args...);
}

template<typename T>
T _max(T a, T b) {
    return (a > b) ? a : b;
}

template<typename T, typename... Args>
T _max(T a, T b, Args... args) {
    return _max(a > b ? a : b, args...);
}

inline void endswap(unsigned char *arr, size_t arrLen) {
    std::reverse(arr, arr + arrLen);
}

#endif
