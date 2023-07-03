#ifndef MarvinHexEngine_H
#define MarvinHexEngine_H

#include "IHexEngine.h"
#include "MarvinHexEngine.h"
#include "Node.h"
#include "tree.hh"
#include <unordered_set>
#include <iostream>

namespace MarvinNAMESPACE{

   class MarvinHexEngine : public IHexEngine{
      
      /* DESCRIPTION
         Marvin uses Monte Carlo Tree Search (MCTS), which is proven to converge to timeconsuming
         Minimax for sufficient iterations with much less computations. MCTS consists of the following steps:

               ------------------
               | Starting state |  ---------<----- ITERATE FROM START ---------<----------|
               ------------------                                                         |
                        |                                                                 |
                        V                                                                 |
               -------------------                                               -----------------
               | Add child nodes |                                               | Backpropagate | 
               -------------------                                               -----------------
                        |                                                                 |
                        V                                                                 |
               ::::::::::::::::::::             :::::::::::::::::::::            -----------------
         |->---: Is current State :--- YES ---> : Was current State :--- NO ---> | Do a Rollout  |
         |     : a leaf node?     :             : visited before?   :            -----------------
         |     ::::::::::::::::::::             :::::::::::::::::::::                     |
         |              |                                |                                |
         |              NO                              YES                              /|\
        /|\             |                                |                                |
         |              V                                V                                |
         |     ---------------------            --------------------                      |
         |     | Choose Child node |            |  Add child nodes |                      |
         |-----| that maximises    |            --------------------                      |
               | UCB1 score        |                     |                                |
               ---------------------                     V                                |
                                                ---------------------                     |
                                                | Choose Child Node |--------->-----------|
                                                ---------------------

         Here, Rollout means you fill the board to the terminal state
               Backpropagation is the update of n and t values to all parent nodes
               and UCB1 value is calculated via
            UCB1 = t / n  + C * sqrt( ln(N) / n )                     
         with: 
            t = number of won simulations of state s
            n = number of simulations of state s
            C = a constant, normally order 1
            N = number of simulations of parentstate S
      */

      public:

         MarvinHexEngine(int isize, bool horizontalMovesFirst, FieldType player) :
            IHexEngine(isize, horizontalMovesFirst, player) {  }
      
         ~MarvinHexEngine() { }

         TString GetName(){return TString("MarvinHexEngine");}

         HexLocation GetNextMove();

         Node GetCurrentBoardState();

         void AddAllChildNodes(tree<Node> gameTree, tree<Node>::iterator iter);

         void CheckForLeaveNode(tree<Node> gameTree, tree<Node>::iterator iter);

         int Rollout(tree<Node> gameTree, tree<Node>::iterator iter);

         bool DetermineWinner(tree<Node> gameTree, tree<Node>::iterator iter);

         bool RecursiveCheck(int i, int j, unordered_set<int> (&winningSet), tree<Node>::iterator iter);

         void ChooseChildNode(tree<Node> gameTree, tree<Node>::iterator iter);

         void Backpropagate(bool win, tree<Node> gameTree, tree<Node>::iterator iter);

   };

}

#endif