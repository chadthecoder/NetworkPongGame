#include <Pong.hpp>

#include <iostream>
// using namespace std;

SDL_Texture *textLeftScore, *textRightScore;
SDL_Rect rectLeftSrc, rectRightSrc, rectDest;

Pong::Pong(std::string ip, std::string port) : net(ip, "1025"), networkMessage(""), mIsRunning(true), mTicksCount(0), gameBall(1500.0f, 500.0f, -100.0f, 117.5f, 15, 15), leftPoints(0), rightPoints(0)
{
  // print the ref of io_context to show that it worked for now
  // this->io_context = io_context;
  //this->StartSend();
}

bool Pong::checkHorizWallColl(Ball ball)
{
  if ((ball.y >= (this->screenHeight - this->thickness)) && (ball.yVelocity > 0.0f))
  {
    return true;
  }
  else if ((ball.y <= this->thickness) && (ball.yVelocity < 0.0f))
  {
    return true;
  }
  else return false;
}

int Pong::CheckWin()
{
  // game over because one side has 7 points
  if (this->leftPoints == 7)
  {
    //std::cout << "The left side wins!\n";
    /*
      Doesn't do anything right now since returning false stops loop, but could be used in the future.
      this->mIsRunning = false;
    */
   this->mIsRunning = false;
    return 2;
  }

  if (this->rightPoints == 7)
  {
    //std::cout << "The right side wins!\n";
    /*
      Doesn't do anything right now since returning false stops loop, but could be used in the future.
      this->mIsRunning = false;
    */
   this->mIsRunning = false;
    return 3;
  }

  else { return 1; }
}

void Pong::centerVector2(Vector2 vec)
{
  vec.x = this->screenWidth / 2;
  vec.y = this->screenHeight / 2;
}

bool Pong::Initialize()
{
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
  {
    SDL_Log("Error: %s", SDL_GetError());
    return false;
  }

  SDL_GetCurrentDisplayMode(0, &this->DM);
  this->screenHeight = this->DM.h - 90; //-80 to not be under bottom bar
  this->screenWidth = this->DM.w;
  this->thickness = this->screenWidth / 100;
  this->paddleHeight = this->thickness * 6;
  this->paddleU.height = paddleHeight;
  this->paddleWidth = this->thickness;
  this->paddleU.width = this->paddleWidth;

  /* Making it fullscreen only is the only non complicated way   *
   * to make it look good on Ubuntu because of Ubuntu's top bar. */
  this->mWindow = SDL_CreateWindow(
      "Pong Game",
      SDL_WINDOWPOS_CENTERED,
      0, // start from top
      this->screenWidth,
      this->screenHeight,
      0); //SDL_WINDOW_FULLSCREEN);

  /*this->mWindow = SDL_CreateWindow(
      "Pong Game",
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      800,
      800,
      0);*/

  if (!this->mWindow)
  {
    SDL_Log("Error: %s", SDL_GetError());
    return false;
  }

  // make window icon
  SDL_Surface *icon = IMG_Load("../assets/imgs/pongIcon.png");

  if (!icon)
  {
    std::cout << "icon failed/n" << std::endl;
    // return error? or should be break?
    return 1;
  }

  SDL_SetWindowIcon(mWindow, icon);

  mRenderer = SDL_CreateRenderer(
      this->mWindow,
      -1,
      SDL_RENDERER_ACCELERATED || SDL_RENDERER_PRESENTVSYNC);

  if (!mRenderer)
  {
    SDL_Log("Failed to create renderer: %s", SDL_GetError());
    return false;
  }

  // mBallPos.x = this->screenWidth / 2;
  // mBallPos.y = this->screenHeight / 2;
  // this->centerVector2(this->mBallPos);
  gameBall.x = this->screenWidth / 2;
  gameBall.y = this->screenHeight / 2;

  paddleU.x = this->screenWidth / 16;
  paddleU.y = this->screenHeight / 2;

  // funny.x = this->screenWidth * 15 / 16;
  // funny.y = this->screenHeight / 2;

  // keep this and remove initalize objects function?
  this->funny = this->createPaddle(
      this->screenWidth * 15 / 16, // static_cast<int>(paddleU.x - this->thickness / 2),
      this->screenHeight / 2,      // static_cast<int>(paddleU.y - this->thickness / 2),
      this->paddleU.width,
      this->paddleU.height,
      0);

  // scorekeeper?

  rectLeftSrc.h = 32;
  rectLeftSrc.w = 32;
  rectLeftSrc.x = 50;
  rectLeftSrc.y = 50;

  rectScoreLine = {
      100,
      65,
      20,
      5};

  rectRightSrc.h = 32;
  rectRightSrc.w = 32;
  rectRightSrc.x = 130;
  rectRightSrc.y = 50;

  return true;
}

