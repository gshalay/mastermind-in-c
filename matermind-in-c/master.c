#include "master.h"

char legend[LEGEND_SIZE] = "\nLEGEND:\n"
                      "R = Red     O = Orange   Y = Yellow\n"
		      "W = White   P = Purple   G = Green\n\n"
		      "H = Hit  (Right colour + position)\n"
		      "B = Blow (Colour present in solution, but wrong postion)\n"
                      "M = Miss (Colour not present in solution)\n\n";

Board BLANK_BOARD = " F01   F02   F03   F04   F05   F06   F07   F08   F09   F10   SLN \n"
                    "| - | | - | | - | | - | | - | | - | | - | | - | | - | | - | | ? |\n"
		    "| - | | - | | - | | - | | - | | - | | - | | - | | - | | - | | ? |\n"
		    "| - | | - | | - | | - | | - | | - | | - | | - | | - | | - | | ? |\n"
		    "| - | | - | | - | | - | | - | | - | | - | | - | | - | | - | | ? |\n"
		    "\n"
		    "|- -| |- -| |- -| |- -| |- -| |- -| |- -| |- -| |- -| |- -|\n"
		    "|- -| |- -| |- -| |- -| |- -| |- -| |- -| |- -| |- -| |- -|\n\n";
		    
int frame_pos2idx[10][4] = { 
                             {68, 134, 200, 266}, 
                             {74, 140, 206, 272},
                             {80, 146, 212, 278},
                             {86, 152, 218, 284}, 
                             {92, 158, 224, 290}, 
                             {98, 164, 230, 296},
 	                     {104, 170, 236, 302},
                             {110, 176, 242, 308},
                             {116, 182, 248, 314},
                             {122, 188, 254, 320}
                           };
  
int stat_pos2idx[10][4] = {
                            {332, 334, 392, 394},
                            {338, 340, 398, 400},
                            {344, 346, 404, 406},
			    {350, 352, 410, 412},
                            {356, 358, 416, 418},
                 	    {362, 364, 422, 424},
         		    {368, 370, 428, 430},
 			    {374, 376, 434, 436},
			    {380, 382, 440, 442},
			    {386, 388, 446, 448}
                          };
				  
int sln_pos2idx[4] = {128, 194, 260, 326};

char colours[7] = {'R', 'O', 'Y', 'W', 'P', 'G'};

struct GameStats * stats;

char solution[5];

char guess[10];
int frameStats[3];

char * guessPtr;
char * solutionPtr;
int * frameStatsPtr;

void init_game(){
   int startTurn = rand() % 100;
   int p1Start = 1;
   int p2Start = 0;
   
   generateSolution();
   
   stats = (struct GameStats *) malloc(sizeof(struct GameStats));;

   if(startTurn % 2 == p1Start){
     strcpy(stats->playerTurn, "P1");
   }
   else if(startTurn % 2 == p2Start){
     strcpy(stats->playerTurn, "P2");
   }
   else{
     printf("Error determining turn.\n");
   }
   
   strcpy(stats->board, BLANK_BOARD);
   stats->foundSolution = false;
   stats->currentFrame = 1; 
}

void sanitizeGuess(char guess[]){
  for(int i = 0; i < strlen(guess); i++){
    guess[i] = toupper(guess[i]);
  }
  guess[4] = '\0';
  
  revealSolution(solution);
}

void getGuess(){ 
  printf("Please enter your guess starting at the top position and ending with the bottom position.\nEnter the colour characters like 'abcd': ");
  fgets(guess, sizeof(guess), stdin);
  
  sanitizeGuess(guess);
  
  determineGuessStats(guess, solution);
  
  writeGuessToBoard(stats->currentFrame, guess);
  
  stats->currentFrame++;
  
}

bool isWin(){
  if(strcmp(guess, solution) == 0){
    revealSolution(solution);
    stats->foundSolution = true;
    return true;
  } 
  return false;
}

bool isOver(){
  if(stats->currentFrame == FINAL_FRAME - 1 && !isWin()){
    revealSolution(solution);
    return true;
  }
  else{
    return false;
  }
}

