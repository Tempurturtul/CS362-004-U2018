#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include "unittest_helpers.h"

void testAdventurerCard()
{
    printf ("TESTING adventurer card:\n");

    struct gameState state;

    int numPlayers = 4;
    int k[10] = {
        adventurer,
        council_room,
        feast,
        gardens,
        minion,
        remodel,
        smithy,
        village,
        baron,
        great_hall
    };

    resetGameState(&state, numPlayers, k);

    int choice1 = 0;
    int choice2 = 0;
    int choice3 = 0;
    int handPos = 0;
    int bonus = 0;

    // Make sure effect succeeds.
    assert(cardEffect(adventurer, choice1, choice2, choice3, &state, handPos, &bonus) == 0, "effect succeeds");

    if (CONTINUE_ON_FAIL) {
        printf("Done testing.\n");
    } else {
        printf("All tests passed!\n");
    }
}

int main() {
    testAdventurerCard();

    return 0;
}
