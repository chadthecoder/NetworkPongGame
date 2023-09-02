#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "Pong.hpp"

int main(int argc, char *argv[])
{
  Pong game;

  bool success = game.Initialize();
  std::cout << std::to_string(success) << std::endl;
  if (success)
  {
    game.RunLoop();
  }
  game.Shutdown();
  return 0;
}
