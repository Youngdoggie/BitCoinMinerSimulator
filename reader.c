//
// Created by 25967 on 7/13/2022.
//

#include "reader.h"
#include "event.h"
#define itemSize 300
#define types "Transaction"
#define inputSize 100
pthread_t thread;
extern LinkedList* globalList;

void startReader(){
    pthread_create(&thread, NULL, &runner2,NULL);
}

void stopReader(){
    pthread_join(thread,NULL);
}

void *runner2(void *arg){
    //variable created to keep in track previousID and previousSig
    int PreviousID = 0, PreviousSig = 0, TransactionsCount = 0, Nonce = 0;
    char command[5];
    char buffer[inputSize];
    while (true){
        fgets(buffer, inputSize, stdin);
        sscanf(buffer, "%s", command);
        if (strcmp(command, "END") == 0) {
            printf("Received event END\n");
            exit(0);
        }else if(strcmp(command,"TRX") == 0){
            int TID = 0, Amount = 0, Fee = 0;
            char command2[5];
            char Payer[50];
            char Payee[50];
            sscanf(buffer, "%s %d %s %s %d %d", command2, &TID, Payer, Payee, &Amount, &Fee);
            Transaction* transaction = Transaction_decode(TID,Payer,Payee, Amount, Fee);
            Event* event = eventInit(command2);
            event->transaction = transaction;
            event->Name = command2;
            printf("Received event %s with ID %d\n",command2,TID);
            addEvent(event);
        }else if(strcmp(command,"BLK") == 0){
            int ID = 0;
            sscanf(buffer, "%s %d %d %i %d", command, &ID, &PreviousID, &PreviousSig, &TransactionsCount);
            ArrayList* allTransactions = alist_initialize(10,itemSize,types);
            for(int i = 0; i < TransactionsCount; i++){
                int TID, Amount, Fee;
                char Payer[25];
                char Payee[25];
                scanf("%d %s %s %d %d", &TID, Payer, Payee, &Amount, &Fee);
                Transaction* input = Transaction_decode(TID,Payer,Payee,Amount,Fee);
                alist_add(allTransactions,input);
            }
            scanf("%i %i", &Nonce, &PreviousSig);
            Event* event = eventInit(command);
            event->transactionList = allTransactions;
            event->PreviousSig = PreviousSig;
            event->Nonce = Nonce;
            printf("Received event BLK with ID %d\n",ID);
            addEvent(event);
        } else if(strcmp(command,"MINE") == 0){
            int threadNumber;
            char command2[5];
            sscanf(buffer,"%s %d", command2, &threadNumber);
            Event* event = eventInit(command2);
            event->threadNumber = threadNumber;
            printf("Received event MINE \n");
            addEvent(event);
        }
        memset(command,0,5);
    }
    return NULL;
}

/*
 * Function : TransactionDecode
 * ----------------------------
 * decodes the user input information and store them into a type of Transaction
 *
 *   return:A type of Transaction includes transaction information
 */
Transaction* Transaction_decode(int TID, char* Payer, char* Payee, int Amount, int Fee){
    Transaction* transaction = malloc(sizeof(Transaction));
    transaction->TID = TID;
    transaction->Payer = malloc(strlen(Payer)+1);
    strcpy(transaction->Payer,Payer);
    transaction->Payee = malloc(strlen(Payee)+1);
    strcpy(transaction->Payee,Payee);
    transaction->Amount = Amount;
    transaction->Fee = Fee;
    return transaction;
}