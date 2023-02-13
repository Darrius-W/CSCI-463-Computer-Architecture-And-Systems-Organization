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
#include "memory.h"



/**
 * memory constructor is used to round and initialize the mem vector by siz bytes.
 *
 * @param siz is the amount of elements added to the mem vector.
 *
 ********************************************************************************/
memory::memory(uint32_t siz)
{
  siz = (siz + 15) & 0xfffffff0;  //round the length up, mod-16
  mem.resize(siz, 0xa5);  //initialize values of mem vector
}



/**
 * memory class deconstructor for any clean up.
 *
 ********************************************************************************/
memory::~memory(){}



/**
 * member function check_illegal() checks if a given address(i) is within the
 * memory vector or not.
 *
 * @param i is a 32bit unsigned integer address.
 *
 * @return return true if the address is an error and false if it isn't.
 *
 ********************************************************************************/
bool memory::check_illegal(uint32_t i) const
{
  //added =
  if(i >= memory::get_size()){  //check if i represents and actual element in memory
    std::cout << "WARNING: Address out of range: "
	      << hex::to_hex0x32(i) << std::endl;

    return true;  //there is an error
  }
  else
    return false;  //no error
}



/**
 * member function get_size() returns the size of the memory vector.
 *
 * @return a rounded up number of size bytes in the simulated memory.
 *
 ********************************************************************************/
uint32_t memory::get_size() const
{
  return mem.size();
}



/**
 * get8 member function is used to check if an addr is in memory and if it is
 * the bytes in the given addr parameter are returned.
 *
 * @param addr 32bit unsigned integer to be checked if in mem.
 *
 * @return byte from simulated memory if addr is in valid range, else return 0.
 *
 ********************************************************************************/
uint8_t memory::get8(uint32_t addr) const
{
  if(memory::check_illegal(addr))  //not in valid range
    return 0;

  else  //in valid range
    return mem[addr];
}



/**
 * get16 member function is used to call get8() twice and get two valid bytes that
 * are to be combined, creating a 16bit return value.
 *
 * @param addr 32bit unsigend integer address.
 *
 * @return two bytes combined from get8().
 *
 ********************************************************************************/
uint16_t memory::get16(uint32_t addr) const
{
  uint8_t secByte = get8(addr);
  uint8_t firstByte = get8(addr + 1);

  return ((uint16_t)firstByte << 8) | secByte;
}



/**
 * get32() member function used to call get16() twice, combine the two results
 * to form a 32bit return value.
 *
 * @param addr holds the 32bit address to be extracted.
 *
 * @return two 16bit values combined into a 32bit value.
 *
 ********************************************************************************/
uint32_t memory::get32(uint32_t addr) const
{
  uint16_t sec2Bytes = get16(addr);
  uint16_t first2Bytes = get16(addr + 2);

  return ((uint32_t)first2Bytes << 16) | sec2Bytes;
}



/**
 * get8_sx() member function used to call get8() and return sign-extended value
 * of the byte as a 32bit signed integer.
 *
 * @param addr 32bit unsigned integer value.
 *
 * @return 32bit signed integer version of unsiged 8bit value.
 *
 ********************************************************************************/
int32_t memory::get8_sx(uint32_t addr) const
{
  return(int32_t)memory::get8(addr);
}



/**
 * get16_sx() member function used to call get16() and return sign-extended
 * value of the 16bits as a 32bit signed integer.
 *
 * @param addr 32bit unsigned integer value.
 *
 * @return 32bit signed integer version of unsigned 16bit value.
 *
 ********************************************************************************/
int32_t memory::get16_sx(uint32_t addr) const
{
  return (int32_t)memory::get16(addr);
}



/**
 * get32_sx() member function calls get32() and returns value as a 32bit signed integer
 *
 * @param addr 32bit unsigned intger value.
 *
 * @return 32bit signed intger version of unsigned 32bit value.
 *
 ********************************************************************************/
int32_t memory::get32_sx(uint32_t addr) const
{
  return memory::get32(addr);
}



/**
 * set8() member function calls check_illegeal to make sure addr is valid and
 * if it is then function will set the value(val) at address(addr), else the
 * data will be discarded.
 *
 * @param addr to be checked if a valid address.
 * @param val stored within memory at given addr if valid.
 *
 ********************************************************************************/
void memory::set8(uint32_t addr, uint8_t val)
{
  if(!memory::check_illegal(addr))  //if address is valid
    mem[addr] = val;
  else
    return;
}



/**
 * set16() member function calls set8() twice to store given val into the
 * simulated memory in little endian order.
 *
 * @param addr address given.
 * @param val value given to be stored in memory.
 *
 ********************************************************************************/
void memory::set16(uint32_t addr, uint16_t val)
{
  uint8_t firstByte = (uint8_t)((val & 0xff00) >> 8);  //first byte compared
  uint8_t secByte = (uint8_t)(val & 0x00ff);  //second byte compared

  memory::set8(addr + 1, firstByte);
  memory::set8(addr, secByte);
}



/**
 * set32() member function calls set16() twice to store the given value into
 * simulated memory in little endian order.
 *
 * @param addr address given.
 * @param val value given to be stored in simulated memory.
 *
 ********************************************************************************/
void memory::set32(uint32_t addr, uint32_t val)
{
  uint16_t first2Bytes = (uint16_t)((val & 0xffff0000) >> 16);
  uint16_t sec2Bytes = (uint16_t)(val & 0x0000ffff);

  memory::set16(addr + 2, first2Bytes);
  memory::set16(addr, sec2Bytes);
}



/**
 * dump() member function is used to dump the entire contents of the memory
 * formatted and in hex with corresponding ASCII characters.
 *
 ********************************************************************************/
void memory::dump() const
{
  std::string dumpLine = "";

  for(uint32_t addr = 0; addr < get_size(); ++addr)
  {
    if(addr % 16 == 0)  //new address line
      std::cout << to_hex32(addr) << ": ";

    std::cout << hex::to_hex8(get8(addr)) << " ";  //print 2 hex digits

    uint8_t ch = get8(addr);
    ch = isprint(ch) ? ch : '.';  //determine to print ascii character or a '.'
    dumpLine += ch;

    if(addr % 16 == 7)  //formatted space after 16 hex digits printed
      std::cout << " ";

    if(addr % 16 == 15){  //dump output including ascii
      std::cout << "*" << dumpLine << "*" << std::endl;
      dumpLine.clear();
    }
  }
}



/**
 * load_file() member function opens a file in binary mode and reads its
 * contenst into the simulated memory.
 *
 * @param fname file to be opend in binary mode.
 *
 * @return true if the file is loaded ok, otherwise returns false.
 *
 ********************************************************************************/
bool memory::load_file(const std::string &fname)
{
  std::ifstream infile(fname, std::ios::in|std::ios::binary);  //open file in binary mode

  if(!infile){  //failure to open file
    std::cerr << "Can't open file '" << fname << "' for reading." << std::endl;
    return false;
  }
  else{
    uint8_t i;
    infile >> std::noskipws;

    for(uint32_t addr = 0; infile >> i; ++addr){  //check each byte for valid address
      if(memory::check_illegal(addr)){
        std::cerr << "Program too big." << std::endl;
        return false;
      }
      set8(addr,i);
    }
    return true;
  }
}
