#include "TLine.h"
#include "TStopwatch.h" 
#include "TImage.h" 
#include "TPad.h" 
#include "IHexEngine.h" 
#include "SimpleHexEngine.h"
#include "HexBoard.h"
#include <algorithm>

using namespace SimpleNAMESPACE;

/*! constructor for user against computer games, user starts by clicking on a fild, user has to create a continuous, horizontal path
*/
HexBoard::HexBoard(int isize, IHexEngine* en1, bool useBarrier): TCanvas("hex","Hexboard",10,10,900,700){
   fTimeLimitPerMove=1.0;
   fSingleUser = true;
   fStartMatch = false;
   fDrawMe     = true;
   fDelay      = 100000;
   fUseBarrier= useBarrier;
   // the Board
   fColorA=2;
   fColorB=4;
   fSize=isize;      
   bool horizontalMovesFirst=true;   
   // timers for total time, not used yet
   fTimer1  = new TStopwatch();
   fTimer1->Stop();
   fTimer2  = new TStopwatch();
   fTimer2->Stop();
   // engines
   fBaseEngine = new SimpleHexEngine(fSize,horizontalMovesFirst, none);
   if (en1==0) fEngine1    = new SimpleHexEngine(fSize,horizontalMovesFirst, vertical);
   else fEngine1=en1;
   fEngine2    = 0;
   fDebug      = true;
   if (fUseBarrier) SetRandomBarrier();
   if (fDrawMe) DrawBoard();

}  

 // engine versus engine constructor
HexBoard::HexBoard(int isize, IHexEngine* engine1, IHexEngine* engine2, bool drawme, TString gamestatus, bool useBarrier) : TCanvas("hex","Hexboard",2,2,1000,650){
   fTimeLimitPerMove=1.0;
   fSingleUser = false;
   fStartMatch=false;
   // the Board
   fColorA=2;
   fColorB=4;
   fSize=isize;      
   fDrawMe = drawme;
   fDelay      = 200000;
   fGameStatus = gamestatus;
   fUseBarrier = useBarrier;

   bool horizontalMovesFirst=true;
   
   // timers for total time, not used yet
   fTimer1  = new TStopwatch();
   fTimer1->Stop();
   fTimer2  = new TStopwatch();
   fTimer2->Stop();
   // game engines
   fBaseEngine = new SimpleHexEngine(fSize,horizontalMovesFirst, none);
   fEngine1    = engine1;
   fEngine2    = engine2;
   
   fDebug      = false;
   if (fUseBarrier) SetRandomBarrier();
   if (fDrawMe) DrawBoard();   
}  

HexBoard::~HexBoard() 
{    
   if (fDrawMe){
      for (int ix=0;ix<fSize;ix++) 
         for (int iy=0;iy<fSize;iy++) delete field[ix][iy];
      delete fb1;
      delete fb2;
      delete fb3;
      delete fb4;
      delete fText1;
      delete fText2;
      delete fText3;
   }
   delete fTimer1;
   delete fTimer2;
   delete fBaseEngine;

}


