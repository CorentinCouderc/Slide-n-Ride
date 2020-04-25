#ifndef UTILS_H
#define UTILS_H

enum AnimationState
{
    IDLE,
    WIN,
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_UP,
    MOVE_DOWN
};

int fmax (int a, int b)
{
    return a > b ? a : b;
}

int fmin (int a, int b)
{
    return a < b ? a : b;
}


int clamp(int value, int minValue = 0, int maxValue = 1)
{
  return max(min(value, maxValue), minValue);
}

void SetAnimationState (AnimationState newAnimation);
bool CanPlayerInput();


#endif