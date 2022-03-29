#include "master.h"

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

int main(int argc, char * argv[]){ 
  init_game();
  
  while(stats->foundSolution == false && !isOver()){
    if(!isWin()){
      printBoard(stats->board);
      getGuess();
    }
    else{
      break;
    }
  };
  
  if(stats->foundSolution == true){
    printBoard(stats->board);
    printf("Congrats! You guessed the correct solution on frame %d!\n", (stats->currentFrame - 1));
  }
  else if(!isWin() && isOver()){
    printBoard(stats->board);
    printf("Game over! Better luck next time!\n");
  }
  
  end_game();
}
