#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>
#include "types.h"

struct details piece[] = {
    {"Yellow", {1, 2, 3, 4},{100,100,100,100},4,0,{-1,-1,-1,-1},{0,0,0,0},{0,0,0,0},{-1,-1,-1,-1}},
    {"Blue", {1, 2, 3, 4},{100,100,100,100},4,0,{-1,-1,-1,-1},{0,0,0,0},{0,0,0,0},{-1,-1,-1,-1}},
    {"Red", {1, 2, 3, 4},{100,100,100,100},4,0,{-1,-1,-1,-1},{0,0,0,0},{0,0,0,0},{-1,-1,-1,-1}},
    {"Green", {1, 2, 3, 4},{100,100,100,100},4,0,{-1,-1,-1,-1},{0,0,0,0},{0,0,0,0},{-1,-1,-1,-1}}
};


struct game gameData = {0,-1,-1,-1};

//Array for store first dice roll value
int firstDiceRollValue[NUMOFPLAYERS];

//Dice roll order
int playerOrder[NUMOFPLAYERS];

//
int diceRollValues[4];//............................................................................................................

//Approach Cell value for each color
int approachCells[4] = {0,13,26,39};

//Player Winning Order
int winningOrder[4];

//Generate Dice Roll Value from 1 to 6
short diceRoll()
{
    return (rand() % 6) + 1;
}

//Generate number 1 and 0 randomly
short coinToss()
{
    return rand() % 2;
}

//Function to finding the game start player
short firstPlayer(int yellowValue,int blueValue,int redValue,int greenValue)
{
    //Returned highest dice roll value player as the dice roll starting player
    int maxValue = yellowValue;
    int playerIndex = 0;

    if (blueValue > maxValue) 
    {
        maxValue = blueValue;
        playerIndex = 1;
    }

    if (redValue > maxValue) 
    {
        maxValue = redValue;
        playerIndex = 2;
    }

    if (greenValue > maxValue) 
    {
        maxValue = greenValue;
        playerIndex = 3;
    }

    return playerIndex;
}

//Function to making player order for dice roll
void diceRollOrder(int playerIndex)
{
    for(int i = 0;i < NUMOFPLAYERS;i++)
    {
        playerOrder[i] = (playerIndex + i) % 4;
    }
}

//Function to first dice roll for find the starting player
void firstDiceRoll()
{
    printf("The Red player has four pieces named R1,R2,R3,R4.\n");
    printf("The Green player has four pieces named G1,G2,G3,G4.\n");
    printf("The Yellow player has four pieces named Y1,Y2,Y3,Y4.\n");
    printf("The Blue player has four pieces named B1,B2,B3,B4.\n");

    //First dice roll
    for(int i = 0;i < NUMOFPLAYERS;i++)
    {
        firstDiceRollValue[i] = diceRoll();
        printf("[%s] rolls %d \n",piece[i].color,firstDiceRollValue[i]);
    }

    printf("[%s] Player has the highest roll and will begin the game. \n",piece[firstPlayer(firstDiceRollValue[0],firstDiceRollValue[1],firstDiceRollValue[2],firstDiceRollValue[3])].color);

    diceRollOrder(firstPlayer(firstDiceRollValue[0],firstDiceRollValue[1],firstDiceRollValue[2],firstDiceRollValue[3]));

}

//Function to return staring location for each color player
int startingLocation(int currentPlayerIndex)
{
    for(int i = 0;i < 4;i++)
    {
        switch(currentPlayerIndex)
        {
            case 0:
                return 2;

            case 1:
                return 15;

            case 2:
                return 28;

            case 3:
                return 41;
        }
    }
}

