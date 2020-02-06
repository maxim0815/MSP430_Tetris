/*
 * Figures.h
 *
 *  Created on: 31.01.2020
 *      Author: max
 */

#ifndef INCLUDE_FIGURES_H_
#define INCLUDE_FIGURES_H_

enum Movement { left, down, top, right, none };

class Block{
public:
    Block(){}
    uint8_t x;
    uint8_t y;
    void print(){
        moveCursorTo(x, y);
        serialPrint("$");
    }
    void down(){
        y += 1;
    }
    void right(){
        x += 1;
    }
    void left(){
        x -= 1;
    }
private:
};

class Figure{
private:
    void initADC(){
        ADC10CTL0 = ADC10ON + ADC10SHT_2;
    }

    int getADC(){
        ADC10AE0 |= BIT5;
        ADC10CTL1 = INCH_5;

        ADC10CTL0 |= ENC + ADC10SC;
        // Wait until result is ready
        while(ADC10CTL1 & ADC10BUSY);
        // If result is ready, copy it to m1

        ADC10CTL0 &= ~ENC;
        return ADC10MEM;
    }

    int genSeed(){
        initADC();
        int value = getADC();
        return value;
    }

    int genRandomNumber(){
        int seed = genSeed();//using floating ADC as seed
        srand(seed);
        int randnum = 1 + (rand() % static_cast<int>(5 - 1 + 1)); //gen random number between 1 and 5; change maximum if more figures are added
        return randnum;
    }

public:
    Figure(){};
    ~Figure(){};
    std::vector<Block> Blocks;
    bool stop_movement = false;
    int figure_state = 1;

    int rand_block_init = genRandomNumber();
    bool checkCollision(Movement movement){
       for(int i = 0; i < Blocks.size(); i ++){
           if(movement == Movement::down){
               if(Blocks[i].y >= vertical_size-1){
                   stop_movement = true;
                   return true;
               }
               if(collisionMap(Blocks[i].x-2, Blocks[i].y-1) == true){
                   stop_movement = true;
                   return true;
               }
           }
           if(movement == Movement::left){
               if(Blocks[i].x < 3){
                  return true;
              }
               if(collisionMap(Blocks[i].x-3, Blocks[i].y-2) == true){
                   return true;
               }
           }
           if(movement == Movement::right){
               if(Blocks[i].x >= horizontal_size -1){
                   return true;
               }
               if(collisionMap(Blocks[i].x-1, Blocks[i].y-2) == true){
                   return true;
               }
           }
           if(movement == Movement::top){
               // collision top border?
               if(Blocks[i].y < 2){
                   return true;
               }
               // collision bottom border?
               if(Blocks[i].y >= vertical_size-1){
                   return true;
               }
               // collision with object below?
               if(collisionMap(Blocks[i].x-2, Blocks[i].y-1) == true){
                   return true;
               }
               // collision left border?
               if(Blocks[i].x < 3){
                   return true;
               }
               // collision with object on left side?
               if(collisionMap(Blocks[i].x-3, Blocks[i].y-2) == true){
                   return true;
               }
               // collision with right border?
               if(Blocks[i].x >= horizontal_size -1){
                   return true;
               }
               // collision with object on right side?
               if(collisionMap(Blocks[i].x-1, Blocks[i].y-2) == true){
                   return true;
               }
           }
       }
       return false;
    }

    void moveDown(){
       if(checkCollision(Movement::down) == false){
           for(int i = 0; i < Blocks.size(); i ++){
               Blocks[i].down();
           }
       }
    }

    void moveLeft(){
        if(checkCollision(Movement::left) == false && stop_movement == false){
           for(int i = 0; i < Blocks.size(); i ++){
               Blocks[i].left();
           }
       }
    }

    void moveRight(){
        if(checkCollision(Movement::right) == false && stop_movement == false){
           for(int i = 0; i < Blocks.size(); i ++){
               Blocks[i].right();
           }
       }
    }

    void print(){
        for(int i = 0; i < Blocks.size(); i ++){
            Blocks[i].print();
        }
    }

