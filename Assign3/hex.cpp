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

#include "hex.h"

/**
 * Member function to_hex8 is responsible for returning a formatted 2 hex digit
 * string to represent the 8 bits of the i argument (4 bits each hex digit).
 *
 * @param i is an unsigned 8bit integer.
 *
 * @return returns a string representing 2 hex digits.
********************************************************************************/
std::string hex::to_hex8(uint8_t i)
{
  std::ostringstream os;
  os << std::hex << std::setfill('0') << std::setw(2)
     << static_cast<uint16_t>(i);

  return os.str();
}


/**
 * Member function to_hex32 is responsible for returning a formatted 8 hex digit
 * string to represent the 32 bits of the i argument (4 bits each hex digit).
 *
 * @param i is an unsigned 32bit integer.
 *
 * @return returns a string representing 8 hex digits
********************************************************************************/
std::string hex::to_hex32(uint32_t i)
{
  std::ostringstream os;
  os << std::hex << std::setfill('0') << std::setw(8) << i;

  return os.str();
}


/**
 * Member function to_hex0x32 is responsible for returning "0x" concatenated
 * with the returned string from to_hex32(i).
 *
 * @param i is an unsigned 32bit integer.
 *
 * @return returns a string representing 8 hex digits concatenated with "0x".
********************************************************************************/

std::string hex::to_hex0x32(uint32_t i)
{
  return std::string("0x")+to_hex32(i);
}
