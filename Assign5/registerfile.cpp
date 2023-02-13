//***************************************************************************
//
//  Darrius White
//  Z1900146
//  Assign 5
//  CSCI 463-1
//
//  I certify that this is my own work and where appropriate an extension
//  of the starter code provided for the assignment.
//
//***************************************************************************/

#include "registerfile.h"
#include <cstdint>
#include <iostream>
#include <iomanip>

/**
 * @brief constructor is responsible for using reset() to initialize all the registers.
 *
 **********************************************************************************************/
registerfile::registerfile()
{
  reset();
}



/**
 * @brief reset() is reponsible for Initializing register x0 to zero and all other registers
 * 	  to 0xf0f0f0f0.
 *
 **********************************************************************************************/
void registerfile::reset()
{
  reg[0] = 0x0;

  for (int i = 1; i < 32; i++)
    reg[i] = 0xf0f0f0f0;
}



/**
 * @brief set() is responsible for assigning the given value to a given register.
 *
 * @param r, is a given register.
 * @param val, is the value to assign to a given register.
 *
 **********************************************************************************************/
void registerfile::set(uint32_t r, int32_t val)
{
  if (r == 0)
    return;
  else
    reg[r] = val;
}



/**
 * @brief get() is responsible for returning the value of a register.
 *
 * @param r, register who's value is to be returned.
 *
 * @return the value of a given register r.
 *
 **********************************************************************************************/
int32_t registerfile::get(uint32_t r) const
{
  if (r == 0)
    return 0;
  else
    return reg[r];
}



/**
 * @brief dump() is responsible for dumping the registers.
 *
 * @param hdr, is the name of the header of each dump line.
 *
 **********************************************************************************************/
void registerfile::dump(const std::string &hdr) const
{
  //printing dump
  for(size_t i = 0; i < 32; i++)
  {
    if (i != 0 && i % 8 == 0)//prints newline
      std::cout << std::endl;

    if (i % 8 == 0)
    {
      std::string h;
      if (i == 0)
        h = "x0";

      else if (i == 8)
        h = "x8";

      else if(i == 16)
        h = "x16";

      else if(i == 24)
        h = "x24";


      std::cout << hdr << std::dec << std::right << std::setw(3) << std::setfill(' ') << h << " ";
    }

    //print register contents
    if(i % 8 == 7)
      std::cout << hex::to_hex32(reg[i]);

    else if (i % 4 == 0 && i != 0 && i % 8 != 0)
      std::cout << " " << hex::to_hex32(reg[i]) << " ";

    else
      std::cout << hex::to_hex32(reg[i]) << " ";
  }
  std::cout << std::endl;
}