// Function to teleport a piece based on the mystery cell
void teleportPiece(int currentPlayerIndex,int pieceIndex,int destination)
{
    int pieceLocation = piece[currentPlayerIndex].pieceLocation[pieceIndex];

    // Apply effects based on the destination
    switch (destination)
    {
    case BHAWANA_POSITION:
        piece[currentPlayerIndex].pieceLocation[pieceIndex] = BHAWANA_POSITION;
        printf("[%s] player piece %d teleported to Bhawana.\n",piece[currentPlayerIndex].color,pieceIndex + 1);
        if(coinToss() == 1)
        {
            piece[currentPlayerIndex].sickOrEnergized[pieceIndex] = 1; //Energized
            printf("[%s] player piece %d feels energized,and movement speed doubles.\n",piece[currentPlayerIndex].color,pieceIndex + 1);
            gameData.effetRoundsRemaining = 4;
        }
        else
        {
            piece[currentPlayerIndex].sickOrEnergized[pieceIndex] = 0; //SICK
            printf("[%s] player piece %d feels sick,and movement speed halves.\n",piece[currentPlayerIndex].color,pieceIndex + 1);
            gameData.effetRoundsRemaining = 4;
        }
        break;

    case KOTUWA_POSITION:
        piece[currentPlayerIndex].pieceLocation[pieceIndex] = KOTUWA_POSITION;
        printf("[%s] player piece %d teleported to Kotuwa.\n",piece[currentPlayerIndex].color,pieceIndex + 1);
        gameData.effetRoundsRemaining = 4;
        break;

    case PITA_KOTUWA_POSITION:
        piece[currentPlayerIndex].pieceLocation[pieceIndex] = PITA_KOTUWA_POSITION;
        printf("[%s] player piece %d teleported to Pita-Kotuwa.\n",piece[currentPlayerIndex].color,pieceIndex + 1);
        if(piece[currentPlayerIndex].pieceDirection[pieceIndex] == 1) //ClockWise changes into Counter-ClockWise
        {
            piece[currentPlayerIndex].pieceDirection[pieceIndex] = 0 ; 
            printf("The [%s] piece %d,Which was moving clockwise has changed to moving counter-clockwise.\n",piece[currentPlayerIndex].color,pieceIndex + 1);
        }
        else
        {
            teleportPiece(currentPlayerIndex,pieceIndex,KOTUWA_POSITION);
            printf("The [%s] piece %d,Which was moving counter-clockwise direction.Teleported to Kotuwa from Pita-Kotuwa.\n",piece[currentPlayerIndex].color,pieceIndex + 1);
        }
        break;

    case 4:
        piece[currentPlayerIndex].pieceLocation[pieceIndex] = 100; //Piece teleport to the base
        printf("[%s] player piece %d teleported to Base.\n",piece[currentPlayerIndex].color,pieceIndex + 1);
        break;

    case 5:
        piece[currentPlayerIndex].pieceLocation[pieceIndex] = startingLocation(currentPlayerIndex); //Piece teleport to the starting location
        printf("[%s] player piece %d teleported to Starting Location.\n",piece[currentPlayerIndex].color,pieceIndex + 1);
        break;

    case 6:
        piece[currentPlayerIndex].pieceLocation[pieceIndex] = approachCells[currentPlayerIndex]; //Piece teleport to the approach cell
        printf("[%s] player piece %d teleported to Approach Cell.\n",piece[currentPlayerIndex].color,pieceIndex + 1);
        break;
    }
}

// Function to spawn the mystery cell on an empty cell
int spwanMysteryCell()
{
    int mysteryCellPosition;
    int isOccupied; 
    int previousMysteryCell = gameData.previousMysteryCellValue;
    
    do
    {
        mysteryCellPosition = rand() % 52 ; //Mystery cell Generate
        isOccupied = false;

        for(int playerIndex = 0;playerIndex < NUMOFPLAYERS;playerIndex++) //Loop player
        {
            for(int pieceIndex = 0;pieceIndex < NUMOFPIECES;pieceIndex++)//Loop piece
            {
                if(piece[playerIndex].pieceLocation[pieceIndex] == mysteryCellPosition) //check if player is occupied that cell
                {
                    isOccupied = true;
                    break;
                }
            }
            if(isOccupied) break;
        }
        gameData.myteryCellValue = mysteryCellPosition;
    } 
    while (isOccupied && previousMysteryCell == mysteryCellPosition);

    printf("A mystery cell has spawned in location %d and will be at this location for the next four rounds.\n",mysteryCellPosition);

    gameData.previousMysteryCellValue= mysteryCellPosition;


    return mysteryCellPosition; 
}

