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

#include "cpu_single_hart.h"



/**
 * @brief run() is responsible for running the cpu with a single hart.
 *
 * @param exec_limit, amount of times execution will be ticked.
 *
 **************************************************************************/
void cpu_single_hart::run(uint64_t exec_limit)
{
  regs.set(2, mem.get_size());

  while( ( is_halted() != true && exec_limit == 0 ) || ( is_halted() != true && get_insn_counter() < exec_limit ) )
    tick();

  if(is_halted())
    std::cout << "Execution terminated. Reason: " << get_halt_reason() << std::endl;

  std::cout << get_insn_counter() <<  " instructions executed" << std::endl;
}
