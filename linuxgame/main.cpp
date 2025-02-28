#include "game.h"

SDL_Window * window;
SDL_Surface * surface;
SDL_Surface * numbermap;
SDL_Surface * startScreen;
SDL_Surface * gameOver;
SDL_Rect startSrc = (SDL_Rect) {0,0,750,340};
SDL_Rect gameOverSrc = (SDL_Rect) {0,0,700,250};
SDL_Rect gameOverDst;
SDL_Rect startDst;

int GameSpeed = 10;
 
PlayerList players;

Ball * ballPtr; //need to create ball inside main, this is just so it is accesable outside main


static int score[2] {0,0};


int main()
{
  if(SCREEN_SIZE_X <= 240 || SCREEN_SIZE_Y <= 180)
  {
    GameSpeed = 20; Player::PLAYER_SPEED = Player::PLAYER_SPEED * 3;
  }//to compensaste for slow player in smaller game speeds

  //made more playable on low res
  
  if( !initSDL() ) return 1;

  if(SCREEN_SIZE_X > 750 && SCREEN_SIZE_Y > 340)
  {
   gameOverDst = (SDL_Rect) {(SCREEN_SIZE_X/2 - 350),(SCREEN_SIZE_Y/2 - 125)};
   startDst = (SDL_Rect) {(SCREEN_SIZE_X/2 - 375),(SCREEN_SIZE_Y/2 - 170)};
  }
  else
  {
    //this is just  lazier and more messy way of me putting the image out of the screen on smaller res
    //because the image is too big anyway and putting it 500 pixels away from each side makes it 
    //not appear on the screen
    gameOverDst= (SDL_Rect){500,500};
    startDst= (SDL_Rect){500,500};
  }


  int *player1move = players.addPlayer(LEFT,surface);
  int *player2move = players.addPlayer(RIGHT,surface);
//  int *player3move = players.addPlayer(RIGHT,surface);
//  int *player4move = players.addPlayer(RIGHT,surface);
//  int *player5move = players.addPlayer(RIGHT,surface);
//  int *player6move = players.addPlayer(RIGHT,surface);
  
  players.calculatePlayersPos(); //calculates their size and initial position
                                 
  Ball ball(surface);

  ballPtr = &ball;

  bool waiting = true;
    while( waiting )
  {
    int choice = waitForStart();
    if(choice < 0) return 0;
    else if(choice > 0) waiting = false;
    SDL_BlitSurface(startScreen,&startSrc,surface,&startDst);
    SDL_UpdateWindowSurface(window); 
    SDL_Delay(10);
  }//not starting till pressing Space or quitting by pressing ESC
    
  while( loop(player1move,player2move))//,player3move,player4move,player5move,player6move) )
  {
    SDL_UpdateWindowSurface(window);
    SDL_Delay(GameSpeed);
  }
  //the main game loop
  //all of the drawing is done inside the loop
  //it just updates the screen here
  
  SDL_FillRect(surface,NULL,black);
  SDL_BlitSurface(gameOver,&gameOverSrc,surface,&gameOverDst);
  SDL_UpdateWindowSurface(window);
  if(SCREEN_SIZE_X > 750 && SCREEN_SIZE_Y > 340) SDL_Delay(1000);
  //again lazy ...
  //game over screen only kept visible for 1 sec if it is displayed (high enough res)
  return 0;
}


int waitForStart()
{
  SDL_Event e; 
  while(SDL_PollEvent(&e))
  {
    switch(e.type)
    {
      case SDL_QUIT:
        return -1;
    }
  }
  const Uint8 *keystate = SDL_GetKeyboardState(NULL); 
  if(keystate[SDL_SCANCODE_SPACE]) return 1;
  if(keystate[SDL_SCANCODE_ESCAPE]) return -1;
  return 0;
}



bool initSDL()
{
  if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
  {
    std::cout << "Error initializing SDL\n";
    return false;
  }
  window = SDL_CreateWindow("pong",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_SIZE_X,SCREEN_SIZE_Y,SDL_WINDOW_SHOWN); 
  if(!window) 
  {
    std::cout << "Error creating a window\n";
    return false;
  }
  surface = SDL_GetWindowSurface(window);
  if(!surface)
  {
    std::cout << "Error getting window surface\n";
    return false;
  }
  numbermap = SDL_LoadBMP("numberz.bmp");
  if(!numbermap)
  {
    std::cout << "Error loading numberz.bmp\n";
    return false;
  }
  startScreen = SDL_LoadBMP("startScreen.bmp");
  if(!startScreen)
  {
    std::cout << "Error loading startScreen.bmp\n";
    return false;
  }
  gameOver = SDL_LoadBMP("gameOver.bmp");
  if(!gameOver)
  {
    std::cout << "Error loading gameOver.bmp\n";
    return false;
  }
  return true;
}


