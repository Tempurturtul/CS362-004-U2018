#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include "unittest_helpers.h"

void testAdventurerEffect()
{
    printf ("TESTING adventurer_func():\n");

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

    int currentPlayer = whoseTurn(&state);

    struct gameState prevState = state;

    int handPos = 0;

    // Make sure effect succeeds.
    assert(adventurer_func(handPos, &state) == 0, "effect succeeds");

    // Make sure adventurer card is discarded.
    assert(state.handCount[currentPlayer] == prevState.handCount[currentPlayer]-1, "a card is discarded");
    assert(false, "adventurer card is discarded"); // We never pass the position of the adventurer card in the player's hand to the effect, so it clearly isn't being discarded.

    // NOTE: Following tests operate on assumption that adventurer card was not discarded. Once this bug is fixed they will need to be adjusted.

    // Player should now have +2 treasure cards.

    // Make sure player only has +2 cards.
    assert(state.handCount[currentPlayer] == prevState.handCount[currentPlayer] + 2, "player gains +2 cards");

    // Make sure player has all their original cards.
    int i;
    for (i = 0; i < prevState.handCount[currentPlayer]; i++) {
        assert(prevState.hand[currentPlayer][i] == state.hand[currentPlayer][i], "rest of player's hand doesn't change");
    }

    // Make sure 2 new cards are treasure cards.
    for (i = prevState.handCount[currentPlayer]; i < prevState.handCount[currentPlayer]+2; i++) {
        int card = state.hand[currentPlayer][i];
        assert(card >= copper && card <= gold, "drawn cards are treasure cards");
    }

    // Make sure 2 new cards came from own player's deck.
    assert(state.deckCount[currentPlayer] == prevState.deckCount[currentPlayer] - 2, "drawn cards came from player's own deck");

    // Make sure other players cards haven't changed.
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
    testAdventurerEffect();

    return 0;
}
