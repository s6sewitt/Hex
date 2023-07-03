#include "MarvinHexEngine.h"
#include "Node.h"
#include "tree.hh"
#include <algorithm>
#include <unordered_set>
#include <cmath>
#include <iterator>
#include <iostream>

using namespace MarvinNAMESPACE;

Node MarvinHexEngine::GetCurrentBoardState(){
   // Convert the current Board State to a usable format and make the root node from it
   cout << "GetCurrentBoardState Start" << endl;
   cout << "BoardState: " << endl;
   Node currentRootNode(fSize,0,0,true,fSize*fSize);
   for (int i=0; i<fSize; i++){
      cout << endl;
      for (int j=0; j<fSize; j++){
         cout << "(" << i << "," << j << "): " << fTyp[i][j] << "   ";
         if (fPlayer == horizontal){
            if (fTyp[i][j] == horizontal) {currentRootNode.setBoardState(i,j,1); currentRootNode.decreaseEmptyFields();}
            else if (fTyp[i][j] == vertical) {currentRootNode.setBoardState(i,j,2); currentRootNode.decreaseEmptyFields();}
            else if (fTyp[i][j] == none) {currentRootNode.setBoardState(i,j,0);}
         }
         else if (fPlayer == vertical){
            if (fTyp[i][j] == horizontal) {currentRootNode.setBoardState(i,j,2); currentRootNode.decreaseEmptyFields();}
            else if (fTyp[i][j] == vertical) {currentRootNode.setBoardState(i,j,1); currentRootNode.decreaseEmptyFields();}
            else if (fTyp[i][j] == none) {currentRootNode.setBoardState(i,j,0);}
         }
      }
   }
   cout << endl << "GetCurrentBoardState End" << endl;
   return currentRootNode;
}

void MarvinHexEngine::AddAllChildNodes(tree<Node> gameTree, tree<Node>::iterator iter){
   // Add all possible child nodes to the current node
   cout << "AddAllChildNodes Start" << endl;
   int fieldcolor;
   if (iter->getCurrentPlayer() == true){fieldcolor = 1;}
   else if (iter->getCurrentPlayer() == false){fieldcolor = 2;}
   cout << "Will make following nodes: " << endl;
   for (int i=0; i<fSize; i++){
      for (int j=0; j<fSize; j++){
         if (iter->getBoardState(i,j) == 0){
            cout << "(" << i << "," << j << ") ";
            Node childnode = (*iter);
            childnode.setCurrentPlayer(!(iter->getCurrentPlayer()));
            childnode.setBoardState(i,j,fieldcolor);
            childnode.setEmptyFields(iter->getEmptyFields()-1);
            gameTree.append_child(iter, childnode);
            cout << "emptyfields childnode: " << childnode.getEmptyFields() << endl;
         }
      }
   }
   cout << endl << "AddAllChildNodes End" << endl;
   return;
}

void MarvinHexEngine::CheckForLeaveNode(tree<Node> gameTree, tree<Node>::iterator iter){
   // Check if current Node is a leave node; and if it is check if it has been visited before
   cout << "CheckForLeaveNode Start" << endl;
   cout << "emptyfields check: " << iter->getEmptyFields() << endl;
   if (iter.number_of_children() == 0){
      if (iter->getVisitingNumber() == 0){cout << "CheckForLeaveNode End -> Rollout" << endl; Rollout(gameTree, iter);}
      else if (iter->getVisitingNumber() > 0){
         cout << "CheckForLeaveNode End -> AddAllChildNodes" << endl;
         AddAllChildNodes(gameTree, iter);
         CheckForLeaveNode(gameTree, iter);
      }
   }
   else if (iter.number_of_children() > 0){
      cout << "CheckForLeaveNode End -> ChooseChildNode" << endl;
      ChooseChildNode(gameTree, iter);
   }
   return;
}

