//
//  writebuffer.h
//  WriteBuffer
//
//  Created by Meng Lei on 12/15/14.
//  Copyright (c) 2014 Meng Lei. All rights reserved.
//

#ifndef WriteBuffer_writebuffer_h
#define WriteBuffer_writebuffer_h

using namespace std;

#define MAX_SIZE    25        // Define maximum length of the queue

class buffer
{
private:
    int head, tail;           // Index to top of the queue
    char theQueue[MAX_SIZE];  // The queue
    
public:
    buffer();         // Class constructor
    ~buffer();        // Class destuctor
    void ClearQueue();       // Remove all items from the queue
    bool Enqueue(char ch);   // Enter an item in the queue
    char Dequeue();          // Remove an item from the queue
    bool isEmpty();          // Return true if queue is empty
    bool isFull();           // Return true if queue is full
};

#endif
