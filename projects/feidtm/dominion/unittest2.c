#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include "unittest_helpers.h"

void testMinionEffect()
{
    printf ("TESTING minionEffect():\n");

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

    // We're just going to pretend the first card in the player's hand is the Minion.
    int handPos = 0;

    int choice1;
    int choice2;

    // Choice 1: +1 action, +2 coins.
    printf("TESTING choice 1:\n");

    choice1 = 1;
    choice2 = 0;

    // Make sure effect succeeds.
    assert(minionEffect(choice1, choice2, &state, handPos) == 0, "effect succeeds");

    // Make sure the Minion was discarded.
    assert(state.handCount[currentPlayer] == prevState.handCount[currentPlayer]-1, "a card is discarded");
    assert(state.hand[currentPlayer][handPos] == prevState.hand[currentPlayer][handPos+1], "minion card is discarded");

    // Make sure +1 action was granted.
    assert(state.numActions == prevState.numActions + 1, "+1 action gained");

    // Make sure +2 coins were gained.
    assert(state.coins == prevState.coins + 2, "+2 coins gained");

    // Make sure hand, deck, and discard for other players is the same.
    int i;
    for (i = 0; i < numPlayers; i++) {
        if (i == currentPlayer) {
            continue;
        }
        assert(sameHand(&state, &prevState, i), "other players' hands don't change");
        assert(sameDeck(&state, &prevState, i), "other players' decks don't change");
        assert(sameDiscard(&state, &prevState, i), "other players' discard piles don't change");
    }

    // Choice 2: +1 action, discard hand, +4 cards, each other player with 5+ cards also discards and draws +4 cards.
    printf("TESTING choice 2:\n");

    resetGameState(&state, numPlayers, k);
    currentPlayer = whoseTurn(&state);
    prevState = state;

    choice1 = 0;
    choice2 = 1;
    handPos = 0;

    // Make sure effect succeeds.
    assert(minionEffect(choice1, choice2, &state, handPos) == 0, "effect succeeds");

    // Make sure the Minion was discarded.
    assert(state.handCount[currentPlayer] == prevState.handCount[currentPlayer]-1, "a card is discarded");
    assert(state.hand[currentPlayer][handPos] == prevState.hand[currentPlayer][handPos+1], "minion card is discarded");

    // Make sure +1 action was granted.
    assert(state.numActions == prevState.numActions+1, "+1 action gained");

    // Make sure hand discarded and +4 cards drawn.
    assert(state.discardCount[currentPlayer] == prevState.discardCount[currentPlayer] + prevState.handCount[currentPlayer], "hand discarded");
    assert(state.handCount[currentPlayer] == 4, "4 cards drawn");
    assert(state.deckCount[currentPlayer] == prevState.deckCount[currentPlayer] - 4, "drawn cards came from player's own deck");

    // Make sure each other player with 5+ cards have hand discarded and 4 cards drawn.
    // Also make sure each other player with <= 4 cards has no change.
    for (i = 0; i < numPlayers; i++) {
        if (i == currentPlayer) {
            continue;
        }

        if (prevState.handCount[i] >= 5) {
            // Should discard hand and draw 4.
            assert(state.discardCount[i] == prevState.discardCount[i] + prevState.handCount[i], "player with 5+ cards has hand discarded");
            assert(state.handCount[i] == 4, "player with 5+ cards has 4 cards drawn");
            assert(state.deckCount[i] == prevState.deckCount[i] - 4, "player with 5+ cards has drawn cards came from own deck");
        } else {
            // Should be no change.
            assert(sameHand(&state, &prevState, i), "player with <= 4 cards hand doesn't change");
            assert(sameDeck(&state, &prevState, i), "player with <= 4 cards decks doesn't change");
            assert(sameDiscard(&state, &prevState, i), "player with <= 4 cards discard piles doesn't change");
        }
    }

    if (CONTINUE_ON_FAIL) {
        printf("Done testing.\n");
    } else {
        printf("All tests passed!\n");
    }
}

int main() {
    testMinionEffect();

    return 0;
}
