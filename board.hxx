#ifndef BOARD_HXX
#define BOARD_HXX

#include "box.hxx"
#include <iostream>
#include <curses.h>
#include <vector>
#include <stdlib.h>
#include <boost/lexical_cast.hpp>

class Board
{
	public:
		//constructor taking a size value to create a square matrix
		Board(size_t size);

		//begins the game
		void play(void);

		//return the size of the board
		size_t size();

	private:
		//print out the matrix holding the board
		void print(void);

		//returns an array of the open spaces on the board
		std::vector<size_t> open_spaces(void);

		//add a number to the board
		void generate(void);

		//movement
		void move_up(void);
		void move_down(void);
		void move_left(void);
		void move_right(void);

		//push all blocks as far as they can go without merging
		void push_up(void);
		void push_down(void);
		void push_left(void);
		void push_right(void);

		//returns true if there are any legal moves left on the board
		bool available_moves(void);

		//prompt for end of game screen
		bool endGame(void);

		void restart(void);

		//one dimensional size of the matrix
		size_t size_;

		Box board_[4][4];

		size_t score_ = 0;

		size_t highScore_ = 0;

		bool move_flag = false;

		bool exit_flag = false;
};

#endif