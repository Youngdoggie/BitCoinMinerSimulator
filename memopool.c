//
// Created by 25967 on 6/19/2022.
//
#include "memopool.h"

ArrayList* memopool_initialize(){
    ArrayList* memopool = alist_initialize(10,sizeof(ArrayList),"memopool");
    ArrayList* Priority0 = alist_initialize(10, sizeof(Transaction), "0");
    ArrayList* Priority1 = alist_initialize(10, sizeof(Transaction), "1");
    ArrayList* Priority2 = alist_initialize(10, sizeof(Transaction), "2");
    ArrayList* Priority3 = alist_initialize(10, sizeof(Transaction), "3");
    ArrayList* Priority4 = alist_initialize(10, sizeof(Transaction), "4");
    ArrayList* Priority5 = alist_initialize(10, sizeof(Transaction), "5");
    ArrayList* Priority6 = alist_initialize(10, sizeof(Transaction), "6");
    ArrayList* Priority7 = alist_initialize(10, sizeof(Transaction), "7");
    ArrayList* Priority8 = alist_initialize(10, sizeof(Transaction), "8");
    ArrayList* Priority9 = alist_initialize(10, sizeof(Transaction), "9");
    alist_add(memopool,Priority0);
    alist_add(memopool,Priority1);
    alist_add(memopool,Priority2);
    alist_add(memopool,Priority3);
    alist_add(memopool,Priority4);
    alist_add(memopool,Priority5);
    alist_add(memopool,Priority6);
    alist_add(memopool,Priority7);
    alist_add(memopool,Priority8);
    alist_add(memopool,Priority9);
    return memopool;
}

bool memopool_add(ArrayList* memopool,Transaction* transaction){
    if(memopool == NULL){
        return false;
    }

    int fee = transaction->Fee;
    int size = 12 + strlen(transaction->Payer) + 1  + strlen(transaction->Payee) + 1;
    int priority = fee / size;
    if(priority > 9){
        priority = 9;
    }

    alist_add(memopool->arr[priority],transaction);

    return true;
}