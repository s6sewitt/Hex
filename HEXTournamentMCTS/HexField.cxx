#ifndef HEXFIELD_CPP
#define HEXFIELD_CPP
#include "HexField.h"
#include "IHexEngine.h"
#include "SimpleHexEngine.h"
#include "HexBoard.h"
#include "math.h"

HexField::HexField(HexLocation* u, HexBoard *c) :
   fBoard(c), fX(u->x), fY(u->y)
{
   fId = 100+fX + HexBoard::fMaxSize * fY;
   const double twopi=2.*acos(-1.);
   const double offs = 0.02;
   const double offsy = 0.05;
   const double wid = 0.5;
   const double wid2 = 3.1;
   /*const double offs = 0.3;
   const double wid = 0.5;
   const double wid2 = 5.;*/
   double baseX=offs+wid/c->GetSize();
   double baseY=offs+wid/c->GetSize()+offsy;
   double lx=1./c->GetSize()/wid2;
   double ly=lx*1.1;
   double rx=lx/cos(twopi/12.)*0.94;
   double ry=ly/cos(twopi/12.)*0.96;
   
   for (int i=0;i<7;i++){
      double ang = twopi/4.+twopi/6.0*i;
      x[i] = baseX + fY*lx + 2.*fX*lx + rx * cos(ang);
      y[i] = baseY + 2.*fY*ly + ry * sin(ang);
   }
   
   fNotTaken = true;
   
   //-------  append box to pad
   SetBit(kMustCleanup);
   SetBit(kCanDelete);
   SetBit(kNoContextMenu);
   SetPolyLine(7,x,y);
   
   SetUniqueID(fId);

   SetFillColor(1);
   SetLineWidth(2);
   Draw("f");
   c->GetListOfPrimitives()->Add(this);
}

void HexField::ExecuteEvent(int a, int b, int c)  
{
   //cout << "HexField  ExecuteEvent a="<<a<< " b="<<b<<" c="<<c<< endl;
   if (a !=1) return;
   if (fNotTaken) {
      //cout << "reporting move x,y=" << fX << " " << fY<< endl;
      fBoard->ReceiveUserMove(this);
   }
   return; 
} 

#endif
