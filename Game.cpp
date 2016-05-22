// Game.cpp
// Tim Connors @ 5/20/2016

#include "provided.h"
#include <iostream>
using namespace std;




class GameImpl
{
  public:
    GameImpl(int nColumns, int nLevels, int N, Player* red, Player* black); 
    bool completed(int& winner) const; 
    bool takeTurn(); 
    void play(); 
    int checkerAt(int c, int r) const;
private:
	Scaffold* scaf;
	int numToWin = 0;
	Player* red;
	Player* black;
	bool turn = 0;			// RED goes first, if 1 then BLACK goes first
	int lastMove = 0;
};











GameImpl::GameImpl(int nColumns, int nLevels, int N, Player* redplayer, Player* blackplayer)
{
	scaf = new Scaffold(nColumns, nLevels);
	numToWin = N;
	red = redplayer;
	black = blackplayer;
	turn = 0; // set the player who goes first.  0 means redplayer
}













bool GameImpl::completed(int& winner) const		
{
	int num = 0;
	int i_v = 0;		// returns -1 if tie, 1 if win, 0 if neither
	int j_v = 0;
	int i = lastMove;
	int j = scaf->levels();
	int k = !turn;		// we've already swapped turns by the time this func is called, so swap back

	if (scaf->numberEmpty() >= scaf->cols() * scaf->levels() - (numToWin * 2 - 2)) {		// Check if enough spots have been filled for a win to be possible
		winner = -1;
		return 0;
	}

	while (scaf->checkerAt(i, j) == VACANT)  j--;			// setting j to the row of the last added

	// ****~~~~ Checking for VERTICAL win ~~~~**** //
	if (scaf->levels() >= numToWin) {						// only check if there are enough levels to win vertically

		num = 1; j_v = j;
		if (j_v != 1) {
			while (scaf->checkerAt(i, j_v - 1) == k){				// only looks below the last added
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

	if (scaf->cols() >= numToWin) {				// only check if there are enough columns to win horizontally
		i = lastMove; num = 1; i_v = i;
		if (i_v != 1) {
			while (scaf->checkerAt(i_v - 1, j) == k) {				// looking to the left
				num++;
				i_v--;
				if (i_v == 1) break;
			}
		}
		i_v = i;
		if (i_v != scaf->cols()) {
			while (scaf->checkerAt(i_v + 1, j) == k) {			// looking to the right
				num++;
				i_v++;
				if (i_v == scaf->cols()) break;
			}
		}
		if (num == numToWin) {
			winner = k;
			return 1;
		}
	}


	// ****~~~~ Checking for UPWARDS DIAGONAL win ~~~~**** //
	if (scaf->levels() >= numToWin) {							// only check for a diagonal win if possible with the number of levels

		num = 0;
		j = 1;
		for (i = 1; i <= scaf->cols() - numToWin + 1;) {			// Checking diagonal sequence lower left to upper right
			num = 0;
			if (scaf->checkerAt(i, j) == k)
			{
				num++;
				int j_v = j;
				int i_v = i;
				if (i_v != scaf->cols() && j_v != scaf->levels()) {
					while (scaf->checkerAt(i_v + 1, j_v + 1) == k){
						num++;
						j_v++; i_v++;
						if (j_v == scaf->levels() || i_v == scaf->cols()) break;
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
		j = scaf->levels();
		for (int i = 1; i <= scaf->cols() - numToWin + 1;) {			// Checking diagonal sequence top left to lower right
			num = 0;
			if (scaf->checkerAt(i, j) == k)
			{
				num++;
				int j_v = j;
				int i_v = i;
				if (i_v != scaf->cols() && j_v != 1) {
					while (scaf->checkerAt(i_v + 1, j_v - 1) == k){
						num++;
						j_v--; i_v++;
						if (j_v == 1 || i_v == scaf->cols()) break;
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
				j = scaf->levels();
				i++;
			}
		}
	}

	if (scaf->numberEmpty() == 0) {
		winner = -1;					// checking if tie
		return -1;
	}

	return 0;					// otherwise, return 0
}









bool GameImpl::takeTurn()
{
	int i;
	
	if (turn == 0){
		cout << "RED is going..." << endl;

		i = red->chooseMove(*scaf, numToWin, RED);		// RED moves

		if (!scaf->makeMove(i, RED)) cout << "SHIT!" << endl;
		cout << endl << "Red moves to column " << i << ": " << endl;
		turn = BLACK;					// next is black move
		lastMove = i;
		return 1;
	}
	
	if (turn == 1){
		cout << "BLACK is going..." << endl;
		i = black->chooseMove(*scaf, numToWin, BLACK);	// BLACK moves
		scaf->makeMove(i, BLACK);
		cout << endl << "Black moves to column " << i << ": " << endl;
		turn = RED;					// next is red move
		lastMove = i;
		return 1; 
	}
}








void GameImpl::play()
{
	int i = -1; bool a = 0;
	cout << "Starting game!" << endl;
	scaf->display();				// displays the starting scaffold
	do {
		takeTurn();					// take turns and display after each turn while the game is not completed
		scaf->display();
		cout << "Press enter to continue..." << endl;
		cin.ignore();
	} while (!completed(i));
	scaf->display();				// display the ending state
	if (i == -1) cout << "TIE GAME!" << endl;
	if (i == 0) cout << "RED WINS " << endl;
	if (i == 1)  cout << "BLACK WINS " << endl;
	cout << endl << "Game Over!" << endl;
}






int GameImpl::checkerAt(int c, int r) const
{
	return scaf->checkerAt(c, r);
} 







//******************** Game functions *******************************

// These functions simply delegate to GameImpl's functions.
// You probably don't want to change any of this code.

Game::Game(int nColumns, int nLevels, int N, Player* red, Player* black)
{
    m_impl = new GameImpl(nColumns, nLevels, N, red, black);
}
 
Game::~Game()
{
    delete m_impl;
}
 
bool Game::completed(int& winner) const
{
    return m_impl->completed(winner);
}

bool Game::takeTurn()
{
    return m_impl->takeTurn();
}

void Game::play()
{
    m_impl->play();
}
 
int Game::checkerAt(int c, int r) const
{
    return m_impl->checkerAt(c, r);
}
