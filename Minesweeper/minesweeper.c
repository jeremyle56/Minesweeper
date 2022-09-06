// Minesweeper in C, minesweeper.c
//
// Written by Jeremy
// on 5/09/2022 
//
// This program is an modified implmentation of the classic Microsoft game, Minesweeper.
// https://en.wikipedia.org/wiki/Minesweeper_(video_game)
// This program ultises simple C features including 
// int arrays, loops and if statments. 

// Included libraries 
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Difficulites and respective side lengths and number of mines
#define BEGINNER 0
#define INTERMEDIATE 1
#define ADVANCED 2 
#define BEGINNER_LENGTH 9 
#define INTERMEDIATE_LENGTH 16
#define ADVANCED_LENGTH 22 
#define BEGINNER_NUM_MINES 10
#define INTERMEDIATE_NUM_MINES 40
#define ADVANCED_NUM_MINES 99

// Global Variables 
int side_length;
int num_mines;

// Function Prototypes 
void play_minesweeper();
void select_difficulty();
void initialise_game(char board[side_length][side_length], char mines[side_length][side_length]);
void place_mines(int mine_locations[num_mines][2], char mines[side_length][side_length]);
void print_board(char board[side_length][side_length]);
void make_move(int *x, int *y);
bool is_mine(int row, int col, char mines[side_length][side_length]);
void replace_mine(int row, int col, char mines[side_length][side_length], int mine_locations[num_mines][2]);
bool play_minesweeper_util(char board[side_length][side_length], char mines[side_length][side_length],
                           int mine_locations[num_mines][2], int row, int col, int *moves_left
                           );
