//***************************************************************************
//
//  Darrius White
//  Z1900146
//  Assign 4
//  CSCI 463-1
//
//  I certify that this is my own work and where appropriate an extension
//  of the starter code provided for the assignment.
//
//***************************************************************************/


#include <iostream>
#include "hex.h"
#include "memory.h"
#include "rv32i_decode.h"
//#include <unistd.h>
#include <sstream>



/**
 * usage function shows user valid command line usage and arguments to use
 * within a standard error message.
 *
 ********************************************************************************/
static void usage()
{
  std::cerr << "Usage: rv32i [-m hex-mem-size] infile" << std::endl;
  std::cerr << "    -m specify memory size (default = 0x100)" << std::endl;
  exit(1);
}



/**
 * disassemble() disassembles memory instruction.
 *
 * @param mem memory to be decoded.
 *
 ********************************************************************************/
static void disassemble(const memory &mem)
{
  uint32_t memoryAddr = 0;   //address of memory instruction
  uint32_t insn = 0;         //instruction output
  std::string decodedInsn;   //decoded instruction

  //for each byte in memory (divide by 4 because each insn is 4 bytes()
  for (unsigned i = 0; i < mem.get_size() / 4; i++)
  {
    insn = mem.get32(memoryAddr);  //setting instruction in memory
    decodedInsn = rv32i_decode::decode(memoryAddr, insn);  //decoding and storing instruction

    //print the memory address, instruction hex value, and the instruction mnemonic
    std::cout << hex::to_hex32(memoryAddr) << ": " << hex::to_hex32(insn)
	      << "  " << decodedInsn << std::endl;

    memoryAddr += 4; //move to next instruction in memory
  }
}



/**
 * main function is used to read in, print out, and format the dump output.
 *
 * @param argc number of arguments being passed in from command line.
 * @param argv array of arguments.
 *
 * @return return 0.
 *
 ********************************************************************************/
int main(int argc, char **argv)
{
  uint32_t memory_limit = 0x100; // default memory size = 256 bytes
  int opt;
  while ((opt = getopt(argc, argv, "m:")) != -1)
  {
    switch (opt)
    {
    case 'm':
      {
        std::istringstream iss(optarg);
        iss >> std::hex >> memory_limit;
      }
      break;
    default: /* ’?’ */
      usage();
    }
  }

  if (optind >= argc)
    usage(); // missing filename

  memory mem(memory_limit);

  if (!mem.load_file(argv[optind]))
    usage();

  disassemble(mem);
  mem.dump();

  return 0;
}
