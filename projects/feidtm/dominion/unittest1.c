#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"

typedef enum { false, true } bool;

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

bool noChange(struct gameState *state, struct gameState *prevState, int player)
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

    // Make sure deck hasn't changed.
    if (state->deckCount[player] != prevState->deckCount[player]) {
        return false;
    }
    for (i = 0; i < state->deckCount[player]; i++) {
        if (state->deck[player][i] != prevState->deck[player][i]) {
            return false;
        }
    }

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

void testAdventurerEffect()
{
    printf ("TESTING adventurerEffect():\n");

    struct gameState state;

    int numPlayers = 4;
    int k[10] = {
        adventurer,
        council_room,
        feast,
        gardens,
        mine,
        remodel,
        smithy,
        village,
        baron,
        great_hall
    };

    resetGameState(&state, numPlayers, k);

    int currentPlayer = whoseTurn(&state);

    struct gameState prevState = state;

    // Make sure effect succeeds.
    assert(adventurerEffect(&state) == 0);
    
    // Player should now have +2 treasure cards.

    // Make sure player only has +2 cards.
    assert(state.handCount[currentPlayer] == prevState.handCount[currentPlayer] + 2);

    // Make sure player has all their original cards.
    int i;
    for (i = 0; i < prevState.handCount[currentPlayer]; i++) {
        assert(prevState.hand[currentPlayer][i] == state.hand[currentPlayer][i]);
    }

    // Make sure 2 new cards are treasure cards.
    for (i = prevState.handCount[currentPlayer]; i < prevState.handCount[currentPlayer]+2; i++) {
        int card = state.hand[currentPlayer][i];
        assert(card >= copper && card <= gold);
    }

    // Make sure 2 new cards came from own player's deck.
    assert(state.deckCount[currentPlayer] == prevState.deckCount[currentPlayer] - 2);

    // Make sure other players cards haven't changed.
    for (i = 0; i < numPlayers; i++) {
        if (i == currentPlayer) {
            continue;
        }
        assert(noChange(&state, &prevState, i));
    }

    printf("All tests passed!\n");
}

void testMinionEffect()
{
    printf ("TESTING minionEffect():\n");
}

void testRemodelEffect()
{
    printf ("TESTING remodelEffect():\n");
}

void testSmithyEffect()
{
    printf ("TESTING smithyEffect():\n");
}

int main() {
    testAdventurerEffect();
    testMinionEffect();
    testRemodelEffect();
    testSmithyEffect();

    return 0;
}
