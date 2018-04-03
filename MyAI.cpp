// ======================================================================
// FILE:        MyAI.cpp
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

#include "MyAI.hpp"
#include <limits>
#include <cstdlib>
#include <stack>
#include <iterator>
#include <algorithm>
using namespace std;




MyAI::MyAI() : Agent()
{
	// ======================================================================
	// YOUR CODE BEGINS
	// ======================================================================
	/*for (int x = 0; x < boardSizeX; x++)
	{
		for (int y = 0; y < boardSizeY; y++)
		{
			wumpusPos.insert(make_pair(x, y));
		}
	}*/
	// ======================================================================
	// YOUR CODE ENDS
	// ======================================================================
}
	
Agent::Action MyAI::getAction
(
	bool stench,
	bool breeze,
	bool glitter,
	bool bump,
	bool scream //consider this later
)
{
	// ======================================================================
	// YOUR CODE BEGINS
	// =====================================================================
	numSteps++;

	//Execute any actions should be made before sensing
	if(!followingActions.empty())
	{
		Action act = followingActions.front();
		followingActions.pop();
		updatePosDir(act);
		return act;
	}

	if (numSteps > 100 && !bump)
	{
		is_backwards = true;
		searchReturnPath();
		Action act = followingActions.front();
		followingActions.pop();
		return act;
		//return CLIMB;
	}

	if(!is_backwards)
	{
		if(!bump) //update board state
		{
			board[agentX][agentY].stench = stench;
			board[agentX][agentY].breeze = breeze;
			board[agentX][agentY].glitter = glitter;
			board[agentX][agentY].safe = true;
			board[agentX][agentY].visitCount++;

			vector<int> posXVec, posYVec;
			getNeighbors(posXVec, posYVec);
			set<pair<int, int>> possible_wumpus;
			for (int i = 0; i < posXVec.size(); i++)
			{
				if (!stench && !breeze) //update if the neighbors are safe 
					board[posXVec[i]][posYVec[i]].safe = true;
				/*if (stench)
				{
					if (!board[posXVec[i]][posYVec[i]].safe)
						possible_wumpus.insert(make_pair(posXVec[i], posYVec[i]));
				}*/
			}
			if (stench)
			{
				/*if (wumpusPos.size() == 0) wumpusPos = possible_wumpus;
				else
				{
					set<pair<int, int>> intersect;
					set_intersection(wumpusPos.begin(), wumpusPos.end(), possible_wumpus.begin(), possible_wumpus.end(),
						std::inserter(intersect, intersect.begin()));
					wumpusPos = intersect;
				}*/
				//if (wumpusPos.size() == 1) //know the wumpus position 
				//{
				//	Action act = SHOOT;
				//	auto it = wumpusPos.begin();
				//	int nx = -1, ny = -1;
				//	if (getFaceNei(nx, ny) && nx == it->first && ny == it->second)
				//		act = SHOOT;
				//	else if (getLeftNei(nx, ny) && nx == it->first && ny == it->second)
				//	{
				//		act = TURN_LEFT;
				//		followingActions.push(SHOOT);
				//	}
				//	else if (getRightNei(nx, ny) && nx == it->first && ny == it->second)
				//	{
				//		act = TURN_RIGHT;
				//		followingActions.push(SHOOT);
				//	}
				//	else if (getBackNei(nx, ny) && nx == it->first && ny == it->second)
				//	{
				//		act = TURN_LEFT;
				//		followingActions.push(TURN_LEFT);
				//		followingActions.push(SHOOT);
				//	}
				//		
				//	wumpusPos.clear();
				//	updatePosDir(act);
				//	return act;
				//}
			}
		}
		else
		{
			//revise the current coordinates, and also update board size
			switch (agentCurDir)
			{
			case 0: //right
				agentX--;
				boardSizeX = agentX+1;
				break;
			case 1: //down
				agentY++; break;
			case 2: //left
				agentX++; break;
			case 3: //up
				agentY--;
				boardSizeY = agentY+1;
				break;
			}
		}

		Action act = FORWARD; //in default forward

		if(glitter)
		{
			is_backwards = true;
			//grab the gold, should return and climb
			//search for the shortest return path (for now just search one path)
			//and put them in followingActions

			searchReturnPath();
			return GRAB;
		}

		//Determine the next action 
		int faceX, faceY,leftX,leftY,rightX,rightY,backX,backY;
		bool faceValid, leftValid, rightValid, backValid;
		faceValid = getFaceNei(faceX, faceY);
		leftValid = getLeftNei(leftX, leftY);
		rightValid = getRightNei(rightX, rightY);
		backValid = getBackNei(backX, backY);
		if (faceValid && board[faceX][faceY].safe && board[faceX][faceY].visitCount == 0)
		{
			act = FORWARD;
		}
		else if (leftValid && board[leftX][leftY].safe && board[leftX][leftY].visitCount == 0)
		{
			act = TURN_LEFT;
		}
		else if (rightValid && board[rightX][rightY].safe && board[rightX][rightY].visitCount == 0)
		{
			act = TURN_RIGHT;
		}
		else if (agentX == 0 && agentY == 0) //if already in start position, climb up
		{
			act = CLIMB;
		}
		//else if (backValid && board[backX][backY].safe) //backwards
		//{
		//	act = TURN_LEFT;
		//	followingActions.push(TURN_LEFT);
		//	followingActions.push(FORWARD);
		//}
		else if (!faceValid || !board[faceX][faceY].safe)
		{
			if (!leftValid || !board[leftX][leftY].safe)
			{
				act = TURN_RIGHT;
			}
			else if (!rightValid || !board[rightX][rightY].safe)
			{
				act = TURN_LEFT;
			}
			else if (leftValid && board[leftX][leftY].safe && rightValid && board[rightX][rightY].safe)
			{
				if (rand() % 2 == 0)
				{
					act = TURN_LEFT;
				}
				else
				{
					act = TURN_RIGHT; 
				}
			}
		}
		else if (faceValid || board[faceX][faceY].safe)
		{
			act = FORWARD;
		}

		updatePosDir(act);
		return act;
	}

	return FORWARD;
	// ======================================================================
	// YOUR CODE ENDS
	// ======================================================================
}