// Function to handle the mystery cell logic
void handleMysteryCell(int currentPlayerIndex,int pieceIndex)
{
    int isPieceOnBoard = -1;
    int game_Round;
    int mysteryPosition;
    
    for(int i = 0;i < NUMOFPLAYERS;i++)
    {
        if(piece[i].piecesAtHome != 4)
        {
            isPieceOnBoard = true;
            game_Round = gameData.gameRound;
            break;
        }
    }

    if(isPieceOnBoard && gameData.gameRound == game_Round + 2)
    {
        if(gameData.previousMysteryCellValue = -1)
        {
            mysteryPosition = spwanMysteryCell(); //To generate a mystery cell
            gameData.effetRoundsRemaining = MYSTERY_CELL_DUARTION;
        }

        else if(gameData.effetRoundsRemaining > 0)
        {
            gameData.effetRoundsRemaining--; 
        }
        else
        {
            gameData.myteryCellValue = -1;//Remove mystery cell after duration
        }

        if(piece[currentPlayerIndex].pieceLocation[pieceIndex] == gameData.myteryCellValue)
        {
            int teleportOption = diceRoll(); //Randomly select the teleport destination
            switch(teleportOption)
            {
                case 1:
                    teleportPiece(currentPlayerIndex,pieceIndex,BHAWANA_POSITION);
                    break;
                case 2:
                    teleportPiece(currentPlayerIndex,pieceIndex,KOTUWA_POSITION);
                    break;

                case 3:
                    teleportPiece(currentPlayerIndex,pieceIndex,PITA_KOTUWA_POSITION);
                    break;
                case 4:
                    teleportPiece(currentPlayerIndex,pieceIndex,4);
                    break;
                case 5:
                    teleportPiece(currentPlayerIndex,pieceIndex,5);
                    break;
                case 6:
                    teleportPiece(currentPlayerIndex,pieceIndex,6);
                    break;
            }
        }
    }
}

//Function for piece capturing
void capture(int currentPlayerIndex, int pieceLocation,int capturedBy) 
{
    for (int i = 0; i < NUMOFPLAYERS; i++) {
        if (i != currentPlayerIndex) {  // Check only the opponent players
            for (int j = 0; j < NUMOFPIECES; j++) {
                if (piece[i].pieceLocation[j] == pieceLocation && pieceLocation >= 0 && pieceLocation <= 52) {
                    // Opponent's piece is captured
                    printf("[%s] piece %d lands on cell %d, captures [%s] piece %d and returns it to base.\n",piece[currentPlayerIndex].color,capturedBy + 1,pieceLocation,piece[i].color,j + 1);
                    
                    // Return the opponent's piece to the base
                    piece[i].pieceLocation[j] = 100;
                    piece[i].piecesAtBase++;

                    piece[currentPlayerIndex].hasCaptured[capturedBy] = 1;
                    piece[i].hasCaptured[j] = 0;
                    piece[i].approachCellPassCount[j] = 0;
                    
                    printf("[%s] player now has %d pieces on the board and %d pieces on the base.\n",piece[i].color,4 - (piece[i].piecesAtBase + piece[i].piecesAtHome),piece[i].piecesAtBase);

                    diceRollCheck(currentPlayerIndex,capturedBy); //Bonus Chance for capture
                }
            }
        }
    }
}

