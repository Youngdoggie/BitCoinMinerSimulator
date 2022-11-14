/*
 * Author : Yang Lu
 * Date : 10/20/2021
 * Purpose: This is a array_list implementation from Yang Lu as a submission to
 * Lab4 for class CSCI2122 Modified in 5/29/2022 added new transaction struct in use of
 * program miner
 */
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "array_list.h"

ArrayList* alist_initialize(int i, int j, char* c){
    ArrayList* arrayList = malloc(sizeof(ArrayList));
    arrayList -> arr = malloc(200);
    arrayList -> size = 0;
    arrayList -> maxSize = i;
    arrayList -> itemSize = j;
    arrayList -> type = c;
    return arrayList;
}

bool alist_add(ArrayList* alist, void* element){
    // If the incoming array list is null, we can return
    // false here. The element is checked in "add_at".
    if(alist == NULL)
        return false;

    // Attempt to add the element to the end of the list.
    return alist_add_at(alist, alist->size, element);
}

bool alist_add_at(ArrayList* alist, int index, void* element){
    if(alist == NULL || element == NULL)
        return false;

    if(index < 0 || index > alist->size)
        return false;

    if(alist->size == alist->maxSize)
    {
        bool success = alist_resize(alist);
        if(!success)
            return false;
    }

    for(int i=alist->size; i > index; i--)
        alist->arr[i] = alist->arr[i-1];

    alist->arr[index] = malloc(alist->itemSize);

    memcpy(alist->arr[index], element, alist->itemSize);

    alist->size++;

    return true;
}

void alist_clear(ArrayList* list){
    int i = 0;
    for(i=0; i<list->size; i++){
        free(list->arr[i]);
    }
    list->size = 0;
}

void* alist_get(ArrayList* list, int index){
    if(list == NULL){
        return false;
    }

    if(index > list->size){
        return false;
    }

    return list->arr[index];
}

int alist_index_of(ArrayList* list, void* v){
    int i;

    if(v == NULL || list == NULL){
        return -1;
    }

    for(i=0; i<list->size;i++){
        if(memcmp(list->arr[i],v,list->itemSize) == 0){
            return i;
        }
    }
    return -1;
}

void* alist_remove(ArrayList* alist, int index) {
    if (alist == NULL) {
        return NULL;
    }

    if (index > alist->size || index < 0) {
        return NULL;
    }

    void *element = alist->arr[index];

    for (int i = index; i < alist->size - 1; i++){
        alist->arr[i] = alist->arr[i + 1];
    }

    alist->size--;
    return element;

}

bool alist_destroy(ArrayList* list){
    int i;
    for (i = 0; i < list->size; ++i) {
        free(list->arr[i]);
    }
    free(list->arr);
    free(list);
    return true;
}

bool alist_resize(ArrayList* list){
    if(list == NULL){
        return false;
    }

    if(list->size < list->maxSize){
        return false;
    }

    list->arr = realloc(list->arr,list->itemSize * list->maxSize*2);
    list->maxSize = list->maxSize*2;
    return true;
}