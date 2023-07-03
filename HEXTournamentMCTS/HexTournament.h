#include "HexBoard.h"
#include "TString.h"
bool GLOBAL_DO_BARRIER=false;

template<class E1, class E2>
class HexTournament{
public:
   int operator()(bool drawme=true){

      TString name1,name2, GameStatus;
      //int sizes[]={2,3,4,5};
      int sizes[]={8,10,12,14}; 
      int win=0,win1=0,win2=0;
      for (int i=0;i<4;i++){
         int siz = sizes[i];
         E1* e1 = new E1(siz,true,horizontal);
         E2* e2 = new E2(siz,true,vertical);
         if (i==0){
            name1= e1->GetName();
            name2= e2->GetName();
         }
         GameStatus = Form ("%d:%d",win1,win2);
         HexBoard* c1 = new HexBoard(siz, e1, e2, drawme, GameStatus, GLOBAL_DO_BARRIER);
         if (c1->DoMatchEngineVsEngine()==1) win1++; else win2++;
         delete c1;
         delete e1;
         delete e2;
         cout<< "current score: "<<name1<<" wins="<<win1<<", "<< name2<<" wins="<<win2<<endl;
         e1 = new E1(siz,true,vertical);
         e2 = new E2(siz,true,horizontal);
         GameStatus = Form ("%d:%d",win2,win1);
         c1 = new HexBoard(siz, e2, e1, drawme, GameStatus, GLOBAL_DO_BARRIER);
         if (c1->DoMatchEngineVsEngine()==1) win2++; else win1++;
         delete c1;
         delete e1;
         delete e2;
         cout<< "current score: "<<name1<<" wins="<<win1<<", "<< name2<<" wins="<<win2<<endl;
      }
      cout<< "Final Result: "<<name1<<" wins="<<win1<<", "<< name2<<" wins="<<win2<<endl;
      return win1;
   }   
};
