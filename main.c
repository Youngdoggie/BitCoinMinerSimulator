/*
 * Author : Yang Lu
 * Date : 5/29/2022
 * Purpose: This is the main class for the program miner which takes user input in the main function
 * and decode user input by determining three different kind of action including Transaction(TRX)
 * Block(BLK) and Mining(MINE) then outputting the calculated nonce and signature
 * */
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "siggen.h"
#include "nonce.h"

#define itemSize 300
#define types "Transaction"
#define inputSize 100
LinkedList* globalList;
pthread_mutex_t mutexEvent;
pthread_cond_t condEvent;
/*
 * Function : Main
 * ---------------
 *   Main function decoding user input and then either store input information in mempool when decoding
 *   Transaction(TRX) or removing information from mempool when decoding Block(BLK) or Mine a block
 */
int main() {
    unsigned int  PreviousSig = 0;
    //variable created to keep in track previousID and previousSig
    int ID = 0, PreviousID = 0, TransactionsCount, Nonce;
    char command[5];
    char buffer[inputSize];
    startReader();
    //mempool storing all transactions
    ArrayList* memopool = memopool_initialize();
    globalList = llist_initialize(300, "event");
    pthread_mutex_init(&mutexEvent,NULL);
    pthread_cond_init(&condEvent,NULL);
    while (true){
        Event* currentEvent = removeEvent();
        if(strcmp(currentEvent->Name,"TRX") == 0){
            Transaction* transaction = currentEvent->transaction;
            printf("Adding transaction: %d %s %s %d %d\n", transaction->TID, transaction->Payer, transaction->Payee, transaction->Amount, transaction->Fee);
            memopool_add(memopool,transaction);
        }else if(strcmp(currentEvent->Name, "BLK")==0){
            Block_decode(memopool,currentEvent->transactionList);
        }else if(strcmp(currentEvent->Name,"MINE") == 0) {
            int threadNumber = currentEvent->threadNumber;
            Mine_decode(memopool,&ID,&PreviousID, &PreviousSig, 8);
        }
    }
    /*while (strcmp(command, "END") != 0){
        fgets(buffer, inputSize, stdin);
        sscanf(buffer, "%s", command);
        if(strcmp(command,"MINE") == 0){
            //Mine a block jump to mine event
            int threadNumber;
            sscanf(buffer,"%s %d", command ,&threadNumber);
            Mine_decode(memopool,&ID,&PreviousID, &PreviousSig,threadNumber);
        }else if(strcmp(command,"TRX") == 0){
            //Decode user input TRX store all transaction information into mempool
            int TID = 0, Amount = 0, Fee = 0;
            char Payer[50];
            char Payee[50];
            sscanf(buffer, "%s %d %s %s %d %d", command, &TID, Payer, Payee, &Amount, &Fee);
            Transaction* transaction = Transaction_decode(TID,Payer,Payee, Amount, Fee);
            printf("Adding transaction: %d %s %s %d %d\n", transaction->TID, transaction->Payer, transaction->Payee, transaction->Amount, transaction->Fee);
            memopool_add(memopool,transaction);
        }else if(strcmp(command,"BLK") == 0){
            //Decode user input BLK removing transaction information from mempool
            sscanf(buffer, "%s %d %d %i %d", command, &ID, &PreviousID, &PreviousSig, &TransactionsCount);
            ArrayList * allTransactions = alist_initialize(10,itemSize,types);
            for(int i = 0; i < TransactionsCount; i++){
                int TID, Amount, Fee;
                char Payer[25];
                char Payee[25];
                scanf("%d %s %s %d %d", &TID, Payer, Payee, &Amount, &Fee);
                Transaction* input = Transaction_decode(TID,Payer,Payee,Amount,Fee);
                alist_add_at(allTransactions,allTransactions->size,input);
            }
            scanf("%i %i", &Nonce, &PreviousSig);
            Block_decode(memopool,allTransactions);
        }else if(strcmp(command,"EPOCH") == 0){
            memopool = aging_decode(memopool);
        }
    }
     */
    stopReader();
    return 0;
}

ArrayList* aging_decode(ArrayList* memopool){
    if(memopool == NULL){
        return memopool;
    }

    for (int i = 8; i >= 0; i--) {
        ArrayList* list = memopool->arr[i];
        if(list->size > 0){
            Transaction* removed = alist_remove(memopool->arr[i], 0);
            alist_add(memopool->arr[i + 1],removed);
            ArrayList* addToList = alist_get(memopool,i+1);
            Transaction* transaction = alist_get(addToList,addToList->size-1);
            printf("Aging transaction (%s):%d %s %s %d %d\n",
            addToList->type,transaction->TID,transaction->Payer,transaction->Payee,transaction->Amount,transaction->Fee);
        }
    }
    return memopool;
}

