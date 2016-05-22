// Player.cpp
// Tim Connors @ 5/20/2016

#include "provided.h"
#include <iostream>
using namespace std;


//========================================================================
// Timer t;                 // create a timer and start it
// t.start();               // start the timer
// double d = t.elapsed();  // milliseconds since timer was last started
//========================================================================

#include <chrono>

class Timer
{
public:
	Timer()
	{
		start();
	}
	void start()
	{
		m_time = std::chrono::high_resolution_clock::now();
	}
	double elapsed() const
	{
		std::chrono::duration<double, std::milli> diff =
			std::chrono::high_resolution_clock::now() - m_time;
		return diff.count();
	}
private:
	std::chrono::high_resolution_clock::time_point m_time;
};

Timer timer;
void f(bool& startWrappingItUp)
{
	if (!startWrappingItUp)
	{
		int callcount = 0;
		for (int k = 0; k < 1000000; k++)
		{
			if (++callcount == 1000)
			{
				if (timer.elapsed() > 4900)  // have 4.9 seconds elapsed?
				{
					startWrappingItUp = true;
					return;
				}
				callcount = 0;
			}
			// ... do some computation here ...
		}
	}
}

//==================================================================
//					END OF TIMER STUFF
//==================================================================


//////////////////////////////////////////////////
//		    COMPLETED HELPER FUNCTION		    //
//   checks if the game is over and returns     //
//     an integer representing the outcome      //
//											     //
//////////////////////////////////////////////////

int completed(Scaffold scaf, int& winner, int numToWin, int turn, int lastMove) 		// "If the game isn't over (i.e., more moves are possible)" <-- are those the same thing?
{
	int num = 0;
	int i_v = 0;						// returns -1 if tie, 1 if win, 0 if neither
	int j_v = 0;
	int i = lastMove;
	int j = scaf.levels();
	int k = turn;
	winner = -1;

	while (j > 1 && scaf.checkerAt(i, j) == VACANT)
		if (scaf.checkerAt(i, j) == VACANT) j--;			// setting j to the row of the last added

	// ****~~~~ Checking for VERTICAL win ~~~~**** //
	if (scaf.levels() >= numToWin) {						// only check if there are enough levels to win vertically

		num = 1; j_v = j;
		if (j_v != 1) {
			while (scaf.checkerAt(i, j_v - 1) == k){				// only looks below the last added
				num++;
				j_v--;
				if (j_v == 1) break;
			}
		}
		if (num == numToWin) {
			winner = k;
			return 1;
		}
	}


	// ****~~~~ Checking for HORIZONTAL win ~~~~**** //
	if (scaf.cols() >= numToWin) {								// only check if there are enough columns to win horizontally
		i = lastMove; num = 1; i_v = i;
		if (i_v != 1) {
			while (scaf.checkerAt(i_v - 1, j) == k) {				// looking to the left
				num++;
				i_v--;
				if (i_v == 1) break;
			}
		}
		i_v = i;
		if (i_v != scaf.cols()) {
			//cout << endl << "k = " << k << "i_v = " << i_v << "j = " << j << "ChckerAt(i_v + 1, j) : " << scaf.checkerAt(i_v + 1, j) << endl;
			
			while (scaf.checkerAt(i_v + 1, j) == k) {				// looking to the right and adding
				num++;
				i_v++;
				if (i_v == scaf.cols()) break;
			}
		}
		if (num == numToWin) {
			winner = k;
			return 1;
		}
	}


	// ****~~~~ Checking for UPWARDS DIAGONAL win ~~~~**** //
	if (scaf.levels() >= numToWin) {							// only check for a diagonal win if possible with the number of levels

		num = 0;
		j = 1;
		for (i = 1; i <= scaf.cols() - numToWin + 1;) {			// only scan the spaces where a winning diagonal can start
			num = 0;
			if (scaf.checkerAt(i, j) == k)
			{
				num++;
				int j_v = j;
				int i_v = i;
				if (i_v != scaf.cols() && j_v != scaf.levels()) {
					while (scaf.checkerAt(i_v + 1, j_v + 1) == k){
						num++;
						j_v++; i_v++;
						if (j_v == scaf.levels() || i_v == scaf.cols()) break;
					}
				}
				if (num == numToWin) {
					winner = k;
					return 1;
				}
			}
			j++;
			if (j > numToWin - 1)		// if it's too low then its impossile to win on a diagonal
			{							// so we only check valid boxes 
				j = 1;
				i++;
			}
		}



		// ****~~~~ Checking for DOWNWARDS DIAGONAL win ~~~~**** //
		num = 0;
		j = scaf.levels();
		for (int i = 1; i <= scaf.cols() - numToWin + 1;) {			// only scan the spaces where a winning diagonal can start
			num = 0;
			if (scaf.checkerAt(i, j) == k)
			{
				num++;
				int j_v = j;
				int i_v = i;
				if (i_v != scaf.cols() && j_v != 1) {
					while (scaf.checkerAt(i_v + 1, j_v - 1) == k){
						num++;
						j_v--; i_v++;
						if (j_v == 1 || i_v == scaf.cols()) break;
					}
				}
				if (num == numToWin) {
					winner = k;
					return 1;
				}
			}
			j--;
			if (j <= numToWin - 1)		// if it's too low then its impossile to win on a diagonal
			{							// so we only check valid boxes 
				j = scaf.levels();
				i++;
			}
		}
	}

	if (scaf.numberEmpty() == 0) {
		winner = -1;					// checking if tie
		return -1;
	}



	return 0;					// otherwise, return 0
}

