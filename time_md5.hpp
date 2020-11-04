#pragma once

#include <iostream>
#include <sys/time.h>
#include <openssl/md5.h>

class TimeUtil {
public:
    static int64_t TimeStamp() {
        struct timeval tv;
        ::gettimeofday(&tv, NULL);
        return tv.tv_sec;

    }

    static int64_t TimeStampMS() {
        struct timeval tv;
        ::gettimeofday(&tv, NULL);
        return tv.tv_sec * 1000 + tv.tv_usec / 1000;

    }

    static std::string FormatTime() {
        time_t time_stamp = TimeStamp();
        struct tm t;
        localtime_r(&time_stamp, &t);
        char buf[100] = {0};
        strftime(buf, sizeof(buf), "%Y/%m/%d %H:%M:%S", &t);
        return std::string(buf);

    }
};

std::string StringMD5(const std::string& str) {
    const int MD5LENTH = 16;
    unsigned char MD5result[MD5LENTH];
    // 调用 openssl 的函数计算 md5
    MD5((const unsigned char*)str.c_str(),str.size(),MD5result);
    // 转换成字符串的形式方便存储和观察
    char output[1024] = {0};
    int offset = 0;
    for (int i = 0; i < MD5LENTH; ++i) {
        offset += sprintf(output + offset, "%x", MD5result[i]);
    }
    return std::string(output);
} 
