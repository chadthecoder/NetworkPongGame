#include <utils.hpp>

namespace utils
{
    void push4char(std::string& pushString, int pushInt)
    {
        std::string str1 = std::to_string(pushInt);
        //std::string ans1;

        int str1length = str1.length();

        if(str1length>4) std::cout << "You messed up!\n";
        else if(str1length==4)
        {
            for(int i=0; i<str1length; i++)
            {
                pushString.push_back(str1[i]);
            }
        }
        else if(str1length<4)
        {
            //std::cout << "Loop start: " << str1length << "\n";
            for(int i = 0; i<4-str1length; i++)
            {
                pushString.push_back('0');
                //std::cout << "Pushing back 0\n";
            }
            for(int i=0; i<str1length; i++)
            {
                std::cout << "Pushing back " << str1[i] << "\n";
                pushString.push_back(str1[i]);
            }
        }
    }
}