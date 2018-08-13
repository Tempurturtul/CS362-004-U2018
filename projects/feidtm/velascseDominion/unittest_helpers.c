#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include "unittest_helpers.h"

void assert(bool val, char *description)
{
    if (!val) {
        printf("test failed: %s\n", description);
        if (!CONTINUE_ON_FAIL) {
            exit(1);
        }
    }
}

void resetGameState(struct gameState *G, int numPlayers, int kingdomCards[10])
{
    int seed = 1000;

    // Clear game state.
    memset(G, 0, sizeof(struct gameState));

    // Initialize new game state.
    if (initializeGame(numPlayers, kingdomCards, seed, G) == -1) {
        printf("error initializing gamestate\n");
        exit(1);
    }
}

bool sameHand(struct gameState *state, struct gameState *prevState, int player)
{
    int i;

    // Make sure hand hasn't changed.
    if (state->handCount[player] != prevState->handCount[player]) {
        return false;
    }
    for (i = 0; i < state->handCount[player]; i++) {
        if (state->hand[player][i] != prevState->hand[player][i]) {
            return false;
        }
    }

    return true;
}

bool sameDeck(struct gameState *state, struct gameState *prevState, int player)
{
    int i;

    // Make sure deck hasn't changed.
    if (state->deckCount[player] != prevState->deckCount[player]) {
        return false;
    }
    for (i = 0; i < state->deckCount[player]; i++) {
        if (state->deck[player][i] != prevState->deck[player][i]) {
            return false;
        }
    }

    return true;
}

bool sameDiscard(struct gameState *state, struct gameState *prevState, int player)
{
    int i;

    // Make sure discard hasn't changed.
    if (state->discardCount[player] != prevState->discardCount[player]) {
        return false;
    }
    for (i = 0; i < state->discardCount[player]; i++) {
        if (state->discard[player][i] != prevState->discard[player][i]) {
            return false;
        }
    }

    return true;
}