int MarvinHexEngine::Rollout(tree<Node> gameTree, tree<Node>::iterator iter){
   // Make Array with right number of moves for both players, shuffle it, fill it in
   cout << "Rollout Start" << endl;
   int emptyfields = iter->getEmptyFields();
   cout << "emptyfields = " << emptyfields << endl;
   vector<int> randomvector(emptyfields);
   cout << "check 2" << endl;

   if (emptyfields % 2 == 0){
      for (int i=0; i<emptyfields/2; ++i){
         randomvector.at(i) = 1;
         randomvector.at(i + emptyfields/2) = 2;
         }
   }
   else if (emptyfields % 2 == 1 && iter->getCurrentPlayer() == true){
      randomvector[0] = 1;
      for (int i=0; i<(emptyfields-1)/2; ++i){
         randomvector[i+1] = 1;
         randomvector[i + 1 + (emptyfields-1)/2] = 2;
      }
   }
   else if (emptyfields % 2 == 1 && iter->getCurrentPlayer() == false){
      randomvector[0] = 2;
      for (int i=0; i<(emptyfields-1)/2; ++i){
         randomvector[i+1] = 1;
         randomvector[i + 1 + (emptyfields-1)/2] = 2;
      }
   }

   cout << "RandomVector length: " << randomvector.size() << endl;
   random_shuffle(randomvector.begin(), randomvector.end());
   tree<Node>::iterator copy = iter;
   int p=0;

   cout << "rolled out board state is: " << endl;
   for (int i=0; i<fSize; ++i){
      for (int j=0; j<fSize; ++j){
         cout << "(" << i << "," << j << "): " << randomvector[p] << "   ";
         if (copy->getBoardState(i,j) == 0){
            copy->setBoardState(i,j,randomvector[p]);
            ++p;
         }
      }
      cout << endl;
   }
   cout << endl << "Rollout End -> Backpropagate of DetermineWinner" << endl;

   // fester boardstate for analysis
   copy->setBoardState(0,0,2);
   copy->setBoardState(0,1,1);
   copy->setBoardState(0,2,1);
   copy->setBoardState(0,3,2);
   copy->setBoardState(0,4,2);
   copy->setBoardState(0,5,2);
   copy->setBoardState(0,6,2);
   copy->setBoardState(0,7,2);
   copy->setBoardState(0,8,1);
   copy->setBoardState(0,9,1);

   copy->setBoardState(1,0,2);
   copy->setBoardState(1,1,1);
   copy->setBoardState(1,2,2);
   copy->setBoardState(1,3,2);
   copy->setBoardState(1,4,1);
   copy->setBoardState(1,5,2);
   copy->setBoardState(1,6,1);
   copy->setBoardState(1,7,2);
   copy->setBoardState(1,8,2);
   copy->setBoardState(1,9,2);

   copy->setBoardState(2,0,1);
   copy->setBoardState(2,1,2);
   copy->setBoardState(2,2,2);
   copy->setBoardState(2,3,2);
   copy->setBoardState(2,4,2);
   copy->setBoardState(2,5,2);
   copy->setBoardState(2,6,1);
   copy->setBoardState(2,7,2);
   copy->setBoardState(2,8,2);
   copy->setBoardState(2,9,2);

   copy->setBoardState(3,0,2);
   copy->setBoardState(3,1,1);
   copy->setBoardState(3,2,1);
   copy->setBoardState(3,3,1);
   copy->setBoardState(3,4,1);
   copy->setBoardState(3,5,1);
   copy->setBoardState(3,6,1);
   copy->setBoardState(3,7,2);
   copy->setBoardState(3,8,2);
   copy->setBoardState(3,9,1);

   copy->setBoardState(4,0,1);
   copy->setBoardState(4,1,1);
   copy->setBoardState(4,2,1);
   copy->setBoardState(4,3,1);
   copy->setBoardState(4,4,2);
   copy->setBoardState(4,5,1);
   copy->setBoardState(4,6,2);
   copy->setBoardState(4,7,1);
   copy->setBoardState(4,8,2);
   copy->setBoardState(4,9,1);

   copy->setBoardState(5,0,2);
   copy->setBoardState(5,1,2);
   copy->setBoardState(5,2,1);
   copy->setBoardState(5,3,2);
   copy->setBoardState(5,4,1);
   copy->setBoardState(5,5,1);
   copy->setBoardState(5,6,1);
   copy->setBoardState(5,7,2);
   copy->setBoardState(5,8,1);
   copy->setBoardState(5,9,1);

   copy->setBoardState(6,0,1);
   copy->setBoardState(6,1,2);
   copy->setBoardState(6,2,1);
   copy->setBoardState(6,3,2);
   copy->setBoardState(6,4,2);
   copy->setBoardState(6,5,2);
   copy->setBoardState(6,6,1);
   copy->setBoardState(6,7,1);
   copy->setBoardState(6,8,2);
   copy->setBoardState(6,9,2);

   copy->setBoardState(7,0,2);
   copy->setBoardState(7,1,2);
   copy->setBoardState(7,2,1);
   copy->setBoardState(7,3,2);
   copy->setBoardState(7,4,1);
   copy->setBoardState(7,5,1);
   copy->setBoardState(7,6,2);
   copy->setBoardState(7,7,2);
   copy->setBoardState(7,8,1);
   copy->setBoardState(7,9,1);

   copy->setBoardState(8,0,2);
   copy->setBoardState(8,1,1);
   copy->setBoardState(8,2,1);
   copy->setBoardState(8,3,2);
   copy->setBoardState(8,4,2);
   copy->setBoardState(8,5,2);
   copy->setBoardState(8,6,1);
   copy->setBoardState(8,7,2);
   copy->setBoardState(8,8,1);
   copy->setBoardState(8,9,1);

   copy->setBoardState(9,0,1);
   copy->setBoardState(9,1,1);
   copy->setBoardState(9,2,2);
   copy->setBoardState(9,3,1);
   copy->setBoardState(9,4,1);
   copy->setBoardState(9,5,2);
   copy->setBoardState(9,6,1);
   copy->setBoardState(9,7,2);
   copy->setBoardState(9,8,1);
   copy->setBoardState(9,9,2);

   Backpropagate(DetermineWinner(gameTree, copy), gameTree, iter);
}

