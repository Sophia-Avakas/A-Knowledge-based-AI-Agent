// ======================================================================
// FILE:        MyAI.hpp
//
// AUTHOR:      Abdullah Younis
//
// DESCRIPTION: This file contains your agent class, which you will
//              implement. You are responsible for implementing the
//              'getAction' function and any helper methods you feel you
//              need.
//
// NOTES:       - If you are having trouble understanding how the shell
//                works, look at the other parts of the code, as well as
//                the documentation.
//
//              - You are only allowed to make changes to this portion of
//                the code. Any changes to other portions of the code will
//                be lost when the tournament runs your code.
// ======================================================================

#ifndef MYAI_LOCK
#define MYAI_LOCK

#include "Agent.hpp"
#include <vector>
#include <iostream>
#include <queue>
#include <stack>
#include <set>

using namespace std;

class MyAI : public Agent
{
public:
	MyAI ( void );
	
	Action getAction
	(
		bool stench,
		bool breeze,
		bool glitter,
		bool bump,
		bool scream
	);
	
	// ======================================================================
	// YOUR CODE BEGINS
	// ======================================================================

private:
	
	struct MyTile
		{
			bool pit    = false;
			bool wumpus = false;
			bool gold   = false;
			bool breeze = false;
			bool stench = false;
			bool glitter = false;
			bool safe = false;
			int visitCount = 0;
		};

	void searchReturnPath();
	void searchLeastCostPath(vector<pair<int, int>>& leastCostPath);
	bool getFaceNei(int& posX,int& posY); //get the neighbor the current agent is facing 
	bool getLeftNei(int& posX, int& posY); //get the neighbor on the left side
	bool getRightNei(int& posX, int& posY); //get the neighbor on the right side
	bool getBackNei(int& posX, int& posY); //get the neighbor on the back
	bool getFaceNei(int curX,int curY,int curDir,int& posX, int& posY); //get the neighbor the current agent is facing 
	bool getLeftNei(int curX, int curY, int curDir, int& posX, int& posY); //get the neighbor on the left side
	bool getRightNei(int curX, int curY, int curDir, int& posX, int& posY); //get the neighbor on the right side
	bool getBackNei(int curX, int curY, int curDir, int& posX, int& posY); //get the neighbor on the back
	void getNeighbors(vector<int>& posXVec, vector<int>& posYVec);
	void updatePosDir(Action act);

	size_t	agentCurDir {0};     // The current direction the agent is facing: 0 - right, 1 - down, 2 - left, 3 - up
	size_t  agentPrevDir {0};    // The last direction of the agent
	int	agentX {0};			// The column where the agent is located ( x-coord = col-coord )
	int	agentY {0};			// The row where the agent is located ( y-coord = row-coord )
	bool start {true};          //mark if the agent starts at the initial location
	bool is_backwards {false};   //mark if the agent should backwards to its initial location
	bool found_gold {false}; //mark if gold found, if true then should begin returning and climbing cave
	queue<Action> followingActions; //record the following actions should be made, such as turn backwards without sensing the world
	// Board Variables
	size_t	colDimension {0};	// The number of columns the game board has, 0 indicating unknown
	size_t	rowDimension {0};	// The number of rows the game board has, 0 indicating unknown
	const int MAX_DIM {10}; //The maximum dimension of the game board
	const static int MAX_COST{ 1000000 }; //the maximum cost of a path
	int boardSizeX { MAX_DIM };
	int boardSizeY { MAX_DIM };
	int numSteps{ 0 }; //mark the number of steps that have been executed 
	const int MAX_STEP{ 30 }; //If the number of steps surpass 50, then if the agent should climb when come back to its origin
	vector<vector<MyTile>> board {vector<vector<MyTile>>(MAX_DIM,vector<MyTile>(MAX_DIM))};
	vector<pair<int,int>> foundPath;
	set<pair<int, int>> wumpusPos; 

	// ======================================================================
	// YOUR CODE ENDS
	// ======================================================================
};

#endif

