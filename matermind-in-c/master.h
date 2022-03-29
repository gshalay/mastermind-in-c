#ifndef _MASTER_H
  #define _MASTER_H
  
  #include <stdio.h>
  #include <ctype.h>
  #include <stdlib.h>
  #include <string.h>
  #include <stdbool.h>
  #include <limits.h>
  #include <time.h>
  
  //PRE-DEFINED CONSTANTS
  #define BOARD_SIZE 500 
  #define LEGEND_SIZE 250
  #define NUM_GUESSES 8
  #define NUM_PLACES 4
  #define FINAL_FRAME 12

  //TYPEDEFS AND STRUCTURE DEFINITIONS	
  typedef char Board[BOARD_SIZE];
  
  struct GameStats{
    char playerTurn[3];
    Board board;
    bool foundSolution;
    int currentFrame; 
  };
  
  //EXTERNAL VARIABLES
  extern char legend[LEGEND_SIZE];	    
  extern Board BLANK_BOARD;
  extern int frame_pos2idx[10][4];
  extern int hit_pos2idx[10][4];
  extern int sln_pos2idx[4];
  extern char solution[5];
  extern char colours[7];
  extern struct GameStats * stats;
  
  //FUNCTION PROTOTYPES
  void init_game();
  void getGuess();
  void generateSolution();
  bool isGuessCorrectSolution(char guess[], char solution[]);
  void revealSolution(char solution[]);
  void determineGuessStats(char guess[], char solution[]);
  void writeGuessToBoard(int currentFrame, char guess[]);
  void printBoard(Board board);
  bool isWin();
  bool isOver();
  void end_game();
#endif