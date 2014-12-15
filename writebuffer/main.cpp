//
//  main.cpp
//  WriteBuffer
//
//  Created by Meng Lei on 12/15/14.
//  Copyright (c) 2014 Meng Lei. All rights reserved.
//

#include <iostream>
#include "writebuffer.h"


    // Constructor
    buffer::buffer()
    {
        head = tail = -1;
    }
    
    // Destructor
    buffer::~buffer()
    {
    
    }
    
    // Remove everything from queue
    void buffer::ClearQueue()
    {
        head = tail = -1; // Reset indices to start over
    }
    
    //--------------------------------------------
    // Function: Enqueue()
    // Purpose: Enqueue an item into the queue.
    // Returns: true if enqueue was successful
    //		or false if the enqueue failed.
    // Note: We let head and tail continuing
    //		increasing and use [head % MAX_SIZE]
    //		and [tail % MAX_SIZE] to get the real
    //		indices.  This automatically handles
    //		wrap-around when the end of the array
    //		is reached.
    //--------------------------------------------
    bool buffer::Enqueue(char ch)
    {
        // Check to see if the Queue is full
        if(isFull()) return false;
        
        // Increment tail index
        tail++;
        // Add the item to the Queue
        theQueue[tail % MAX_SIZE] = ch;
        return true;
    }
    
    //--------------------------------------------
    // Function: Dequeue()
    // Purpose: Dequeue an item from the Queue.
    // Returns: true if dequeue was successful
    //		or false if the dequeue failed.
    //--------------------------------------------
    char buffer::Dequeue()
    {
        char ch;
        
        // Check for empty Queue
        if(isEmpty()) return '\0';  // Return null character if queue is empty
        else
        {
            head++;
            ch = theQueue[head % MAX_SIZE];		// Get character to return
            return ch;				// Return popped character
        }
    }
    
    //--------------------------------------------
    // Function: isEmpty()
    // Purpose: Return true if the queue is empty
    // Returns: true if empty, otherwise false
    //--------------------------------------------
    bool buffer::isEmpty()
    {
        return (head == tail);
    }
    
    //--------------------------------------------
    // Function: isFull()
    // Purpose: Return true if the queue is full
    // Returns: true if full, otherwise false
    //--------------------------------------------
    bool buffer::isFull()
    {
        // Queue is full if tail has wrapped around
        //	to location of the head.  See note in
        //	Enqueue() function.
        return ((tail - MAX_SIZE) == head);
    }


int main()
{
    char testString[MAX_SIZE + 2];
    int i;
    char ch;
    buffer *Q;
    
    cout << "Queue implemented as an Array - Queue data type is character.\n\n";
    cout << "Creating a queue\n";
    
    Q = new buffer();
    cout << "Queue created...\n";
    
    // Test enqueuing and dequing item on a queue
    cout << "Testing enqueue and dequeue of single item.\n";
    Q->Enqueue('A');
    cout << "Enqueued: " << Q->Dequeue() << "\n";
    cout << "Enqueued and dequeued.\n\n";
    
    // Test queue by enqueing letters in a string
    strcpy(testString, "abcdefghijklmnopqrasuvwxyz");
    
    // Try to Enqueue all letters in the string
    i = 0;
    cout << "Testing enqueuing of string: " << testString << "\n";
    while(testString[i] != '\0')
    {
        if(!Q->Enqueue(testString[i]))
        {
            cout << "Queue is full. Unable to enqueue " << testString[i] << "\n";
        }
        i++;
    }
    
    cout << "\n\nDone testing enqueue.\n\nNow testing dequeue.\n";
    // Dequeue letters and print them
    cout << "Dequeued letters are: ";
    while((ch = Q->Dequeue()) != '\0') // Dequeue returns null terminator
        cout << ch;            // when queue is empty
    
    cout << "\nEnd of queue encountered\n";
    
    cout << "\n\nAll done\n";
    return 0;
}