    void printWithMapLine(){
        for(int i = 0; i < Blocks.size(); i ++){
            Blocks[i].print();
            drawOccupancyMapLine(Blocks[i].y);
        }
    }

    void clear(){
        for(int i = 0; i < Blocks.size(); i ++){
            eraseLine(Blocks[i].y);
            drawOccupancyMapLine(Blocks[i].y-2);
        }
    }

    void fillOccupancyMap(){
        for(int i = 0; i < Blocks.size(); i ++){
           occupancyMap[Blocks[i].x-2][Blocks[i].y-2] = true;
        }
    }
};

class RandomFigure : public Figure {
private:
    void initT(Block& zero, Block& one, Block& two, Block& three,int x,int y){
        zero.x = x;
        zero.y = y;
        one.x = x - 1;
        one.y = y + 1;
        two.x = x;
        two.y = y + 1;
        three.x = x + 1;
        three.y = y + 1;
        Blocks.push_back(zero);
        Blocks.push_back(one);
        Blocks.push_back(two);
        Blocks.push_back(three);
    }
    void initSuare(Block& zero, Block& one, Block& two, Block& three,int x,int y){
        zero.x = x;
        zero.y = y;

        one.x = x + 1;
        one.y = y;

        two.x = x;
        two.y = y + 1;

        three.x = x + 1;
        three.y = y + 1;

        Blocks.push_back(zero);
        Blocks.push_back(one);
        Blocks.push_back(two);
        Blocks.push_back(three);
    }
    void initBar(Block& zero, Block& one, Block& two, Block& three,int x,int y){
        zero.x = x - 1;
        zero.y = y;

        one.x = x;
        one.y = y;

        two.x = x + 1;
        two.y = y;

        three.x = x + 2;
        three.y = y;

        Blocks.push_back(zero);
        Blocks.push_back(one);
        Blocks.push_back(two);
        Blocks.push_back(three);
    }
    void initLleft(Block& zero, Block& one, Block& two, Block& three,int x,int y){
        zero.x = x - 1;
        zero.y = y;

        one.x = x-1;
        one.y = y+1;

        two.x = x;
        two.y = y+1;

        three.x = x + 1;
        three.y = y+1;

        Blocks.push_back(zero);
        Blocks.push_back(one);
        Blocks.push_back(two);
        Blocks.push_back(three);
    }
    void initLright(Block& zero, Block& one, Block& two, Block& three,int x,int y){
        zero.x = x + 1;
        zero.y = y;

        one.x = x-1;
        one.y = y+1;

        two.x = x;
        two.y = y+1;

        three.x = x + 1;
        three.y = y+1;

        Blocks.push_back(zero);
        Blocks.push_back(one);
        Blocks.push_back(two);
        Blocks.push_back(three);
    }



    void rotateT(){
        // remember old figure in case rotation is not allowed
        std::vector<Block> Blocks_rem;
        for(int i = 0; i < Blocks.size(); i ++){
            Blocks_rem.push_back(Blocks[i]);
        }
        if(figure_state == 1){
            figure_state += 1;

            Blocks[0].x +=1;
            Blocks[0].y +=1;
            Blocks[1].x +=1;
            Blocks[1].y -=1;
            Blocks[3].x -=1;
            Blocks[3].y +=1;

            if(checkCollision(Movement::top) == true){
                Blocks.clear();
                for(int i = 0; i < Blocks_rem.size(); i ++){
                    Blocks.push_back(Blocks_rem[i]);
                }
                figure_state =1;
            }

        }
        else if (figure_state == 2){
            figure_state +=1 ;

            Blocks[0].x -=1;
            Blocks[0].y +=1;
            Blocks[1].x +=1;
            Blocks[1].y +=1;
            Blocks[3].x -=1;
            Blocks[3].y -=1;

            if(checkCollision(Movement::top) == true){
                Blocks.clear();
                for(int i = 0; i < Blocks_rem.size(); i ++){
                    Blocks.push_back(Blocks_rem[i]);
                }
                figure_state =2;
            }
        }
        else if (figure_state == 3){
            figure_state +=1 ;

            Blocks[0].x -=1;
            Blocks[0].y -=1;
            Blocks[1].x -=1;
            Blocks[1].y +=1;
            Blocks[3].x +=1;
            Blocks[3].y -=1;

            if(checkCollision(Movement::top) == true){
                Blocks.clear();
                for(int i = 0; i < Blocks_rem.size(); i ++){
                    Blocks.push_back(Blocks_rem[i]);
                }
                figure_state =3;
            }

        }
        else if (figure_state == 4) {
            figure_state = 1;

            Blocks[0].x +=1;
            Blocks[0].y -=1;
            Blocks[1].x -=1;
            Blocks[1].y -=1;
            Blocks[3].x +=1;
            Blocks[3].y +=1;

            if(checkCollision(Movement::top) == true){
                Blocks.clear();
                for(int i = 0; i < Blocks_rem.size(); i ++){
                    Blocks.push_back(Blocks_rem[i]);
                }
                figure_state =4;
            }
        }
    }

