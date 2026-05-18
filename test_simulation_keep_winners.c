#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N_HANDS 1000000
#define BET 1

static int choose_double_key_from_card(int card)
{
    int rank = gameReduceCard(card) + 1; // 2..14 (A=14)

    if(rank <= 6)
    {
        return 'H';
    }

    if(rank >= 9)
    {
        return 'L';
    }

    return 'K'; // keep between 7 and 8
}

static int play_double_strategy(const int *deck, int result)
{
    int cardNumber = 13; // same sequence as game flow

    while(result != 0)
    {
        int key;

        if((cardNumber + 1) > 51)
        {
            break;
        }

        key = choose_double_key_from_card(deck[cardNumber]);
        if(key == 'K')
        {
            break;
        }

        result = gameResolveDoubleChoice(result, deck[cardNumber], deck[cardNumber + 1], key);
        cardNumber = cardNumber + 1;

        if(cardNumber > 50)
        {
            break;
        }
    }

    return result;
}

// Simule une main où on garde toutes les cartes si la main initiale est gagnante (Jack or Better ou mieux), sinon on défausse tout
int simulate_hand_keep_winners(void) {
    int deck[52];
    int hand[5];
    int holdMask[5];
    int result;
    int i;

    gameCreateDeck(deck);
    gameDealInitialHand(deck, hand);
    int win = gameCheckHandWinOrLose(hand);
    if(win >= JackOrBetter) {
        // On garde tout
        for(i = 0; i < 5; i++) holdMask[i] = HOLD;
    } else {
        // On défausse tout
        for(i = 0; i < 5; i++) holdMask[i] = DISCARD;
    }
    gameDrawSelectedCards(deck, hand, holdMask);
    result = gameCheckHandWinOrLose(hand);
    return result;
}

int simulate_hand_keep_winners_with_double(void) {
    int deck[52];
    int hand[5];
    int holdMask[5];
    int result;
    int i;

    gameCreateDeck(deck);
    gameDealInitialHand(deck, hand);

    if(gameCheckHandWinOrLose(hand) >= JackOrBetter) {
        for(i = 0; i < 5; i++) {
            holdMask[i] = HOLD;
        }
    } else {
        for(i = 0; i < 5; i++) {
            holdMask[i] = DISCARD;
        }
    }

    gameDrawSelectedCards(deck, hand, holdMask);
    result = gameCheckHandWinOrLose(hand);

    if(result != Lose) {
        result = play_double_strategy(deck, result);
    }

    return result;
}

int main(void) {
    long long total_bet = 0;
    long long total_return = 0;
    long long total_return_with_double = 0;
    int i, win;
    int win_with_double;
    srand((unsigned int)time(NULL));

    for(i = 0; i < N_HANDS; i++) {
        win = simulate_hand_keep_winners();
        win_with_double = simulate_hand_keep_winners_with_double();
        total_bet += BET;
        total_return += win;
        total_return_with_double += win_with_double;
    }

    {
        double rtp = 100.0 * (double)total_return / (double)total_bet;
        double rtp_with_double = 100.0 * (double)total_return_with_double / (double)total_bet;

    printf("Simulation sur %d mains (on garde toutes les mains gagnantes au 1er tirage):\n", N_HANDS);
    printf("Total misé: %lld\n", total_bet);
    printf("Total retour: %lld\n", total_return);
    printf("Taux de retour (RTP): %.2f %%\n", rtp);
    printf("Perte moyenne par main: %.4f\n", (double)(total_bet-total_return)/N_HANDS);
    printf("\nSimulation avec double sur mains gagnantes:\n");
    printf("Regle double: H si carte <= 6, L si carte >= 9, garder si 7-8\n");
    printf("Total retour (avec double): %lld\n", total_return_with_double);
    printf("Taux de retour (RTP, avec double): %.2f %%\n", rtp_with_double);
    printf("Perte moyenne par main (avec double): %.4f\n", (double)(total_bet-total_return_with_double)/N_HANDS);
    }

    return 0;
}
