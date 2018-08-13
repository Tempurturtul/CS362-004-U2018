#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include "randomtest_helpers.h"

struct gameState *randomGameState()
{
    // Allocate memory for game state we're going to return.
    struct gameState *g = malloc(sizeof(struct gameState));

    // Set 2 to 4 players.
    int numPlayers = rand() % 3 + 2;

    // Define all kingdom cards.
    int allKingdomCards[(treasure_map + 1) - adventurer];
    int i;
    for (i = 0; i < (treasure_map + 1) - adventurer; i++) {
        allKingdomCards[i] = i + adventurer;
    }

    // Shuffle list of all possible cards (Fisher-Yates shuffle).
    for (i = treasure_map - adventurer; i > 1; i--) {
        int j = rand() % (i + 1);
        int tempCard = allKingdomCards[j];
        allKingdomCards[j] = allKingdomCards[i];
        allKingdomCards[i] = tempCard;
    }

    // Use first 10 from shuffled set as kingdom cards.
    int kingdomCards[10];
    for (i = 0; i < 10; i++) {
        kingdomCards[i] = allKingdomCards[i];
    }

    // Use a seed from 1 to 1000
    int seed = rand() % 1000 + 1;

    // Initialize game state.
    if (initializeGame(numPlayers, kingdomCards, seed, g) == -1) {
        printf("error initializing gamestate\n");
        exit(1);
    }

    // Return game state.
    return g;
}
