/*
 * Author : Yang Lu
 * Date : 7/4/2022
 * Purpose: This is the nonce class that creates number of threads which is  required and print out each step then
 * return calculated nonce and signature
 * */

#include "nonce.h"
#include "siggen.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "array_list.h"
#include <unistd.h>
// shared:
pthread_mutex_t mutex;

/*
 * Function : get_nonce
 * ----------------------
 * This is the function for creating threads based on the number and get returned nonce and signature
 * then store it into an arraylist and sort them from smallest to the largest and return the
 * smallest nonce with the signature calculated
 *
 * input: partial Signature and threadNumber required to create
 * return : returning a struct of returned_value which consist of a nonce and calculated signature
 */
returned_value* get_nonce(unsigned int partialSig, int threadNumber){
    pthread_t thread[threadNumber];
    pthread_mutex_init(&mutex, NULL);
    //creating thread based on number given
    for (int i = 0; i < threadNumber; i++) {
        arg_struct* args = malloc(sizeof(*args));
        args->signature = partialSig;
        args->index = malloc(sizeof(int));
        *args->index = i;
        args->threadNumber = threadNumber;
        if(pthread_create(thread+i, NULL, &runner,(void*)args) != 0){
            //gives a head up if thread is not created correctly
            perror("failed creating thread");
        }
    }

    returned_value* value = malloc(sizeof(*value));
    //creating a list in order to sort the calculated nonce and find smallest
    ArrayList* list = alist_initialize(threadNumber, sizeof(returned_value), "returned_value");
    //joining each thread
    for(int i = 0; i < threadNumber; i++){
        if(pthread_join(thread[i], (void**)&value) != 0){
            //gives a head up if thread is not joined correctly
            perror("failed joining thread");
        }
        if(list->size == 0) {
            alist_add(list, value);
        }else{
            returned_value* temp = alist_get(list,0);
            if(temp->nonce > value->nonce){
                alist_add_at(list,0,value);
            }else{
                alist_add(list,value);
            }
        }
    }

    pthread_mutex_destroy(&mutex);
    return alist_get(list,0);
}

/*
 * Function : runner
 * ----------------------
 * This is the runner function for thread to run and calculate the nonce
 *
 * input: index of each thread and partial calculated signature
 * return : returning a struct of returned_value which consist of a nonce and calculated signature
 */
void* runner(void* arguments){
    pthread_mutex_lock(&mutex);
    //decoding
    arg_struct* passedArgs = arguments;
    int index = *(int*) passedArgs->index;
    unsigned int partialSignature = passedArgs->signature;
    unsigned nonce = index;
    unsigned int signature = siggen_int(partialSignature,nonce);
    //calculating signature and print out which thread doing which nonce
    if(signature < 0x00ffffff && nonce!=0){
        printf("Thread %d checking nonce 0x%8.8x\n", index, nonce);
    }
    while (true){
        if(signature > 0x00ffffff){
            signature = siggen_int(partialSignature, nonce);
            printf("Thread %d checking nonce 0x%8.8x\n", index, nonce);
            nonce = nonce + passedArgs->threadNumber;
        }else{
            break;
        }
    }
    returned_value* value = malloc(sizeof(*value));
    value->signature = signature;
    value->nonce = nonce;
    pthread_mutex_unlock(&mutex);
    free(passedArgs->index);
    free(passedArgs);
    return value;
}