#ifndef BALL_H_
#define BALL_H_

#include "common.h"

class PlayerList;
class Player;

class Ball
{
  public:
    Ball(SDL_Surface * surface);
    int move(); 
    void checkColl(const PlayerList &pl);
    void reset();
    friend class PlayerList;
    friend class Player;

  private:
    SDL_Rect shape;
    SDL_Surface *surface;
    int x;
    int y;
};




#endif