//Piece movement function
void moovePiece(int currentPlayerIndex ,int pieceIndex ,int diceRollValue)
{
    int pieceLocation = piece[currentPlayerIndex].pieceLocation[pieceIndex];
    int pieceDirection;
    
    if(pieceLocation == 100 && diceRollValue == 6)
    {
        //Base to the starting location
        pieceLocation = startingLocation(currentPlayerIndex);
        piece[currentPlayerIndex].piecesAtBase--;
        printf("[%s] player moves piece %d to the starting point %d \n",piece[currentPlayerIndex].color,pieceIndex + 1,pieceLocation);
        printf("[%s] player now has %d pieces on the board and %d pieces on the base.\n",piece[currentPlayerIndex].color,NUMOFPIECES-(piece[currentPlayerIndex].piecesAtBase + piece[currentPlayerIndex].piecesAtHome),piece[currentPlayerIndex].piecesAtBase);

        pieceDirection = coinToss();//Select piece direction
        if(pieceDirection == 1)
        {
            piece[currentPlayerIndex].pieceDirection[pieceIndex] = 1; // ClockWise Direction
            printf("[%s] player piece %d move clockwise.\n",piece[currentPlayerIndex].color,pieceIndex + 1); 
        }
        else
        {
            piece[currentPlayerIndex].pieceDirection[pieceIndex] = 0; //Counter-ClockWise Durection
            printf("[%s] player piece %d move Counter-clockwise.\n",piece[currentPlayerIndex].color,pieceIndex + 1);
        }
    }
    //ClockWise Direction piece movement
    else if(pieceLocation >= 0 && pieceLocation <= 52  && (piece[currentPlayerIndex].pieceDirection[pieceIndex] == 1))
    {
        int beforePieceLocation = pieceLocation;
        //Move forward through standard path
        pieceLocation += diceRollValue;

        //mysteryCell
        if(pieceLocation == gameData.myteryCellValue)
        {
            if(piece[currentPlayerIndex].sickOrEnergized[pieceIndex] == 1)
            {
                pieceLocation += diceRollValue;
            }
            else if(piece[currentPlayerIndex].sickOrEnergized[pieceIndex] == 0)
            {
                pieceLocation = pieceLocation - (diceRollValue / 2);
            }
            else if(pieceLocation == KOTUWA_POSITION && gameData.effetRoundsRemaining != 0)
            {
                piece[currentPlayerIndex].pieceLocation[pieceIndex] = KOTUWA_POSITION;
                printf("[%s] piece %d attends briefing and cannot moove for four rounds.\n",piece[currentPlayerIndex].color,pieceIndex + 1);
            }

            if(gameData.effetRoundsRemaining > 0)
            {
                gameData.effetRoundsRemaining--;
                if(gameData.effetRoundsRemaining == 0)
                {
                    piece[currentPlayerIndex].sickOrEnergized[pieceIndex] = -1;
                }
            }
            
        }

        if(pieceLocation >= 52)
        {
            pieceLocation -= 52;
        }

        if(pieceLocation == approachCells[currentPlayerIndex])
        {
            printf("[%s] player piece %d reached the approach cell.Ready to enter to home straight path.\n", piece[currentPlayerIndex].color, pieceIndex + 1);
            pieceLocation = approachCells[currentPlayerIndex]; //To identify entering the home path
        }

        else if (beforePieceLocation < approachCells[currentPlayerIndex] && pieceLocation > approachCells[currentPlayerIndex] && currentPlayerIndex != 0)
        {
            if(piece[currentPlayerIndex].hasCaptured[pieceIndex])
            {
                pieceLocation = -(pieceLocation - approachCells[currentPlayerIndex]); // Move to home straight path
            }

            else
            {
                printf("[%s] player piece %d needs to capture an opponent before entering the home straight path.\n", piece[currentPlayerIndex].color, pieceIndex + 1);
            }
            
        }

        else if (beforePieceLocation >= 47 && pieceLocation <= 5 && currentPlayerIndex == 0) //Only for Yellow Player
        {
            if(piece[currentPlayerIndex].hasCaptured[pieceIndex])
            {
                pieceLocation = -(pieceLocation - approachCells[currentPlayerIndex]);
            }

            else
            {
                printf("[%s] player piece %d needs to capture an opponent before entering the home straight path.\n", piece[currentPlayerIndex].color, pieceIndex + 1);
            }
        }

        printf("[%s] piece %d moved to position %d on the board.\n", piece[currentPlayerIndex].color, pieceIndex + 1, pieceLocation);

        // Check for capture after moving the piece
        capture(currentPlayerIndex, pieceLocation,pieceIndex);
    }

    //Counter-ClockWise piece movement
    else if(pieceLocation >= 0 && pieceLocation <= 52 && (piece[currentPlayerIndex].pieceDirection[pieceIndex] == 0))
    {
        int beforePieceLocation = pieceLocation;
        //Move forward through standard path
        pieceLocation -= diceRollValue;

        int approachCellPassCount = piece[currentPlayerIndex].approachCellPassCount[pieceIndex];

        //mysteryCell
        if(pieceLocation == gameData.myteryCellValue)
        {
            if(piece[currentPlayerIndex].sickOrEnergized[pieceIndex] == 1)
            {
                pieceLocation -= diceRollValue;
            }
            else if(piece[currentPlayerIndex].sickOrEnergized[pieceIndex] == 0)
            {
                pieceLocation = pieceLocation +(diceRollValue / 2) + 1;
            }

            if(gameData.effetRoundsRemaining > 0)
            {
                gameData.effetRoundsRemaining--;
                if(gameData.effetRoundsRemaining == 0)
                {
                    piece[currentPlayerIndex].sickOrEnergized[pieceIndex] = -1;
                }
            }
            
        }
        
        if(pieceLocation < 0)
        {
            pieceLocation += 52;
        }

        if(pieceLocation == approachCells[currentPlayerIndex])
        {
            approachCellPassCount += 1;
            
            if(approachCellPassCount == 1)
            {
                printf("[%s] piece %d reached the approach cell for the first time.Must need to pass approach cell again.\n", piece[currentPlayerIndex].color, pieceIndex + 1);
            }
            else
            {
                printf("[%s] piece %d reached the approach cell for second time.Ready to enter to home straight path.\n", piece[currentPlayerIndex].color, pieceIndex + 1);
            }
            pieceLocation = approachCells[currentPlayerIndex]; //To identify entering the home path
        }

        else if (beforePieceLocation > approachCells[currentPlayerIndex] && pieceLocation < approachCells[currentPlayerIndex] && currentPlayerIndex != 0 &&  piece[currentPlayerIndex].approachCellPassCount[pieceIndex] >= 2 )
        {
            
            if(piece[currentPlayerIndex].hasCaptured[pieceIndex])
            {
                pieceLocation = -(approachCells[currentPlayerIndex] - pieceLocation); // Move to home straight path
            }

            else
            {
                printf("[%s] player piece %d needs to capture an opponent before entering the home straight path.\n", piece[currentPlayerIndex].color, pieceIndex + 1);
            }
        }

        else if (beforePieceLocation <= 5 && pieceLocation >= 47 && currentPlayerIndex == 0 && piece[currentPlayerIndex].approachCellPassCount[pieceIndex] >= 2) //Only for Yellow Player
        {
            
            if(piece[currentPlayerIndex].hasCaptured[pieceIndex])
            {
                pieceLocation = -(52 - (approachCells[currentPlayerIndex] - pieceLocation));
            }

            else
            {
                printf("[%s] player piece %d needs to capture an opponent before entering the home straight path.\n", piece[currentPlayerIndex].color, pieceIndex + 1);
            }      
        }

        printf("[%s] piece %d moved to position %d on the board.\n", piece[currentPlayerIndex].color, pieceIndex + 1, pieceLocation);

        // Check for capture after moving the piece
        capture(currentPlayerIndex, pieceLocation,pieceIndex);
    }

    else if (pieceLocation < 0 && pieceLocation > -6)
    {
        int remainingSteps = 6 + pieceLocation;

        if(diceRollValue <= remainingSteps)
        {
            pieceLocation -= diceRollValue;

            if(pieceLocation == -6)
            {
               printf("[%s] piece %d has reached home!\n", piece[currentPlayerIndex].color, pieceIndex + 1);
               piece[currentPlayerIndex].piecesAtHome++ ;
               pieceLocation = -6; //To record piece is in home 
            }

            else
            {
                printf("[%s] piece %d moved to position %d in the home straight path.\n", piece[currentPlayerIndex].color, pieceIndex + 1,-pieceLocation);
            }
        }

        else
        {
             printf("[%s] rolled %d but needs exactly %d or less to move in the home straight path.\n", piece[currentPlayerIndex].color, diceRollValue, remainingSteps);
        }
    }

    piece[currentPlayerIndex].pieceLocation[pieceIndex] = pieceLocation;  // Update the piece location
    
}