bool MarvinHexEngine::DetermineWinner(tree<Node> gameTree, tree<Node>::iterator iter){
   // Check who won a full board from Rollout by starting at one side & adding neighbors to a set
   cout << "DetermineWinner Start" << endl;
   unordered_set<int> winningSet;
   if (fPlayer == horizontal){
      for (int j=0; j<fSize; ++j){
         if (iter->getBoardState(0,j) == 1){
            cout << "Start horizontal Recursive for j = " << j << endl;
            if (RecursiveCheck(0,j,winningSet,iter) == true){
               cout << "its a win" << endl;
               return true;
            }
         }
      }
      return false;
   }
   else if (fPlayer == vertical){
      for (int i=0; i<fSize; ++i){
         if (iter->getBoardState(i,0) == 1){
            cout << "Start vertical Recursive for i = " << i << endl;
            if (RecursiveCheck(i,0,winningSet,iter) == true){
               cout << "its a win" << endl;
               return true;
            }
         }
      }
      cout << "its a loose" << endl;
      return false;
   }
}

bool MarvinHexEngine::RecursiveCheck(int i, int j, unordered_set<int> (&winningSet), tree<Node>::iterator iter){
   // Add neighbors if in same color & not already added & in the field; check if one is on the opposite side of the board for a win
   cout << "Recursive Start with i,j = " << i << "," << j << endl;
   winningSet.insert(i+j*fSize);

   if (i-1 >= 0 && j+1 < fSize){
      if (iter->getBoardState(i-1,j+1) == 1 && winningSet.find((i-1)+(j+1)*fSize) == winningSet.end()){
         cout << "(top left)" << "->" << (i-1)+(j+1)*fSize << endl;
         if (fPlayer == vertical && j+1 == fSize-1){return true;}
         if (RecursiveCheck(i-1,j+1, winningSet, iter)==true){return true;}
      }
   }
   if (j+1 < fSize){
      if (iter->getBoardState(i,j+1) == 1 && winningSet.find((i)+(j+1)*fSize) == winningSet.end()){
         cout << "(top right)" << "->" << (i)+(j+1)*fSize << endl;
         if (fPlayer == vertical && j+1 == fSize-1){return true;}
         if (RecursiveCheck(i,j+1, winningSet, iter)==true){return true;}
      }
   }
   if (i-1 >= 0){
      if (iter->getBoardState(i-1,j) == 1 && winningSet.find((i-1)+(j)*fSize) == winningSet.end()){
         cout << "(left)" << "->" << (i-1)+(j)*fSize << endl;
         if (RecursiveCheck(i-1,j, winningSet, iter)==true){return true;}
      }
   }
   if (i+1 < fSize){
      if (iter->getBoardState(i+1,j) == 1 && winningSet.find((i+1)+(j)*fSize) == winningSet.end()){
         cout << "(right)" << "->" << (i+1)+(j)*fSize << endl;
         if (fPlayer == horizontal && i+1 == fSize-1){return true;}
         if (RecursiveCheck(i+1,j, winningSet, iter)==true){return true;}
      }
   }
   if (j-1 >= 0){
      if (iter->getBoardState(i,j-1) == 1 && winningSet.find((i)+(j-1)*fSize) == winningSet.end()){
         cout << "(bottom left)" << "->" << (i)+(j-1)*fSize << endl;
         if (RecursiveCheck(i,j-1, winningSet, iter)==true){return true;}
      }
   }
   if (i+1 < fSize && j-1 >= 0){
      if (iter->getBoardState(i+1,j-1) == 1 && winningSet.find((i+1)+(j-1)*fSize) == winningSet.end()){
         cout << "(bottom right)" << "->" << (i+1)+(j-1)*fSize << endl;
         if (fPlayer == horizontal && i+1 == fSize-1){return true;}
         if (RecursiveCheck(i+1,j-1, winningSet, iter)==true){return true;}
      }
   }
   cout << "recursion end" << endl;
   return false;
}

