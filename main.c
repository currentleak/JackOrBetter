#include "game.h"
#include "ui.h"

#include <stdlib.h>
#include <time.h>
#include <wchar.h>

static int playDouble(const int *deck, int result)
{
    int key;
    int cardNumber = 13;

    uiPrintDoubleStartCard(deck[cardNumber]);

    do
    {
        if((cardNumber + 1) > 51)
        {
            break;
        }

        key = uiWaitKey();
        uiPrintCard(deck[cardNumber + 1]);

        if((key != 'H') && (key != 'h') && (key != 'L') && (key != 'l'))
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
    while(result != 0);

    return result;
}

int main(void)
{
    int credit = 10;
    int bet = 1;
    int deck[52];
    int hand[5];
    int holdMask[5];
    int result;
    int key;

    srand((unsigned int)time(NULL));
    uiInit();

    do
    {
        do
        {
            uiClearScreen();
            uiPrintMainHeader(credit);
            uiPrintBetMenu(bet);

            key = uiWaitKey();
            if((key == 'M') || (key == 'm'))
            {
                if(bet < credit)
                {
                    bet = bet + 1;
                }
            }
            else if((key == 'L') || (key == 'l'))
            {
                if(bet >= 2)
                {
                    bet = bet - 1;
                }
            }
        }
        while((key != SpaceBar) || (bet > credit));

        credit = credit - bet;
        uiPrintCreditLine(credit);

        gameCreateDeck(deck);
        gameDealInitialHand(deck, hand);

        wprintf(L"\n            1     2     3     4     5");
        uiPrintHand(L"hand 1st : ", hand);
        uiPrintHoldPrompt();
        uiGetHoldSelection(holdMask);

        gameDrawSelectedCards(deck, hand, holdMask);
        uiPrintHand(L"hand 2nd : ", hand);

        result = bet * gameCheckHandWinOrLose(hand);
        uiPrintResultName(result);
        uiPrintResultLine(result);

        if(result != Lose)
        {
            uiPrintDoublePrompt();
            key = uiWaitKey();
            if((key == 'D') || (key == 'd'))
            {
                result = playDouble(deck, result);
                uiPrintResultLine(result);
                credit = credit + result;
            }
            else
            {
                credit = credit + bet + result;
            }
        }

        uiPrintCreditLine(credit);
        uiPrintPlayAgainPrompt(credit);
    }
    while((uiWaitKey() == SpaceBar) && (credit > 0));

    wprintf(L"\npoker END\n");
    uiClose();
    return 0;
}