//Dice Roll value will check in this function
void diceRollCheck(int currentPlayerIndex,int pieceIndex)
{
    int diceRollCount = 0;
    while(1)
    {
        int diceRollValue = diceRoll();
        diceRollCount++;
        printf("[%s] rolls %d \n",piece[currentPlayerIndex].color,diceRollValue);

        if(diceRollCount == 3 && diceRollValue == 6)
        {
            printf("[%s] player rolls 6 three times ! Turn Ends \n",piece[currentPlayerIndex].color);
            return ;
        }

        if(diceRollValue == 6)
        {
            if(piece[currentPlayerIndex].piecesAtBase > 0)
            {
                for(int i = 0;i < NUMOFPIECES;i++)
                {
                    if(piece[currentPlayerIndex].pieceLocation[i] == 100)//Checking the if any piece avilable in the base
                    {
                        moovePiece(currentPlayerIndex,i,diceRollValue);
                        break;
                    }
                }
            }

            else
            {
                moovePiece(currentPlayerIndex,pieceIndex,diceRollValue);
            }
            continue; //Roll again
            
        }

        else 
        {
            // Move a piece based on the roll
            moovePiece(currentPlayerIndex,pieceIndex, diceRollValue);
            break; // End the player's turn
        }
    }
    
}

/*void gameOutputs()
{
    for(int i = 0;i < NUMOFPLAYERS;i++)
    {
        printf("==================================\n");
        printf("[%s] player now has %d/4 pieces on the board and %d/4 piece on the base.\n",piece[playerOrder[i]].color,4 - (piece[playerOrder[i]].piecesAtBase + piece[playerOrder[i]].piecesAtHome),piece[playerOrder[i]].piecesAtBase);
        printf("==================================\n");
        printf("Location of pieces [%s] \n",piece[playerOrder[i]].color);
        printf("==================================\n");
        printf("Piece %d --> %d \n",);
        printf("Piece %d --> %d \n");
        printf("Piece %d --> %d \n");
        printf("Piece %d --> %d \n");
    }
    
}*/

