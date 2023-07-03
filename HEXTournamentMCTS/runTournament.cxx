#include "TApplication.h"
#include "HexBoard.h"
#include "HexTournament.h"
#include "SimpleHexEngine.h"
#include "MarvinHexEngine.h"
#include <string>
using std::string;
// insert include statements for your engine

//#include "YOURHexEngine.h"


/*!
  Example code for a simple HEX implementation
  created by Eckhard von Toerne, 
             Physikalisches Institut, University of Bonn, 2017
*/
int main(int argc, char **argv)
{
   TApplication theApp("App", &argc, argv);
   int iloop =0; 
   if (argc<2) cout <<" no index number given, using index=0. Set drawboard=true to see the games"<<endl;
   else 
   iloop=atoi(argv[1]);
   string flag = "";
   if (argc>2) flag=argv[2];
   std::cout << "loop="<<iloop<<std::endl;
   bool drawBoard = true; // set false to suppress display of board
   if (flag != "") drawBoard=true;

//    switch(iloop){
// #include "tournament.h"
//       case 0: HexTournament<SimpleNAMESPACE::SimpleHexEngine,BulldozerNAMESPACE::BulldozerHexEngine> tour0;tour0(drawBoard);break;
//    }
   //HexTournament<SimpleNAMESPACE::SimpleHexEngine,SimpleNAMESPACE::SimpleHexEngine> tour0;
   HexTournament<MarvinNAMESPACE::MarvinHexEngine,MarvinNAMESPACE::MarvinHexEngine> tour0;
   tour0(drawBoard);

   return 0;
}


