#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>

#include "types.h"


int main()
{
    srand(time(0)); //Seeds the random number generator with the current time
    
    firstDiceRoll();

    runGame();


    return 0;
}