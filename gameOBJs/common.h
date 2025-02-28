#ifndef COMMON_H_
#define COMMON_H_

#include <SDL2/SDL.h>
#include <iostream>


#define SCREEN_SIZE_X 800 
#define SCREEN_SIZE_Y 600 

#define playerSpeed (SCREEN_SIZE_Y/150)+1
//i dont know how to define speed in any other way
//because if i define it as an intiger compiler says im redifining it
//it may be due to me including this file in more than one header 

const Uint32 white = 0xffffff;
const Uint32 black = 0x000000;

const Uint8 numberW = 80;
const Uint8 numberH = 100;

const Uint16 leftScoreX = numberW;
const Uint16 rightScoreX = SCREEN_SIZE_X - numberW*2 ;


const bool LEFT = true;
const bool RIGHT = false;

bool initSDL();
void resetGame();
void printScore(int *score);
void drawBorder();
bool loop(int *mvPl1=NULL,int *mvPl2=NULL,int *mvPl3=NULL,int *mvPl4=NULL,int *mvPl5=NULL,int *mvPl6=NULL);
int waitForStart();


#endif