// ======================================================================
// YOUR CODE BEGINS
// ======================================================================

void MyAI::searchReturnPath()
{
	vector<pair<int,int>> curPath;
	searchLeastCostPath(foundPath);
	//construct actions
	int agentTempDir = agentCurDir;
	for(int i=0; i<(int)foundPath.size()-1; i++)
	{
		int dx = foundPath[i+1].first - foundPath[i].first;
		int dy = foundPath[i+1].second - foundPath[i].second;

		//adjust direction if necessary
		if(dx > 0) //should move right
		{
			switch(agentTempDir)
			{
			case 0: //right
				break;
			case 1: //down
				followingActions.push(TURN_LEFT);
				break;
			case 2: //left
				followingActions.push(TURN_LEFT);
				followingActions.push(TURN_LEFT);
				break;
			case 3: //up
				followingActions.push(TURN_RIGHT);
				break;
			}
			agentTempDir = 0;
		}
		else if(dx < 0) //should move left
		{
			switch(agentTempDir)
			{
			case 0: //right
				followingActions.push(TURN_LEFT);
				followingActions.push(TURN_LEFT);
				break;
			case 1: //down
				followingActions.push(TURN_RIGHT);
				break;
			case 2: //left
				break;
			case 3: //up
				followingActions.push(TURN_LEFT);
				break;
			}
			agentTempDir = 2;
		}
		else if(dy > 0) //should move up
		{
			switch(agentTempDir)
			{
			case 0: //right
				followingActions.push(TURN_LEFT);
				break;
			case 1: //down
				followingActions.push(TURN_LEFT);
				followingActions.push(TURN_LEFT);
				break;
			case 2: //left
				followingActions.push(TURN_RIGHT);
				break;
			case 3: //up
				break;
			}
			agentTempDir = 3;
		}
		else if(dy < 0) //should move down
		{
			switch(agentTempDir)
			{
			case 0: //right
				followingActions.push(TURN_RIGHT);
				break;
			case 1: //down
				break;
			case 2: //left
				followingActions.push(TURN_LEFT);
				break;
			case 3: //up
				followingActions.push(TURN_LEFT);
				followingActions.push(TURN_LEFT);
				break;
			}
			agentTempDir = 1;
		}

		//move forward
		followingActions.push(FORWARD);
	}
	//last step, climb out
	followingActions.push(CLIMB);
}

