#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <vector>

namespace MarvinNAMESPACE{

	class Node {

		private:
			int fVisited;
			int fReward;
			bool fcurrentPlayer;
			int femptyFields;
			vector<vector<int>> fBoardState;

		public:
			// Empty Constructor
			Node() : fVisited(0), fReward(0), fcurrentPlayer(true), femptyFields(0) {}

			// Constructor with Values
			Node(int size, int visit, int reward, bool currentplayer, int empty) : fVisited(visit), fReward(reward), fcurrentPlayer(currentplayer), femptyFields(empty) { 
				fBoardState.resize(size, vector<int>(size, 0));
			}

			// Destructor
			~Node(){}

			// Copy Constructor
			Node(const Node &sketch) {
				fVisited = sketch.fVisited;
				fReward = sketch.fReward;
				fcurrentPlayer = sketch.fcurrentPlayer;
				femptyFields =sketch.femptyFields;
				fBoardState = sketch.fBoardState;
			}

			// Getter Functions
			int getVisitingNumber(){return fVisited;}
			int getReward(){return fReward;}
			bool getCurrentPlayer(){return fcurrentPlayer;}
			int getBoardState(int i, int j){return fBoardState[i][j];}
			int getEmptyFields(){return femptyFields;}

			// Setter Functions
			void setVisitingNumber(int visit){fVisited = visit;}
			void setReward(int reward){fReward = reward;}
			void setCurrentPlayer(bool currentplayer){fcurrentPlayer = currentplayer;}
			void setBoardState(int i, int j, int val){fBoardState[i][j] = val;}
			void setEmptyFields(int empty){femptyFields = empty;}

			// Change-by-one functions
			void decreaseEmptyFields(){femptyFields--;}
			void increaseVisitingNumber(){fVisited++;}
			void increaseReward(){fReward++;}

			//TODO: Slim Down by deleting unused Functions
	};

}
#endif