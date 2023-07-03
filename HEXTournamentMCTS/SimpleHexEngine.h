#ifndef SIMPLEHEXENGINE_H
#define SIMPLEHEXENGINE_H

#include "IHexEngine.h"

namespace SimpleNAMESPACE{

class SimpleHexEngine : public IHexEngine{
   /*!
     Example code for a simple Engine playing the HEX game
     created by Eckhard von Toerne, University of Bonn, 2009
    */

private:
   // random number generator
   TRandom3 fR;
   
public:

   SimpleHexEngine(int isize, bool horizontalMovesFirst, FieldType player) :
      IHexEngine(isize, horizontalMovesFirst, player), fR(0)
      {  }
  
   ~SimpleHexEngine() { }

   TString GetName(){return TString("SimpleHexEngine");}

   HexLocation GetNextMove();

   double EvalLocation(int ix, int iy);

};

} // end namespace

#endif