void MyAI::searchLeastCostPath(vector<pair<int, int>>& leastCostPath)
{
	struct info
	{
		int cost{ MAX_COST };
		int prevNeiX{ -1 };
		int prevNeiY{ -1 };
		bool alreayPutStack{ false };
	};
	struct pos
	{
		pos(int nx,int ny, int ndir)
		{
			x = nx;
			y = ny;
			dir = ndir;
		}
		int x{ -1 };
		int y{ -1 };
		int dir{ -1 };
	};

	//check agent position 
	if (agentX < 0 || agentX >= boardSizeX || agentY < 0 || agentY >= boardSizeY)
		return;

	vector<vector<info>> boardInfo{ vector<vector<info>>(boardSizeX,vector<info>(boardSizeY)) };
	stack<pos> neighbors;
	neighbors.push(pos(agentX,agentY, int(agentCurDir)));
	boardInfo[agentX][agentY].cost = 0; //initilize as 0
	while (!neighbors.empty())
	{
		stack<pos> nextlayer;
		while (!neighbors.empty())
		{
			pos p = neighbors.top();
			neighbors.pop();
			boardInfo[p.x][p.y].alreayPutStack = true;
			if (p.x == 0 && p.y == 0) continue; //already in origin, no need to consider neighbors

												//search neighbors
			int nx, ny;
			bool bValid = getFaceNei(p.x, p.y, p.dir, nx, ny);
			if (bValid && board[nx][ny].visitCount > 0)
			{
				if (boardInfo[nx][ny].cost > boardInfo[p.x][p.y].cost + 1) //just move forward
				{
					boardInfo[nx][ny].cost = boardInfo[p.x][p.y].cost + 1;
					boardInfo[nx][ny].prevNeiX = p.x;
					boardInfo[nx][ny].prevNeiY = p.y;
				}
				if (!boardInfo[nx][ny].alreayPutStack)
				{
					nextlayer.push(pos(nx,ny,p.dir));
				}
			}
			bValid = getLeftNei(p.x, p.y, p.dir, nx, ny);
			if (bValid && board[nx][ny].visitCount > 0)
			{
				if (boardInfo[nx][ny].cost > boardInfo[p.x][p.y].cost + 2) //turn left, move forward
				{
					boardInfo[nx][ny].cost = boardInfo[p.x][p.y].cost + 2;
					boardInfo[nx][ny].prevNeiX = p.x;
					boardInfo[nx][ny].prevNeiY = p.y;
				}
				if (!boardInfo[nx][ny].alreayPutStack)
				{
					int dir = p.dir - 1;
					if (dir < 0) dir = 3;
					nextlayer.push(pos(nx,ny,dir));
				}
			}
			bValid = getRightNei(p.x, p.y, p.dir, nx, ny);
			if (bValid && board[nx][ny].visitCount > 0)
			{
				if (boardInfo[nx][ny].cost > boardInfo[p.x][p.y].cost + 2) //turn right, move forward
				{
					boardInfo[nx][ny].cost = boardInfo[p.x][p.y].cost + 2;
					boardInfo[nx][ny].prevNeiX = p.x;
					boardInfo[nx][ny].prevNeiY = p.y;
				}
				if (!boardInfo[nx][ny].alreayPutStack)
				{
					int dir = p.dir + 1;
					if (dir > 3) dir = 0;
					nextlayer.push(pos(nx,ny,dir));
				}
			}
			bValid = getBackNei(p.x, p.y, p.dir, nx, ny);
			if (bValid && board[nx][ny].visitCount > 0)
			{
				if (boardInfo[nx][ny].cost > boardInfo[p.x][p.y].cost + 3) //turn left, turn left, move forward
				{
					boardInfo[nx][ny].cost = boardInfo[p.x][p.y].cost + 3;
					boardInfo[nx][ny].prevNeiX = p.x;
					boardInfo[nx][ny].prevNeiY = p.y;
				}
				if (!boardInfo[nx][ny].alreayPutStack)
				{
					int dir = p.dir - 2;
					if (dir < 0) dir += 4;
					nextlayer.push(pos(nx,ny,dir));
				}
			}
		}

		neighbors = nextlayer;
	}
	//Get the shortest path
	int x = 0, y = 0;
	while (x != -1 && y != -1)
	{
		leastCostPath.insert(leastCostPath.begin(),make_pair(x, y));
		info i = boardInfo[x][y];
		x = i.prevNeiX;
		y = i.prevNeiY;
	}
}


