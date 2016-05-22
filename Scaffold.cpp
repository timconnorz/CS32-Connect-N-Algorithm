// Scaffold.cpp
// Tim Connors @ 5/20/2016

#include "provided.h"
#include <vector>
#include <iostream>
#include <stack>
using namespace std;

class ScaffoldImpl
{
public:
	ScaffoldImpl(int nColumns, int nLevels);
	ScaffoldImpl(const Scaffold& scaf);
	int cols() const;
	int levels() const;
	int isValid(int col) const;
	int numberEmpty() const;
	int checkerAt(int column, int level) const;
	friend void undo2(ScaffoldImpl& scaf, int col);
	void display() const;
	bool makeMove(int column, int color);
	int undoMove();
private:
	int nCols;
	int nLevs;
	vector<int>* board;		// a 1D vector to track the board
	stack<int> moveStack;
};


ScaffoldImpl::ScaffoldImpl(int nColumns, int nLevels)
{
	nCols = nColumns;
	nLevs = nLevels;
	board = new vector<int>(nColumns * nLevels);
	for (int i = 0; i < nColumns * nLevels; i++) {
		(*board)[i] = VACANT;
	}
}


int ScaffoldImpl::cols() const
{
	return nCols;
}

int ScaffoldImpl::levels() const
{
	return nLevs;
}

int ScaffoldImpl::isValid(int col) const		// returns -1 if not valid otherwise returns
													//	level of insertion starting at 0
{
	if (col < 1 || col > nCols){			// not a valid column
		return -1;
	}
	for (int i = col - 1; i < nCols * nLevs; i += nCols)	// returns the first vacant spot in that column
	{
		if ((*board)[i] == VACANT){
			return i / nCols;
		}
	}
	return -1;
}

int ScaffoldImpl::numberEmpty() const
{
	int number = 0;
	for (int i = 0; i < nCols * nLevs; i++)
	{
		if ((*board)[i] == -1){
			number++;
		}
	}
	return number;
}

int ScaffoldImpl::checkerAt(int column, int level) const 
{
	int l = level - 1;
	int c = column - 1;
	return (*board)[l*nCols + c];		// translation from 2 coordinate system to my 1D vector
}

void ScaffoldImpl::display() const
{
	cout << endl;
	for (int i = nCols * nLevs - nCols; i >= 0; i -= nCols)
	{
		cout << "|";
		for (int j = i; j < i + nCols; j++)			// prints out the scaffold in the correct formatting
		{
			if ((*board)[j] == RED) cout << "R" << "|";
			else if ((*board)[j] == BLACK) cout << "B" << "|";
			else if ((*board)[j] == VACANT) cout << " " << "|";
			else cout << (*board)[j] << "|";
		}
		cout << endl;
	}
	cout << "+";
	for (int i = 0; i < nCols; i++)
		cout << "-+";
	cout << endl;
}

bool ScaffoldImpl::makeMove(int column, int color)
{
	int v = isValid(column);					// check the the validity of the move..
	if (v > -1)
	{
		int level = v;
		int lastAdded = level*nCols + (column - 1);
		(*board)[lastAdded] = color;
		moveStack.push(lastAdded);				// add the move to the moveStack
		return 1;
	}
	else
		return 0;

}

int ScaffoldImpl::undoMove()
{
	int lastMove = moveStack.top();
	if (moveStack.size() == 0 || lastMove < 0) {		// if there are no moves on the stack return 0;
		cout << "cant undo!" << endl;
		return 0;
	}
	moveStack.pop();					// pop off the stack
									// reset that position to vacant
	(*board)[lastMove] = VACANT;
	return lastMove;
	
}

//******************** Scaffold functions *******************************

// These functions simply delegate to ScaffoldImpl's functions.
// You probably don't want to change any of this code.

Scaffold::Scaffold(int nColumns, int nLevels)
{
    m_impl = new ScaffoldImpl(nColumns, nLevels);
}
 
Scaffold::~Scaffold()
{
    delete m_impl;
}
 
Scaffold::Scaffold(const Scaffold& other)
{
    m_impl = new ScaffoldImpl(*other.m_impl);
}
 
Scaffold& Scaffold::operator=(const Scaffold& rhs)
{
    if (this != &rhs)
    {
        Scaffold temp(rhs);
        swap(m_impl, temp.m_impl);
    }
    return *this;
}
 
int Scaffold::cols() const
{
    return m_impl->cols();
}

int Scaffold::levels() const
{
    return m_impl->levels();
}

int Scaffold::numberEmpty() const
{
    return m_impl->numberEmpty();
}

int Scaffold::checkerAt(int column, int level) const // ERROR ************CHANGED RETURN TYPE TO STRING
{
    return m_impl->checkerAt(column, level);
}
 
void Scaffold::display() const
{
    m_impl->display();
}
 
bool Scaffold::makeMove(int column, int color)
{
    return m_impl->makeMove(column, color);
}
 
int Scaffold::undoMove()
{
    return m_impl->undoMove();
}