//==================================================================
//				END OF COMPLETED HELPER FUNCTION
//==================================================================

class HumanPlayerImpl
{
  public:
    int chooseMove(const Scaffold& s, int N, int color);
};

class BadPlayerImpl
{
  public:
    int chooseMove(const Scaffold& s, int N, int color);
};

class SmartPlayerImpl
{
  public:
    int chooseMove(const Scaffold& s, int N, int color);
};

int HumanPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
	int col = 0;
	cout << "Human, pick a column: ";							// ask for column
	cin >> col;
	while (col > s.cols() || col < 1) {
		cout << "Please enter a column 1 - " << s.cols() << " : ";
		cin >> col;
	}
	while (col > s.cols() || col < 1 || s.checkerAt(col, s.levels()) != -1)		// checks if it is a valid column
	{
		cout << "Not a valid move! Pick another column: ";
		cin >> col;
	}
	return col;													// if so, returns the column
		
}

int BadPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
	int j = s.levels();							// simply populates  spaces from left to right
	for (int i = 1; i <= s.cols(); i++) {
		if (s.checkerAt(i, j) != VACANT)
			continue;
		return i;
	}
	return 0;
}


void computerMove(Scaffold& s, int color, int& winner, int N, double& score, int& depth, Timer& timer);

void opponentMove(Scaffold& s, int color, int& winner, int N, double& score, int& depth, Timer& timer)
{
	int DEPTH_MAX = 100;							// keep track of how many times we call this function
												// since leaving the original for loop.
	//if (depth % 2 == 0)									
		//if (timer.elapsed() > 1900)						// checks on the elapsed time intermittently
			//DEPTH_MAX = depth-1;						// if the time is almost up, set a max for the depth 
												// if there is a move that can win the game for 
	for (int i = 1; i <= s.cols(); i++) {			// this player in the next move, prioritize that 
		if (!s.makeMove(i, color))
			continue;
		depth++;
		int outcome = completed(s, winner, N, color, i);
		if (outcome && winner == color){
			cout << "Opponent can win in " << depth << " moves!" << endl;
			score += -10000 + depth;
			s.undoMove(); depth--;
		}
		else if (outcome && winner == TIE_GAME) {
			score += 1;
			s.undoMove(); depth--;
		}
		else {
			if (depth < DEPTH_MAX)						// while the depth isn't insanely high, continue entering recursion						
				computerMove(s, !color, winner, N, score, depth, timer);
			else s.undoMove();
		}
			
	}
	return;
}

