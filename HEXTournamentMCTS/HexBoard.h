#ifndef HEXBOARD_H
#define HEXBOARD_H

#include <cstdlib>
#include "TLine.h"
#include "TStopwatch.h" 
#include "IHexEngine.h" 
#include "SimpleHexEngine.h"

/*
  the Board for the HEX game based on ROOTs XWindows functionality
*/
class HexBoard: public TCanvas {
   
   friend class HexField;
   
public:
   static const int fMaxSize = 100;
   
private:
   int       fSize; //!< board size
   bool      fSingleUser; //!< single user mode or engine vs. engine mode
   bool      fStartMatch; //!< single user mode or engine vs. engine mode
   bool      fDrawMe;     //!< draw the board or not
   IHexEngine* fBaseEngine; // base engine to check moves for validity
   IHexEngine* fEngine1;  //!< first engine
   IHexEngine* fEngine2;  //!< second engine
   TStopwatch* fTimer1;   //!< time counter for game engine 1 
   TStopwatch* fTimer2;   //!< time counter for game engine 2
   HexField*   field[fMaxSize][fMaxSize];
   TLine* fb1; // field delimiters
   TLine* fb2;
   TLine* fb3;
   TLine* fb4;
   TLine* fBarrierLine;
   TText * fText1;
   TText * fText2;
   TText * fText3;
   TString fGameStatus;

   int fColorA; // color of playerA
   int fColorB;
   bool fDebug;
   int fDelay;
   double fTimeLimitPerMove;
   bool fUseBarrier;
   HexLocation fBarrier1,fBarrier2;
   void MakeMove(int engineNumber, int& winner, double& cputime );

   bool IsValidMove(HexLocation& pos){return fBaseEngine->IsValidMove(pos);}
   
   bool IsWon(){return fBaseEngine->IsWon();}
   
   void ReportBadMove(int enginenumber);
   
   void ReceiveUserMove(HexField* field);
   void SetWinner(TString winner);

   bool Delay(double delayseconds){
      TStopwatch t;
      t.Start();
      bool val=true;
      while (t.RealTime()<delayseconds/15) {
         //cout << t.RealTime()<<" ";
         for (int i=0;i<1000;i++) val = !val;
         t.Stop();
         t.Continue();
      }
      return val;
   }
public:
   // user versus engine constructor
   HexBoard(int isize, IHexEngine* engine1=0, bool useBarrier=false);

   // engine versus engine constructor
   HexBoard(int isize, IHexEngine* engine1, IHexEngine* engine2, bool drawme=true, TString gamestatus="", bool useBarrier=false);

   ~HexBoard(); 

   void DrawBoard();
   void Reset(){ cout << "Reset: needs to be implemented"<<endl;} //ToDo

   int GetSize(){return fSize;}
   int GetColorA(){return fColorA;}
   int GetColorB(){return fColorB;}
   
   int DoMatchEngineVsEngine();

   void SetDelay(int del){fDelay=del;}
   void SetRandomBarrier();

};

#endif