/*
 * Function : BlockDecode
 * ----------------------
 * decode the user input information and removing transaction from mempool
 *
 * return : true or false indicate job done or not
 */
bool Block_decode(ArrayList* pool, ArrayList* input){
    if(pool == NULL || input == NULL){
        return false;
    }

    for (int k = 0; k < 10; k++) {
        ArrayList* priority_list = alist_get(pool,k);
        if(priority_list->size>0) {
            for (int i = 0; i < priority_list->size; i++) {
                for (int j = 0; j < input->size; j++) {
                    Transaction* inPool = alist_get(priority_list, i);
                    Transaction* inputTransaction = alist_get(input, j);
                    if (inPool->TID == inputTransaction->TID) {
                        Transaction* removed = alist_remove(pool->arr[k],i);
                        printf("Removing transaction: %d %s %s %d %d\n", removed->TID, removed->Payer, removed->Payee,removed->Amount, removed->Fee);
                    }
                }
            }
        }
    }
    return true;
}

/*
 * Function : MineDecode
 * ----------------------
 * decode the user input information and create a block then fill block with signature and calculated nonce
 * and transactions with calculated size less than 256
 *
 * return : true or false indicate job done or not
 */
bool Mine_decode(ArrayList* pool, int *ID, int *PreviousId, unsigned int *PreviousSig, int threadNumber){
    if(pool == NULL){
        return false;
    }

    //updated ID in special situation
    if(ID == 0 ){
        *ID = 1;
        *PreviousId = 0;
        *PreviousSig = 0;
    }else{
        *PreviousId = *ID;
        *ID += 1;
    }


    LinkedList* TransactionList = llist_initialize(itemSize,types);
    //calculated availableSpace 256 - 4 constant integer
    int availableSpace = 232;
    int transactionCount = 0;
    int transactionSpace = 0;
    //calculate availableSpace
    for (int i = 9; i >= 0; i--) {
        ArrayList *priorityList = alist_get(pool, i);
        if(priorityList->size > 0) {
            for (int j = 0; j < priorityList->size; j++) {
                Transaction *transaction = alist_get(priorityList, j);
                int stringLength = strlen(transaction->Payer) + 1 + strlen(transaction->Payee) + 1;
                transactionSpace = stringLength + 4 * 3;
                if (availableSpace - transactionSpace >= 0) {
                    llist_add_last(TransactionList, transaction);
                    transactionCount++;
                    availableSpace = availableSpace - transactionSpace;
                }
            }
        }
    }

    for (int k = 0; k < 10; k++) {
        ArrayList* priority_list = pool->arr[k];
        if(priority_list->size>0) {
            for (int i = 0; i < priority_list->size; i++) {
                for (int j = 0; j < TransactionList->size; j++) {
                    Transaction* inPool = priority_list->arr[i];
                    Transaction* inputTransaction = llist_get(TransactionList, j);
                    if(inPool->TID == inputTransaction->TID){
                        alist_remove(priority_list,i);
                    }
                }
            }
        }
    }
    printf("Block mined: %d %d 0x%8.8x %d\n", *ID,*PreviousId,*PreviousSig,transactionCount);

    //generate signature
    unsigned int sig = siggen_new();
    sig = siggen_int(sig,*ID);
    sig = siggen_int(sig, *PreviousId);
    sig = siggen_int(sig, *PreviousSig);
    sig = siggen_int(sig, transactionCount);

    for(int i = 0; i < transactionCount; i++){
        Transaction* removed = llist_get(TransactionList, i);
        printf("%d %s %s %d %d\n",removed->TID,removed->Payer,removed->Payee,removed->Amount,removed->Fee);
        sig = siggen_int(sig, removed->TID);
        sig = siggen_string(sig,removed->Payer);
        sig = siggen_string(sig,removed->Payee);
        sig = siggen_int(sig, removed->Amount);
        sig = siggen_int(sig, removed->Fee);
    }

    //calculate nonce
    unsigned int nonce = 0;
    returned_value* value = get_nonce(sig,threadNumber);
    *PreviousSig = value->signature;
    if(value->nonce > threadNumber) {
        printf("0x%8.8x 0x%8.8x\n", value->nonce - threadNumber, value->signature);
    }else{
        printf("0x%8.8x 0x%8.8x\n", value->nonce, value->signature);
    }
    return true;
}