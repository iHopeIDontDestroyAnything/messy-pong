#include "player.h"

int Player::PLAYER_SPEED = playerSpeed;

Player::Player(bool left,SDL_Surface * _surface,int * moveValPtr)
{
  move = moveValPtr;
  side = left;
  next = NULL;
  
  surface = _surface;
  //stores pointer to surface in order to be able to be draw from inside the class
  //
  shape.w = (SCREEN_SIZE_X / 96) + 2; //constant 2 for smaller windows to be more visible 
  shape.h = shape.w *4; //height is 5 times more than width for proportions
  if(left)
  {
    shape.x = (4*shape.w);
  }
  else
  {
    shape.x = (SCREEN_SIZE_X - (5*shape.w));
  }
  shape.y = (SCREEN_SIZE_Y / 2);

}


void Player::movePlayer()
{
  switch(*move)
  {
    case 1:
      if(!((shape.y - PLAYER_SPEED) <= 0))
      {
      shape.y -= PLAYER_SPEED;
      }
      break;
    case -1:
      if(!((shape.y + PLAYER_SPEED) >= (SCREEN_SIZE_Y - shape.h)))
      {
      shape.y += PLAYER_SPEED;
      }
      break; 
  }
  SDL_FillRect(surface,&shape,white);
}

int Player::calculateBounce(Ball &ball)
{
  float BallY, PlayerY;
  BallY = ball.shape.y + ball.shape.h/2 -2;  //center of the Y side of the ball 
  PlayerY = shape.y;


  float offset = BallY - PlayerY;  
  if(SCREEN_SIZE_Y >= 600)//this is the impact ofset from the top of the player 
  {                          
    float playerTenth = shape.h /20; //devided by twenty but named tenth because i am lazy to rename it
    //if screen res Y is more than 600 because on low res it is too fast
    if(offset <= playerTenth)
      {
        return -4;
      }
    else if(offset > playerTenth && offset <= 2*playerTenth)
      {
        return -3;
      }
    else if(offset > 2*playerTenth && offset <= 5*playerTenth)
      {
        return -2;
      }
    else if(offset > 5*playerTenth && offset <= 9*playerTenth)
      {
        return -1;
      }
    else if(offset > 9*playerTenth && offset <= 11*playerTenth)
      {
        return 0;
      }
    else if(offset > 11*playerTenth && offset <= 15*playerTenth)
      {
        return 1;
      }
    else if(offset > 15*playerTenth && offset <= 18*playerTenth)
      {
        return 2;
      }
    else if(offset > 18*playerTenth && offset <= 19*playerTenth)
      {
        return 3;
      }
    else 
      {
        return 4;
      } 
  }

  else if(SCREEN_SIZE_Y  >= 120 && SCREEN_SIZE_Y < 600)
  {
    //medium res
    float playerTenth = shape.h / 19; //again named tenth but I  was lazy to rename
    if(offset <= 2*playerTenth)
      {
        return -2;
      }
    else if(offset > 2*playerTenth && offset <=8*playerTenth)
      {
        return -1;
      }
    else if(offset > 8*playerTenth && offset <=12*playerTenth)
      {
        return 0;
      }
    else if(offset > 12*playerTenth && offset <= 18*playerTenth)
      {
      return 1;
      }
    else
      {
        return 2;
      }
  }
  else
  {
    //small res
    float playerNinth = shape.h / 9;
    if(offset < 3*playerNinth)
      {
        return -1;
      }
    else if(offset >= 3*playerNinth && offset < 6* playerNinth)
      {
        return 0;
      }
    else
      {
        return 1;
      }
  }
  //if none ef the above (definitely a bug) return 0 ==flying straight
  return 0;
}

PlayerList::PlayerList()
{
   first = NULL; 
   last = NULL;
}

int* PlayerList::addPlayer(bool side,SDL_Surface * surface)
{
  int * playerMovePtr = new int ;
  if(!first)//empty list
  { first = new Player(side,surface,playerMovePtr); 
    last = first;
  }
  else
  {
    Player * temp = last; 
    last = new Player(side,surface,playerMovePtr);
    temp->next = last;
  }

  return playerMovePtr;//pointer to intiger that is looked at when deciding player movement  
}


void Player::resetPos()
{
  shape.y = SCREEN_SIZE_Y /2;
}


PlayerList::~PlayerList()
{
  while(first)
  {
    Player * temp = first;
    delete first;
    first = temp->next;
  }

}


void PlayerList::calculatePlayersPos()
{
  float LeftPlayerCount = 0;
  float RightPlayerCount = 0;
  float amplification;
  int Side; // -1 == both sides , 0 == right , 1 == left
  Player *temp = first; 
  while(temp)
  {
    switch(temp->side)
    {
      case true:

        LeftPlayerCount++;
        break;
      case false:
        RightPlayerCount++;
    }
    temp = temp->next;
  }//calculates right and left players
   
  if(RightPlayerCount == LeftPlayerCount && RightPlayerCount > 1)
  {//when player counts are same on both sides but there are more players
    Side = -1; //both sides should be amplified (neither true or false)
    amplification = RightPlayerCount / 2; //full player size is too much, devided by 2 seems ideal
  }
  else if(RightPlayerCount > LeftPlayerCount)  
  {
     Side = RIGHT;
     amplification = RightPlayerCount / LeftPlayerCount; 
  }
  else
  {
     Side = LEFT;
     amplification = LeftPlayerCount / RightPlayerCount; 
  }

  temp = first;
  while(temp)
  {
    if(Side >= 0) //checks if both  sides should be amplified or not 
    {
      if(temp->side == Side)
      {//amplifies only the side with more players
        temp->shape.h = temp->shape.h / amplification + 2;
        temp->shape.w = temp->shape.w / amplification + 2;
      } 
    }
    else
    {//amplifies both sides
      temp->shape.h = temp->shape.h / amplification + 2;
      temp->shape.w = temp->shape.w / amplification + 2;
    }
    temp = temp->next;
  }
  //player size multiplier (negative so it devides their size)
  temp = first; 

  float indent = temp->shape.w * 1.5; //next player has at least a player sized gap between previus player 
    
  int indentMultiplierLeft = 0; //for every player on one side it is higher
  int indentMultiplierRight = 0;//same on the other side 

  while(temp)
  {
    if(temp->side == RIGHT && RightPlayerCount > 1)
    {
      temp->shape.x -= indent * indentMultiplierRight;     
      indentMultiplierRight++;
    }
    else if(temp->side == LEFT && LeftPlayerCount > 1)
    {
      temp->shape.x += indent * indentMultiplierLeft;
      indentMultiplierLeft++;
    }
    temp = temp->next;
  }
}


void PlayerList::movePlayers()
{
  Player * temp = first;
  while(temp)
  {
    temp->movePlayer();
    temp = temp->next;
  }
}



void PlayerList::resetPos()
{
  Player *temp = first;
  while(temp)
  {
    temp->resetPos();
    temp = temp->next;
  }

}
