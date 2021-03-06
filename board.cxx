#include "board.hxx"

	/** 
	 * Constructs board object of size size
	 */
 	Board::Board(size_t size):
		size_(size)
	{
	}

	/** 
	 * Begins the 2048 Game
	 */
	void Board::play(void)
	{
		//matrix holding values
		int ch;

		// Curses Initialisations //
		initscr();
		raw();
		keypad(stdscr, TRUE);
		noecho();

		//add first two playable values to the board
		generate();
		generate();

		//display board
		print();
		addstr("Press 'esc' to exit\n");

		while((ch = getch()) != 27 && !exit_flag)
		{
			clear();

	    	switch(ch)
  			{
    			case KEY_UP:
     	    	   	move_up();
        	    	break;
	    		case KEY_DOWN: 
		    		move_down();
       		    	break;
    			case KEY_LEFT:    
    				move_left();
	    	        break;
		    	case KEY_RIGHT:   
    				move_right();
        		    break;
        		case 'S':
        		case 's':
        			restart();
        			break;
    			default:    
        		    break;
			}
			if (move_flag == true)
				generate();
			if (score_ > highScore_)
				highScore_ = score_;
			print();
			addstr("Press 'esc' to exit\n");
			addstr("Press 's' to restart");
			move_flag = false;
			if (!available_moves())
			{
				exit_flag = endGame();
				if(!exit_flag)
					restart();
			}
		}

		//printw("\n\Exiting Now\n");
		endwin();

	}

	/** 
	 * Returns the board size
	 */
	size_t Board::size()
	{
		return size_;
	}

	/** 
	 * Prints the game board
	 */
	void Board::print(void)
	{
		char x;
		addstr("SCORE: ");
		addstr(boost::lexical_cast<std::string>(score_).c_str());
		addstr("\n");

		addstr("HIGH: ");
		addstr(boost::lexical_cast<std::string>(highScore_).c_str());
		addstr("\n");

		for(size_t i = 0; i < size_*6+2;++i)
		{
			addstr("~");
		}
		addstr("\n");
		for(size_t i = 0; i < size_; ++i)
		{
			for(size_t j = 0; j < size_; ++j)
			{
				if (j==0)
				{
					addstr("|");
				}
				if (board_[j][i].value() == 0)
				{
					addstr("  -- ");
				}
				
				if (board_[j][i].value() != 0)
				{
					if (board_[j][i].value() <= 9)
					{
						addstr("   ");
					}
					else if (board_[i][j].value() <= 99)
					{
						addstr("  ");
					}
					else if (board_[i][j].value() <= 999)
					{
						addstr(" ");
					}
					addstr(boost::lexical_cast<std::string>(board_[j][i].value()).c_str());
					addstr(" ");
				}
				if (j < size_)
					addstr(" ");
			}
			addstr("|\n");
		}
		for(size_t i = 0; i < size_*6+2;++i)
		{
			addstr("~");
		}
		addstr("\n");
	}

	/** 
	 * Determines which spaces on the board are not currently occupied
	 *
	 *  @return the array of unoccupied spaces
	 */
	std::vector<size_t> Board::open_spaces(void)
	{
		std::vector<size_t> temp;
		for (size_t i = 0; i < size_; ++i)
		{
			for (size_t j = 0; j < size_; ++j)
			{
				if(board_[i][j].value()==0)
				{
					temp.push_back(i);
					temp.push_back(j);
				}
			}
		}
		return temp;
	}

	/** 
	 * Creates a new box on the board with either 2 or 4 as its value in a 
	 * random open space on the board
	 */
	void Board::generate(void)
	{
		//number of seconds since January 1, 1970
		srand(time(0));

		std::vector<size_t> values = open_spaces();
		if (values.size()>1)
		{
			int pick = rand() % values.size()/2;
			board_[values[pick*2]][values[pick*2+1]] = Box(((rand()%2)+1)*2);
		}
		else
		{
			endGame();
		}
	}

	/** 
	 * Pushes boxes up, then retallies the score and generates a new piece in an
	 * available space
	 */
	void Board::move_up(void)
	{
		push_up();
		for(size_t i = 0; i < size_; ++i)
		{
			for (size_t j = 0; j < size_-1; ++j)
			{
				if (board_[i][j].value() == board_[i][j+1].value() 
					&& board_[i][j].value() != 0)
				{
					score_ += board_[i][j].merge();
					board_[i][j+1] = Box(0);
					move_flag = true;
					--j;
				}
				push_up();
			}
		}
	}

	/** 
	 * Pushes boxes down, then retallies the score and generates a new piece in an
	 * available space
	 */
	void Board::move_down(void)
	{
		push_down();
		for(size_t i = 0; i < size_; ++i)
		{
			for (size_t j = size_ - 1; j > 0; --j)
			{
				if (board_[i][j].value() == board_[i][j-1].value() 
					&& board_[i][j].value() != 0)
				{
					score_ += board_[i][j].merge();
					board_[i][j-1] = Box(0);
					move_flag = true;
					++j;
				}
				push_down();
			}
		}
	}

	/** 
	 * Pushes boxes left, then retallies the score and generates a new piece in an
	 * available space
	 */
	void Board::move_left(void)
	{
		push_left();
		for(size_t i = 0; i < size_-1; ++i)
		{
			for (size_t j = 0; j < size_; ++j)
			{
				if (board_[i][j].value() == board_[i+1][j].value() 
					&& board_[i][j].value() != 0)
				{
					score_ += board_[i][j].merge();
					board_[i+1][j] = Box(0);
					move_flag = true;
					--j;
				}
				push_left();
			}
		}
	}

	/** 
	 * Pushes boxes right, then retallies the score and generates a new piece in an
	 * available space
	 */
	void Board::move_right(void)
	{
		push_right();
		for(size_t i = size_-1; i > 0; --i)
		{
			for (size_t j = 0; j < size_; ++j)
			{
				if (board_[i][j].value() == board_[i-1][j].value() 
					&& board_[i][j].value() != 0)
				{
					score_ += board_[i][j].merge();
					board_[i-1][j] = Box(0);
					move_flag = true;
					--j;
				}
				push_right();
			}
		}
	}

	/** 
	 * Moves all possible pieces as far up as they can slide, combining like
	 * pairs of boxes
	 */
	void Board::push_up(void)
	{
		for(size_t i = 0; i < size_; ++i)
		{
			for (size_t j = size_ - 1; j > 0; --j)
			{
				if (board_[i][j-1].value()==0  && board_[i][j].value() != 0)
				{ 
					board_[i][j-1] = board_[i][j];
					board_[i][j] = Box(0);
					move_flag = true;
				}
			}
		}
	}

	/** 
	 * Moves all possible pieces as far down as they can slide, combining like
	 * pairs of boxes
	 */
	void Board::push_down(void)
	{
		for(size_t i = 0; i < size_; ++i)
		{
			for (size_t j = 0; j < size_-1; ++j)
			{
				if (board_[i][j+1].value()==0 && board_[i][j].value() != 0)
				{ 
					board_[i][j+1] = board_[i][j];
					board_[i][j] = Box(0);
					move_flag = true;
				}
			}
		}		
	}

	/** 
	 * Moves all possible pieces as far left as they can slide, combining like
	 * pairs of boxes
	 */
	void Board::push_left(void)
	{
		for (size_t i = size_ - 1; i > 0; --i)
		{
			for (size_t j = 0; j < size_; ++j)
			{
				if (board_[i-1][j].value()==0 && board_[i][j].value() != 0)
				{ 
					board_[i-1][j] = board_[i][j];
					board_[i][j] = Box(0);
					move_flag = true;
				}
			}
		}
	}

	/** 
	 * Moves all possible pieces as far right as they can slide, combining like
	 * pairs of boxes
	 */
	void Board::push_right(void)
	{
		for (size_t i = 0; i < size_-1; ++i)
		{
			for (size_t j = 0; j < size_; ++j)
			{
				if (board_[i+1][j].value()==0 && board_[i][j].value() != 0)
				{ 
					board_[i+1][j] = board_[i][j];
					board_[i][j] = Box(0);
					move_flag = true;
				}
			}
		}
	}

	/** 
	 * Determines if there are still available moves on the board
	 *
	 * @return  true if there are possible moves still on the board
	 */
	bool Board::available_moves(void)
	{
		if (open_spaces().size() <= 0)
		{
			for (size_t i = 0; i < size_-1; ++i)
			{
				for (size_t j = 0; j < size_-1; ++j)
				{
					if (board_[i][j].value() == board_[i+1][j].value() 
						|| board_[i][j].value() == board_[i][j+1].value())
						return true;
				}
			}
			return false;
		}
		return true;
	}

	/** 
	 * Ends the 2048 game and requests a restart or termination on user input
	 */
	bool Board::endGame(void)
	{
		sleep(2);
		clear();
		refresh();
		addstr("GAME OVER\n");
		addstr("Play Again? (Y/N)\n");
		char ch;
		while (true)
		{
			ch = getch();
			if (ch == 'n' || ch == 'N')
				return true;
			else if (ch == 'y' || ch == 'Y')
				return false;
		}
	}
	
	/** 
	 * Restarts the game board, clearing boxes, resetting score, and placing two
	 * new voxes on the board
	 */
	void Board::restart(void)
	{
		for (size_t i = 0; i < size_; ++i)
			{
				for (size_t j = 0; j < size_; ++j)
				{
					board_[i][j] = Box(0);
				}
			}
		score_ = 0;
		generate();
		generate();
	}