void HexBoard::DrawBoard(){
   SetBit(kNoContextMenu);
   for (int ix=0;ix<fSize;ix++) {
      for (int iy=0;iy<fSize;iy++) {
         HexLocation a(ix,iy);
         field[ix][iy]=new HexField(&a,this);
      }
   }
   
   fb1 = new TLine(0.01,0.1,0.3,0.75); 
   fb1->SetLineColor(fColorA);
   fb1->SetLineWidth(10);
   fb1->Draw();
   fb2 = new TLine(0.7,0.08,0.99,0.72); 
   fb2->SetLineColor(fColorA);
   fb2->SetLineWidth(10);
   fb2->Draw();
   fb3 = new TLine(0.03,0.06,0.68,0.06); 
   fb3->SetLineColor(fColorB);
   fb3->SetLineWidth(10);
   fb3->Draw();
   fb4 = new TLine(0.36,0.815,0.97,0.815); 
   fb4->SetLineColor(fColorB);
   fb4->SetLineWidth(10);
   fb4->Draw();
   if (fUseBarrier) {
      HexField* f1=field[fBarrier1.x][fBarrier1.y];
      HexField* f2=field[fBarrier2.x][fBarrier2.y];
      // match corners
      float dist,maxdist=1.e6,xx1,yy1,xx2,yy2;
      for (int i1=0;i1<6;i1++){
         for (int i2=0;i2<6;i2++){
            //cout <<" f1="<<f1->x[i1]<< " "<<f1->y[i1]<<" f2="<<f2->x[i2]<< " "<<f2->y[i2]<<endl;
            dist = fabs(f1->x[i1]-f2->x[i2])+fabs(f1->y[i1]-f2->y[i2]);
            if (dist<maxdist){
               maxdist = dist;
               xx1=0.5*(f1->x[i1]+f2->x[i2]);yy1=0.5*(f1->y[i1]+f2->y[i2]);
            }
         }
      }
      maxdist=1.e6;
      for (int i1=0;i1<6;i1++){
         for (int i2=0;i2<6;i2++){
            dist = fabs(f1->x[i1]-f2->x[i2])+fabs(f1->y[i1]-f2->y[i2]);
            if (fabs(f1->x[i1]-xx1)+fabs(f1->y[i1]-yy1)<0.025) continue;
            if (dist<maxdist){
               maxdist = dist;
               xx2=0.5*(f1->x[i1]+f2->x[i2]);yy2=0.5*(f1->y[i1]+f2->y[i2]); 
            }
         }
      }      
      fBarrierLine =  new TLine(xx1,yy1,xx2,yy2);
      fBarrierLine->SetLineColor(kViolet);
      fBarrierLine->SetLineWidth(8);
      fBarrierLine->Draw();
   }
   fText1=new TText(0.05, 0.94, fEngine1->GetName().Data());
   fText2=new TText(0.05, 0.87, fEngine2? fEngine2->GetName().Data() : " ");
   fText3=new TText(0.05, 0.80, fGameStatus.Data());
   fText1->SetTextColor(2);
   fText2->SetTextColor(4);
   fText1->Draw();
   fText2->Draw();
   fText3->Draw();
   // draw images
   TPad *pad1 = new TPad("imgpad", "imgpad", 0.5, 0.825, 0.65, 0.995 );
   pad1->SetRightMargin(0);
   pad1->SetBottomMargin(0);
   pad1->SetLeftMargin(0);
   pad1->SetTopMargin(0);
   pad1->Draw();
   pad1->cd();
   TString imgname=fEngine1->GetName();
   //imgname.ToLower();
   TImage* img1(0);
   cout << "opening " <<imgname<<endl;
   img1 = TImage::Open(imgname+".png");
   if (img1>0) {
      cout << "drawing " <<endl;
      img1->Draw();
   }
   cd(); // this cd is important

   TPad *pad2 = new TPad("imgpad", "imgpad", 0.7, 0.825, 0.85, 0.995 );
   pad2->SetRightMargin(0);
   pad2->SetBottomMargin(0);
   pad2->SetLeftMargin(0);
   pad2->SetTopMargin(0);
   pad2->Draw();
   pad2->cd();
   imgname = fEngine2? fEngine2->GetName() : "dummy.png";
   TImage* img2(0);
   cout << "opening " <<imgname<<endl;
   img2 = TImage::Open(imgname+".png");
   if (img2>0) {
      cout << "drawing " <<endl;
      img2->Draw();
   }
   cd(); // this cd is important
}

int HexBoard::DoMatchEngineVsEngine()
{
   fEngine1->SetPlayer(horizontal);
   fEngine1->Reset();
   fEngine2->SetPlayer(vertical);
   fEngine2->Reset();
   TString gameName =fEngine1->GetName()+TString("_vs_")+fEngine2->GetName()+Form("_size%d",fSize);
   //cout << "---------------------------------------"<<endl;
   //cout << "this is game "<<gameName<<endl;
   cout << "Match of "
        << fEngine1->GetName() 
        <<" in Red vs " 
        << fEngine2->GetName() << " in Blue, FieldSize = " << fSize<<endl;
   cout << "Engine "<< fEngine1->GetName() << "plays horizontal (in Red) and moves first"<<endl;
   double cputime;
   int winner = -1;
   while (winner < 0){
      //cout << "game loop"<<endl;
      if (fDrawMe) Delay(1.);
      if (winner<0) MakeMove(1, winner, cputime ); //player 1 moves (red)
      if (fDrawMe) Delay(1.);
      if (winner<0) MakeMove(2, winner, cputime ); //player 2 moves (blue)
   }
   //cout << "out of game loop"<<endl;
   if (winner ==1) SetWinner(fEngine1->GetName());
   if (winner ==2) SetWinner(fEngine2->GetName());   
   fBaseEngine->Write(gameName+TString(".txt"));
   return winner;
}
           