void computerMove(Scaffold& s, int color, int& winner, int N, double& score, int& depth, Timer& timer)
{
	int DEPTH_MAX = 100;
												// does all the same things as the opponentMove but in regards to the opposite color
	//if (depth % 2 == 0)
		//if (timer.elapsed() > 1900)
			//DEPTH_MAX = depth-11;

	for (int i = 1; i <= s.cols(); i++) {
		if (!s.makeMove(i, color))
			continue;
		depth++;
		int outcome = completed(s, winner, N, color, i);
		if (outcome && winner == color){
			cout << "Computer can win in " << depth << " moves!" << endl;
			score += 10000 - depth;
			s.undoMove(); depth--;
		}
		else if (outcome && winner == TIE_GAME) {
			score += 1;
			s.undoMove(); depth--;
		}
		else{
			if (depth < DEPTH_MAX)
				opponentMove(s, !color, winner, N, score, depth, timer);
			else s.undoMove();
		}
	}
	return;
}

int bestMove(Scaffold s, int N, int color, Timer& timer)
{
	
	double bestScore = INT_MIN;							
	int bestMove = 1;
	int winner = -2;
	
	if (s.numberEmpty() == s.cols() * s.levels() - 1 && s.levels() > 1 && s.levels() >= N && N > 3)
	{
		int i = 1; int j = 1;
		while (s.checkerAt(i, j) == VACANT)		// something I learned from a strategy guide: 
			i++;								// if there's only one peice, play on top
		return i;
	}
	
	for (int i = 1; i <= s.cols(); i++)			// loop through each possible move / open column
	{
		double score = 0;
		int depth = 0;
		int time = 0;
		if (!s.makeMove(i, !color))
			continue;
		
		if (completed(s, winner, N, !color, i))		// if the game can be won by the opponent in one move...
		{
			if (winner == !color) {
				cout << "OPPONENT ABOUT TO WIN" << endl;
				s.undoMove();
				return i;			// return right away in order to block that
			}
		}
		s.undoMove();

		s.makeMove(i, color); depth++;
		if (completed(s, winner, N, color, i))		// if the game can be won by this player in one move...
		{
			if (winner == color) {
				s.undoMove();
				return i;			// return right away
			}
		}
		
		else 
			opponentMove(s, !color, winner, N, score, depth, timer);	// otherwise, enter recursion...

		s.display();
		cout << "COL : " << i << "SCORE : " << score << endl;

		if (score >= bestScore)
		{	
			if (score == bestScore && abs(bestMove - (s.cols() / 2 + 1)) < abs(i - (s.cols() / 2 + 1)))		// if the scores of two possible routes are the same
				;																							// prioritize the column closest to the center
			else{
				bestScore = score;
				bestMove = i;
			}
		}
		s.undoMove();			// undo the moves made to end with the original scaffold
		depth--;
	}
	return bestMove;
}

int SmartPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
	timer.start(); // start tracking time...
	if (s.numberEmpty() == (s.cols() * s.levels())) return s.cols() / 2 + 1;		// If comp is the first to go, place peice nearest to the center
	int m = bestMove(s, N, color, timer);
	return m;
}


//******************** Player derived class functions *************************

// These functions simply delegate to the Impl classes' functions.
// You probably don't want to change any of this code.

HumanPlayer::HumanPlayer(string nm)
 : Player(nm)
{
    m_impl = new HumanPlayerImpl;
}
 
HumanPlayer::~HumanPlayer()
{
    delete m_impl; 
}
 
int HumanPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}

BadPlayer::BadPlayer(string nm)
 : Player(nm)
{
    m_impl = new BadPlayerImpl;
}
 
BadPlayer::~BadPlayer()
{
    delete m_impl;
}
 
int BadPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}

SmartPlayer::SmartPlayer(string nm)
 : Player(nm)
{
    m_impl = new SmartPlayerImpl;
}
 
SmartPlayer::~SmartPlayer()
{
    delete m_impl;
}
 
int SmartPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}
