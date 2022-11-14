//
// Created by 25967 on 6/19/2022.
//
#include "linked_list.h"
#include "array_list.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#ifndef MINER_MEMOPOOL_H
#define MINER_MEMOPOOL_H

ArrayList* memopool_initialize();
bool memopool_add(ArrayList*,Transaction*);
#endif //MINER_MEMOPOOL_H
