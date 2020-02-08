/*
 * Exercise 9 Maximilian Kellner and Thomas Kötzner
 *
 * Buttons :
 *      PB1     left
 *      PB2     down
 *      PB3     up/turn
 *      PB4     right
 */

#include <templateEMP.h>

#include <vector>

#include "include/Display.h"
#include "include/Figures.h"
#include "include/Buttons.h"

int move_time;
int delayTime;
int score;

void initInterrupt(){
    TA0CCTL0 = CCIE;                    // Interrupt enabled
    TA0CTL = TASSEL_2 + MC_1 + ID_0;    // SMCLK, Up mode (counting up to TA0CCR0), /1 as clock divider
    TA0CCR0 = 1000;                     // 1ms Interrupt
}

void playGame(int delay){
    bool alive = true;
    //seed = genSeed(); //generate random seed with floating ADC
    Movement button;
    while(alive){
        //generate object --> needs to be randomized
        RandomFigure figureT(horizontal_size/2, 2);
        while(figureT.stop_movement == false){
            move_time = 0;
            while(move_time < delay){
                button = getButton();
                if(button == Movement::right){
                    figureT.clear();
                    figureT.moveRight();
                    figureT.print();
                }
                if(button == Movement::left){
                    figureT.clear();
                    figureT.moveLeft();
                    figureT.print();
                }
                if(button == Movement::down){
                    figureT.clear();
                    figureT.moveDown();
                    figureT.print();
                    break;
                }
                if(button == Movement::top){
                    figureT.clear();
                    figureT.rotate();
                    figureT.print();
                    break;
                }

                moveCursorHome();

                __delay_cycles(30000);

            }
            figureT.clear();
            figureT.moveDown();
            figureT.print();
        }
        figureT.fillOccupancyMap();
        figureT.~RandomFigure();
        checkOccupancyMapForFullLine();
        clearInsideField();
        drawOccupancyMap();
        if(checkOccupancyMapLine(0) == true){
            alive = false;
        }
    }
    clearInsideField();
    moveCursorTo(horizontal_size/2 - 5, vertical_size/2);
    serialPrintln("GAME OVER");
}

int main(void)
{
   initMSP();
    initButtons();
    initInterrupt();
    eraseScreen();
    printGameField();
    while(1){
        score = 0;
        printGameField();
        moveCursorTo(horizontal_size/2 -8, vertical_size/2-4);
        serialPrintln("Choose Difficulty:");
        moveCursorTo(horizontal_size/2 -8, vertical_size/2-2);
        serialPrintln("PRESS PB1 for Easy");
        moveCursorTo(horizontal_size/2 -8, vertical_size/2-1);
        serialPrintln("PRESS PB2 for Medium");
        moveCursorTo(horizontal_size/2 -8, vertical_size/2);
        serialPrintln("PRESS PB3 for Hard");
        while(1){
            if (getButton() == Movement::left){
                delayTime = 1100;
                break;
            }
            if (getButton() == Movement::down){
                delayTime = 700;
                break;
            }
            if (getButton() == Movement::top){
                delayTime = 200;
                break;
            }
        }

        eraseScreen();
        printGameField();
        printScore(score);

        playGame(delayTime);

        printGameField();
        moveCursorTo(horizontal_size/2 -3, vertical_size/2+1);
        serialPrintln("AGAIN?");
        moveCursorTo(horizontal_size/2 -5, vertical_size/2+2);
        serialPrintln("PRESS PB4!");
        printScore(score);
        while(getButton() != Movement::right){
            //wait for button
        }
        eraseScreen();
        printGameField();
        eraseOccupancyMap();
    }
}

//TA0CCR0
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A (void) {
    move_time++;
    TA0CTL &= ~TAIFG; // clear interrupt flag.
}
