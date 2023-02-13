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

#include "memory.h"
#include "hex.h"
#include <getopt.h>


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
	uint32_t memory_limit = 0x100;	// default memory size is 0x100

	int opt;
	while ((opt = getopt(argc, argv, "m:")) != -1)
	{
		switch(opt)
		{
		case 'm':
			{
				std::istringstream iss(optarg);
				iss >> std::hex >> memory_limit;
			}
			break;

		default:
			usage();
		}
	}

	if (optind >= argc)
		usage();	// missing filename

	memory mem(memory_limit);
	mem.dump();

	if (!mem.load_file(argv[optind]))
		usage();

	mem.dump();

	std::cout << mem.get_size() << std::endl;
	std::cout << hex::to_hex32(mem.get8(0)) << std::endl;
	std::cout << hex::to_hex32(mem.get16(0)) << std::endl;
	std::cout << hex::to_hex32(mem.get32(0)) << std::endl;
	std::cout << hex::to_hex0x32(mem.get8(0)) << std::endl;
	std::cout << hex::to_hex0x32(mem.get16(0)) << std::endl;
	std::cout << hex::to_hex0x32(mem.get32(0)) << std::endl;
	std::cout << hex::to_hex8(mem.get8(0)) << std::endl;
	std::cout << hex::to_hex8(mem.get16(0)) << std::endl;
	std::cout << hex::to_hex8(mem.get32(0)) << std::endl;

	std::cout << hex::to_hex0x32(mem.get32(0x1000)) << std::endl;

	mem.set8(0x10, 0x12);
	mem.set16(0x14, 0x1234);
	mem.set32(0x18, 0x87654321);


	std::cout << hex::to_hex0x32(mem.get8_sx(0x0f)) << std::endl;
	std::cout << hex::to_hex0x32(mem.get8_sx(0x7f)) << std::endl;
	std::cout << hex::to_hex0x32(mem.get8_sx(0x80)) << std::endl;
	std::cout << hex::to_hex0x32(mem.get8_sx(0xe3)) << std::endl;

	std::cout << hex::to_hex0x32(mem.get16_sx(0xe0)) << std::endl;
	std::cout << hex::to_hex0x32(mem.get32_sx(0xe0)) << std::endl;

	mem.dump();

	return 0;
}
