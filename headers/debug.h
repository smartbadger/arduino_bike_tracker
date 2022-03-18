#include <iostream>
#ifndef _Debug_H
#define _Debug_H

// cout console
// to file need fstring
// endl to flush string buffer, helps ensure console output

void log(std::string debugText){
    std::cout << debugText << std::endl;
}

void log(int debugNumber){ 
    std::cout << debugNumber << std::endl; 
}

#endif