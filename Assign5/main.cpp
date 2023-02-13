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
#include <string>
#include <unistd.h>
#include "hex.h"
#include "rv32i_decode.h"
#include "memory.h"
#include "rv32i_hart.h"
#include "cpu_single_hart.h"

using namespace std;



/**
 * usage function shows user valid command line usage and arguments to use
 * within a standard error message.
 *
 ********************************************************************************/
static void usage()
{
  cerr << "Usage: rv32i [-d] [-i] [-r] [-z] [-l exec-limit] [-m hex-mem-size] infile" << endl;
  cerr << "    -d show disassembly before program execution" << endl;
  cerr << "    -i show instruction printing during execution" << endl;
  cerr << "    -l maximum number of instructions to exec" << endl;
  cerr << "    -m specify memory size (default = 0x100)" << endl;
  cerr << "    -r show register printing during execution" << endl;
  cerr << "    -z show a dump of the regs & memory after simulation" << endl;
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
  uint32_t pc = 0;//program counter
  uint32_t insn = 0;

  while(pc < mem.get_size())
  {
    cout << hex::to_hex32(pc) << ": ";
    insn = mem.get32(pc);
    cout << hex::to_hex32(insn) << "  ";
    cout << rv32i_decode::decode(pc, insn) << endl;
    pc += 4;
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
  uint32_t memory_limit = 0x100;
  int opt;
  int execution_limit = 0;
  int dflag = 0;
  int iflag = 0;
  int rflag = 0;
  int zflag = 0;

  while((opt = getopt(argc, argv, "m:l:dirz")) != -1)
  {
    switch(opt)
    {
       case 'm':
         memory_limit = stoul(optarg, nullptr, 16);
         break;

       case 'd':
         dflag = 1;
         break;

       case 'i':
         iflag = 1;
         break;

       case 'r':
         rflag = 1;
         break;

       case 'z':
         zflag = 1;
         break;

       case 'l':
         execution_limit = std::stoul(optarg, nullptr, 0);
         break;

       default:
         usage();
    }
  }

  if(optind >= argc)
    usage();

  memory mem(memory_limit);

  if(!mem.load_file(argv[optind]))
    usage();

  rv32i_hart sim(mem);
  cpu_single_hart cpu(mem);

  if(dflag == 1)
  {
    disassemble(mem);
    sim.reset();
  }

  if(iflag == 1)
    cpu.set_show_instructions(true);

  if(rflag == 1)
    cpu.set_show_registers(true);

  cpu.run(execution_limit);

  if(zflag == 1)
  {
    cpu.dump();
    mem.dump();
  }

  return 0;
}
