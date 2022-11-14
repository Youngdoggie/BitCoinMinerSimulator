/*
 * Author : Yang Lu
 * Date : 10/20/2021
 * Purpose: This is a Linked_list implementation from Yang Lu as a submission to
 * Lab4 for class CSCI2122 Modified in 5/29/2022 added new transaction struct in use of
 * program miner
 */
#include "linked_list.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

LinkedList* llist_initialize(int i, char* c){
    LinkedList* list = malloc(sizeof(LinkedList));
    list->first = NULL;
    list->last = NULL;
    list->size = 0;
    list->itemSize = i;
    list->type  = c;
    return list;
}

bool llist_add_at(LinkedList* list, int index, void* v){
    if(v == NULL){
        return false;
    }

    if(index < 0 || index > list->size){
        return false;
    }

    Node* new_node = malloc(sizeof(Node));
    new_node->event = malloc(list->itemSize);
    memcpy(new_node->event,v,list->itemSize);
    new_node->prev = NULL;
    new_node->next = NULL;
    Node* temp = list->first;

    if(list->size == 0){
        list->first = new_node;
        list->last = new_node;
    }

    if(index == 0 && list->size > 0){
        new_node->next = temp;
        temp->prev = new_node;
        list->first = new_node;
    }

    if(index == list->size){
        list->last->next = new_node;
        new_node->prev = list->last;
        list->last = new_node;
    }

    if(index < list->size && index != 0) {
        int i;
        for (i = 0; i < list->size; i++) {
            if (i == index) {
                new_node->next = temp->next;
                new_node->prev = temp;
                temp->next = new_node;
                new_node->next->prev = new_node;
            } else {
                temp = temp->next;
            }
        }
    }
    list->size++;
    return true;
}

bool llist_add_first(LinkedList* list, void* v){
    if(list == NULL || v == NULL){
        return false;
    }
    llist_add_at(list,0,v);
    return true;
}

bool llist_add_last(LinkedList* list, void* v){
    if(list == NULL || v == NULL){
        return false;
    }
    llist_add_at(list,list->size,v);
    return true;
}

void* llist_get(LinkedList* list, int index){
    if(index > list->size+1 || index < 0){
        return NULL;
    }

    Node* temp = list->first;
    int i;
    for(i = 0; i < index; i++){
        temp = temp->next;
    }
    return temp->event;
}

int llist_index_of(LinkedList* list, void* v){
    if(v == NULL){
        return -1;
    }
    Node* temp = list->first;

    int i;
    for(i = 0; i < list->size; i++){
        if(memcmp(temp->event,v,list->itemSize) == 0){
            return i;
        }
        temp = temp->next;
    }
    return -1;
}

void* llist_remove(LinkedList* list, int index){
    if(list == NULL){
        return false;
    }

    Node* temp = list->first;
    int i;
    for (i = 0; i < index; ++i) {
        temp = temp->next;
    }
    void* removed = malloc(sizeof(void*)*list->itemSize);
    memcpy(removed,temp->event,list->itemSize);
    if(list->size == 1){
        list->first = NULL;
        list->last  = NULL;
    }
    else if(index == 0){
        list->first = temp->next;
        list->first->prev = NULL;
    }
    else if(index == list->size-1){
        list->last = temp->prev;
        list->last->next = NULL;
    }else {
        temp->prev->next = temp->next;
        temp->next->prev = temp->prev;
    }
    free(temp->event);
    free(temp);
    list->size--;
    return removed;
}

void* llist_remove_first(LinkedList* list){
    if(list == NULL){
        return false;
    }
    return llist_remove(list, 0);
}

void* llist_remove_last(LinkedList* list){
    if(list == NULL){
        return false;
    }
    
    void* removed = malloc(sizeof(void*)*list->itemSize);
    memcpy(removed,list->last->event,list->itemSize);

    Node* temp = list->last;
    temp->prev->next = NULL;
    list->last = temp->prev;
    free(temp->event);
    free(temp);

    list->size--;
    return removed;
}
bool llist_destroy(LinkedList* list){
    if(list == NULL){
        return false;
    }

    Node* current = list->first;
    Node* next;
    for (int i = 0; i < list->size; ++i) {
        next = current->next;
        free(current->event);
        free(current);
        current = next;
    }
    free(list);
    return true;
}