//
// Created by 25967 on 6/30/2022.
//

#ifndef MINER_NONCE_H
#define MINER_NONCE_H

typedef struct arg_struct{
    int* index;
    int threadNumber;
    unsigned int signature;
}arg_struct;

typedef struct returned_value{
    unsigned int nonce;
    unsigned int signature;
}returned_value;
returned_value* get_nonce(unsigned int, int);
void *runner();
#endif //MINER_NONCE_H
