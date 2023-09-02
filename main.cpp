#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "Pong.hpp"

int main(int argc, char *argv[])
{
  Pong game;

  bool success = game.Initialize();
  if (success)
  {
    game.RunLoop();
  }
  game.Shutdown();
  return 0;
}