void generateSolution(){
  time_t t;
  srand((unsigned) time(&t));
  
  for(int i = 0; i < 4; i++){
    int r = rand() % 6;
    solution[i] = colours[r]; 
  }
  
  solution[4] = '\0';
}
 
bool isGuessCorrectSolution(char guess[], char solution[]){

  if(strcmp(guess, solution) == 0){
    revealSolution(solution);
    return true;
  }
  
  return false;
} 

void revealSolution(char solution[]){
  stats->board[sln_pos2idx[0]] = solution[0];
  stats->board[sln_pos2idx[1]] = solution[1];
  stats->board[sln_pos2idx[2]] = solution[2];
  stats->board[sln_pos2idx[3]] = solution[3];
}

int countHits(char guess[], char solution[], int hitIndices[]){
  int hits = 0;
  char colour;
  
  
  for(int i = 0; i < 4; i++){
    colour = guess[i];
    if(hitIndices[i] == 1){
      continue;
    }
    else{
      if(colour == solution[i]){
        hitIndices[i] = 1;
	hits++;
      }    
    }
  }
  
  return hits;
}

int countOccurancesOfColour(char colour, char solution[]){
  int count = 0;
  
  for(int i = 0; i < 4; i++){
    if(colour == solution[i]){
      count++;
    }
  }
  
  return count;
}

int countBlows(int hits, int misses){
  int blows = 4;
  
  blows = blows - hits - misses;
  
  return blows;
}

int countMisses(char solution[], char guess[], int hitIndices[]){
  int misses = 0;
  
  char temp[5];
  char * retval;
  
  for(int i = 0; i < 4; i++){
    strncpy(temp, guess + i, 4);
    retval = strchr(solution, [i]);
    
    if(retval == NULL){
      misses++;
    }
  }
  
  return misses;
}

void determineGuessStats(char guess[], char solution[]){
  int hits = 0;
  int blows = 0;
  int misses = 0;
  
  int hitIndices[4];
  
  hits = countHits(guess, solution, hitIndices);
  misses = countMisses(solution, guess, hitIndices);
  blows = countBlows(hits, misses);
  
  frameStats[0] = hits;
  frameStats[1] = blows;
  frameStats[2] = misses;
  
}

void writeGuessStats(int topLeft, int topRight, int bottomLeft, int bottomRight, int fStats[]){
  int statsWritten = 0;
  
  printf("%d, %d, %d\n", fStats[0], fStats[1], fStats[2]);
  
  if(fStats[0] > 0){
    while(fStats[0] > 0){
      switch(statsWritten){
        case 0:
	  stats->board[topLeft] = 'H';
	  break;
	case 1:
	  stats->board[topRight] = 'H';
	  break;
	case 2:
	  stats->board[bottomLeft] = 'H';
	  break;
	case 3:
	  stats->board[bottomRight] = 'H';
	  break;
      }
      
      statsWritten++;
      fStats[0]--;
    }
  }
  
  if(fStats[1] > 0){
    while(fStats[1] > 0){
      switch(statsWritten){
        case 0:
	  stats->board[topLeft] = 'B';
	  break;
	case 1:
	  stats->board[topRight] = 'B';
	  break;
	case 2:
	  stats->board[bottomLeft] = 'B';
	  break;
	case 3:
	  stats->board[bottomRight] = 'B';
	  break;
      }
      
      statsWritten++;
      fStats[1]--;
    }
  }
  
  if(fStats[2] > 0){
    while(fStats[2] > 0){
      switch(statsWritten){
        case 0:
	  stats->board[topLeft] = 'M';
	  break;
	case 1:
	  stats->board[topRight] = 'M';
	  break;
	case 2:
	  stats->board[bottomLeft] = 'M';
	  break;
	case 3:
	  stats->board[bottomRight] = 'M';
	  break;
      }
      
      statsWritten++;
      fStats[2]--;
    }
  }
}

