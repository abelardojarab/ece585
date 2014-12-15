/******************************************************************************
 * Authors: Sean Hendrickson, khalid alkhulayfi,
 *          Abelardo Jara-Berrocal, Meng Lei
 * File: main.cpp
 * Last Modified: 12/15/2014
 * Description: 
 ******************************************************************************/

#include "L3Cache.h"
#include "L3Set.h"
#include "L3Line.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <bitset>

using namespace std;

// arg1 = trace file
// arg2 = lines per set
// arg3 = number of sets
// arg4 = size of line in bytes
int main(int argc, char* argv[])
{
  // NOTE: numSets = 8MB/(linesPerSet * BytesPerLine)
  int numLines = 16;
  int numSets = 8192;
  int lineSize = 64;

  if ((argc < 2) || (argc > 5) || (argc == 3)) {
    cerr << "Usage: " << argv[0] << " maininputFile" << endl;
    return -1;
  }

  if (argc == 5)
    {
      lineSize = atoi(argv[2]);
      numLines = atoi(argv[3]);
      numSets = atoi(argv[4]);
    }

  L3Cache* mycache;
  mycache = new L3Cache(lineSize, numLines, numSets);

  string line;
  ifstream myfile (argv[1]);
  int opcode;
  unsigned int address;
  std::stringstream ss;
  string hexaddress;

  if (myfile.is_open())
    {
      while ( getline (myfile,line) )
        {
          if (line.compare(0,1,"") != 0) {

            istringstream(line) >> opcode >> hexaddress;
            ss.clear(); // reset stream state
            ss.str("");
            ss << std::hex << hexaddress;
            ss >> address;
            bitset<32> b(static_cast<int>(address));

            // Make cache receive the opcode and address
            mycache->processOpcode(opcode,b.to_string());

            // cache command
          }
        }
      myfile.close();
    }

  else cout << "Unable to open file";

  delete mycache;
  return 0;
}
