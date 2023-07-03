#ifndef HEXFIELD_H
#define HEXFIELD_H

#include <cstdlib>
#include <TCanvas.h>
#include <TGFrame.h>
#include <TRandom.h>
#include <TButton.h>
#include <TPolyLine.h>
#include <TText.h>
#include <TSystem.h>
#include <iostream>
using namespace std;
#include "HexLocation.h"

class HexBoard; // forward declaration of HexBoard 

//------------------------------------------------------------
class HexField : public TPolyLine {
   /* graphical representation of a game field in the shape of a hexagon
      derived from an X object 
      created by Eckhard von Toerne, University of Bonn, 2017
   */
   friend class HexBoard;

private:
   int       fX;      // X position(column)  in the fPad
   int       fY;      // Y position(line)  in the fPad
   int       fId;     // unique ID
   bool      fNotTaken;
   HexBoard* fBoard; // pointer to Canvas where box is in
   double    x[7];    // x position of hexagon corners
   double    y[7];    // y position of hexagon corners

public:
   HexField(HexLocation* u, HexBoard *c);
   
   virtual ~HexField() { }
   
   int GetX() const {return fX;}
   int GetY() const {return fY;}
  
   void SetOccupied(int col){
      fNotTaken = false;
      SetFillColor(col);
      Draw("f");
   }
  
   void ExecuteEvent(int a, int b, int c); //forward declaration 

};

#endif
