//
// Created by wangzhehao on 2/7/24.
//

#include "util.h"
#include <cstdio>
#include <cstdlib>

void errif(bool cond, const char *msg) {
    if (cond) {
        perror(msg);
        exit(EXIT_FAILURE);
    }
}
