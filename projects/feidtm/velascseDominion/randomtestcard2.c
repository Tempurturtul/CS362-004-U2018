#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include "unittest_helpers.h"
#include "randomtest_helpers.h"

void testMinionEffect(struct gameState *state, int choice1, int choice2, int handPos)
{
    int i;
    int currentPlayer = whoseTurn(state);

    struct gameState prevState;
    memcpy(&prevState, state, sizeof(struct gameState));

    int effectReturnVal = minionEffect(choice1, choice2, state, handPos);

    // If inputs are valid, ensure effect succeeds, otherwise ensure effect fails and stop testing.

    int effectReturnValShouldBe = 0;

    if (choice1 == choice2 || choice1 > 1 || choice1 < 0 || choice2 > 1 || choice2 < 0) {
        effectReturnValShouldBe = -1;
    }
    if (handPos > prevState.handCount[currentPlayer] - 1) {
        effectReturnValShouldBe = -1;
    }

    if (effectReturnValShouldBe == 0) {
        assert(effectReturnVal == 0, "effect succeeds");
    } else {
        assert(effectReturnVal != 0, "effect fails");
        return;
    }

    if (choice1 == 1) {
        // Make sure the Minion was discarded.
        assert(state->handCount[currentPlayer] == prevState.handCount[currentPlayer]-1, "a card is discarded");
        assert(state->hand[currentPlayer][handPos] == prevState.hand[currentPlayer][handPos+1], "minion card is discarded");

        // Make sure +1 action was granted.
        assert(state->numActions == prevState.numActions + 1, "+1 action gained");

        // Make sure +2 coins were gained.
        assert(state->coins == prevState.coins + 2, "+2 coins gained");

        // Make sure hand, deck, and discard for other players is the same.
        for (i = 0; i < prevState.numPlayers; i++) {
            if (i == currentPlayer) {
                continue;
            }
            assert(sameHand(state, &prevState, i), "other players' hands don't change");
            assert(sameDeck(state, &prevState, i), "other players' decks don't change");
            assert(sameDiscard(state, &prevState, i), "other players' discard piles don't change");
        }
    } else {
        // Choice 2: +1 action, discard hand, +4 cards, each other player with 5+ cards also discards and draws +4 cards.

        // Make sure the Minion was discarded.
        assert(state->handCount[currentPlayer] == prevState.handCount[currentPlayer]-1, "a card is discarded");
        assert(state->hand[currentPlayer][handPos] == prevState.hand[currentPlayer][handPos+1], "minion card is discarded");

        // Make sure +1 action was granted.
        assert(state->numActions == prevState.numActions+1, "+1 action gained");

        // Make sure hand discarded and +4 cards drawn.
        assert(state->playedCardCount == prevState.playedCardCount + prevState.handCount[currentPlayer], "hand discarded");

        assert(state->handCount[currentPlayer] == 4, "4 cards drawn");
        assert(state->deckCount[currentPlayer] == prevState.deckCount[currentPlayer] - 4, "drawn cards came from player's own deck");

        // Make sure each other player with 5+ cards have hand discarded and 4 cards drawn.
        // Also make sure each other player with <= 4 cards has no change.
        for (i = 0; i < prevState.numPlayers; i++) {
            if (i == currentPlayer) {
                continue;
            }

            if (prevState.handCount[i] >= 5) {
                // Should discard hand and draw 4.
                assert(state->discardCount[i] == prevState.discardCount[i] + prevState.handCount[i], "player with 5+ cards has hand discarded");
                assert(state->handCount[i] == 4, "player with 5+ cards has 4 cards drawn");
                assert(state->deckCount[i] == prevState.deckCount[i] - 4, "player with 5+ cards has drawn cards came from own deck");
            } else {
                // Should be no change.
                assert(sameHand(state, &prevState, i), "player with <= 4 cards hand doesn't change");
                assert(sameDeck(state, &prevState, i), "player with <= 4 cards decks doesn't change");
                assert(sameDiscard(state, &prevState, i), "player with <= 4 cards discard piles doesn't change");
            }
        }
    }
}

int main()
{
    // Initialize random seed.
    srand(time(NULL));

    printf("RANDOM TESTING minionEffect():\n");

    struct gameState *g;

    int i;
    for (i = 0; i < NUM_TESTS; i++) {
        // Get a random game state.
        g = randomGameState();

        int choice1, choice2, handPos;

        handPos = rand() % 7; // Which card in the player's hand is the minion card.
        choice1 = rand() % 2; // Zero or one.
        choice2 = rand() % 2; // Zero or one.

        // Test the effect.
        testMinionEffect(g, choice1, choice2, handPos);
    }

    printf("DONE RANDOM TESTING minionEffect()\n");

    return 0;
}