void Pong::Shutdown()
{
  SDL_DestroyRenderer(mRenderer);
  SDL_DestroyWindow(mWindow);
  SDL_Quit();
}

bool Pong::getIsRunning()
{
  return mIsRunning;
}

/*void Pong::RunLoop()
{
  while (this->mIsRunning)
  {
    this->ProcessInput();
    if (!this->UpdateGame())
    {
      break;
    }
    this->Render();
  }
}*/

void Pong::UpdateScore()
{
  // this->leftPoints, this->rightPoints

  std::string fileLeft = "../assets/imgs/num" + std::to_string(this->leftPoints) + ".png";
  std::string fileRight = "../assets/imgs/num" + std::to_string(this->rightPoints) + ".png";

  SDL_Surface *surfaceLeftScore = IMG_Load(fileLeft.c_str());
  textLeftScore = SDL_CreateTextureFromSurface(mRenderer, surfaceLeftScore);
  SDL_FreeSurface(surfaceLeftScore);

  SDL_Surface *surfaceRightScore = IMG_Load(fileRight.c_str());
  textRightScore = SDL_CreateTextureFromSurface(mRenderer, surfaceRightScore);
  SDL_FreeSurface(surfaceRightScore);
}

void Pong::ProcessInput()
{
  //std::cout << "in process input\n";

  while (SDL_PollEvent(&this->event))
  {
    //std::cout << "in while loop\n";
    switch (this->event.type)
    {
      case SDL_QUIT:
        //std::cout << "sdl_quit\n";
        mIsRunning = false;
        break;
    }
  }

  const Uint8 *state = SDL_GetKeyboardState(NULL);

  if (state[SDL_SCANCODE_ESCAPE])
  {
    mIsRunning = false;
  }

  // paddle direction
  this->paddleU.direction = 0;
  if (state[SDL_SCANCODE_W])
  {
    this->paddleU.direction -= 1;
  }

  if (state[SDL_SCANCODE_S])
  {
    this->paddleU.direction += 1;
  }

  // paddle2, funny
  this->funny.direction = 0;
  if (state[SDL_SCANCODE_I])
  {
    this->funny.direction -= 1;
  }

  if (state[SDL_SCANCODE_K])
  {
    this->funny.direction += 1;
  }
}