//Game Function
void runGame()
{
    int j = 1;
    int k = 0;
    while (1) // Loop until all players have finished
    {
        gameData.gameRound = j;
        printf("Round %d \n", j);

        for (int i = 0; i < NUMOFPLAYERS; i++) // one round
        {
            if (piece[playerOrder[i]].piecesAtHome == 4) // Skip the roll for player who already won
            {
                continue;
            }

            int piecesOnBoard = NUMOFPIECES - (piece[playerOrder[i]].piecesAtBase + piece[playerOrder[i]].piecesAtHome);

            if (piecesOnBoard > 1)
            {

                // Randomly select a piece to move if the player has more than 1 piece on the board
                int pieceIndex;
                do
                {
                    pieceIndex = rand() % NUMOFPIECES;
                } while (piece[playerOrder[i]].pieceLocation[pieceIndex] == 100 || piece[playerOrder[i]].pieceLocation[pieceIndex] == -6);

                handleMysteryCell(playerOrder[i],pieceIndex);//mysterCell

                // Move the selected piece
                diceRollCheck(playerOrder[i],pieceIndex);
            }

            else if(piecesOnBoard == 0)
            {
                // Move the first available piece
                for(int pieceIndex = 0; pieceIndex < NUMOFPIECES; pieceIndex++)
                {
                    if(piece[playerOrder[i]].pieceLocation[pieceIndex] != 100 || piece[playerOrder[i]].pieceLocation[pieceIndex] != -6)
                    {
                        diceRollCheck(playerOrder[i], pieceIndex);
                        break;
                    }
                }
            }

            else
            {
                // Move the first available piece
                for (int pieceIndex = 0; pieceIndex < NUMOFPIECES; pieceIndex++)
                {
                    if (piece[playerOrder[i]].pieceLocation[pieceIndex] != 100 && piece[playerOrder[i]].pieceLocation[pieceIndex] != -6)
                    {
                        diceRollCheck(playerOrder[i],pieceIndex);
                        break;
                    }
                }
            }

            if (piece[playerOrder[i]].piecesAtHome == 4)
            {
                winningOrder[k] = playerOrder[i];
                printf("[%s] player wins and finishes in position %d!\n", piece[playerOrder[i]].color, k + 1);
                k++;
            }
        }
        j++;

        if (k == NUMOFPLAYERS - 1) // End the loop when only one player is left
        {
            for (int i = 0; i < NUMOFPLAYERS; i++)
            {
                if (piece[playerOrder[i]].piecesAtHome != 4)
                {
                    winningOrder[k] = playerOrder[i];
                    printf("[%s] player finishes in position %d!\n", piece[playerOrder[i]].color, k + 1);
                    k++;
                }
            }
            break;
        }
    }

    // Display the final rankings
    printf("\nFinal Rankings:\n");
    for (int i = 0; i < NUMOFPLAYERS; i++)
    {
        printf("%d. %s\n", i + 1, piece[winningOrder[i]].color);
    }
}
