/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   logger.h
 * Author: angelo
 *
 * Created on 7 marzo 2019, 14.59
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <chrono>
#include <string>
#include <syslog.h>

class logger {
private :
    static logger * instance ;
public :
    logger(const char *name );
    ~logger();
};


class duration{
    std::chrono::steady_clock::time_point t_start;
    std::string function;
public:
    duration(const char *fname);
    ~duration();
};


#define LOGSYSLOG 1

#if LOGSYSLOG

#define LogE(...)   syslog(LOG_ERR,__VA_ARGS__)
#define LogW(...)   syslog(LOG_WARNING,__VA_ARGS__)
#define LogN(...)   syslog(LOG_NOTICE,__VA_ARGS__)
#define LogI(...)   syslog(LOG_INFO,__VA_ARGS__)
#define LogD(...)   syslog(LOG_DEBUG,__VA_ARGS__)

#define LogF()     duration __duration__(__func__)

#else

#define LogE(...) 
#define LogW(...) 
#define LogN(...) 
#define LogI(...) 
#define LogD(...)
#define LogF()

#endif

#endif /* LOGGER_H */

