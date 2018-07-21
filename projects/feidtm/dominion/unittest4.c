#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include "unittest_helpers.h"

void testSmithyEffect()
{
    printf ("TESTING smithyEffect():\n");

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

    struct gameState prevState = state;

    int currentPlayer = whoseTurn(&state);

    // We're just going to pretend the first card in the player's hand is Smithy.
    int handPos = 0;

    // Make sure effect succeeds.
    assert(smithyEffect(&state, handPos) == 0, "effect succeeds");

    // Make sure Smithy was discarded.
    assert(state.playedCardCount == prevState.playedCardCount + 1, "a card is discarded");
    assert(state.playedCards[state.playedCardCount-1] == prevState.hand[currentPlayer][handPos], "smithy is discarded");

    // 3 cards are gained.
    assert(state.handCount[currentPlayer] == prevState.handCount[currentPlayer] - 3, "3 cards are drawn");
    assert(state.deckCount[currentPlayer] == prevState.deckCount[currentPlayer] - 3, "drawn cards came from player's own deck");

    // Make sure other players cards haven't changed.
    int i;
    for (i = 0; i < numPlayers; i++) {
        if (i == currentPlayer) {
            continue;
        }
        assert(sameHand(&state, &prevState, i), "other players' hands don't change");
        assert(sameDeck(&state, &prevState, i), "other players' decks don't change");
        assert(sameDiscard(&state, &prevState, i), "other players' discard piles don't change");
    }

    if (CONTINUE_ON_FAIL) {
        printf("Done testing.\n");
    } else {
        printf("All tests passed!\n");
    }
}

int main() {
    testSmithyEffect();

    return 0;
}