    void rotateBar(){
        // remember old figure in case rotation is not allowed
        std::vector<Block> Blocks_rem;
        for(int i = 0; i < Blocks.size(); i ++){
            Blocks_rem.push_back(Blocks[i]);
        }
        if(figure_state == 1){
            figure_state += 1;

            Blocks[0].x +=1;
            Blocks[0].y -=1;
            Blocks[2].x -=1;
            Blocks[2].y +=1;
            Blocks[3].x -=2;
            Blocks[3].y +=2;

            if(checkCollision(Movement::top) == true){
                Blocks.clear();
                for(int i = 0; i < Blocks_rem.size(); i ++){
                    Blocks.push_back(Blocks_rem[i]);
                }
                figure_state =1;
            }
        }
        else if (figure_state == 2){
            figure_state = 1 ;

            Blocks[0].x -=1;
            Blocks[0].y +=1;
            Blocks[2].x +=1;
            Blocks[2].y -=1;
            Blocks[3].x +=2;
            Blocks[3].y -=2;

            if(checkCollision(Movement::top) == true){
                Blocks.clear();
                for(int i = 0; i < Blocks_rem.size(); i ++){
                    Blocks.push_back(Blocks_rem[i]);
                }
                figure_state = 2;
            }
        }
    }
    void rotateLleft(){
        // remember old figure in case rotation is not allowed
        std::vector<Block> Blocks_rem;
        for(int i = 0; i < Blocks.size(); i ++){
            Blocks_rem.push_back(Blocks[i]);
        }
        if(figure_state == 1){
            figure_state += 1;

            Blocks[0].x +=2;
            //Blocks[0].y -=1;
            Blocks[1].x +=1;
            Blocks[1].y -=1;
            Blocks[3].x -=1;
            Blocks[3].y +=1;

            if(checkCollision(Movement::top) == true){
                Blocks.clear();
                for(int i = 0; i < Blocks_rem.size(); i ++){
                    Blocks.push_back(Blocks_rem[i]);
                }
                figure_state =1;
            }

        }
        else if (figure_state == 2){
            figure_state +=1 ;

            //Blocks[0].x -=1;
            Blocks[0].y +=2;
            Blocks[1].x +=1;
            Blocks[1].y +=1;
            Blocks[3].x -=1;
            Blocks[3].y -=1;

            if(checkCollision(Movement::top) == true){
                Blocks.clear();
                for(int i = 0; i < Blocks_rem.size(); i ++){
                    Blocks.push_back(Blocks_rem[i]);
                }
                figure_state =2;
            }
        }
        else if (figure_state == 3){
            figure_state += 1 ;

            Blocks[0].x -=2;
            //Blocks[0].y +=2;
            Blocks[1].x -=1;
            Blocks[1].y +=1;
            Blocks[3].x +=1;
            Blocks[3].y -=1;

            if(checkCollision(Movement::top) == true){
                Blocks.clear();
                for(int i = 0; i < Blocks_rem.size(); i ++){
                    Blocks.push_back(Blocks_rem[i]);
                }
                figure_state =3;
            }
        }
        else if (figure_state == 4){
            figure_state = 1 ;

            //Blocks[0].x +=2;
            Blocks[0].y -=2;
            Blocks[1].x -=1;
            Blocks[1].y -=1;
            Blocks[3].x +=1;
            Blocks[3].y +=1;

            if(checkCollision(Movement::top) == true){
                Blocks.clear();
                for(int i = 0; i < Blocks_rem.size(); i ++){
                    Blocks.push_back(Blocks_rem[i]);
                }
                figure_state =4;
            }
        }

    }

