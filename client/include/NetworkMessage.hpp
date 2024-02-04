struct NetworkMessage
{
  //1st variable: 0==continue-game, 1==left-wins, 2==right-wins
  //2nd variable: 0==no-points, 1==left-point, 2==right-point
  //3rd-6th variable: left paddle x
  //7th-10th variable: left paddle y
  //11th-14th variable: right paddle x
  //15th-18th variable: right paddle y
  //19th variable: left score
  //20th variable: right score
  //21st-24th variable: game ball x
  //25th-28th variable: game ball y
  //29th-32nd variable: game ball x velocity
  //33rd-36th variable: game ball y velocity
};