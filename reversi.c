

#include "reversi.h"
#include "stdio.h"



void printBoard(char board[][26], int n) {
    
    printf ("  ");

    for (int i = 0; i < n; i++){
      char letter = 'a';
      printf ("%c", letter + i);
    }

    printf ("\n");

    for (int i = 0; i < n; i++){
      char letter = 'a';
      printf ("%c ", letter + i);
      for (int j = 0; j < n; j++){
        printf ("%c", board[i][j]);
      }
      printf ("\n");
    }
}

bool positionInBounds(int n, int row, int col) {
    if (row < 0 || row >= n || col < 0 || col >=n){
      return false;
    } else {
      return true;
    }
}

bool checkLegalInDirection(char board[][26], int n, int row, int col, char colour, int deltaRow, int deltaCol) {

    int i = 2;

    if (board[row + deltaRow][col + deltaCol] == colour || board[row + deltaRow][col + deltaCol] == 'U' || !positionInBounds(n, row, col)){
        return false;
    }

    while (1){

        if (!positionInBounds(n, (row + i*deltaRow), (col + i*deltaCol))){
            return false;
        } else if (board[row + i*deltaRow][col + i*deltaCol] == 'U'){
            return false;
        } else if (board[row + i*deltaRow][col + i*deltaCol] == colour){
            return true;
        }

        i++;
    }

    return false;
}

int makeMove(char board[26][26], int n, char turn, int *row, int *col) {
    int hor [8] = {0, 1, 1, 1, 0, -1, -1, -1};
    int ver [8] = {-1, -1, 0, 1, 1, 1, 0, -1};
    int score = 1;
    int bestScore = 0;
    int move = 1;
    *row = -1;

    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            score = 0;

            for (int k = 0; k < 8; k++){
                move = 1;
                if (board[i][j] == 'U' && checkLegalInDirection(board, n, i, j, turn, hor[k], ver[k])){
                    


                    while (board[i + (move*hor[k])][j + (move*ver[k])] != turn){
                        score++;
                        move++;
                    }                    

                }
            }

            if (score > bestScore){
            bestScore = score;
            *row = i;
            *col = j;
            }

        }
    }
    return 0;
}

void gameState(char board [26][26], int n){
    int B = 0;
    int W = 0;

    for (int row = 0; row < n; row++){
        for (int col = 0; col < n; col ++){
            if (board[row][col] == 'B'){
                B++;
            } else if (board[row][col] == 'W'){
                W++;
            }
        }
    }
    
    if ( B > W){
        printf ("B player wins.\n");
    } else if ( W > B){
        printf ("W player wins.\n");
    } else {
        printf("Draw!\n");
    }

    return;
}

int main(void) {

    int n, row, col;
    char useless, botColor, playerColor, input [3];
    bool playerTurn, botTurn;

    int hor [8] = {0, 1, 1, 1, 0, -1, -1, -1};
    int ver [8] = {-1, -1, 0, 1, 1, 1, 0, -1};

    printf ("Enter the board dimension: ");
    scanf ("%d", &n);

    printf ("Computer plays (B/W): ");
    scanf ("%c%c", &useless, &botColor);

    if (botColor == 'B'){
        botTurn = true;
        playerTurn = false;
        playerColor = 'W';
    } else {
        playerTurn = true;
        botTurn = false;
        playerColor = 'B';
    }

    char board [26][26];

    for (int i = 0; i < n; i ++){
      for (int j = 0; j < n; j++){

        if ((i == n/2 - 1 && j == n/2 - 1) || (i == n/2 && j == n/2)){
          board [i][j] = 'W';
        } else if ((i == n/2 - 1 && j == n/2) || (i == n/2 && j == n/2 - 1)){
          board [i][j] = 'B';
        } else {
          board [i][j] = 'U';
        }
      }
    }

    printBoard(board, n);

    while (1){
        if (playerTurn){
            printf ("Enter move for colour %c (RowCol): ", playerColor);
            scanf ("%s", input);
            row = input[0] - 'a';
            col = input[1] - 'a';

            if (board[row][col] == 'B' || board[row][col] == 'W' || !positionInBounds(n, row, col)){
                printf ("Invalid move.\n");
                gameState(board, n);
                return 0;
            }
            
            int move;
            bool validMove = false;

            for (int k = 0; k < 8; k++){
                if (checkLegalInDirection(board, n, row, col, playerColor, hor[k], ver[k])){
                    move = 1;
                    validMove = true;

                    while(board[row + (move*hor[k])][col + (move*ver[k])] != playerColor){

                        board[row + (move*hor[k])][col + (move*ver[k])] = playerColor;

                        move++;
                    }
                }
            } 

            if (!validMove){
                printf ("Invalid move.\n");
                gameState(board, n);
                return 0;
            }

            board[row][col] = playerColor;
            printBoard(board, n);
            botTurn = true;
            playerTurn = false;

        } else if (botTurn){
            int bestRow, bestCol, uselessOut;

            uselessOut = makeMove(board, n, botColor, &bestRow, &bestCol);

            if (bestRow == -1){
                printf ("%c player has no valid move.\n", botColor);
            } else {

                printf ("Computer places %c at %c%c.\n", botColor, (char)(bestRow + 'a'), (char)(bestCol + 'a'));
                for (int k = 0; k < 8; k++){
                    if (checkLegalInDirection(board, n, bestRow, bestCol, botColor, hor[k], ver[k])){
                        int move = 1;

                        while(board[bestRow + (move*hor[k])][bestCol + (move*ver[k])] != botColor){

                            board[bestRow + (move*hor[k])][bestCol + (move*ver[k])] = botColor;

                            move++;
                        }
                    }
                } 
            
                board[bestRow][bestCol] = botColor;
                printBoard(board, n);
            }
            playerTurn = true;
            botTurn = false;
        }

        int countU = 0;

        for (int i = 0; i < n; i++){
            for (int j = 0; j < n; j++){
                if (board [i][j] == 'U'){
                    countU++;
                }
            }
        }

        if (countU == 0){
            gameState(board, n);
            return 0;
        }
    }

return 0;
}

