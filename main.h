//
// Created by 25967 on 5/19/2022.
//
#include "linked_list.h"
#include "memopool.h"
#include "reader.h"
#include "event.h"
#ifndef MINER_MAIN_H
#define MINER_MAIN_H

#endif //MINER_MAIN_H
bool Block_decode(ArrayList*, ArrayList*);
bool Mine_decode(ArrayList*,int*,int*,unsigned int*, int);
ArrayList* aging_decode(ArrayList*);