//***************************************************************************
//
//  Darrius White
//  Z1900146
//  Assign 2
//  CSCI 463-1
//
//  I certify that this is my own work and where appropriate an extension
//  of the starter code provided for the assignment.
//
//***************************************************************************/
#include <iostream>
#include <iomanip>

void printBinFloat(uint32_t);



/**
 * Main function is responsible for running a continuous while loop,
 * reading user inputted 32bit hex decimal representations of IEEE-754
 * Floating point Numbers and passing them to printBinFloat to be processed.
 *
 ********************************************************************************/
int main()
{
  uint32_t hex_val;

  while (std::cin >> std::hex >> hex_val)  //reading user input and storing them in hex form
  {
    printBinFloat(hex_val);  //Pass hex value to printBinFloat
  }

  return 0;
}




/**
 * printBinFloat function is responsible for taking the read-in 32bit hex values
 * and displaying them in binary form, printing the sign, exponent,
 * significand, and the full value of the number in binary.
 *
 * @param hex_val is a user entered unsigned 32bit hexadecimal value.
 *
 ********************************************************************************/
void printBinFloat(uint32_t hex_val)
{
  int bitCtr = 1;

  uint32_t sign;
  uint32_t bitMask = 0x80000000;
  uint32_t sig = (hex_val & 0x7fffff);  //Collecting the Significand

  int32_t exp = ((hex_val & 0x7f800000) >> 23) - 0x7f;  //Collecting the Exponent shifting 23 bits and subtract 127



  std::cout << "0x" << std::setw(8) << std::setfill('0')  //Displays the Read-in u32bit HEX value
            << std::hex << hex_val << " = ";


  //Displays binary value of u32bit hex numbers
  while (bitCtr != 33){
    std::cout << (hex_val & bitMask ? '1':'0');

    if (bitCtr % 4 == 0 && bitCtr != 32)  //Space after every 4 bits
      std::cout << ' ';

    if (bitCtr == 1)
      (hex_val & bitMask ? sign = 1:sign = 0);  //Collects the sign of binary value


    bitMask = bitMask >> 1;  //shift to next bit
    bitCtr++;
  }


  std::cout << "\n" << "sign: " << sign;  //Displays the sign of value

  std::cout << "\n" << " exp: "<< "0x" << std::setw(8) << std::setfill('0')  //Displays the exponent
	    << std::hex << exp << " (" << std::dec << exp << ")";

  std::cout << "\n" << " sig: " << "0x" << std::setw(8) << std::setfill('0')  //Displays the significand
	    << std::hex << sig;



  std::cout << "\n" << (hex_val & 0x80000000 ? '-':'+');  //When sign is 0 the value is positive & vice versa


  //Case 0(when exponent is equal to -127 and significand is all 0's)
  if (exp == -127 && sig == 00000000)
    std::cout << "0";

  //Case infinity(when exponent is all 1's & significand all 0's)
  else if (exp == 128 && sig == 00000000)
    std::cout << "inf";


  //Floating point number with whole digits
  else if (exp >= 0)
  {
    int fpNum = exp + 23;

    std::cout << '1';  //Floating point sign
    sig = (sig << 9);

    for (int indx = 0; indx < fpNum; indx++)  //Displaying all floating point digits
    {
      if (indx == exp)  //Locating the decimal
        std::cout << '.';

      std::cout << ((sig & 0x80000000) ? '1':'0');  //Printing each bit
      sig = (sig << 1);

      if (indx >= exp && indx > 23)  //Avoid printing additional 0's
        break;
    }
  }



  //Floating point number with no whole digits
  else if (exp < 0)
  {
    int zeros;

    std::cout << "0.";
    sig = (sig << 9);

    zeros = abs(exp) - 1 ;  //Making sure all leading nums are zeros

    for (int indx = 0; indx < zeros; indx++)  //Loops through printing all lead zeros
      std::cout << '0';


    std::cout << '1';

    for (int indx = 0; indx < 23; indx++)  //Prints rest of decimal values
    {
      std::cout << ((sig & 0x80000000) ? '1':'0');
      sig = (sig << 1);  //Next bit
    }
  }

  std::cout << std::endl;
}
