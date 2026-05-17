#include "ui.h"
#include "game.h"

#include <stdio.h>
#include <wchar.h>
#include <locale.h>

#ifdef _WIN32
#include <conio.h>
#else
#include <unistd.h>
#include <termios.h>
static struct termios stdinSettingsOld;
#endif

void uiInit(void)
{
    setlocale(LC_CTYPE, "");

#ifndef _WIN32
    {
        struct termios stdinSettings;

        tcgetattr(STDIN_FILENO, &stdinSettings);
        stdinSettingsOld = stdinSettings;

        stdinSettings.c_lflag &= ~(ECHO | ECHOE | ISIG);
        stdinSettings.c_lflag &= ~(ICANON);

        stdinSettings.c_cc[VMIN] = 1;
        stdinSettings.c_cc[VTIME] = 0;

        if(tcsetattr(STDIN_FILENO, TCSANOW, &stdinSettings) != 0)
        {
            wprintf(L"\n  Erreur! configuration des attributs du port serie");
        }
    }
#endif
}

void uiClose(void)
{
#ifndef _WIN32
    tcsetattr(STDIN_FILENO, TCSANOW, &stdinSettingsOld);
#endif
}

int uiWaitKey(void)
{
#ifdef _WIN32
    return _getch();
#else
    return getc(stdin);
#endif
}

void uiClearScreen(void)
{
#ifdef _WIN32
    system("cls");
#else
    wprintf(L"\e[1;1H\e[2J");
#endif
}

void uiPrintMainHeader(int credit)
{
    wprintf(L"\nJack-Or-Better...                            Credit = %6d", credit);
}

void uiPrintBetMenu(int bet)
{
    wprintf(L"\nBet 'M'ore or 'L'ess, press 'Space Bar' to draw or 'Q' to quit");
    wprintf(L"\nBet = %6d", bet);
}

void uiPrintCreditLine(int credit)
{
    wprintf(L"\nJack-Or-Better...                      Credit=%6d", credit);
}

void uiPrintHoldPrompt(void)
{
    wprintf(L"\nSelect Card(s) to Hold, Press 'Space Bar' to Draw");
}

void uiPrintResultLine(int result)
{
    wprintf(L"\nResult= %d", result);
}

void uiPrintResultName(int result)
{
    switch(result)
    {
    case JackOrBetter:
        wprintf(L"JackOrBetter");
        break;
    case TwoPairs:
        wprintf(L"TwoPairs");
        break;
    case ThreeOfAKind:
        wprintf(L"ThreeOfAKind");
        break;
    case Straight:
        wprintf(L"Straight");
        break;
    case Flush:
        wprintf(L"Flush");
        break;
    case FullHouse:
        wprintf(L"FullHouse");
        break;
    case FourOfAKind:
        wprintf(L"FourOfAKind");
        break;
    case StraightFlush:
        wprintf(L"StraightFlush");
        break;
    case RoyalFlush:
        wprintf(L"RoyalFlush");
        break;
    default:
        wprintf(L"Lose");
        break;
    }
}

void uiPrintDoublePrompt(void)
{
    wprintf(L"\nPress 'D' to Double or any other Key to Keep");
}

void uiPrintPlayAgainPrompt(int credit)
{
    if(credit == 0)
    {
        wprintf(L"\nNo more credit!");
        return;
    }

    wprintf(L"\nPress 'Space Bar' to Play Again or 'Q' to quit");
}

void uiPrintDoubleStartCard(int card)
{
    wprintf(L"\nPress 'H'igh or 'L'ow, any other key to keep");
    wprintf(L"\ndouble: ");
    uiPrintCard(card);
}

void uiPrintCard(int card)
{
    int reducedCard = gameReduceCard(card);

    if(reducedCard == AsPique)
    {
        wprintf(L"AS");
    }
    else if(reducedCard == RoiPique)
    {
        wprintf(L" K");
    }
    else if(reducedCard == DamePique)
    {
        wprintf(L" D");
    }
    else if(reducedCard == JackPique)
    {
        wprintf(L" J");
    }
    else
    {
        wprintf(L"%2d", reducedCard + 1);
    }

    if(card <= AsPique)
    {
        wprintf(L"%lc...", PiqueChar);
    }
    else if(card <= AsCoeur)
    {
        wprintf(L"%lc...", CoeurChar);
    }
    else if(card <= AsCarreau)
    {
        wprintf(L"%lc...", CarreauChar);
    }
    else if(card <= AsTrefle)
    {
        wprintf(L"%lc...", TrefleChar);
    }
}

void uiPrintHand(const wchar_t *label, const int *hand)
{
    int i;

    wprintf(L"\n%ls", label);
    for(i = 0; i < 5; i++)
    {
        uiPrintCard(hand[i]);
    }
}

void uiGetHoldSelection(int *holdMask)
{
    int selection;
    int i;

    for(i = 0; i < 5; i++)
    {
        holdMask[i] = DISCARD;
    }

    wprintf(L"\n");
    do
    {
        selection = uiWaitKey();
        switch(selection)
        {
        case '1':
            holdMask[0] = !holdMask[0];
            break;
        case '2':
            holdMask[1] = !holdMask[1];
            break;
        case '3':
            holdMask[2] = !holdMask[2];
            break;
        case '4':
            holdMask[3] = !holdMask[3];
            break;
        case '5':
            holdMask[4] = !holdMask[4];
            break;
        default:
            break;
        }

        wprintf(L"\r            ");
        for(i = 0; i < 5; i++)
        {
            if(holdMask[i] == HOLD)
            {
                wprintf(L"H     ");
            }
            else
            {
                wprintf(L"      ");
            }
        }
    }
    while(selection != SpaceBar);
}
