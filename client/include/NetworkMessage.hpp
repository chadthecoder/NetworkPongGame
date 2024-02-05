#include <string>

struct NetworkMessage //max 32 vars at 4 chars each, convert to binary?
{
  //1st variable: 0000==continue-game, 0001==left-wins, 0002==right-wins
  int win;
  //2nd variable: 0000==no-points, 0001==left-point, 0002==right-point
  int point;
  //3rd variable: left paddle x
  int leftPaddleX;
  //4th variable: left paddle y
  int leftPaddleY;
  //5th variable: right paddle x
  int rightPaddleX;
  //6th variable: right paddle y
  int rightPaddleY;
  //7th variable: 000-left score
  int leftScore;
  //8th variable: 000-right score
  int rightScore;
  //9th variable: game ball x
  int gameballX;
  //10th variable: game ball y
  int gameballY;
  //11th variable: game ball x velocity
  int gameballXVel;
  //12th variable: game ball y velocity
  int gameballYVel;
};