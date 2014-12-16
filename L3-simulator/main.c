#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#define numLines 16483
#define numWays 16

#define M 0
#define E 1
#define S 2
#define I 3
#define F 4

typedef struct
{
  int MESIbits;
  int LRUbits;
  int tag;
  int address;
} cacheLine;

// initialize a multi-dimensional array [row][col]
// creates the single global cache
cacheLine L3cache[numLines][numWays];

FILE *ifp,*ofp;

// step through the cache and set initial values
// LRU bit will be the same value as the way
// MESI bit begins invalid (empty)
// Tag bits set to null because access decisions based on MESI

void setLRUbitsToWay()
{
  int index;
  int way;
  for (index = 0; index <= numLines-1; index++)
    {
      for (way = 0; way <= numWays-1; way++)
        {
          L3cache[index][way].LRUbits = way;
          L3cache[index][way].MESIbits = 3;
          L3cache[index][way].tag = 0;
        }
    }
}

// check every way in the given index to see if the given tag exists
// tag in each way of the appropriate index
int checkTag(int index, int tag)
{
  int way;
  for (way = 0; way <= numWays-1; way++)
    {
      if (L3cache[index][way].tag == tag)
        return way;
    }
  return numWays;
}

// given an index, this function returns the least recently used way
// LRU bits of 0 give the least recently used way
// LRU bits of numWays-1 give most recently used way
int checkLRU(int index)
{
  int way;
  for (way = 0; way <= numWays-1; way++)
    {
      if (L3cache[index][way].LRUbits == 0)
        return way;
    }
  return numWays;
}

// this function updates the LRU bits to reflect a new most recently used way
void updateLRU(int index, int ourway)
{
  // if the LRU bits of our way are already the most recently used, we do nothing
  if (L3cache[index][ourway].LRUbits == numWays-1)
    return;
  else
    {
      int ourbits = L3cache[index][ourway].LRUbits;
      int testbits = ourbits++;
      int testway;
      for (testbits = 0; testbits <= numWays-1; testbits++)
        {
          testway = 0;
          while (testway <= numWays-1)
            {
              if (testbits == L3cache[index][testway].LRUbits)
                {
                  L3cache[index][testway].LRUbits--;
                  break;
                }
              else
                testway++;
            }
        }
      L3cache[index][ourway].LRUbits = numWays-1;
    }
}

// This function takes in an index and tests all numWays within that index,
// returning a 0 if it finds any valid way and a 1 if it does not.

int testIndex(int index, int way)
{
  for (way = 0; way <= numWays-1; way++)
    {
      if (L3cache[index][way].MESIbits != I)
        {
          return 0;
        }
    }
  return 1;
}

// The cache displays all indices containing at least one way with a
// valid MESI bit

void cacheDisplay()
{
  int index;
  int way;
  ofp = fopen("results.out", "a");

  for (index = 0; index <= numLines-1; index++)
    {
      if (testIndex(index,way) == 0)
        {

          fprintf(ofp,"Index number: 0x%-8x\n",index);
          fflush(ofp);

          for (way = 0; way <= numWays-1; way++)
            {
              fprintf(ofp,"Way ---> %-8d LRU ---> %-4d MESI bits ----> %-10d Tag ----> %-8d"
                      " Address: 0x%-8x\n",
                      way,
                      L3cache[index][way].LRUbits,
                      L3cache[index][way].MESIbits,
                      L3cache[index][way].tag,
                      L3cache[index][way].address);
              fflush(ofp);
            }
        }
    }

  fflush(ofp);
}