//get the neighbor the current agent is facing 
bool MyAI::getFaceNei(int curX, int curY, int curDir, int& posX, int& posY)
{
	bool bValid = true;
	switch (curDir)
	{
	case 0: //moving right
		posX = curX + 1;
		posY = curY;
		break;
	case 1: //moving down
		posX = curX;
		posY = curY - 1;
		break;
	case 2: //moving left
		posX = curX - 1;
		posY = curY;
		break;
	case 3: //moving up
		posX = curX;
		posY = curY + 1;
		break;
	default:
		break;
	}
	if (posX < 0 || posX >= boardSizeX || posY < 0 || posY >= boardSizeY) bValid = false;
	return bValid;
}

//get the neighbor on the left side
bool MyAI::getLeftNei(int curX, int curY, int curDir, int& posX, int& posY)
{
	bool bValid = true;
	switch (curDir)
	{
	case 0: //moving right
		posX = curX;
		posY = curY + 1;
		break;
	case 1: //moving down
		posX = curX + 1;
		posY = curY;
		break;
	case 2: //moving left
		posX = curX;
		posY = curY - 1;
		break;
	case 3: //moving up
		posX = curX - 1;
		posY = curY;
		break;
	default:
		break;
	}
	if (posX < 0 || posX >= boardSizeX || posY < 0 || posY >= boardSizeY) bValid = false;
	return bValid;
}

//get the neighbor on the right side
bool MyAI::getRightNei(int curX, int curY, int curDir, int& posX, int& posY)
{
	bool bValid = true;
	switch (curDir)
	{
	case 0: //moving right
		posX = curX;
		posY = curY - 1;
		break;
	case 1: //moving down
		posX = curX - 1;
		posY = curY;
		break;
	case 2: //moving left
		posX = curX;
		posY = curY + 1;
		break;
	case 3: //moving up
		posX = curX + 1;
		posY = curY;
		break;
	default:
		break;
	}
	if (posX < 0 || posX >= boardSizeX || posY < 0 || posY >= boardSizeY) bValid = false;
	return bValid;
}

//get the neighbor on the back
bool MyAI::getBackNei(int curX, int curY, int curDir, int& posX, int& posY)
{
	bool bValid = true;
	switch (curDir)
	{
	case 0: //moving right
		posX = curX - 1;
		posY = curY;
		break;
	case 1: //moving down
		posX = curX;
		posY = curY + 1;
		break;
	case 2: //moving left
		posX = curX + 1;
		posY = curY;
		break;
	case 3: //moving up
		posX = curX;
		posY = curY - 1;
		break;
	default:
		break;
	}
	if (posX < 0 || posX >= boardSizeX || posY < 0 || posY >= boardSizeY) bValid = false;
	return bValid;
}

bool MyAI::getFaceNei(int& posX, int& posY) //get the neighbor the current agent is facing 
{
	return getFaceNei(agentX, agentY, agentCurDir, posX, posY);
}

bool MyAI::getLeftNei(int& posX, int& posY) //get the neighbor on the left side
{
	return getLeftNei(agentX, agentY, agentCurDir, posX, posY);
}

bool MyAI::getRightNei(int& posX, int& posY) //get the neighbor on the right side
{
	return getRightNei(agentX, agentY, agentCurDir, posX, posY);
}

bool MyAI::getBackNei(int& posX, int& posY) //get the neighbor on the back
{
	return getBackNei(agentX, agentY, agentCurDir, posX, posY);
}

void MyAI::getNeighbors(vector<int>& posXVec, vector<int>& posYVec)
{
	if (agentX + 1 < boardSizeX)
	{
		posXVec.push_back(agentX + 1);
		posYVec.push_back(agentY);
	}
	if (agentX - 1 >= 0)
	{
		posXVec.push_back(agentX - 1);
		posYVec.push_back(agentY);
	}
	if (agentY + 1 < boardSizeY)
	{
		posXVec.push_back(agentX);
		posYVec.push_back(agentY+1);
	}
	if (agentY - 1 >= 0)
	{
		posXVec.push_back(agentX);
		posYVec.push_back(agentY - 1);
	}
}

void MyAI::updatePosDir(Action act)
{
	if (act == TURN_LEFT)
	{
		if (--agentCurDir > 3) agentCurDir = 3;
	}
	else if (act == TURN_RIGHT)
	{
		if (++agentCurDir > 3) agentCurDir = 0;
	}
	else if (act == FORWARD)
	{
		//update current agent position, this is important
		switch (agentCurDir)
		{
		case 0:
			agentX++; break;
		case 1:
			agentY--; break;
		case 2:
			agentX--; break;
		case 3:
			agentY++; break;
		}
	}
}

// ======================================================================
// YOUR CODE ENDS
// ======================================================================

