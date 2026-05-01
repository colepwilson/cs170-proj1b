#include<stdio.h>
#include <iostream> //


#include "rwlock.h"

//Implement all methods of RWLock defined in rwlock.h
//Your solution for  Task 3 
//TODO

RWLock::RWLock() {
    lock = new Lock((char *)"rwlock lock");
    readers_ok = new Condition((char *)"readers ok");
    writers_ok = new Condition((char *)"writers ok");

    active_readers = 0;
    waiting_writers = 0;
    active_writer = false;
}

RWLock::~RWLock() {
    delete lock;
    delete readers_ok;
    delete writers_ok;
}

void RWLock::startRead() {
    lock->Acquire();

    while (active_writer || waiting_writers > 0) {
        readers_ok->Wait(lock);
    }

    active_readers++;

    lock->Release();
}

void RWLock::doneRead() {
    lock->Acquire();

    active_readers--;

    if (active_readers == 0 && waiting_writers > 0) {
        writers_ok->Signal(lock);
    }

    lock->Release();
}

void RWLock::startWrite() {
    lock->Acquire();

    waiting_writers++;

    while (active_writer || active_readers > 0) {
        writers_ok->Wait(lock);
    }

    waiting_writers--;
    active_writer = true;

    lock->Release();
}

void RWLock::doneWrite() {
    lock->Acquire();

    active_writer = false;

    if (waiting_writers > 0) {
        writers_ok->Signal(lock);
    } else {
        readers_ok->Broadcast(lock);
    }

    lock->Release();
}