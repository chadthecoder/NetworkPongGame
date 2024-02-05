#include <NetworkMessage.hpp>

std::ostream& operator<<(std::ostream& os, const NetworkMessage& mess)
{
    os << mess.win << mess.point
       << mess.leftPaddleX << mess.leftPaddleY
       << mess.rightPaddleX << mess.rightPaddleY
       << mess.leftScore << mess.rightScore
       << mess.gameballX << mess.gameballY
       << mess.gameballXVel << mess.gameballYVel;
       
    return os;
}