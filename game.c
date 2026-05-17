#include "game.h"

#include <stdlib.h>

static void sortHand(int *hand)
{
    int i;
    int j;
    int temp;

    for(i = 0; i < 4; i++)
    {
        for(j = 0; j < (4 - i); j++)
        {
            if(hand[j] > hand[j + 1])
            {
                temp = hand[j];
                hand[j] = hand[j + 1];
                hand[j + 1] = temp;
            }
        }
    }
}

static void reduceHand(int *hand)
{
    int i;
    for(i = 0; i < 5; i++)
    {
        hand[i] = gameReduceCard(hand[i]);
    }
}

void gameCreateDeck(int *deck)
{
    int i;
    int tempCardValue;
    int cardPosition1;
    int cardPosition2;

    for(i = 0; i < 52; i++)
    {
        deck[i] = i + 1;
    }

    for(i = 0; i < SHUFFLE_COUNT; i++)
    {
        cardPosition1 = rand() % 52;
        tempCardValue = deck[cardPosition1];
        cardPosition2 = rand() % 52;
        deck[cardPosition1] = deck[cardPosition2];
        deck[cardPosition2] = tempCardValue;
    }
}

void gameDealInitialHand(const int *deck, int *hand)
{
    int i;

    for(i = 0; i < 5; i++)
    {
        hand[i] = deck[i + 1];
    }
}

void gameDrawSelectedCards(const int *deck, int *hand, const int *holdMask)
{
    int i;
    int drawCardNumber = 7;

    for(i = 0; i < 5; i++)
    {
        if(holdMask[i] == DISCARD)
        {
            hand[i] = deck[drawCardNumber];
            drawCardNumber++;
        }
    }
}

int gameReduceCard(int card)
{
    if(card > AsCarreau)
    {
        card = card - AsCarreau;
    }
    else if(card > AsCoeur)
    {
        card = card - AsCoeur;
    }
    else if(card > AsPique)
    {
        card = card - AsPique;
    }

    return card;
}

int gameCheckHandWinOrLose(const int *handInput)
{
    int hand[5];
    int result = Lose;
    int i;
    int pairCount = 0;
    int isThreeOfAKind = 0;
    int isFourOfAKind = 0;
    int isFullHouse = 0;
    int isFlush = 0;
    int isStraight = 0;
    int isRoyal = 0;
    int hasJackOrBetterPair = 0;

    for(i = 0; i < 5; i++)
    {
        hand[i] = handInput[i];
    }

    sortHand(hand);
    if((hand[0] >= DeuxPique && hand[4] <= AsPique) ||
       (hand[0] >= DeuxCoeur && hand[4] <= AsCoeur) ||
       (hand[0] >= DeuxCarreau && hand[4] <= AsCarreau) ||
       (hand[0] >= DeuxTrefle && hand[4] <= AsTrefle))
    {
        isFlush = 1;
    }

    reduceHand(hand);
    sortHand(hand);

    if((hand[0] + 1 == hand[1]) &&
       (hand[1] + 1 == hand[2]) &&
       (hand[2] + 1 == hand[3]) &&
       (hand[3] + 1 == hand[4]))
    {
        isStraight = 1;
    }
    else if((hand[0] == 1) && (hand[1] == 2) && (hand[2] == 3) && (hand[3] == 4) && (hand[4] == 13))
    {
        isStraight = 1;
    }

    if(isFlush && isStraight && hand[4] == AsPique)
    {
        isRoyal = 1;
    }

    for(i = 0; i < 4; i++)
    {
        if(hand[i] == hand[i + 1])
        {
            pairCount++;
            if(hand[i] >= JackPique)
            {
                hasJackOrBetterPair = 1;
            }
        }
    }

    for(i = 0; i < 3; i++)
    {
        if((hand[i] == hand[i + 1]) && (hand[i + 1] == hand[i + 2]))
        {
            isThreeOfAKind = 1;
            break;
        }
    }

    for(i = 0; i < 2; i++)
    {
        if((hand[i] == hand[i + 1]) && (hand[i + 1] == hand[i + 2]) && (hand[i + 2] == hand[i + 3]))
        {
            isFourOfAKind = 1;
            break;
        }
    }

    if(isThreeOfAKind && pairCount == 3 && !isFourOfAKind)
    {
        isFullHouse = 1;
    }

    if(isRoyal)
    {
        result = RoyalFlush;
    }
    else if(isFlush && isStraight)
    {
        result = StraightFlush;
    }
    else if(isFourOfAKind)
    {
        result = FourOfAKind;
    }
    else if(isFullHouse)
    {
        result = FullHouse;
    }
    else if(isFlush)
    {
        result = Flush;
    }
    else if(isStraight)
    {
        result = Straight;
    }
    else if(isThreeOfAKind)
    {
        result = ThreeOfAKind;
    }
    else if(pairCount == 2)
    {
        result = TwoPairs;
    }
    else if(hasJackOrBetterPair)
    {
        result = JackOrBetter;
    }

    return result;
}

int gameResolveDoubleChoice(int currentResult, int currentCard, int nextCard, int key)
{
    int reducedCurrent = gameReduceCard(currentCard);
    int reducedNext = gameReduceCard(nextCard);

    if((key == 'H') || (key == 'h'))
    {
        if(reducedCurrent < reducedNext)
        {
            return currentResult * 2;
        }
        return 0;
    }

    if((key == 'L') || (key == 'l'))
    {
        if(reducedCurrent > reducedNext)
        {
            return currentResult * 2;
        }
        return 0;
    }

    return currentResult;
}
