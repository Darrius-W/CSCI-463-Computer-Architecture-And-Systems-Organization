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
#ifndef REGISTERFILE_H
#define REGISTERFILE_H

#include "hex.h"
#include <string>

using namespace std;


class registerfile
{
  public:
           registerfile();
           void reset();
           void set(uint32_t r, int32_t val);
           int32_t get(uint32_t r) const;
           void dump(const std::string &hdr) const;

  private:
           int32_t reg[32];
};

#endif
