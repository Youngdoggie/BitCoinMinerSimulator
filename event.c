//
// Created by 25967 on 7/13/2022.
//

#include "event.h"
extern LinkedList* globalList;
extern pthread_mutex_t mutexEvent;
extern pthread_cond_t condEvent;

Event* eventInit(char* type){
    Event* event = malloc(sizeof(Event));
    event->transaction = malloc(sizeof(Transaction));
    event->Name = type;
    event->threadNumber = 0;
    event->PreviousSig = 0;
    event->Nonce = 0;
    event->ID = 0;
    return event;
}
void addEvent(Event* event){
    pthread_mutex_lock(&mutexEvent);
    llist_add_last(globalList,event);
    pthread_mutex_unlock(&mutexEvent);
    pthread_cond_signal(&condEvent);
}

Event* removeEvent(){
    pthread_mutex_lock(&mutexEvent);
    pthread_cond_wait(&condEvent,&mutexEvent);
    Event* temp = llist_remove_first(globalList);
    pthread_mutex_unlock(&mutexEvent);
    return temp;
}