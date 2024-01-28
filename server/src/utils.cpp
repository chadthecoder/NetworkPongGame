#include <utils.hpp>

std::string make_daytime_string()
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  //std::cout << "size of time ans: " << sizeof(ctime(&now)) << "\n";
  return ctime(&now);
}