int main()
{
  memset(L3cache, 0, sizeof (cacheLine));

  // initialize the counters
  int refCount = 0;
  int readCount = 0;
  int writeCount = 0;
  int hitCount = 0;
  int missCount = 0;
  int hitM = 0;
  int hit = 0;
  int way;

  // initialize the input from the tracefile
  int n;
  int addr;

  // initialize the LRU bits to the way
  setLRUbitsToWay();

  // open the tracefile, make it available to 'r' read
  // open the output file to make it available to append each iteration's result
  ifp = fopen("testfile.din", "r");
  ofp = fopen("results.out", "w");

  // set it up to read line by line, set n and addr accordingly
  while (fscanf(ifp, "%d %x\n", &n, &addr) != EOF)
    {
      // parse 32-bit hex address
      int tag = addr >> 20;
      int index = (addr >> 6) & 16383;
      // get 1 bit from the fifth position to check later whether 1 or 0
      // int byteSelect = (addr >> 5) & ~(~0 << 1);
      refCount++;

      switch (n)
        {
          // n = 0 read data request from L1 cache
          // n = 2 instruction fetch (treated as a read request from L1 cache)
        case 0:
        case 2:
          readCount++;
          way = checkTag(index, tag);
          // if the tag exists
          if (way <= numWays-1)
            {
              int MESI = L3cache[index][way].MESIbits;
              // if this tag exists and it's valid as per its MESI bits
              if (MESI == M || MESI == E || MESI == S)
                {
                  hitCount++;
                  updateLRU(index, way);
                  // MESI remains unchanged
                }
              // if this tag exists but it's been invalidated and can't be used...
              // we fetch from DRAM and pass on to L1 cache, update to exclusive
              else
                {
                  missCount++;
                  updateLRU(index, way);
                  L3cache[index][way].MESIbits = E;
                }
            }
          // this tag simply doesn't exist in the cache in any form
          else
            {
              missCount++;
              // use the LRU bits to determine which way to evict
              way = checkLRU(index);
              updateLRU(index, way);
              L3cache[index][way].tag = tag;
              L3cache[index][way].MESIbits = E;
            }
          L3cache[index][way].address = addr;
          break;
          // 1 write data request from L1 cache
        case 1:
          writeCount++;
          way = checkTag(index, tag);
          if (way <= numWays-1)
            {
              int MESI = L3cache[index][way].MESIbits;
              // if this tag exists and it's valid per its MESI bits...
              if (MESI == M || MESI == E || MESI == S)
                hitCount++;
              // if this tag exists MESI bits say invalid, needs to be set M
              else
                missCount++;
            }
          // if this tag simply doesn't exist in the cache in any form...
          // this covers the very unlikely odd case where L1 has what L3 doesn't
          else
            {
              missCount++;
              way = checkLRU(index);
              L3cache[index][way].tag = tag;
            }
          updateLRU(index, way);
          L3cache[index][way].MESIbits = M;
          L3cache[index][way].address = addr;
          break;
          // 4 snooped a read request from another processor
        case 4:
          readCount++;
          way = checkTag(index, tag);
          // if the tag exists
          if (way <= numWays-1)
            {
              int MESI = L3cache[index][way].MESIbits;
              // if this tag exists and is valid and modified per its MESI bits...
              if (MESI == M || MESI == E || MESI == S)
                {
                  hitCount++;
                  // if modified, send copy to other cache, then L1, then to DRAM
                  if (MESI == M)
                    hitM++;
                  else
                    hit++;
                  // then set MESI to shared
                  L3cache[index][way].MESIbits = S;
                  L3cache[index][way].address = addr;
                }
              // if the tag exists but it's invalid then we don't have it...
              else
                missCount++;
            }
          break;
          // 3 snooped invalidate command from another processor
          // 5 snooped write request from another processor
        case 3:
        case 5:
          if (n == 5)
            writeCount++;
          way = checkTag(index, tag);
          // if the tag exists...
          if (way <= numWays-1)
            {
              int MESI = L3cache[index][way].MESIbits;
              if (MESI == M || MESI == E || MESI == S)
                {
                  hitCount++;
                  L3cache[index][way].MESIbits = I;
                  L3cache[index][way].address = addr;
                  updateLRU(index, way);
                }
              else
                missCount++;
            }
          else
            missCount++;
          break;
          // 6 snooped read for ownership request
        case 6:
          readCount++;
          way = checkTag(index, tag);
          // if the tag exists
          if (way <= numWays-1)
            {
              int MESI = L3cache[index][way].MESIbits;
              // serve if modified tag exists, then invalidate
              if (MESI == M || MESI == E || MESI == S)
                {
                  hitCount++;
                  if (MESI == M)
                    hitM++;
                  L3cache[index][way].MESIbits = I;
                  updateLRU(index, way);
                  L3cache[index][way].address = addr;
                }
              else // if we don't have it, do nothing
                missCount++;
            }
          break;
          // 8 clear the cache entirely
        case 8:
          fprintf(ofp,"Reference %d called for the cache to be reset. No numWays are valid.\n",
                  refCount);
          fflush(ofp);
          setLRUbitsToWay();
          break;
          // 9 print the cache but change/destroy nothing
        case 9:
          fprintf(ofp,"Reference %d displayed only indices containing valid numWays.\n",refCount);
          fflush(ofp);
          cacheDisplay();
          break;
        } // end switch statement
    } // end while loop

  float hitRatio = (float) hitCount / refCount;

  printf(" Results:\n\n"
         "Total memory accesses: %d\n-- Number of reads: %d\n-- Number of writes: %d\n-- Hits: %d\n"
         "-- Misses %d\n-- Hit ratio: %f\n",
         refCount,readCount,writeCount,hitCount,missCount,hitRatio);
  fflush(ofp);

  return 0;

} // end main()
