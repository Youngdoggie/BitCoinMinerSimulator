//
// Created by 25967 on 7/13/2022.
//
#include "nonce.h"
#include "siggen.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "array_list.h"
#include <string.h>
#include "event.h"
#ifndef MINER_READER_H
#define MINER_READER_H

void startReader();
void stopReader();
void* runner2(void*);
Transaction* Transaction_decode(int, char*, char*, int, int);
#endif //MINER_READER_H
