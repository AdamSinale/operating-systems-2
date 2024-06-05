/*
    This file contains the implementation of the tic-tac-toe game.
    The game is played between the computer and the user.
    

    The computer plays as "X" and the user plays as "O".


    FFLUSH!!!!!!!!!!!!!!!!!!!!!!!


*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to insert the strategy array.
int validate_strategy(const char *strategy) {
    if (strlen(strategy) != 9){ 
        printf("Error (put exactly 9 steps!)\n");
        return 0;
    }
    int digits[10] = {0}; //
    for (int i = 0; i < 9; i++) {
        if (strategy[i] < '1' || strategy[i] > '9'){
            printf("Error (steps can be only 1-9!)\n");
            return 0;
        }
        if (digits[strategy[i] - '0']++){
            printf("Error (only 1 of each step!)\n");
            return 0;
        }
    }
    return 1;
}

// Function to print the board.
void print_board(const char board[9]) {
    for (int i = 0; i < 6; i=i+3) {
        printf("%c|%c|%c\n", board[i], board[i+1], board[i+2]);
        printf("------\n");
    }
        printf("%c|%c|%c\n", board[6], board[7], board[8]);
}

// Function to check if the player has won.
int check_win(const char board[9]) {
    const int win_conditions[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, // 
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, // 
        {0, 4, 8}, {2, 4, 6}             // 
    };
    for (int i = 0; i < 8; i++) {
        if (board[win_conditions[i][0]] == board[win_conditions[i][1]] &&
            board[win_conditions[i][2]] == board[win_conditions[i][1]] &&
            board[win_conditions[i][2]] != ' ') {
            return 1;
        }
    }
    return 0;
}

void ttt(const char *strategy) {
    if (!validate_strategy(strategy)) {
        return;
    }

    int priority[9];
    for (int i = 0; i < 9; i++) {
        priority[i] = strategy[i] - '1'; // adjust for matrix indexing.   
    }

    char board[9] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };

    print_board(board);
    for (int turn = 0; turn < 9; turn++) {
        if (turn % 2 == 0) { // if its the computer's turn.
            printf("my turn man\n");
            for(int i = 0; i < 9; i++) {
                int pos = priority[i];
                if(board[pos] == ' ') { // if the position is empty.
                    board[pos] = 'X';
                    printf("%d\n", pos+1); // 
                    break;
                }
            }
        } else { // player's turn.
            char c_move;
            printf("It's your turn human\n");
            while (1) {
            if (scanf(" %c", &c_move) != 1){  // if the input is invalid.
                printf("Thers an ERROR, Please try again.\n");
            } else {
                int move = c_move - '0';
                if(move < 1 || move > 9 || board[move - 1] != ' '){
                    if(move == 0){ return; }
                    printf("Invalid move. Please try again.\n");
                } else{
                    printf("So your choise is %d, I see\n", move);
                    board[move - 1] = 'O';
                    break;
                }
            }
            }
        }

        print_board(board);
        printf("\n");

        if (check_win(board)) {
            if(turn % 2 == 0){ printf("I won\n"); }
            else{printf("I lost\n");}
            return;
        }
        fflush(stdout);
    }

    printf("DRAW\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Error\n");
        return 1;
    }
    ttt(argv[1]);
    return 0;
}