void HexBoard::MakeMove(int engineNumber, int& winner, double& cputime )
{
   winner = -1;
   if (!(engineNumber ==1 ||engineNumber ==2)){
      cout << "bad engine number "<<engineNumber<<endl;
      exit(1);
   }
   TStopwatch ttime;
   HexLocation nxtMv;
   if (engineNumber==1){
      if (fBaseEngine->fLastMove.x>=0) fEngine1->SetMove(fBaseEngine->fLastMove); 
      nxtMv = fEngine1->GetNextMove();
   }
   else if (engineNumber==2){
      if (fBaseEngine->fLastMove.x>=0) fEngine2->SetMove(fBaseEngine->fLastMove);
      nxtMv = fEngine2->GetNextMove();
   }
   ttime.Stop();
   cputime = ttime.CpuTime();
   if (fDebug) cout <<"time="<<cputime<<endl;
   if (cputime>fTimeLimitPerMove){
      cout <<"time beyond limit of " <<fTimeLimitPerMove << endl;
      winner = 3-engineNumber;
      return;
   }
   if (! IsValidMove(nxtMv)){
      cout <<"invalid move " << endl;
      winner = 3-engineNumber;
      return;
   }
   fBaseEngine->SetMove(nxtMv);
   if (fDebug){
      cout << " move of engine"<<endl;
      if (fBaseEngine->GetNMoves()<3) fBaseEngine->Report();
   }
   if (fDrawMe){ 
      (field[nxtMv.x][nxtMv.y])->SetOccupied((engineNumber==1?fColorA:fColorB));
      Update();
   }
   if (IsWon()) {
      winner = engineNumber;
      if (fDebug) cout <<"we have a winner " <<winner<< endl;
      return;
   }

}

void HexBoard::ReportBadMove(int enginenumber){
   cout << "ERROR: found a bad move of engine "<< enginenumber<< endl;
   exit(1);
}
   
void HexBoard::ReceiveUserMove(HexField* f){
   if (!fSingleUser && !fStartMatch){
      fStartMatch=true;
      DoMatchEngineVsEngine();  
      return;
   }   
   HexLocation pos(f->GetX(),f->GetY());
   if (fDebug) cout << "You have clicked hexField " << pos.x << " " << pos.y << " size=" << fSize << endl;
   if (IsValidMove(pos))
      {
         if (fDrawMe) f->SetOccupied(fColorA);
         fBaseEngine->SetMove(pos);
         if (IsWon()) {
            cout << "The USER has won" <<endl;
            SetWinner("USER");
            return;
         }
         if (fDebug) fBaseEngine->Report();
         fEngine1->SetMove(pos);
         fTimer1->Start(false);
         HexLocation nxtMv = fEngine1->GetNextMove();
         fTimer1->Stop();
         if (fDebug) cout << "engine 1 used total time " << fTimer1->CpuTime() << endl;
         if (! IsValidMove(nxtMv)) ReportBadMove(1);
         fBaseEngine->SetMove(nxtMv);
         if (fDrawMe) (field[nxtMv.x][nxtMv.y])->SetOccupied(fColorB);
         if (IsWon()) {
            SetWinner(fEngine1->GetName());
            return;
         }
      }
}

   
void HexBoard::SetWinner(TString winner){
   if (fDrawMe){
      //cout << "**************************************" << endl;
      cout << "Player " << winner << " has won!"<<endl;
      //cout << "**************************************" << endl;
      fBaseEngine->Write(TString("FinalResult.txt")); 
      if (fDrawMe){
         TText text(0.05, 0.6, winner+" has won");
         text.SetTextColor(8);
         text.Draw();
         Update();
         cd();
         Delay(7.);
      }
   }
   // ToDo: add a little song and dance, fireworks etc...
}


void HexBoard::SetRandomBarrier()
{
   TRandom3 fR(0);
   HexPosition p(fSize);
   fBarrier1=HexLocation(fR.Integer(fSize),fR.Integer(fSize));
   while (true){
      fBarrier2=HexLocation(fR.Integer(fSize),fR.Integer(fSize));
      if (p.GetDistance(fBarrier1,fBarrier2)==1) break;
   }
   if (!(fBarrier1<fBarrier2)) std::swap(fBarrier1,fBarrier2);
   if (fBaseEngine) fBaseEngine->SetBarrier(fBarrier1,fBarrier2);
   if (fEngine1) fEngine1->SetBarrier(fBarrier1,fBarrier2);
   if (fEngine2) fEngine2->SetBarrier(fBarrier1,fBarrier2);
   cout << "barrier between ("<<fBarrier1.x<<","<<fBarrier1.y<<")-("<<fBarrier2.x<<","<<fBarrier2.y<<")"<<endl;
}


