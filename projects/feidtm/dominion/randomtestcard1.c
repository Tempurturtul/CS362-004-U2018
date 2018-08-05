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

void testRemodelEffect(struct gameState *state)
{
    int currentPlayer = whoseTurn(state);

    struct gameState prevState;
    memcpy(&prevState, state, sizeof(struct gameState));

    // We're just going to pretend the first card in the player's hand is Remodel.
    int handPos = 0;

    int choice1 = 1; // card to trash
    int choice2 = -1; // card to gain

    int i;
    for (i = estate; i < treasure_map+1; i++) {
        if (state->supplyCount[i] > 0 && getCost(i) <= state->hand[currentPlayer][choice1] + 1) {
            choice2 = i;
            break;
        }
    }

    // Make sure effect succeeds.
    assert(remodelEffect(choice1, choice2, state, handPos) == 0, "effect succeeds");

    // Remodel and selected card are discarded.
    assert(state->playedCards[state->playedCardCount-2] == prevState.hand[currentPlayer][handPos], "remodel discarded");
    assert(state->playedCards[state->playedCardCount-1] == prevState.hand[currentPlayer][choice1], "choice1 discarded");

    // A card is gained.
    assert(state->handCount[currentPlayer] == prevState.handCount[currentPlayer] - 2 + 1, "two cards discarded, one card gained");
    assert(state->hand[currentPlayer][state->handCount[currentPlayer] - 1] == choice2, "selected card gained");

    // Gained card is worth no more than 2 more than the trashed card.
    assert(getCost(choice2) <= prevState.hand[currentPlayer][choice1] + 2, "gained card is worth <= trashed card + 2");

    // No other players have a change of state.
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
        // Test the adventurer effect.
        testRemodelEffect(g);
    }

    printf("DONE RANDOM TESTING remodelEffect()\n");

    return 0;
}
