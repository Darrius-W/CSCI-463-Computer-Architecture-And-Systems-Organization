//***************************************************************************
//
//  Darrius White
//  Z1900146
//  Assign 3
//  CSCI 463-1
//
//  I certify that this is my own work and where appropriate an extension
//  of the starter code provided for the assignment.
//
//***************************************************************************/

#ifndef HEX_H
#define HEX_H

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

class hex
{
public:
    static std::string to_hex8(uint8_t i);
    static std::string to_hex32(uint32_t i);
    static std::string to_hex0x32(uint32_t i);
};

#endif
