#pragma once

#include <cmath>
#include <cstdio>
#include <stack>
#include <set>
#include <iostream>
#include <utility>

using namespace std;

class AStar
{
public:
	static const int ROW = 9;
	static const int COL = 9;

	typedef pair<int, int> Pair;

	// Creating a shortcut for pair<int, pair<int, int>> type 
	typedef pair<double, pair<int, int>> pPair;

	// A structure to hold the neccesary parameters 
	struct cell
	{
		// Row and Column index of its parent 
		// Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1 
		int parent_i, parent_j;
		// f = g + h 
		double f, g, h;
	};

	AStar();
	~AStar();

	bool isValid(int row, int col);
	bool isUnBlocked(int grid[][COL], int row, int col);
	bool isDestination(int row, int col, Pair dest);
	double calculateHValue(int row, int col, Pair dest);
	Pair tracePath(cell cellDetails[][COL], Pair dest);
	Pair aStarSearch(int grid[][COL], Pair src, Pair dest);
	Pair GetPath(int grid[][COL], int row, int col);

private:
	
};

