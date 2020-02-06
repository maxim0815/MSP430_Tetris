/*
 * Buttons.h
 *
 *  Created on: 01.02.2020
 *      Author: max
 */

#ifndef INCLUDE_BUTTONS_H_
#define INCLUDE_BUTTONS_H_

void initButtons(){
    // all pins output instead of P2.7
    P2DIR |= 0b01111111;
    // Modify XTAL pins to be I/O
    P2SEL &= ~( BIT6 | BIT7 );
    // set all outputs to 0
    P2OUT = 0;
    // Clear / reset the shift register , i.e. turn all LEDs off.
    P2OUT &= ~BIT5 ;
    // stop the reset mode (/ CLR = 1) and
    P2OUT |= BIT5;
}

void setClock(){
    // apply a rising clock edge => shift data in
    P2OUT |= BIT4 ;
    // reset the clock
    P2OUT &= ~ BIT4 ;
}

int readButtons(){
    int button = 4;

    //Set the shift register 2 mode
    P2OUT |= BIT2 | BIT3;
    //Set the shift register 1 to mode 0
    P2OUT &= ~(BIT0 + BIT1);
    setClock();
    while(button>=0){
        if((P2IN & BIT7)!=0){
            return button;
        }
        button--;

        P2OUT &= ~BIT3;
        P2OUT |= BIT2;

        setClock();
    }
    return 0;
}

Movement getButton(){
    int button = readButtons();
    if(button == 4){
        return Movement::right;
    }
    if(button == 3){
        return Movement::top;
    }
    if(button == 2){
        return Movement::down;
    }
    if(button == 1){
        return Movement::left;
    }
    return Movement::none;
}


#endif /* INCLUDE_BUTTONS_H_ */
