#ifndef PLAYER_H_
#define PLAYER_H_

#include "common.h"
#include "ball.h"

class Player
{
    Player(bool left,SDL_Surface * _surface,int * moveValPtr);
    void movePlayer();
    int calculateBounce(Ball &ball);
    void resetPos();

    friend class PlayerList;
    friend class Ball;

    SDL_Rect shape;
    SDL_Surface *surface;
    Player *next;
    int *move;
    bool side;  
  public:
    static int PLAYER_SPEED;

};

class PlayerList
{
  public:
    PlayerList();
    ~PlayerList();
    int* addPlayer(bool side,SDL_Surface * _surface);
    void calculatePlayersPos();
    void movePlayers();
    void resetPos();
    friend class Ball;

  private:
    Player *first;
    Player *last;
};

#endif
