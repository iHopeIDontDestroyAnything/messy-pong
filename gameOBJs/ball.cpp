#include "gameObjects.h"


Ball::Ball(SDL_Surface* _surface)
{
  shape.x = SCREEN_SIZE_X /2;
  shape.y = SCREEN_SIZE_Y /2; 
  shape.w = (SCREEN_SIZE_X / 124) +2;//constant 2 for better visibility on low res 
  shape.h = shape.w; //for proportions
  x = (SCREEN_SIZE_X / 1280) + 1; //under 1280 starts on 1 speed, over is more than one 
  y = (SCREEN_SIZE_Y / 720) + 1; 
  surface = _surface;
}

int Ball::move()
{
  int goal = 0;
  if((shape.y +shape.h + y)>SCREEN_SIZE_Y || (shape.y + y) < 0 )
  {
    y = -y;
  }
  if((shape.x + shape.w + x)>SCREEN_SIZE_X || (shape.x + x) < 0)
  {
    goal =  x; 
    resetGame();
  }
  shape.x += x;
  shape.y += y;
  SDL_FillRect(surface,&shape,white);
  return goal; // negative value when goal was scored on the left side an vice versa
}

void Ball::checkColl(const PlayerList &pl)
{
  bool direction;
  if(x < 0) direction = LEFT;   
  else direction = RIGHT; //determine ball direction to only calculate collision with 
                          //players in the way of the ball

  Player * temp = pl.first; 
  while(temp)
  {
    if(temp->side == direction)
    {
      int ball_l ,player_l;
      int ball_r ,player_r;
      int ball_t ,player_t;
      int ball_b ,player_b;

      ball_l = shape.x;
      ball_r = shape.x + shape.w;
      ball_t = shape.y;
      ball_b = shape.y + shape.h;

      player_l = temp->shape.x;
      player_r = temp->shape.x + temp->shape.w;
      player_t = temp->shape.y;
      player_b = temp->shape.y + temp->shape.h;

      if(player_r < ball_l){}
      else if(player_l > ball_r){}
      else if(player_t > ball_b){}
      else if(player_b < ball_t){}
      else
      {
        if(x > 0)
        {
          x++;
        }
        else 
        {
          x--;
        }
        x = -x;
        y = temp->calculateBounce(*this);
      }

    }
    temp = temp->next;
  }
}

void Ball::reset()
{
  x = (SCREEN_SIZE_X / 1280) + 1; //under 1280 starts on 1 speed, over is more than one 
  y = (SCREEN_SIZE_Y / 720) + 1; 
  shape.x = SCREEN_SIZE_X/2;
  shape.y = SCREEN_SIZE_Y/2;
}//ball back to the middle of the screen