    void rotateLright(){
        // remember old figure in case rotation is not allowed
        std::vector<Block> Blocks_rem;
        for(int i = 0; i < Blocks.size(); i ++){
            Blocks_rem.push_back(Blocks[i]);
        }
        if(figure_state == 1){
            figure_state += 1;

            //Blocks[0].x +=2;
            Blocks[0].y +=2;
            Blocks[1].x +=1;
            Blocks[1].y -=1;
            Blocks[3].x -=1;
            Blocks[3].y +=1;

            if(checkCollision(Movement::top) == true){
                Blocks.clear();
                for(int i = 0; i < Blocks_rem.size(); i ++){
                    Blocks.push_back(Blocks_rem[i]);
                }
                figure_state =1;
            }

        }
        else if (figure_state == 2){
            figure_state +=1 ;

            Blocks[0].x -=2;
            //Blocks[0].y +=2;
            Blocks[1].x +=1;
            Blocks[1].y +=1;
            Blocks[3].x -=1;
            Blocks[3].y -=1;

            if(checkCollision(Movement::top) == true){
                Blocks.clear();
                for(int i = 0; i < Blocks_rem.size(); i ++){
                    Blocks.push_back(Blocks_rem[i]);
                }
                figure_state =2;
            }
        }
        else if (figure_state == 3){
            figure_state += 1 ;

            //Blocks[0].x -=2;
            Blocks[0].y -=2;
            Blocks[1].x -=1;
            Blocks[1].y +=1;
            Blocks[3].x +=1;
            Blocks[3].y -=1;

            if(checkCollision(Movement::top) == true){
                Blocks.clear();
                for(int i = 0; i < Blocks_rem.size(); i ++){
                    Blocks.push_back(Blocks_rem[i]);
                }
                figure_state =3;
            }
        }
        else if (figure_state == 4){
            figure_state = 1 ;

            Blocks[0].x +=2;
            //Blocks[0].y -=2;
            Blocks[1].x -=1;
            Blocks[1].y -=1;
            Blocks[3].x +=1;
            Blocks[3].y +=1;

            if(checkCollision(Movement::top) == true){
                Blocks.clear();
                for(int i = 0; i < Blocks_rem.size(); i ++){
                    Blocks.push_back(Blocks_rem[i]);
                }
                figure_state =4;
            }
        }
    }

public:
    RandomFigure(int x, int y){


        Block zero;
        Block one;
        Block two;
        Block three;

        switch (rand_block_init){
        case 1://T
            initT(zero, one, two, three, x, y);
            break;
        case 2://suare
            initSuare(zero, one, two, three, x, y);
            break;
        case 3://Bar
            initBar(zero, one, two, three, x, y);
            break;
        case 4:// L left
            initLleft(zero, one, two, three, x, y);
            break;
        case 5://L right
            initLright(zero, one, two, three, x, y);
            break;
        }

    }
    ~RandomFigure(){
    };

    Movement rotation = Movement::top;


    void rotate(){
        switch (rand_block_init){
        case 1://T
            rotateT();
            break;
        case 2://suare

            break;
        case 3://bar
            rotateBar();
            break;
        case 4://L left
            rotateLleft();
            break;
        case 5:// L right
            rotateLright();
            break;
        }

    }
};

#endif /* INCLUDE_FIGURES_H_ */
