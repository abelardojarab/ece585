/******************************************************************************
 * Authors: Sean Hendrickson, khalid alkhulayfi,
 *          Abelardo Jara-Berrocal, Meng Lei
 * File: main.cpp
 * Last Modified:
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

int main(int argc, char* argv[])
{
  int numLines = 256;
  int numSets = 16;

  if ((argc < 2) || (argc > 4) || (argc == 3)) {
    cerr << "Usage: " << argv[0] << " maininputFile" << endl;
    return -1;
  }

  if (argc == 4)
    {
      numLines = atoi(argv[2]);
      numSets = atoi(argv[3]);
    }

  L3Cache* mycache;
  mycache = new L3Cache(numLines, numSets);

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
            cout<< "Read: opcode = "<<opcode<<", hexaddress = "<< hexaddress << ", address = "<<b.to_string()<<endl;

            // cache command
          }
        }
      myfile.close();
    }

  else cout << "Unable to open file";

  delete mycache;
  return 0;
}
