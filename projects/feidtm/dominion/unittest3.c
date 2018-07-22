#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include "unittest_helpers.h"

void testRemodelEffect()
{
    printf ("TESTING remodelEffect():\n");

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

    // We're just going to pretend the first card in the player's hand is Remodel.
    int handPos = 0;

    int choice1 = 1; // card to trash
    int choice2 = -1; // card to gain

    int i;
    for (i = 0; i < 10; i++) {
        if (getCost(k[i]) <= state.hand[currentPlayer][choice1] + 1) {
            choice2 = k[i];
        }
    }

    // Make sure effect succeeds.
    assert(remodelEffect(choice1, choice2, &state, handPos) == 0, "effect succeeds");

    // Remodel and selected card are discarded.
    assert(state.playedCards[state.playedCardCount-2] == prevState.hand[currentPlayer][handPos], "remodel discarded");
    assert(state.playedCards[state.playedCardCount-1] == prevState.hand[currentPlayer][choice1], "choice1 discarded");

    // A card is gained.
    assert(state.handCount[currentPlayer] == prevState.handCount[currentPlayer] - 2 + 1, "two cards discarded, one card gained");
    assert(state.hand[currentPlayer][state.handCount[currentPlayer] - 1] == choice2, "selected card gained");

    // Gained card is worth no more than 2 more than the trashed card.
    assert(getCost(choice2) <= prevState.hand[currentPlayer][choice1] + 2, "gained card is worth <= trashed card + 2");

    // No other players have a change of state.
    for (i = 0; i < numPlayers; i++) {
        if (i == currentPlayer) {
            continue;
        }

        assert(sameHand(&state, &prevState, i), "player with <= 4 cards hand doesn't change");
        assert(sameDeck(&state, &prevState, i), "player with <= 4 cards decks doesn't change");
        assert(sameDiscard(&state, &prevState, i), "player with <= 4 cards discard piles doesn't change");
    }


    if (CONTINUE_ON_FAIL) {
        printf("Done testing.\n");
    } else {
        printf("All tests passed!\n");
    }
}

int main() {
    testRemodelEffect();

    return 0;
}