int count_adjacent_mines(int row, int col, int mine_locations[][2], char mines[side_length][side_length]);
int count_adjacent_mines(int row, int col, int mine_locations[][2], char mines[side_length][side_length]);
bool is_valid(int row, int col);

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                          FUNCTION IMPLEMENTATION                          //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
int main(void) {
    srand(time(NULL));
    play_minesweeper();

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                                 FUNCTIONS                                 //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
// A function to play Minesweeper game.
void play_minesweeper() {
    printf("Welcome to Minesweeper!\n");
    
    // Sets up the game (initalising boards, variables and placing mines)
    select_difficulty();
    char board[side_length][side_length], mines[side_length][side_length];
    int mine_locations[num_mines][2];
    initialise_game(board, mines);
    place_mines(mine_locations, mines); 
    int moves_left = side_length * side_length - num_mines;
    int x, y; 
    int current_move_index = 0;
    int game_over = false;
    
    // Plays minesweeper game
    while (game_over == false) {
        printf("\nCurrent Status of the Board:\n");
        print_board(board);
        make_move(&x, &y);

        // Ensures that first move is always safe
        if (current_move_index == 0) {
            if (is_mine(x, y, mines) == true) {
                replace_mine(x, y, mines, mine_locations);
            }
        }

        current_move_index++;
        game_over = play_minesweeper_util(board, mines, mine_locations, x, y, &moves_left);

        // Game is not lost and no moves left = win
        if ((game_over == false) && (moves_left == 0)) {
            printf("\033[0;32m");
            printf("  ________    _____      _____  ___________  __      __________    _______ ._.\n"
                   " /  _____/   /  _  \\    /     \\ \\_   _____/ /  \\    /  \\_____  \\   \\      \\| |\n"
                   "/   \\  ___  /  /_\\  \\  /  \\ /  \\ |    __)_  \\   \\/\\/   //   |   \\  /   |   \\ |\n"
                   "\\    \\_\\  \\/    |    \\/    Y    \\|        \\  \\        //    |    \\/    |    \\|\n"
                   " \\______  /\\____|__  /\\____|__  /_______  /   \\__/\\  / \\_______  /\\____|__  /_\n"
                   "        \\/         \\/         \\/        \\/         \\/          \\/         \\/\\/\n");
            game_over = true;
        }
    }
}

// A function to choose the difficulty level of the game.
void select_difficulty() {
    // Presents the difficulty options for the user
    printf("Select the Difficulty Level\n");
    printf("Press 0 for BEGINNER (9 * 9 Cells and 10 Mines)\n");
    printf("Press 1 for INTERMEDIATE (16 * 16 Cells and 40 Mines)\n");
    printf("Press 2 for ADVANCED (23 * 23 Cells and 99 Mines)\n");
    
    // Scans until valid difficulty
    int difficulty;
    while (scanf("%d", &difficulty) != EOF) {
        switch(difficulty) {
            case BEGINNER:      side_length = BEGINNER_LENGTH;
                                num_mines = BEGINNER_NUM_MINES;
                                return; 
            case INTERMEDIATE:  side_length = INTERMEDIATE_LENGTH;
                                num_mines = INTERMEDIATE_NUM_MINES;
                                return; 
            case ADVANCED:      side_length = ADVANCED_LENGTH;
                                num_mines = ADVANCED_NUM_MINES;
                                return;
            default:            printf("Enter a valid difficulty\n");
                                break;
        }
    }
}

// A fuction to nitialise the game.
void initialise_game(char board[side_length][side_length], char mines[side_length][side_length]) {
    for (int i = 0; i < side_length; i++) {
        for (int j = 0; j < side_length; j++) {
            board[i][j] = mines[i][j] = '-';
        }
    }
}

// A function to place the mines randomly on the board.
void place_mines(int mine_locations[num_mines][2], char mines[side_length][side_length]) {
    // Mark the points which already have mines
    int mark[side_length * side_length];
    memset(mark, false, sizeof(mark));
    
    // Continue until all random mines have been created. 
    for (int i = 0; i < num_mines; ) {
        int random = rand() % (side_length * side_length);
        int x = random / side_length;
        int y = random % side_length;

        // Add the mines if no mine is placed at this position on the board
        if (mark[random] == false) {
            // Row and Column Index of the Mine respectively 
            mine_locations[i][0] = x;
            mine_locations[i][1] = y;

            // Place the mine
            mines[mine_locations[i][0]][mine_locations[i][1]] = '*';
            mark[random] = true;
            i++;
        }
    }
}

// A function to print the curent gameplay board.
void print_board(char board[side_length][side_length]) {
    int i, j;
    printf("    ");

    // Prints column index
    for (i = 0; i < side_length; i++) {
        printf("%d  ", i);
    }
    printf("\n");

    // Loops through the rows of the array
    for (i = 0; i < side_length; i++) {
        printf("%d  ", i);
        if (i < 10) printf(" ");

        // Loops through the cols of the array
        for (j = 0; j < side_length; j++) {
            printf("%c  ", board[i][j]);
            if (j > 9) printf(" ");
        }
        printf("\n");
    }
}

// A function to get the user's move.
void make_move(int *x, int *y) {
    printf("\nEnter your move (row col): ");
    
    // Loops until valid move is entered
    while (scanf("%d %d", x, y) != EOF) {
        if (is_valid(*x, *y) == false) {
            printf("Please enter a valid move (0 <= row < %d, 0 <= col < %d): ", side_length, side_length);
        } else {
            return;
        }
    }

    // If CTRL + D is entered end the program 
    printf("\nTerminating Program\n");
    exit(1);
}

// A utility function to check whether given cell (row, col) has a mine or not.
bool is_mine(int row, int col, char mines[side_length][side_length]) {
    if (mines[row][col] == '*') {
        return true;
    } else {
        return false;
    }
}

// A function to replace the mine from (row, col) and put it to a vacant space.
void replace_mine(int row, int col, char mines[side_length][side_length], int mine_locations[num_mines][2]) {
    // Loops through the rows
    for (int i = 0; i < side_length; i++) {
        // Loops through the columns 
        for (int j = 0; j < side_length; j++) {
            // Finds the first non mine coordinate and makes it a mine
            if (mines[i][j] != '*') {
                mines[i][j] = '*';
                mines[row][col] = '-';
                
                // Finds mine being replaced in mine_locations and replaces it
                int k;
                for (k = 0; mine_locations[k][0] != row || mine_locations[k][1] != col; k++) {
                    if (mine_locations[k][0] == row && mine_locations[k][1] == col) break;
                }
                mine_locations[k][0] = i;
                mine_locations[k][1] = j;
                return;
            }
        }
    }
}

// A recursive function to play the Minesweeper game.
bool play_minesweeper_util(char board[side_length][side_length], char mines[side_length][side_length],
                           int mine_locations[num_mines][2], int row, int col, int *moves_left
                           ) {
    // Base case of recursion
    if (board[row][col] != '-') return false; 

    // Mine has been opened, game lost
    if (mines[row][col] == '*') {
        board[row][col] = '*';
        for (int i = 0; i < num_mines; i++) {
            board[mine_locations[i][0]][mine_locations[i][1]] = '*';
        }
        printf("\n");
        print_board(board);
        printf("\n\033[0;31m");
        printf("  ________                        ________                    ._.\n"
               "  /  _____/_____    _____   ____   \\_____  \\___  __ ___________| |\n"
               " /   \\  ___\\__  \\  /     \\_/ __ \\   /   |   \\  \\/ // __ \\_  __ \\ |\n"
               " \\    \\_\\  \\/ __ \\|  Y Y  \\  ___/  /    |    \\   /\\  ___/|  | \\/\\|\n"
               "  \\______  (____  /__|_|  /\\___  > \\_______  /\\_/  \\___  >__|   __\n"
               "         \\/     \\/      \\/     \\/          \\/          \\/       \\/\n");
        return true; 
    } else {
        // Calculate the number of adjacent mines and put it on the board
        int count = count_adjacent_mines(row, col, mine_locations, mines);
        (*moves_left)--;
        board[row][col] = count + '0';

        /*
        Current Cell    ->  (row, col)
        North           ->  (row - 1, col)
        South           ->  (row + 1, col)
        East            ->  (row, col + 1)
        West            ->  (row, col - 1)
        North-East      ->  (row - 1, col + 1)
        North-West      ->  (row - 1, col - 1)
        South-East      ->  (row + 1, col + 1)
        South-West      ->  (row + 1, col - 1)
        */

        // Recur for all 8 adjacent cells, checks if cells are valid first
        if (!count) {
            // North Neighbour
            if (is_valid(row - 1, col) == true) {
                if (is_mine(row - 1, col, mines) == true) {
                    play_minesweeper_util(board, mines, mine_locations, row - 1, col, moves_left);
                }
            }

            // South Neighbour 
            if (is_valid(row + 1, col) == true) {
                if (is_mine(row + 1, col, mines) == true) {
                    play_minesweeper_util(board, mines, mine_locations, row + 1, col, moves_left);
                }
            }

            // East Neighbour 
            if (is_valid(row, col + 1) == true) {
                if (is_mine(row, col + 1, mines) == true) {
                    play_minesweeper_util(board, mines, mine_locations, row, col + 1, moves_left);
                }
            }

            // West Neighbour 
            if (is_valid(row, col - 1) == true) {
                if (is_mine(row, col - 1, mines) == true) {
                    play_minesweeper_util(board, mines, mine_locations, row, col - 1, moves_left);
                }
            }

            // North-East Neighbour 
            if (is_valid(row - 1, col + 1) == true) {
                if (is_mine(row - 1, col + 1, mines) == true) {
                    play_minesweeper_util(board, mines, mine_locations, row - 1, col + 1, moves_left);
                }
            }

            // North-West Neighbour 
            if (is_valid(row - 1, col - 1) == true) {
                if (is_mine(row - 1, col - 1, mines) == true) {
                    play_minesweeper_util(board, mines, mine_locations, row - 1, col - 1, moves_left);
                }
            }

            // South-East Neighbour 
            if (is_valid(row + 1, col + 1) == true) {
                if (is_mine(row + 1, col + 1, mines) == true) {
                    play_minesweeper_util(board, mines, mine_locations, row + 1, col + 1, moves_left);
                }
            }

            // South-West Neighbour 
            if (is_valid(row + 1, col - 1) == true) {
                if (is_mine(row + 1, col - 1, mines) == true) {
                    play_minesweeper_util(board, mines, mine_locations, row + 1, col - 1, moves_left);
                }
            }
        }
        return false;
    }

}

// A function to count the number of mines in the adjacent cells. 
int count_adjacent_mines(int row, int col, int mine_locations[][2], char mines[side_length][side_length]) {
    int count = 0;

    /*
    Counts all the mines in the 8 adjacent cells
    Checks if cells are valid before processing

    Current Cell    ->  (row, col)
    North           ->  (row - 1, col)
    South           ->  (row + 1, col)
    East            ->  (row, col + 1)
    West            ->  (row, col - 1)
    North-East      ->  (row - 1, col + 1)
    North-West      ->  (row - 1, col - 1)
    South-East      ->  (row + 1, col + 1)
    South-West      ->  (row + 1, col - 1)
    */

    // North Neighbour
    if (is_valid(row - 1, col) == true) {
        if (is_mine(row - 1, col, mines) == true) count++;
    }

    // South Neighbour 
    if (is_valid(row + 1, col) == true) {
        if (is_mine(row + 1, col, mines) == true) count++;
    }

    // East Neighbour 
    if (is_valid(row, col + 1) == true) {
        if (is_mine(row, col + 1, mines) == true) count++;
    }

    // West Neighbour 
    if (is_valid(row, col - 1) == true) {
        if (is_mine(row, col - 1, mines) == true) count++;
    }

    // North-East Neighbour 
    if (is_valid(row - 1, col + 1) == true) {
        if (is_mine(row - 1, col + 1, mines) == true) count++;
    }

    // North-West Neighbour 
    if (is_valid(row - 1, col - 1) == true) {
        if (is_mine(row - 1, col - 1, mines) == true) count++;
    }

    // South-East Neighbour 
    if (is_valid(row + 1, col + 1) == true) {
        if (is_mine(row + 1, col + 1, mines) == true) count++;
    }

    // South-West Neighbour 
    if (is_valid(row + 1, col - 1) == true) {
        if (is_mine(row + 1, col - 1, mines) == true) count++;
    }

    return count;
}

// A utility function to check whether given cell (row, col) is a valid cell or not.
bool is_valid(int row, int col) {
    return (row >= 0) && (row < side_length) && (col >= 0) && (col < side_length);
}
