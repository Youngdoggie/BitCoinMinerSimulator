//
// Created by 25967 on 7/13/2022.
//
#include "linked_list.h"
#include "pthread.h"

#ifndef MINER_EVENT_H
#define MINER_EVENT_H
Event* eventInit(char*);
void addEvent(Event*);
Event* removeEvent();
#endif //MINER_EVENT_H
