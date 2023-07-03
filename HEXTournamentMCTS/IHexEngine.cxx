#include "IHexEngine.h"
#include <cassert>

IHexEngine::IHexEngine(int isize, bool horizontalMovesFirst, FieldType player) 
   :   HexPosition(isize)
{ 
   fLastMove.x = -999;
   fLastMove.y = -999;
   fGameWinner = none;
   fNMoves=0;
   fSize=isize;
   fHorizontalMovesFirst=horizontalMovesFirst;
   if (fHorizontalMovesFirst) fNextMover = horizontal;
   else fNextMover = vertical;
   SetPlayer(player);
   for (int ix=0;ix<fSize;ix++) {
      for (int iy=0;iy<fSize;iy++) {
         fTyp[ix][iy]=none;
      }
   }
  
}

void IHexEngine::Reset(){
   fGameWinner = none;
   if (fHorizontalMovesFirst) fNextMover = horizontal;
   else fNextMover = vertical;
   fNMoves=0;    
   for (int ix=0;ix<fSize;ix++) {
      for (int iy=0;iy<fSize;iy++) {
	 fTyp[ix][iy]=none;
      }
   }    
}

bool IHexEngine::IsValidMove(const HexLocation& pos) const {
   if (fTyp[pos.x][pos.y]!=none){
      cout <<"error: IsValidMove: field " << pos.x << " " << pos.y << " already occupied by "<< fTyp[pos.x][pos.y]<<endl;
      return false;
   }
   if (IsWon()){
      cout <<"error: IsValidMove: game is already over"<<endl;
      return false;
   }
   return true;
}
   
bool IHexEngine::IsWon() const {
   if (fGameWinner == none) return false;
   return true;
}
   
void IHexEngine::FinishMove(){
   fNMoves++;
   if (fNextMover==horizontal) fNextMover=vertical; 
   else fNextMover=horizontal;
   if (CheckForWin(horizontal)) fGameWinner = horizontal;
   if (CheckForWin(vertical))   fGameWinner = vertical;
}

void IHexEngine::SetMove(HexLocation& pos){
   if (!IsValidMove(pos)){
      cout << " received invalid move "<<pos.x <<" " << pos.y << endl;
      return;
   }
   fLastMove.x = pos.x;
   fLastMove.y = pos.y;
   fTyp[pos.x][pos.y]=fNextMover;
   //cout << "new move x="<<pos.x <<" y="<< pos.y << "val="<< (int) fTyp[pos.x][pos.y]<<endl;
   FinishMove();
}

bool IHexEngine::isValidLoc(int ix, int iy) const {
   return HexPosition::IsValidLoc(ix,iy);
   //ToDo obsolete --> remove;
}

void IHexEngine::Report() const {
   for (int ix=0;ix<fSize;ix++) {
      for (int iy=0;iy<fSize;iy++) {
         if (fTyp[ix][iy] == none) continue;
         HexLocSet s;
         AddNeighbors(HexLocation(ix,iy), fTyp[ix][iy], s);
         bool hasNeighbors = (s.size()>0);
         cout << " f("<<ix<<","<<iy<<")" << (fTyp[ix][iy]==vertical? "v":"h")<< (hasNeighbors ? " with neighbor(s)" : " " ) << endl; 
      }
   }    
}


