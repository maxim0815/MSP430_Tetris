/*
 * Exercise 9 Maximilian Kellner
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
int delay_time;
int score;

//TA0CCR0
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A (void) {
    move_time++;
    TA0CTL &= ~TAIFG; // clear interrupt flag.
}

void initInterrupt(){
    TA0CCTL0 = CCIE;                    // Interrupt enabled
    TA0CTL = TASSEL_2 + MC_1 + ID_0;    // SMCLK, Up mode (counting up to TA0CCR0), /1 as clock divider
    TA0CCR0 = 1000;                     // 1ms Interrupt
}

void printScore(){
    moveCursorTo(horizontal_size/2 - 5, vertical_size + 3);
    serialPrint("Score: ");
    serialPrintInt(score);

}

void playGame(){
    bool alive = true;
    Movement button;
    while(alive){
        RandomFigure figure(horizontal_size/2, 2);
        while(figure.stop_movement == false){
            move_time = 0;
            while(move_time < delay_time){
                button = getButton();
                if(button == Movement::right){
                    figure.clear();
                    figure.moveRight();
                    figure.print();
                }
                if(button == Movement::left){
                    figure.clear();
                    figure.moveLeft();
                    figure.print();
                }
                if(button == Movement::down){
                    figure.clear();
                    figure.moveDown();
                    figure.print();
                    break;
                }
                if(button == Movement::top){
                    figure.clear();
                    figure.rotate();
                    figure.print();
                }

                __delay_cycles(30000);
            }
            figure.clear();
            figure.moveDown();
            figure.print();
        }
        figure.fillOccupancyMap();
        figure.~RandomFigure();
        checkOccupancyMapForFullLine();
        clearInsideField();
        drawOccupancyMap();
        if(checkOccupancyMapLine(0) == true){
            alive = false;
        }
    }
    clearInsideField();
    moveCursorTo(horizontal_size/2 -2, vertical_size/2);
    serialPrintln("OVER");
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
                delay_time = 1100;
                break;
            }
            if (getButton() == Movement::down){
                delay_time = 700;
                break;
            }
            if (getButton() == Movement::top){
                delay_time = 200;
                break;
            }
        }

        eraseScreen();
        printGameField();
        printScore();
        playGame();
        printGameField();
        moveCursorTo(horizontal_size/2 -3, vertical_size/2+1);
        serialPrintln("AGAIN?");
        moveCursorTo(horizontal_size/2 -6, vertical_size/2+2);
        serialPrintln("PRESS RIGHT!");
        while(getButton() != Movement::right){
            //wait for button
        }
        eraseScreen();
        printGameField();
        eraseOccupancyMap();
    }
}
