#ifndef UI_H
#define UI_H

#include "game.h"
#include <wchar.h>

void uiInit(void);
void uiClose(void);
int uiWaitKey(void);
void uiClearScreen(void);
void uiPrintMainHeader(int credit);
void uiPrintBetMenu(int bet);
void uiPrintCreditLine(int credit);
void uiPrintHoldPrompt(void);
void uiPrintResultLine(int result);
void uiPrintResultName(int result);
void uiPrintDoublePrompt(void);
void uiPrintPlayAgainPrompt(int credit);
void uiPrintDoubleStartCard(int card);
void uiPrintCard(int card);
void uiPrintHand(const wchar_t *label, const int *hand);
void uiGetHoldSelection(int *holdMask);

#endif
