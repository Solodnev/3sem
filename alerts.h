#ifndef ALERTS_H
#define ALERTS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "colors.h"

// Messages
#define OK    BOLD GREEN  "  [OK]  "    NORM
#define ERR   BOLD RED    "  [ERROR]  " NORM
#define EXIT  BOLD YELLOW "  [EXIT]  "  NORM


#define __BUFSIZ  1024
char    __buf[1024];

#define ERROR(...)\
    {\
        int p = sprintf ( __buf, YELLOW "PID %d:" ERR, getpid ( ) );\
        int q = sprintf ( __buf+p,  __VA_ARGS__ );\
        *( __buf + p + q )     = '\n';\
        *( __buf + p + q + 1 ) = '\0';\
        fputs  ( __buf, stderr );\
        exit   ( EXIT_FAILURE );\
    }

#ifdef DEBUG
#define PRINT(...)\
    {\
        int p = sprintf ( __buf, YELLOW "PID %d:  " NORM, getpid ( ) );\
        int q = sprintf ( __buf+p,  __VA_ARGS__ );\
        *( __buf + p + q )     = '\n';\
        *( __buf + p + q + 1 ) = '\0';\
        fputs  ( __buf, stderr );\
    }
#else  // DEBUG
#define PRINT(...)
#endif // DEGUG

#endif // ALERTS_H