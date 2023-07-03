#ifndef HEXPOSITION_H
#define HEXPOSITION_H
#include <cstdlib>
#include <iostream>
#include "TMath.h"
#include <set>
#include <map>
using namespace std;
#include "HexField.h"

typedef enum{none,horizontal,vertical,invalid} FieldType;

// class hexCompLoc{
// public:
//    bool operator() (const HexLocation* a, const HexLocation* b){ 
//       if (a->x != b->x) return (a->x < b->x);
//       return (a->y < b->y);
//    }
// };

typedef set<HexLocation> HexLocSet;

/*!
  the Position object of a HexGame
*/   
class HexPosition
{

protected:
   int       fSize;
   static const int fMaxSize=15; //!< hard-coided max field size
   FieldType   fTyp[fMaxSize][fMaxSize]; //!< fixed-size array to hold field types

protected:
   bool fHasBarrier;
   HexLocation fBarrier1,fBarrier2;
   FieldType fFirstMover;
   int fNFree;
public:   
   // constructor
   HexPosition(int isize, bool hasbarrier=false);
   // copy Constructor
   HexPosition(const HexPosition& h); 
   FieldType Typ(const HexLocation& m) const;
   FieldType Typ(int ix, int iy) const;
   bool IsValidLoc(int ix, int iy) const;

   //assignment operator
   HexPosition& operator=(const HexPosition& h);  
   int Range(int i) const { return min(max(0,i),fSize-1); }
   void Reset(); //!< resets field types

   bool AddMove(const HexLocation& loc, FieldType f); // returns false if move impossible
   void Write(TString filename) const;//!<write out position to a file
   HexPosition& Position() { return *this;}
   HexLocSet GetNeighbors(const HexLocation& ss, FieldType t) const;
   void AddNeighbors(const HexLocation& ss, FieldType t, HexLocSet& hset) const;
   int GetNearestDistance(const HexLocation& ss, FieldType t) const; 
   int GetDistance(const HexLocation& s1, const HexLocation& s2) const;
   bool CheckForWin(FieldType dir) const;
   FieldType IsWonPosition(){ 
      if (CheckForWin(horizontal)) return horizontal;
      if (CheckForWin(vertical)) return vertical;
      return none;
   }
   void SetBarrier(HexLocation f1, HexLocation f2);
   bool GetBarrier(HexLocation& f1, HexLocation& f2);
   bool HasBarrier(){return fHasBarrier;}
   int NFree(){return fNFree;}
   FieldType FirstMover(){return fFirstMover;}
};

#endif
