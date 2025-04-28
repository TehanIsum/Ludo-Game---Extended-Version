#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>

//Constants for Unchange Values
#define NUMOFPLAYERS 4
#define NUMOFPIECES 4
#define MYSTERY_CELL_DUARTION 4
#define BHAWANA_POSITION 9
#define KOTUWA_POSITION 27
#define PITA_KOTUWA_POSITION 46

struct details
{
    char color[7];
    int number[NUMOFPIECES];
    int pieceLocation[NUMOFPIECES];
    int piecesAtBase;
    int piecesAtHome;
    int pieceDirection[4];
    int approachCellPassCount[4];
    int hasCaptured[4];
    int sickOrEnergized[4];
};

struct game
{
    int gameRound;
    int effetRoundsRemaining;
    int myteryCellValue;
    int previousMysteryCellValue;
};

//Function to generate dice roll values from 1 to 6
short diceRoll();

//Function to generate number 0 and 1 randomly
short coinToss();

//Function to finding the game start player
short firstPlayer(int yellowValue,int blueValue,int redValue,int greenValue);

//Function to making player order for dice roll
void diceRollOrder(int playerIndex);

//Function to first dice roll for find the starting player
void firstDiceRoll();

//Function to return staring location for each color player
int startingLocation(int currentPlayerIndex);

//Function to dice roll values checking and taking actions
void diceRollCheck(int currentPlayerIndex,int pieceIndex);

// Function to teleport a piece based on the mystery cell
void teleportPiece(int currentPlayerIndex,int pieceIndex,int destination);

// Function to spawn the mystery cell on an empty cell
int spwanMysteryCell();

// Function to handle the mystery cell logic
void handleMysteryCell(int currentPlayerIndex,int pieceIndex);

//Function for piece capturing
void capture(int currentPlayerIndex, int pieceLocation,int capturedBy);

//Function to piece movemnet
void moovePiece(int currentPlayerIndex ,int pieceIndex ,int diceRollValue);

//Game function
void runGame();

#endif