void MarvinHexEngine::ChooseChildNode(tree<Node> gameTree, tree<Node>::iterator iter){
   // calculate UCB1 value for all children and search for the maximum
   cout << "ChooseChildNode Start" << endl;

   float ucb1_old = 0;
   float c = 2;
   tree<Node>::iterator winnerNode;

   tree<Node>::sibling_iterator beginsibling = gameTree.begin(iter);
   tree<Node>::sibling_iterator endsibling = gameTree.end(iter);

   cout << "calculating UCB1 values: " << endl;
   while (beginsibling != endsibling){
      int np = iter->getVisitingNumber();
      int n = beginsibling->getVisitingNumber();
      int t = beginsibling->getReward();
      float ucb1;
      
      if (n == 0 && t == 0){ucb1 = INFINITY;}
      else {ucb1 = float(t) / n + c * sqrt( log(np) / n );}

      if (ucb1 > ucb1_old){
         ucb1_old = ucb1;
         winnerNode = beginsibling;
      }
      beginsibling++;
   }
   cout << "maximal UCB1 was " << ucb1_old << endl;
   cout << "ChooseChildNode End -> CheckForLeaveNode" << endl;
   CheckForLeaveNode(gameTree, winnerNode);
   return;
}

void MarvinHexEngine::Backpropagate(bool win, tree<Node> gameTree, tree<Node>::iterator iter){
   // backpropagate the values until the root node is reached
   cout << "Backpropagate Start" << endl;
   cout << win << endl;
   iter->increaseVisitingNumber();
   if (win==true){iter->increaseReward();}
   if (gameTree.is_head(iter) != true){
      cout << "not at root yet" << endl;
      tree<Node>::iterator parent = gameTree.parent(iter);
      Backpropagate(win, gameTree, parent);
   }
   cout << "Backpropagate End" << endl;
   return;
}

HexLocation MarvinHexEngine::GetNextMove(){ 
   // this is what is called by the framework, so it acts as the "main" function here

   cout << "GetNextMove Start" << endl;
   Node currentRootNode = GetCurrentBoardState();
   tree<Node> gameTree;
   tree<Node>::iterator root = gameTree.insert(gameTree.begin(),currentRootNode);

   cout << "Made Game Tree with X children: X = ";
   cout << root.number_of_children() << endl;

   cout << "emptyfields: " << root->getEmptyFields() << endl;

   AddAllChildNodes(gameTree, root);

   cout << "Tree now has X Children: X = ";
   cout << root.number_of_children() << endl;

   CheckForLeaveNode(gameTree,root);

   cout << "made a full iteration" << endl;


   // end of 1st iteration
   // CheckForLEaveNode(gameTree,root); for each iteration


   // TODO: TESTEN !!!


   // TODO: Abbruchbedingung: Time Limit
   // TODO: Wie geht das move beenden?
   // ---> TODO: Ergebnis des Algorithmus ausgeben (for loop über childnodes of root mit UCB1)
   // TODO: Add Pattern Search (where? in rollout?)
   // TODO: Add threading (which variant?)
   FinishMove();
   return fLastMove;
   
   
   
   /* while (time condition){
      CheckForLeaveNode(gameTree,root);
   }
   evluate Result somehow;
   FinishMove();
   return fLastMove;
*/
}