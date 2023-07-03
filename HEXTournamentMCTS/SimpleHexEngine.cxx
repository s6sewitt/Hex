

#include "SimpleHexEngine.h"

using namespace SimpleNAMESPACE;

HexLocation SimpleHexEngine::GetNextMove()
{
   // for a demonstration of a time limit violation uncomment the next lines
   // double x=1.3;
   // for (long int jj=0;jj<1000000000;jj++){ x= x*x-1.2; if (x*x>10.) x=x*x-10.;}

   double maxGrade=-1.;
   for (int ix=0;ix<fSize;ix++) {
      for (int iy=0;iy<fSize;iy++) {
         double newGrade = EvalLocation(ix,iy);
         if (newGrade>maxGrade){
            fLastMove=HexLocation(ix,iy);
            maxGrade=newGrade;
         }
      }
   }
   fTyp[fLastMove.x][fLastMove.y]=fNextMover;
   FinishMove();
   return fLastMove;
}

double SimpleHexEngine::EvalLocation(int ix, int iy){
   // randomly chosen un-occupied field;
   if (fTyp[ix][iy] != none) return 0.;
   return 1.0 + fR.Rndm();
}  
