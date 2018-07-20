#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"

#define CONTINUE_ON_FAIL 1

typedef enum { false, true } bool;

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

    // Make sure effect succeeds.
    assert(adventurerEffect(&state) == 0, "effect succeeds");

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

    choice1 = 1;
    choice2 = 0;

    // Make sure effect succeeds.
    assert(minionEffect(choice1, choice2, &state, handPos) == 0, "effect succeeds");

    // Make sure the Minion was discarded.
    assert(state.handCount[currentPlayer] == prevState.handCount[currentPlayer]-1, "a card is discarded");
    assert(state.hand[currentPlayer][handPos] == prevState.hand[currentPlayer][handPos+1], "minion card is discarded");

    // Make sure +1 action was granted.
    // assert(state.numActions == prevState.numActions + 1, "+1 action gained");

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

    // Make sure +1 action was granted.
    // assert(state.numActions == prevState.numActions+1);

    // TODO

    if (CONTINUE_ON_FAIL) {
        printf("Done testing.\n");
    } else {
        printf("All tests passed!\n");
    }
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