//each var is 4 chars
//1st variable: 0000==continue-game, 0001==left-wins, 0002==right-wins
//2nd variable: 0000==no-points, 0001==left-point, 0002==right-point
//3rd variable: left paddle x
//4th variable: left paddle y
//5th variable: right paddle x
//6th variable: right paddle y
//7th variable: 000-left score
//8th variable: 000-right score
//9th variable: game ball x
//10th variable: game ball y
//11th variable: game ball x velocity
//12th variable: game ball y velocity
NetworkMessage Pong::UpdateGame()
{
  NetworkMessage mess;
  mess.win = '0';
  mess.point = '0';

  char var1='0', var2='0';
  //std::string paddlesLocation="0";

  while (!SDL_TICKS_PASSED(SDL_GetTicks(), this->mTicksCount + 16))
    ;

  this->deltaTime = (SDL_GetTicks() - this->mTicksCount);
  this->mTicksCount = SDL_GetTicks();
  if (this->deltaTime > 0.05f)
  {
    this->deltaTime = 0.05f;
  }

  // if paddle collides with side of screen, collision detection
  /*if ((gameBall.y >= (this->screenHeight - this->thickness)) && (gameBall.yVelocity > 0.0f))
  {
    gameBall.yVelocity *= -1;
  }
  else if ((gameBall.y <= this->thickness) && (gameBall.yVelocity < 0.0f))
  {
    gameBall.yVelocity *= -1;
  }*/
  if(checkHorizWallColl(gameBall)) gameBall.yVelocity *= -1;
  else if ((gameBall.x <= this->thickness) && (gameBall.xVelocity < 0.0f))
  {
    this->rightPoints += 1;
    gameBall.x = this->screenWidth / 2;
    gameBall.y = this->screenHeight / 2;
    // gameBall.xVelocity *= -1;

    var2 = '2';
    mess.point = '2';
  }
  else if ((gameBall.x >= (this->screenWidth - this->thickness)) && (gameBall.xVelocity > 0.0f))
  {
    this->leftPoints += 1;
    gameBall.x = this->screenWidth / 2;
    gameBall.y = this->screenHeight / 2;
    // gameBall.xVelocity *= -1;

    var2 = '1';
    mess.point = '1';
  }

  // game over because one side has 7 points
  //if (this->leftPoints == 7)
  //{
  //  std::cout << "The left side wins!\n";
    /*
      Doesn't do anything right now since returning false stops loop, but could be used in the future.
      this->mIsRunning = false;
    */
  //  return false;
  //}

  //if (this->rightPoints == 7)
  //{
  //  std::cout << "The right side wins!\n";
    /*
      Doesn't do anything right now since returning false stops loop, but could be used in the future.
      this->mIsRunning = false;
    */
  //  return false;
  //}


  // update objects using deltaTime
  this->gameBall.x += gameBall.xVelocity * this->deltaTime;
  this->gameBall.y += gameBall.yVelocity * this->deltaTime;

  if (this->paddleU.direction != 0)
  {
    if (this->paddleU.y < ((this->paddleU.height/2) + (this->thickness*2-this->thickness/2)))
    {
      this->paddleU.y = ((this->paddleU.height/2) + (this->thickness*2-this->thickness/2));
    }
    else if (this->paddleU.y > (this->screenHeight - this->paddleU.height + this->thickness))
    {
      this->paddleU.y = this->screenHeight - this->paddleU.height + this->thickness;
    }
    this->paddleU.y += this->paddleU.direction * 300.0f * this->deltaTime;
  }

  if (this->funny.direction != 0)
  {
    if (this->funny.y < ((this->funny.height/2) + (this->thickness*2-this->thickness/2)))
    {
      this->funny.y = ((this->funny.height/2) + (this->thickness*2-this->thickness/2)); 
    }
    else if ((this->funny.y > (this->screenHeight - this->funny.height + this->thickness)))
    {
      this->funny.y = (this->screenHeight - this->funny.height + this->thickness);
    }
    this->funny.y += this->funny.direction * 300.0f * this->deltaTime;
  }

  if (gameBall.collidesWith(paddleU))
  {
    //this->xVelocity *= -1.0f;
    gameBall.xVelocity *= -1;
  }

  if (gameBall.collidesWith(funny))
  {
    //this->xVelocity *= -1.0f;
    gameBall.xVelocity *= -1;
  }

  this->UpdateScore();

  if (CheckWin() == 1)
                {
                    //std::cout << "not updategame\n" << getIsRunning() << "\n";
                    //break;
                    var1 = '0';
                    mess.win = '0';
                }
                else if(CheckWin() == 2)
                {
                    //std::cout << "Left side wins!\n";
                    //serverAns = net.SendAndRecMessage("Left side wins!!");
                    //std:: cout << "sendAns: " << serverAns.sendAns << "\n"
                    //    << "recAns: " << serverAns.recAns << "\n"
                    //    << "recString: " << serverAns.recString << "\n";
                    //break;
                    var1 = '1';
                    mess.win = '1';
                }
                else if(CheckWin() == 3)
                {
                    //std::cout << "Right side wins!\n";
                    //serverAns = net.SendAndRecMessage("Right side wins!");
                    //std:: cout << "sendAns: " << serverAns.sendAns << "\n"
                    //    << "recAns: " << serverAns.recAns << "\n"
                    //    << "recString: " << serverAns.recString << "\n";
                    //break;
                    var1 = '2';
                    mess.win = '2';
                }
                else 
                {
                    //serverAns = net.SendAndRecMessage(game.UpdateGame());
                    //std:: cout << "sendAns: " << serverAns.sendAns << "\n"
                    //    << "recAns: " << serverAns.recAns << "\n"
                    //    << "recString: " << serverAns.recString << "\n";
                    std::cout << "What happened?\n";
                    //break;
                    var1 = '3';
                    mess.win = '3';
                }
                //Render();
                //std::cout << "render done\n";

                //paddlesLocation = char(paddleU.x) + char(paddleU.y) + char(funny.x) + char(funny.y);

                networkMessage.clear();
                networkMessage.push_back(var1);
                networkMessage.push_back(var2);

              //std::cout << "paddle loc 1: " << paddleU.x;

                //char buff1[4], buff2[4], buff3[4], buff4[4];

                //itoa();

                //itoa(paddleU.x, buff1, 10);

                

                //std::cout << "Loop done\n";

                //networkMessage.push_back(ans1);
                //networkMessage.push_back(itoa(paddleU.y, 10));
                //networkMessage.push_back(itoa(funny.x, 10));
                //networkMessage.push_back(itoa(funny.y, 10));

                std::cout << "paddle locs is: "
                          << paddleU.x << " : "
                          << paddleU.y << " : "
                          << funny.x << " : "
                          << funny.y << " : "
                          << "\n";

                //utils::push4char(networkMessage, paddleU.x);
                //utils::push4char(networkMessage, paddleU.y);
                //utils::push4char(networkMessage, funny.x);
                //utils::push4char(networkMessage, funny.y);

                mess.leftPaddleX = paddleU.x;
                mess.leftPaddleY = paddleU.y;
                mess.rightPaddleX = funny.x;
                mess.rightPaddleY = funny.y;


                ////utils::push4char(networkMessage, this->leftPoints);
                ////utils::push4char(networkMessage, this->rightPoints);

                std::string leftScoreStr = std::to_string(this->leftPoints);
                networkMessage.push_back(leftScoreStr[0]);

                mess.leftScore = leftScoreStr[0];

                std::string rightScoreStr = std::to_string(this->rightPoints);
                networkMessage.push_back(rightScoreStr[0]);

                mess.rightScore = rightScoreStr[0];            

                //utils::push4char(networkMessage, gameBall.x);
                //utils::push4char(networkMessage, gameBall.y);
                //utils::push4char(networkMessage, gameBall.xVelocity);
                //utils::push4char(networkMessage, gameBall.yVelocity);

                mess.gameballX = gameBall.x;
                mess.gameballY = gameBall.y;
                mess.gameballXVel = gameBall.xVelocity;
                mess.gameballYVel = gameBall.yVelocity;

                //find way to send mess NetworkMessage obj to net.SendAndRecMessage()

                //SEND_REC_STRUCT fun = net.SendAndRecMessage(networkMessage);

                SEND_REC_STRUCT fun = net.SendAndRecMessage(mess);
                fun.recString.erase(std::remove(fun.recString.begin(), fun.recString.end(), '\n'), fun.recString.cend());
                std::cout << "rec message: " << sizeof(fun.recString) << " 12345 " << fun.recString << "\n";

                return mess;
            }

