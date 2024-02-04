#include <string>

struct Client
{
   //remove port and  extra number for left or right paddle and put them in thier respective variables in the struct
   std::string address;

   std::string port;
   bool paddleSide;

   //unsure? add more variable?
};