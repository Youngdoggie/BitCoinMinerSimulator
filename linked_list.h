#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "array_list.h"

#ifndef LINKEDLIST_LIBRARY_H
#define LINKEDLIST_LIBRARY_H

typedef struct Transaction{
    int TID;
    char* Payer;
    char* Payee;
    int Amount;
    int Fee;
}Transaction;

typedef struct Event{
    Transaction* transaction;
    ArrayList* transactionList;
    int threadNumber;
    char* Name;
    unsigned int Nonce;
    unsigned int PreviousSig;
    int ID;
}Event;

typedef struct Node{
    Event* event;
    struct Node* next;
    struct Node* prev;
}Node;

typedef struct LList{
    Node* first;
    Node* last;
    int size;
    int itemSize;
    char* type;
}LinkedList;

LinkedList* llist_initialize(int, char*);
bool llist_add_first(LinkedList*, void*);
bool llist_add_last(LinkedList*, void*);
void* llist_get(LinkedList*, int);
int llist_index_of(LinkedList*, void*);
void* llist_remove(LinkedList*, int);
void* llist_remove_first(LinkedList*);
void* llist_remove_last(LinkedList*);
bool llist_destroy(LinkedList*);
#endif //LINKEDLIST_LIBRARY_H
