/*
 * Display.h
 *
 *  Created on: 31.01.2020
 *      Author: max
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

const int vertical_size = 16;
const int horizontal_size = 20;

// map is shifted by x-2 and y-2 compared to printer field
bool occupancyMap [horizontal_size-2][vertical_size-2] = { {false} };

void eraseScreen(){
    serialWrite(0x1b);  // ESC
    serialWrite(0x5B);  // [
    serialWrite(0x32);  // 2
    serialWrite(0x4A);  // J
}

void moveCursorHome(){
    serialWrite(0x1b);
    serialWrite(0x5B);
    serialWrite('H');  //H
}

// just values between 0-99 allowed
void moveCursorTo(int x, int y){
    moveCursorHome();
    if(x > 9){
        int x_1_i = x/10;
        int x_2_i = x%10;
        char x_1_c = x_1_i+'0';
        char x_2_c = x_2_i+'0';
        serialWrite(0x1b);  //ESC
        serialWrite(0x5B);  //[
        serialWrite(x_1_c);
        serialWrite(x_2_c);
        serialWrite(0x43);  //C
    }
    else{
        char x_1_c = x+'0';
        serialWrite(0x1b);  //ESC
        serialWrite(0x5B);  //[
        serialWrite(x_1_c);
        serialWrite(0x43);  //C
    }
    if(y > 9){
        int y_1_i = y/10;
        int y_2_i = y%10;
        char y_1_c = y_1_i+'0';
        char y_2_c = y_2_i+'0';
        serialWrite(0x1b);  //ESC
        serialWrite(0x5B);  //[
        serialWrite(y_1_c);
        serialWrite(y_2_c);
        serialWrite(0x42);  //B
    }
    else{
        char y_1_c = y+'0';
        serialWrite(0x1b);  //ESC
        serialWrite(0x5B);  //[
        serialWrite(y_1_c);
        serialWrite(0x42);  //B
    }
}

void PrintHorizontalBorder(int x, int y){
    moveCursorTo(x, y);
    for(int i = 0; i < horizontal_size; i++){
        serialPrint("#");
    }
}

void PrintVerticalBorder(int x, int y){
    moveCursorTo(x, y);
    for(int i = 0; i < vertical_size; i++){
        moveCursorTo(x, y+i);
        serialPrint("#");
    }
}

void printGameField(){
    // start from 0 and 1 gets same output???
    PrintHorizontalBorder(1, 1);
    PrintVerticalBorder(1, 1);
    PrintHorizontalBorder(1, vertical_size);
    PrintVerticalBorder(horizontal_size, 1);
}

void eraseLine(uint8_t y){
    moveCursorTo(2, y);
    serialWrite(0x1b);  // ESC
    serialWrite(0x5B);  //[
    serialWrite('K');
    //draw deleted character
    moveCursorTo(horizontal_size, y);
    serialPrint("#");
}

void clearInsideField(){
    for(uint8_t i = 2; i < vertical_size; i ++){
        eraseLine(i);
    }
}

void drawOccupancyMap(){
    for (uint8_t x = 0; x < horizontal_size-2; x++){
        for (uint8_t y = 0; y < vertical_size-2; y++){
            if(occupancyMap[x][y] == true){
                moveCursorTo(x+2, y+2);
                serialPrint("X");
            }
        }
    }
}

void drawOccupancyMapLine(uint8_t y){
    for (uint8_t x = 0; x < horizontal_size-2; x++){
        if(occupancyMap[x][y] == true){
            moveCursorTo(x+2, y+2);
            serialPrint("X");
        }
    }
}

bool checkOccupancyMapLine(uint8_t y){
    for (uint8_t x = 0; x < horizontal_size-2; x++){
        if(occupancyMap[x][y] == true){
            return true;
        }
    }
    return false;
}

bool collisionMap(uint8_t x, uint8_t y){
    if(occupancyMap[x][y] == true){ return true;}
    else { return false;}
}

void eraseOccupancyMap(){
    for (uint8_t x = 0; x < horizontal_size-2; x++){
        for (uint8_t y = 0; y < vertical_size-2; y++){
            occupancyMap[x][y] = false;
        }
    }
}

void moveOccupancyMapDown(uint8_t& y_max){
    for (uint8_t x = 0; x < horizontal_size-2; x++){
        for (uint8_t y = y_max; y > 0; y--){
            occupancyMap[x][y] = occupancyMap[x][y-1];
        }
    }
}

void checkOccupancyMapForFullLine(){
    for (uint8_t y = 0; y < vertical_size-2; y++){
        uint8_t sum = 0;
        for (uint8_t x = 0; x < horizontal_size-2; x++){
            sum += occupancyMap[x][y];
        }
        if(sum == horizontal_size-2){
            moveCursorTo(40, y+2);
            serialPrint("!");
            moveOccupancyMapDown(y);
        }
    }
}

#endif /* DISPLAY_H_ */
