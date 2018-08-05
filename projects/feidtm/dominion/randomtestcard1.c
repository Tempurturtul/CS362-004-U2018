#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include "unittest_helpers.h"

#define NUM_TESTS 100

struct gameState *randomGameState()
{
    // Allocate memory for game state we're going to return.
    struct gameState *g = malloc(sizeof(struct gameState));

    // Set 2 to 4 players.
    int numPlayers = rand() % 3 + 2;

    // Define all possible cards.
    int allPossibleCards[treasure_map + 1];
    int i;
    for (i = 0; i < treasure_map + 1; i++) {
        allPossibleCards[i] = i;
    }

    // Shuffle list of all possible cards (Fisher-Yates shuffle).
    for (i = treasure_map; i > 1; i--) {
        int j = rand() % (i + 1);
        int tempCard = allPossibleCards[j];
        allPossibleCards[j] = allPossibleCards[i];
        allPossibleCards[i] = tempCard;
    }

    // Use first 10 from shuffled set as kingdom cards.
    int kingdomCards[10];
    for (i = 0; i < 10; i++) {
        kingdomCards[i] = allPossibleCards[i];
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

void testRemodelEffect(struct gameState *state, int choice1, int choice2, int handPos)
{
    int currentPlayer = whoseTurn(state);

    struct gameState prevState;
    memcpy(&prevState, state, sizeof(struct gameState));

    int effectReturnVal = remodelEffect(choice1, choice2, state, handPos);

    // If inputs are valid, ensure effect succeeds, otherwise ensure effect fails and stop testing.

    int effectReturnValShouldBe = 0;

    // handPos must be in player's hand.
    if (handPos > prevState.handCount[currentPlayer] - 1) {
        effectReturnValShouldBe = -1;
    }
    // choice1 must be in player's hand and not handPos.
    if (choice1 == handPos || choice1 > prevState.handCount[currentPlayer] - 1) {
        effectReturnValShouldBe = -1;
    }
    // choice2 must be a possible card with a supply.
    if (choice2 > treasure_map || prevState.supplyCount[choice2] <= 0) {
        effectReturnValShouldBe = -1;
    }
    // Cost of choice2 must be <= cost of choice1 + 2.
    if (getCost(choice2) > getCost(prevState.hand[currentPlayer][choice1]) + 2) {
        effectReturnValShouldBe = -1;
    }

    if (effectReturnValShouldBe == 0) {
        assert(effectReturnVal == effectReturnValShouldBe, "effect succeeds");
    } else {
        assert(effectReturnVal == effectReturnValShouldBe, "effect fails");
        return;
    }

    // Remodel and selected card are discarded.
    assert(state->playedCards[state->playedCardCount-2] == prevState.hand[currentPlayer][handPos], "remodel discarded");
    assert(state->playedCards[state->playedCardCount-1] == prevState.hand[currentPlayer][choice1], "choice1 discarded");

    // A card is gained.
    assert(state->handCount[currentPlayer] == prevState.handCount[currentPlayer] - 2 + 1, "two cards discarded, one card gained");
    assert(state->hand[currentPlayer][state->handCount[currentPlayer] - 1] == choice2, "selected card gained");

    // Gained card is worth no more than 2 more than the trashed card.
    assert(getCost(choice2) <= getCost(prevState.hand[currentPlayer][choice1]) + 2, "gained card is worth <= trashed card + 2");

    // No other players have a change of state.
    int i;
    for (i = 0; i < state->numPlayers; i++) {
        if (i == currentPlayer) {
            continue;
        }

        assert(sameHand(state, &prevState, i), "player with <= 4 cards hand doesn't change");
        assert(sameDeck(state, &prevState, i), "player with <= 4 cards decks doesn't change");
        assert(sameDiscard(state, &prevState, i), "player with <= 4 cards discard piles doesn't change");
    }
}

int main()
{
    // Initialize random seed.
    srand(time(NULL));

    printf("RANDOM TESTING remodelEffect():\n");

    struct gameState *g;

    int i;
    for (i = 0; i < NUM_TESTS; i++) {
        // Get a random game state.
        g = randomGameState();

        int choice1, choice2, handPos;

        // Pick random cards for choice 1 and 2, and for the remodel card.
        handPos = rand() % 7; // Which card in the player's hand is the remodel card.
        choice1 = rand() % 7; // Which card the player will discard.
        choice2 = rand() % treasure_map + 1; // Which card the player will choose to gain.

        // Test the effect.
        testRemodelEffect(g, choice1, choice2, handPos);
    }

    printf("DONE RANDOM TESTING remodelEffect()\n");

    return 0;
}