void writeFrame(int top, int upperMiddle, int lowerMiddle, int bottom, char guess[]){
  stats->board[top] = guess[0];
  stats->board[upperMiddle] = guess[1];
  stats->board[lowerMiddle] = guess[2];
  stats->board[bottom] = guess[3]; 
}

void writeGuessToBoard(int currentFrame, char guess[]){
  switch(stats->currentFrame){
    case 1:
      writeFrame(frame_pos2idx[0][0], frame_pos2idx[0][1], frame_pos2idx[0][2], frame_pos2idx[0][3], guess);
      writeGuessStats(stat_pos2idx[0][0], stat_pos2idx[0][1], stat_pos2idx[0][2], stat_pos2idx[0][3], frameStats);
      break;
    case 2:
      writeFrame(frame_pos2idx[1][0], frame_pos2idx[1][1], frame_pos2idx[1][2], frame_pos2idx[1][3], guess);
      writeGuessStats(stat_pos2idx[1][0], stat_pos2idx[1][1], stat_pos2idx[1][2], stat_pos2idx[1][3], frameStats);
      break;
    case 3:
      writeFrame(frame_pos2idx[2][0], frame_pos2idx[2][1], frame_pos2idx[2][2], frame_pos2idx[2][3], guess);
      writeGuessStats(stat_pos2idx[2][0], stat_pos2idx[2][1], stat_pos2idx[2][2], stat_pos2idx[2][3], frameStats);
      break;
    case 4:
      writeFrame(frame_pos2idx[3][0], frame_pos2idx[3][1], frame_pos2idx[3][2], frame_pos2idx[3][3], guess);
      writeGuessStats(stat_pos2idx[3][0], stat_pos2idx[3][1], stat_pos2idx[3][2], stat_pos2idx[3][3], frameStats);
      break;
    case 5:
      writeFrame(frame_pos2idx[4][0], frame_pos2idx[4][1], frame_pos2idx[4][2], frame_pos2idx[4][3], guess);
      writeGuessStats(stat_pos2idx[4][0], stat_pos2idx[4][1], stat_pos2idx[4][2], stat_pos2idx[4][3], frameStats);
      break;
    case 6:
      writeFrame(frame_pos2idx[5][0], frame_pos2idx[5][1], frame_pos2idx[5][2], frame_pos2idx[5][3], guess);
      writeGuessStats(stat_pos2idx[5][0], stat_pos2idx[5][1], stat_pos2idx[5][2], stat_pos2idx[5][3], frameStats);
      break;
    case 7:
      writeFrame(frame_pos2idx[6][0], frame_pos2idx[6][1], frame_pos2idx[6][2], frame_pos2idx[6][3], guess);
      writeGuessStats(stat_pos2idx[6][0], stat_pos2idx[6][1], stat_pos2idx[6][2], stat_pos2idx[6][3], frameStats);
      break;
    case 8:
      writeFrame(frame_pos2idx[7][0], frame_pos2idx[7][1], frame_pos2idx[7][2], frame_pos2idx[7][3], guess);
      writeGuessStats(stat_pos2idx[7][0], stat_pos2idx[7][1], stat_pos2idx[7][2], stat_pos2idx[7][3], frameStats);
      break;
    case 9:
      writeFrame(frame_pos2idx[8][0], frame_pos2idx[8][1], frame_pos2idx[8][2], frame_pos2idx[8][3], guess);
      writeGuessStats(stat_pos2idx[8][0], stat_pos2idx[8][1], stat_pos2idx[8][2], stat_pos2idx[8][3], frameStats);
      break;
    case 10:
      writeFrame(frame_pos2idx[9][0], frame_pos2idx[9][1], frame_pos2idx[9][2], frame_pos2idx[9][3], guess);
      writeGuessStats(stat_pos2idx[9][0], stat_pos2idx[9][1], stat_pos2idx[9][2], stat_pos2idx[9][3], frameStats);
      break;
  }
}

void printBoard(Board board){
  printf(legend);
  printf(board);
}

void end_game(){
  free(stats);
}