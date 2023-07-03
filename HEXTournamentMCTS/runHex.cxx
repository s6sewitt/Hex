#include "TApplication.h"
#include "HexBoard.h"
#include "HexTournament.h"
#include "SimpleHexEngine.h"
#include "MarvinHexEngine.h"
// insert include statements for your engine


/*!
  Example code for a simple HEX implementation
  created by Eckhard von Toerne, 
             Physikalisches Institut, University of Bonn, 2017
*/
int main(int argc, char **argv)
{
   TApplication theApp("App", &argc, argv); 
   // please note that TApplication reset argc

   int boardsize=10;
   bool doBarrier=false;

   cout <<"starting"<<(doBarrier? " barrier game" : " game") << endl; 

   HexBoard* c1 = new HexBoard(
       boardsize,
       //new SimpleNAMESPACE::SimpleHexEngine(10,true,vertical),
       new MarvinNAMESPACE::MarvinHexEngine(10,true,vertical),
       doBarrier
   );    
   theApp.Run();
   return 0;
}