Paddle Pong::createPaddle(int xq, int yq, int widthq, int heightq, int directionq)
{
  Paddle mPaddle;
  mPaddle.x = xq;
  mPaddle.y = yq;
  mPaddle.width = widthq;
  mPaddle.height = heightq;
  mPaddle.direction = directionq;
  return mPaddle;
}

void Pong::drawPaddle(Paddle mPaddle)
{
  SDL_Rect rectMPaddle{
      (mPaddle.x - (mPaddle.width / 2)),
      (mPaddle.y - (mPaddle.height / 2)),
      mPaddle.width,
      mPaddle.height};
  SDL_RenderFillRect(this->mRenderer, &rectMPaddle);
}

SDL_Rect Pong::createPaddleU()
{
  SDL_Rect myPaddle{
      (paddleU.x - (paddleU.width / 2)),
      (paddleU.y - (paddleU.height / 2)),
      this->paddleU.width,
      this->paddleU.height};
  return myPaddle;
}
void Pong::drawPaddleU(SDL_Rect myPaddle)
{
  SDL_RenderFillRect(this->mRenderer, &myPaddle);
}

void Pong::Render()
{
  // renderer, rgba; black
  SDL_SetRenderDrawColor(
      this->mRenderer,
      0,
      0,
      0,
      255);

  // draw scene

  // background
  SDL_RenderClear(this->mRenderer);

  // white for walls
  SDL_SetRenderDrawColor(
      this->mRenderer,
      255,
      255,
      255,
      255);
  SDL_Rect wall_left{
      0,
      0,
      this->thickness,
      this->screenHeight};
  SDL_Rect wall_right{
      this->screenWidth - (this->thickness),
      0,
      this->thickness,
      this->screenHeight};
  SDL_Rect wall_top{
      0,
      0,
      this->screenWidth,
      this->thickness};
  SDL_Rect wall_bottom{
      0,
      this->screenHeight - (this->thickness),
      this->screenWidth,
      this->thickness};
  SDL_RenderFillRect(this->mRenderer, &wall_left);
  SDL_RenderFillRect(this->mRenderer, &wall_right);
  SDL_RenderFillRect(this->mRenderer, &wall_top);
  SDL_RenderFillRect(this->mRenderer, &wall_bottom);

  SDL_RenderFillRect(this->mRenderer, &rectScoreLine);

  // create sdl rect for ball
  SDL_Rect ball{
      (gameBall.x - (gameBall.width / 2)),
      (gameBall.y - (gameBall.height / 2)),
      gameBall.width,
      gameBall.height};
  // fill sdl rect info into render
  SDL_RenderFillRect(this->mRenderer, &ball);

  /*//sdl drawing test
  SDL_Rect ball2{
      static_cast<int>(0),
      static_cast<int>(0),
      100,
      100};
  SDL_RenderFillRect(this->mRenderer, &ball2);
  */

  SDL_Rect myPaddle = this->createPaddleU();
  this->drawPaddleU(myPaddle);

  // this->funny.x += 5;
  this->drawPaddle(this->funny);

  SDL_RenderCopy(mRenderer, textLeftScore, NULL, &rectLeftSrc);
  SDL_RenderCopy(mRenderer, textRightScore, NULL, &rectRightSrc);

  SDL_RenderPresent(this->mRenderer);
}
