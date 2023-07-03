#ifndef HEXENGINE_H
#define HEXENGINE_H
#include "HexField.h"
#include "HexPosition.h"
#include <set>
#include "TRandom3.h"
#include "TMath.h"
#include <iostream>
using std::set;


/*
  the Base Game Engine, used as an interface in tournaments
*/
class IHexEngine : public HexPosition {
      
 public:
   bool fHorizontalMovesFirst; // true if horizontal players has first move
   int fSize; 
   int fNMoves; // counter of moves
   FieldType fPlayer; // which side is this engine playing
   FieldType fOpponent; // which side is my opponent
   FieldType fGameWinner;
   FieldType fNextMover;
   HexLocation fLastMove;
   
   // constructor
   IHexEngine(int isize, bool horizontalMovesFirst, FieldType player=vertical);
   
   virtual ~IHexEngine(){}

   virtual HexLocation GetNextMove()=0; //!< this is the brain of the engine, to be implemented
   virtual TString GetName()=0; //!< name of your engine

   void Reset();

   void SetPlayer(FieldType player){ fPlayer = player; if (fPlayer==vertical) fOpponent=horizontal; else fOpponent=vertical;}

   bool IsValidMove(const HexLocation& pos) const;
   
   int GetNMoves() const {return fNMoves;}

   bool IsWon() const;
   
   void FinishMove();

   virtual void SetMove(HexLocation& pos);

   //void GetNeighbors(const HexLocation& ss, FieldType t, HexLocSet& neigh) const; 
   //int GetNearestDistance(const HexLocation& ss, FieldType t) const; 

   //int GetDistance(const HexLocation& s1, const HexLocation& s2) const;

   bool isValidLoc(int ix, int iy) const; //ToDo check overlap with HexPosition::isvalidloc
   void Report() const;
   bool IsBarrierHex(){return HasBarrier();} 

};

#endif
