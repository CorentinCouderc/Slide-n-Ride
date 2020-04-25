#include <Gamebuino-Meta.h>

#include "Utils.h"
#include "Maps.h"

#define CHAR_WIDTH 4
#define CHAR_HEIGHT 4

bool isWin = false;
bool isLost = false;
int moves = 0;
int previousWalkedState = 1;

int characterPosX = 0;
int characterPosY = 0;
Color characterColor = BLUE;
AnimationState currentAnimationState = IDLE;

Color wallColor = BLACK;
Color walkableColor = WHITE;
Color firstWalkedColor = YELLOW;
Color secondWalkedColor = ORANGE;
Color thirdWalkedColor = RED;


void setup() 
{
  gb.begin();
  InitPlayerPosition();
}

void InitPlayerPosition()
{
  map_layout[PLAYER_START_Y][PLAYER_START_X] = 5;
  characterPosX = PLAYER_START_X;
  characterPosY = PLAYER_START_Y;
}


void loop() 
{
  while(!gb.update());
  gb.display.clear();


  ReadPlayerInput();
  UpdatePosition();
  isWin = CheckWin();
  if (isWin || isLost)
  {
    SetAnimationState(WIN);
  }
  Display();
}

/****************
 * Player Input *
 ****************/
void ReadPlayerInput()
{
  if (gb.buttons.pressed(BUTTON_MENU))
  {
    Reset();
  }

  if (CanPlayerInput())
    {
      if (gb.buttons.pressed(BUTTON_UP))
      {
        SetAnimationState(MOVE_UP);
        moves++;
      }
      else if (gb.buttons.pressed(BUTTON_DOWN))
      {
        SetAnimationState(MOVE_DOWN);
        moves++;
      }
      else if (gb.buttons.pressed(BUTTON_LEFT))
      {
        SetAnimationState(MOVE_LEFT);
        moves++;
      }
      else if (gb.buttons.pressed(BUTTON_RIGHT))
      {
        SetAnimationState(MOVE_RIGHT);
        moves++;
      }
    }
}

void SetAnimationState(AnimationState newAnimation)
{
  currentAnimationState = newAnimation;
}

bool CanPlayerInput()
{
  return currentAnimationState == IDLE;
}

void Reset()
{
  for (int i=0; i<MAP_HEIGHT; i++)
  {
    for (int j=0; j<MAP_WIDTH; j++)
    {
      if (map_layout[i][j] != 0)
      {
        map_layout[i][j] = 1;
      }
    }
  }
  
  InitPlayerPosition();
  moves = 0;
  isWin = false;
  isLost = false;
  SetAnimationState(IDLE);
}

/**********************
 * Character movement *
 **********************/
void UpdatePosition()
{
  int nextPosX = characterPosX;
  int nextPosY = characterPosY;

  switch (currentAnimationState)
  {
    case IDLE:
    case WIN: return;
    case MOVE_UP: nextPosY --; break;
    case MOVE_DOWN: nextPosY ++; break;
    case MOVE_LEFT: nextPosX --; break;
    case MOVE_RIGHT: nextPosX ++; break;
    default: break;
  }

  if (HasCollision(nextPosX, nextPosY))
  {
    SetAnimationState(IDLE);
  }
  else
  {
    previousWalkedState++;
    if (previousWalkedState == 5)
    {
      isLost = true;
      return;
    }
    map_layout[characterPosY][characterPosX] = previousWalkedState;
    previousWalkedState = map_layout[nextPosY][nextPosX];
    map_layout[nextPosY][nextPosX] = 5;
    characterPosX = nextPosX;
    characterPosY = nextPosY;
  }
  
}

bool HasCollision(int nextPosX, int nextPosY)
{
  return map_layout[nextPosY][nextPosX] == 0;
}

bool CheckWin()
{
  for (int i=0; i<MAP_HEIGHT; i++)
  {
    for (int j=0; j<MAP_WIDTH; j++)
    {
      if (map_layout[i][j] == 1)
      {
        return false;
      }
    }
  }
  return true;
}

/***********
 * Display *
 ***********/
void Display()
{
  Color color;
  for (int i=0; i<MAP_HEIGHT; i++)
  {
    for (int j=0; j<MAP_WIDTH; j++)
    {
      switch (map_layout[i][j])
      {
        case 0:
          color = wallColor;
          break;
        
        case 1:
          color = walkableColor;
          break;

        case 2:
          color = firstWalkedColor;
          break;

        case 3:
        color = secondWalkedColor;
        break;

        case 4:
        color = thirdWalkedColor;
        break;

        case 5:
          color = characterColor;
          break;
        
        default:
          break;
      }

      gb.display.setColor(color);
      gb.display.fillRect(j * CHAR_WIDTH, (i+1) * CHAR_HEIGHT, CHAR_WIDTH, CHAR_HEIGHT);
    }
  }

  gb.display.setCursorX(20);
  gb.display.setCursorY(50);
  gb.display.setColor(WHITE);
  gb.display.printf("MOVES : %d", moves);
}