void resetGame()
{
  SDL_Delay(500);
  ballPtr->reset(); 
  players.resetPos();
}

bool loop(int *mvPl1,int *mvPl2,int *mvPl3,int *mvPl4,int *mvPl5,int *mvPl6)
{
           
  SDL_Event e;

  const Uint8 *keystate = SDL_GetKeyboardState(NULL);
  while(SDL_PollEvent(&e) != 0)
  {
    switch(e.type)
    {
      case SDL_QUIT:
        return false;
    }
  }

  if(keystate[SDL_SCANCODE_ESCAPE])
  {
    return false;
  }

  if(mvPl1)
  {
  *mvPl1 = 0; //Zero to reset player move
  if(!(keystate[SDL_SCANCODE_W] && keystate[SDL_SCANCODE_S]))
  {
    if(keystate[SDL_SCANCODE_W])
    {
      *mvPl1 = 1; //move pl1 up
    }
    else if(keystate[SDL_SCANCODE_S])
    {
      *mvPl1 = -1; //down
    }
  }
  }//checks of player inputs

  if(mvPl2)
  {
  *mvPl2 = 0;
  if(!(keystate[SDL_SCANCODE_UP] && keystate[SDL_SCANCODE_DOWN]))
  {
    if(keystate[SDL_SCANCODE_UP])
    {
      *mvPl2 = 1; //move pl2 up
    }
    else if(keystate[SDL_SCANCODE_DOWN])
    {
      *mvPl2 = -1; //down
    }
  }
  }

  if(mvPl3)
  {
  *mvPl3 = 0;
  if(!(keystate[SDL_SCANCODE_KP_PLUS] && keystate[SDL_SCANCODE_KP_MINUS]))
  {
    if(keystate[SDL_SCANCODE_KP_MINUS])
    {
      *mvPl3 = 1; 
    }
    else if(keystate[SDL_SCANCODE_KP_PLUS])
    {
      *mvPl3 = -1;
    }
  }
  }

  if(mvPl4)
  {
  *mvPl4 = 0;
  if(!(keystate[SDL_SCANCODE_Z] && keystate[SDL_SCANCODE_X]))
  {
    if(keystate[SDL_SCANCODE_Z])
    {
      *mvPl4 = 1; 
    }
    else if(keystate[SDL_SCANCODE_X])
    {
      *mvPl4 = -1;
    }
  }
  }

  if(mvPl5)
  {
  *mvPl5 = 0;
  if(!(keystate[SDL_SCANCODE_V] && keystate[SDL_SCANCODE_B]))
  {
    if(keystate[SDL_SCANCODE_V])
    {
      *mvPl5 = 1;
    }
    else if(keystate[SDL_SCANCODE_B])
    {
      *mvPl5 = -1; 
    }
  }
  }

  if(mvPl6)
  {
  *mvPl6 = 0;
  if(!(keystate[SDL_SCANCODE_PERIOD] && keystate[SDL_SCANCODE_COMMA]))
  {
    if(keystate[SDL_SCANCODE_PERIOD])
    {
      *mvPl6 = 1; 
    }
    else if(keystate[SDL_SCANCODE_COMMA])
    {
      *mvPl6 = -1;
    }
  }
  }


  SDL_FillRect(surface,NULL,black);
  //clear screen
  if(SCREEN_SIZE_X > 320 && SCREEN_SIZE_Y > 240) printScore(score);
  //print score only on high enough res
  
  drawBorder();

  ballPtr->checkColl(players);

  int point = ballPtr->move();
  if(point < 0) score[1]++;
  if(point > 0) score[0]++;
  if(score[0] > 9 || score[1] > 9) return false;
  //if more than 9 points on 1 side quits the game

  players.movePlayers();


  return true;
}


void printScore(int *score)
{
  for(int i = 0; i < 2; i++)
  {
  int side;
  if(i == 0)
  {
    side = leftScoreX;  
  }
  else
  {
    side = rightScoreX;
  }

  SDL_Rect src = (SDL_Rect){0,0,numberW,numberH};
  SDL_Rect dest = (SDL_Rect){side,0,SCREEN_SIZE_X /16 ,SCREEN_SIZE_Y / 7};

  if(score[i] > 0 && score[i] < 10)
  {
    src.x = src.w * score[i];
  }
  SDL_BlitSurface(numbermap,&src,surface,&dest);

  } 
}



void drawBorder()
{
  int gap = SCREEN_SIZE_Y / 30;
  SDL_Rect border = (SDL_Rect) {(SCREEN_SIZE_X/2),gap,2,SCREEN_SIZE_Y};  
  for(int i = 0; i < 15; i++)
  {
    SDL_FillRect(surface,&border,white);
    border.y += gap;
    SDL_FillRect(surface,&border,black);
    border.y += gap;
  }
}



