#include "TMath.h"
#include <set>
#include <map>
#include <fstream>
#include <cassert>
#include "HexPosition.h"
   // constructor
HexPosition::HexPosition(int isize, bool hasbarrier) : 
   fHasBarrier(hasbarrier), fBarrier1(), fBarrier2(), fSize(isize) 
{
   assert(fSize<=fMaxSize);
   for (int ix=0;ix<fSize;ix++) {
      for (int iy=0;iy<fSize;iy++) {
         // in the beginning all fields are empty (owner = none)         
         fTyp[ix][iy]=none;
      }
   }
   fNFree=fSize*fSize;
   fFirstMover=none;
}

bool HexPosition::IsValidLoc(int ix, int iy) const {
   return (ix>=0) && (ix <fSize) && (iy>=0) && (iy <fSize);
}

FieldType HexPosition::Typ(const HexLocation& m) const {
   if (!IsValidLoc(m.x,m.y)) return invalid;   
   return fTyp[m.x][m.y];
}

FieldType HexPosition::Typ(int ix, int iy) const
{
   if (!IsValidLoc(ix,iy)) return invalid;   
   return fTyp[ix][iy];
}

// copy Constructor
HexPosition::HexPosition(const HexPosition& h) 
{
   fSize = h.fSize;
   for (int ix=0;ix<fSize;ix++) {
      for (int iy=0;iy<fSize;iy++) {
         fTyp[ix][iy]=h.fTyp[ix][iy];
      }
   }
   fHasBarrier=h.fHasBarrier;
   fBarrier1=h.fBarrier1;
   fBarrier2=h.fBarrier2;   
   fNFree=h.fNFree;
   fFirstMover=h.fFirstMover;
}

// assignment operator
HexPosition& HexPosition::operator=(const HexPosition& h)
{
   fSize = h.fSize;
   for (int ix=0;ix<fSize;ix++) {
      for (int iy=0;iy<fSize;iy++) {
         fTyp[ix][iy]=h.fTyp[ix][iy];
      }
   }
   fHasBarrier=h.fHasBarrier;
   fBarrier1=h.fBarrier1;
   fBarrier2=h.fBarrier2;
   fNFree=h.fNFree;
   fFirstMover=h.fFirstMover;
   return *this;
}

void HexPosition::Reset()
{ // info barrier is not reset
   for (int ix=0;ix<fSize;ix++) {
      for (int iy=0;iy<fSize;iy++) {
         fTyp[ix][iy]=none;
      }
   }   
   fNFree=fSize*fSize; 
   fFirstMover=none;
}

bool HexPosition::AddMove(const HexLocation& loc, FieldType f)
{
   if (fTyp[loc.x][loc.y] != none) return false;
   fTyp[loc.x][loc.y]=f;
   if (f != none) fNFree--;
   if (fFirstMover==none) fFirstMover=f;
   return true;
}

void HexPosition::Write(TString filename) const
{
   // open infile
   ofstream infile;
   infile.open(filename);
   infile <<"this is a Hexposition created by Hexposition::Write()"<<endl;
   infile <<"Size "<<fSize<<endl;
   infile << "x y fieldtype"<<endl;
   for (int ix=0;ix<fSize;ix++) {
      for (int iy=0;iy<fSize;iy++) {
	 if (fTyp[ix][iy] != none) {
	    infile <<ix<<" "<<iy<<" "<<fTyp[ix][iy]<< endl;
	 }
      }
   }
   infile.close();    
}

HexLocSet HexPosition::GetNeighbors(const HexLocation& ss, FieldType t) const { 
   // return the up-to six neighbors around a location of type t
   HexLocSet neigh;
   int x0=Range(ss.x-1), x1=Range(ss.x+1), y0=Range(ss.y-1), y1=Range(ss.y+1);
   for (int ix=x0; ix<=x1;ix++)
      for (int iy=y0; iy<=y1;iy++){
         if (ix==ss.x && iy==ss.y) continue; // do not use central loc.
         if (ix==ss.x-1 && iy==ss.y-1) continue; 
         if (ix==ss.x+1 && iy==ss.y+1) continue;
         if (fTyp[ix][iy]==t) neigh.insert(HexLocation(ix,iy));
      }   
   if (fHasBarrier){//barrier fields are never each others neighbors
      if (ss == fBarrier1)  neigh.erase(fBarrier2); 
      if (ss == fBarrier2)  neigh.erase(fBarrier1);
   }
   return neigh; 
}

void HexPosition::AddNeighbors(const HexLocation& ss, FieldType t, HexLocSet& hset) const { 
   HexLocSet neigh=GetNeighbors(ss,t);
   hset.insert(neigh.begin(),neigh.end());
}

int HexPosition::GetNearestDistance(const HexLocation& ss, FieldType t) const {
   // distance to nearest field of fieldtype t 
   HexLocation nearest;
   int minDistance=100000;
   for (int ix=0;ix<fSize;ix++) {
      for (int iy=0;iy<fSize;iy++) {
         if (fTyp[ix][iy]!=t) continue;
         int d = GetDistance(HexLocation(ix,iy),ss);
         if (d>0 && d<minDistance){
            minDistance=d;
            nearest=HexLocation(ix,iy);
         }
      }
   }
   return minDistance; 
}

int HexPosition::GetDistance(const HexLocation& s1, const HexLocation& s2) const { 
   // determine points are midstream or crossstream
   int diffX = TMath::Abs(s1.x-s2.x);
   int diffY = TMath::Abs(s1.y-s2.y);
   if ((s1.x-s2.x)*(s1.y-s2.y)>=0)       // midstream
      return diffX + diffY;
   else if (diffX > diffY) return diffX; // cross-stream1
   else return diffY;                    // cross-stream2
}

bool HexPosition::CheckForWin(FieldType dir) const 
{
   if (dir == none) cout << "Error CheckForWin, wrong FieldType none"<<endl;
   HexLocSet s; // define a set of Location pointers
   // Add Locations at 0 index, if of correct type
   for (int i=0;i<fSize;i++){
      if (dir==horizontal && fTyp[0][i]==dir) s.insert(HexLocation(0,i));
      if (dir==vertical   && fTyp[i][0]==dir) s.insert(HexLocation(i,0));
   }
   // start while loop in which we add neighbors
   while (true) {
      int oldSize = s.size();
      HexLocSet neighbors;
      // add neigbors
      for (HexLocSet::iterator iter=s.begin(); iter != s.end(); ++iter){
         AddNeighbors(*iter,dir,neighbors);
      }
      s.insert(neighbors.begin(),neighbors.end());
      // end if set no longer increases 
      if (oldSize==s.size()) break;
   }
   // if highest index reached, return success
   int newSize=s.size();
   for (HexLocSet::iterator iter=s.begin(); iter != s.end(); ++iter){      
      if (dir==horizontal && iter->x==fSize-1) return true;
      if (dir==vertical && iter->y==fSize-1) return true;
   }
   
   //Additional rule for barrier hex: In the unlikely case that all fields have been occupied without a winner, the first mover loses the game.
   if (fNFree==0) return (fFirstMover!=dir);
   
   return false;
}

void HexPosition::SetBarrier(HexLocation f1, HexLocation f2)
{
   assert(GetDistance(f1,f2)==1);
   fBarrier1=f1;
   fBarrier2=f2;
   fHasBarrier=true;
}
bool HexPosition::GetBarrier(HexLocation& f1, HexLocation& f2)
{
   f1=fBarrier1;
   f2=fBarrier2;
   return HasBarrier();
}
