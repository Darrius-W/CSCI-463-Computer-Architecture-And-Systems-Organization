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


#include "rv32i_decode.h"

/**
 * @brief decode() is responsible for decoding instructions by rendering
 * 	  based on the type of instruction they are.
 *
 * @param addr address of the instruction.
 * @param insn is the instruction to be used for rendering.
 *
 * @return a std::string containing the disassembled instruction text.
 *
 **********************************************************************************************/
std::string rv32i_decode::decode(uint32_t addr, uint32_t insn)
{
  //collecting funct3 & funct7 to use within switch statement
  uint32_t funct3 = get_funct3(insn);
  uint32_t funct7 = get_funct7(insn);


  //if instruction matches ecall then it is rendered/decoded
  if (insn == insn_ecall)
    return render_ecall(insn);

  //if the instruction matches ebreak then it is rendered/decoded
  else if(insn == insn_ebreak)
    return render_ebreak(insn);


  //instruction opcode switch statement
  switch (get_opcode(insn))
  {
    default:                    return render_illegal_insn(insn);
    case opcode_lui:            return render_lui(insn);
    case opcode_auipc:          return render_auipc(insn);
    case opcode_jal:            return render_jal(addr, insn);
    case opcode_jalr:           return render_jalr(insn);

    //if case btype, need to check funct3
    case opcode_btype:
      switch (funct3)
      {
        default:                return render_illegal_insn(insn);
        case funct3_beq:        return render_btype(addr, insn, "beq");
        case funct3_bne:        return render_btype(addr, insn, "bne");
        case funct3_blt:        return render_btype(addr, insn, "blt");
        case funct3_bge:        return render_btype(addr, insn, "bge");
        case funct3_bltu:       return render_btype(addr, insn, "bltu");
        case funct3_bgeu:       return render_btype(addr, insn, "bgeu");
      }
      assert (0 && "unrecognized funct3");  //impossible

    //if case load immediate, need to check funct3
    case opcode_load_imm:
      switch (funct3)
      {
        default:                return render_illegal_insn(insn);
        case funct3_lb:         return render_itype_load(insn, "lb");
        case funct3_lh:         return render_itype_load(insn, "lh");
        case funct3_lw:         return render_itype_load(insn, "lw");
        case funct3_lbu:        return render_itype_load(insn, "lbu");
        case funct3_lhu:        return render_itype_load(insn, "lhu");
      }
      assert (0 && "unrecognized funct3");  //impossible

    //if case stype, need to check funct3
    case opcode_stype:
      switch (funct3)
      {
        default:                return render_illegal_insn(insn);
        case funct3_sb:         return render_stype(insn, "sb");
        case funct3_sh:         return render_stype(insn, "sh");
        case funct3_sw:         return render_stype(insn, "sw");
      }
      assert (0 && "unrecognized funct3");  //impossible

    //if case alu immediate, need to check funct3
    case opcode_alu_imm:
      switch (funct3)
      {
        default:                return render_illegal_insn(insn);
        case funct3_add:        return render_itype_alu(insn, "addi", get_imm_i(insn));
        case funct3_slt:        return render_itype_alu(insn, "slti", get_imm_i(insn));
        case funct3_sltu:       return render_itype_alu(insn, "sltiu", get_imm_i(insn));
        case funct3_xor:        return render_itype_alu(insn, "xori", get_imm_i(insn));
        case funct3_or:         return render_itype_alu(insn, "ori", get_imm_i(insn));
        case funct3_and:        return render_itype_alu(insn, "andi", get_imm_i(insn));
        case funct3_sll:        return render_itype_alu(insn, "slli", get_imm_i(insn)%XLEN);

        //if case srx, need to check funct7
        case funct3_srx:
          switch (funct7)
          {
            default:            return render_illegal_insn(insn);
            case funct7_srl:    return render_itype_alu(insn, "srli", get_imm_i(insn)%XLEN);
            case funct7_sra:    return render_itype_alu(insn, "srai", get_imm_i(insn)%XLEN);
          }
          assert(0 && "unrecognized funct7");  //impossible

      }
      assert (0 && "unrecognized funct3");  //impossible

    //if case rtype, need to check funct3
    case opcode_rtype:
      switch (funct3)
      {
        default:                return render_illegal_insn(insn);

        //if case add, need to check funct7
        case funct3_add:
          switch (funct7)
          {
            default:            return render_illegal_insn(insn);
            case funct7_add:    return render_rtype(insn, "add");
            case funct7_sub:    return render_rtype(insn, "sub");
          }
          assert (0 && "unrecognized funct7");  //impossible

        case funct3_sll:        return render_rtype(insn, "sll");
        case funct3_slt:        return render_rtype(insn, "slt");
        case funct3_sltu:       return render_rtype(insn, "sltu");
        case funct3_xor:        return render_rtype(insn, "xor");

        //if case srx, need to check funct7
        case funct3_srx:
          switch (funct7)
          {
            default:            return render_illegal_insn(insn);
            case funct7_srl:    return render_rtype(insn, "srl");
            case funct7_sra:    return render_rtype(insn, "sra");
          }
          assert (0 && "unrecognized funct7");  //impossible

        case funct3_or:         return render_rtype(insn, "or");
        case funct3_and:        return render_rtype(insn, "and");
      }
      assert (0 && "unrecognized funct3");  //impossible

    //if case system, need to check funct3
    case opcode_system:
      switch (funct3)
      {
        default:                //return render_illegal_insn(insn);
        case funct3_csrrw:      return render_csrrx(insn, "csrrw");
        case funct3_csrrs:      return render_csrrx(insn, "csrrs");
        case funct3_csrrc:      return render_csrrx(insn, "csrrc");
        case funct3_csrrwi:     return render_csrrxi(insn, "csrrwi");
        case funct3_csrrsi:     return render_csrrxi(insn, "csrrsi");
        case funct3_csrrci:     return render_csrrxi(insn, "csrrci");
      }
      assert (0 && "unrecognized funct3");  //impossible

  }
  assert (0 && "unrecognized opcode");  //impossible
}



/**
 * @brief render_illegal_insn() is responsible for reporting an error when an invalid
 *        instruction is attempting to be rendered.
 *
 * @param insn is the instruction to be rendered.
 *
 * @return a message that states there was an illegal render attempt.
 **********************************************************************************************/
std::string rv32i_decode::render_illegal_insn(uint32_t insn)
{
  return "ERROR: UNIMPLEMENTED INSTRUCTION";
}



/**
 * @brief render_lui() renders and decodes lui instruction.
 *
 * @param insn is the instruction to be rendered.
 *
 * @return instruction name, rd value, and imm_u in a string.
 ***********************************************************************************************/
std::string rv32i_decode::render_lui(uint32_t insn)
{
  uint32_t rd = get_rd(insn);
  int32_t imm_u = get_imm_u(insn);

  std::ostringstream os;
  os << render_mnemonic("lui") << render_reg(rd) << ","
     << hex::to_hex0x20((imm_u >> 12) & 0x0fffff);

  return os.str();
}



/**
 * @brief render_auipc() renders and decodes auipc instruction.
 *
 * @param insn is the instruction to be rendered.
 *
 * @return instruction name, rd value, and imm_u in a string.
 ***********************************************************************************************/
std::string rv32i_decode::render_auipc(uint32_t insn)
{
  uint32_t rd = get_rd(insn);
  int32_t imm_u = get_imm_u(insn);

  std::ostringstream os;
  os << render_mnemonic("auipc") << render_reg(rd) << ","
     << hex::to_hex0x20((imm_u >> 12) & 0x0fffff);

  return os.str();
}



/**
 * @brief render_jal() renders and decodes jal instruction.
 *
 * @param addr address of the instruction.
 * @param insn is the instruction to be rendered.
 *
 * @return instruction name, rd value, and pcrel_21 in a string.
 **********************************************************************************************/
std::string rv32i_decode::render_jal(uint32_t addr, uint32_t insn)
{
  uint32_t rd = get_rd(insn);
  int32_t imm_j = get_imm_j(insn);
  int32_t pcrel_21 = addr + imm_j;

  std::ostringstream os;
  os << render_mnemonic("jal") << render_reg(rd) << ","
     << hex::to_hex0x32(pcrel_21);

  return os.str();
}




/**
 * @brief render_jalr() renders and decodes jalr instruction.
 *
 * @param insn is the instruction to be rendered.
 *
 * @return instruction name, rd value, imm_i, and rs1 in a string.
 **********************************************************************************************/
std::string rv32i_decode::render_jalr(uint32_t insn)
{
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  int32_t imm_i = get_imm_i(insn);

  std::ostringstream os;
  os << render_mnemonic("jalr") << render_reg(rd) << ","
     << render_base_disp(rs1, imm_i);

  return os.str();
}



/**
 * @brief render_btype() renders and decodes btype instruction.
 *
 * @param addr address of the instruction.
 * @param insn is the instruction to be rendered.
 * @param mnemonic is the name of the instruction.
 *
 * @return instruction name, rs1 value, rs2 value, and pcrel_13 in a string.
 *
 **********************************************************************************************/
std::string rv32i_decode::render_btype(uint32_t addr, uint32_t insn, const char *mnemonic)
{
  uint32_t rs1 = get_rs1(insn);
  uint32_t rs2 = get_rs2(insn);
  int32_t imm_b = get_imm_b(insn);
  int32_t pcrel_13 = addr + imm_b;

  std::ostringstream os;
  os << render_mnemonic(mnemonic) << render_reg(rs1) << ","
     << render_reg(rs2) << "," << hex::to_hex0x32(pcrel_13);

  return os.str();
}



/**
 * @brief render_itype_load() renders and decodes itype load instruction.
 *
 * @param insn is the instruction to be rendered.
 * @param mnemonic is the name of the instruction.
 *
 * @return instruction name, rs1 value, rd value, and imm_i value  in a string.
 *
 **********************************************************************************************/
 std::string rv32i_decode::render_itype_load(uint32_t insn, const char *mnemonic)
{
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  int32_t imm_i = get_imm_i(insn);

  std::ostringstream os;
  os << render_mnemonic(mnemonic) << render_reg(rd) << ","
     << render_base_disp(rs1, imm_i);

  return os.str();
}



/**
 * @brief render_itype_alu() renders and decodes itype alu instruction.
 *
 * @param insn is the instruction to be rendered.
 * @param mnemonic is the name of the instruction.
 * @param imm_i instruction immediate value.
 *
 * @return instruction name, rs1 value, rd value, and imm_i value  in a string.
 **********************************************************************************************/
std::string rv32i_decode::render_itype_alu(uint32_t insn, const char *mnemonic, int32_t imm_i)
{
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);

  std::ostringstream os;
  os << render_mnemonic(mnemonic) << render_reg(rd) << ","
     << render_reg(rs1) << "," << imm_i;

  return os.str();
}



/**
 * @brief render_stype() renders and decodes stype instruction.
 *
 * @param insn is the instruction to be rendered.
 * @param mnemonic is the name of the instruction.
 *
 * @return instruction name, rs1 value, rs2 value, and imm_s value in a string.
 **********************************************************************************************/
std::string rv32i_decode::render_stype(uint32_t insn, const char *mnemonic)
{
  uint32_t rs1 = get_rs1(insn);
  uint32_t rs2 = get_rs2(insn);
  int32_t imm_s = get_imm_s(insn);

  std::ostringstream os;
  os << render_mnemonic(mnemonic) << render_reg(rs2) << ","
     << render_base_disp(rs1, imm_s);

  return os.str();
}



/**
 * @brief render_rtype() renders and decodes rtype instruction.
 *
 * @param insn is the instruction to be rendered.
 * @param mnemonic is the name of the instruction.
 *
 * @return instruction name, rs1 value, rs2 value, and rd value in a string.
 *
 **********************************************************************************************/
std::string rv32i_decode::render_rtype(uint32_t insn, const char *mnemonic)
{
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  uint32_t rs2 = get_rs2(insn);

  std::ostringstream os;
  os << render_mnemonic(mnemonic) << render_reg(rd) << ","
     << render_reg(rs1) << "," << render_reg(rs2);

  return os.str();
}



/**
 * @brief render_ecall() returns string "ecall" when the instruction ecall is present.
 *
 * @param insn is the instruction for ecall.
 *
 * @return string "ecall".
 **********************************************************************************************/
std::string rv32i_decode::render_ecall(uint32_t insn)
{
  std::ostringstream os;
  os << "ecall";

  return os.str();
}



/**
 * @brief render_ebreak() returns string "ebreak" when the instruction ebreak is present.
 *
 * @param insn is the instruction for ecall.
 *
 * @return string "ebreak".
 **********************************************************************************************/
std::string rv32i_decode::render_ebreak(uint32_t insn)
{
  std::ostringstream os;
  os << "ebreak";

  return os.str();
}



/**
 * @brief render_csrrx() renders and decodes csrrx instruction.
 *
 * @param insn is the instruction to be rendered.
 * @param mnemonic is the name of the instruction.
 *
 * @return instruction name, rs1 value, rd value, and csr value in a string.
 *
 **********************************************************************************************/
std::string rv32i_decode::render_csrrx(uint32_t insn, const char *mnemonic)
{
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  int32_t csr = get_imm_i(insn);

  std::ostringstream os;
  os << render_mnemonic(mnemonic) << render_reg(rd) << ","
     << hex::to_hex0x12(csr) << "," << render_reg(rs1);

  return os.str();
}



/**
 * @brief render_csrrxi() renders and decodes csrrxi instruction.
 *
 * @param insn is the instruction to be rendered.
 * @param mnemonic is the name of the instruction.
 *
 * @return instruction name, csr value, rd value, and zimm value in a string.
 *
 **********************************************************************************************/
 std::string rv32i_decode::render_csrrxi(uint32_t insn, const char *mnemonic)
{
  uint32_t rd = get_rd(insn);
  int32_t csr = get_imm_i(insn);
  uint32_t zimm = get_rs1(insn);

  std::ostringstream os;
  os << render_mnemonic(mnemonic) << render_reg(rd) << ","
     << hex::to_hex0x12(csr) << "," << zimm;

  return os.str();
}



/**
 * @brief render_reg() renders given register name.
 *
 * @param r is a register name.
 *
 * @return string representing register name.
 **********************************************************************************************/
std::string rv32i_decode::render_reg(int r)
{
  std::ostringstream os;
  os << "x" << r;

  return os.str();
}



/**
 * @brief render operands of the form imm(register) into std::string.
 *
 * @param r register name to be rendered
 * @param imm immediate value to be rendered
 *
 * @return string of operands in form imm(register)
 **********************************************************************************************/
std::string rv32i_decode::render_base_disp(uint32_t r, int32_t imm)
{
  std::ostringstream os;
  os << imm << "(" << render_reg(r) << ")";

  return os.str();
}



/**
 * @brief render mnemonic into std::string with proper space padding on right.
 *
 * @param mnemonic to be padded.
 *
 * @return Return string of padded right mnemonic.
 **********************************************************************************************/
std::string rv32i_decode::render_mnemonic(const std::string &mnemonic)
{
  std::ostringstream os;
  os << std::setw(mnemonic_width) << std::setfill(' ')
     << std::left << mnemonic;

  return os.str();
}



/**
 * @brief extract and return the opcode field from the given instruction.
 *
 * @param insn instruction name to extract opcode from.
 *
 * @return 7 bits of the opcode.
 **********************************************************************************************/
uint32_t rv32i_decode::get_opcode(uint32_t insn)
{
  return (insn & 0x0000007f);
}



/**
 * @brief exract and return the rd field from the given instruction as an integer value.
 *
 * @param insn instruction name to extract the rd from.
 *
 * @return 5 bits representing rd.
 **********************************************************************************************/
uint32_t rv32i_decode::get_rd(uint32_t insn)
{
  return (insn & 0x00000f80) >> 7;
}



/**
 * @brief extract and return the funct3 field from the given instruction as integer value.
 *
 * @param insn instruction name to extract funct3 from.
 *
 * @return funct3 field as an integer value from 0 to 7.
 **********************************************************************************************/
uint32_t rv32i_decode::get_funct3(uint32_t insn)
{
  return (insn & 0x00007000) >> 12;
}



/**
 * @brief extract and return the rs1 field from the given instruction as an integer value.
 *
 * @param insn instruction name to extract rs1 from.
 *
 * @return rs1 value represnting integer from 0 to 31.
 **********************************************************************************************/
uint32_t rv32i_decode::get_rs1(uint32_t insn)
{
  return (insn & 0x000f8000) >> 15;
}



/**
 * @brief extract and return the rs2 field from the given instruction as an integer value.
 *
 * @param insn instruction name to extract rs2 from.
 *
 * @return rs2 value represnting integer from 0 to 31.
 **********************************************************************************************/
uint32_t rv32i_decode::get_rs2(uint32_t insn)
{
  return (insn & 0x01f00000) >> 20;
}



/**
 * @brief extract and return the funct7 field from the given instruction as integer value.
 *
 * @param insn instruction name to extract funct7 from.
 *
 * @return integer value from 0x00 to 0x7f representing funct7.
 **********************************************************************************************/
uint32_t rv32i_decode::get_funct7(uint32_t insn)
{
  return (insn & 0xfe000000) >> 25;
}



/**
 * @brief extract and return tne imm_i field from the given instruction as 32 bit signed integer.
 *
 * @param insn instruction name to extract imm_i from.
 *
 * @return imm_i represented as integer value from 0x00 to 0x7f.
 **********************************************************************************************/
int32_t rv32i_decode::get_imm_i(uint32_t insn)
{
  int32_t imm_i = (insn & 0xfff00000) >> (20 - 0);  //shift right

  if (insn & 0x80000000)
    imm_i |= 0xfffff000;  //extend sign for msb

  return imm_i;
}



/**
 * @brief extract and return the imm_u field from the given instruction as a 32 bit signed integer.
 *
 * @param insn instruction name to extract imm_u from.
 *
 * @return imm_u represented as 32 bit signed integer.
 **********************************************************************************************/
int32_t rv32i_decode::get_imm_u(uint32_t insn)
{
  int32_t imm_u = (insn & 0x00000fff) & 0;
  imm_u |= (insn & 0xfffff000);

  return imm_u;
}



/**
 * @brief extract and return the imm_b field from the given instruction as 32 bit signed integer.
 *
 * @param insn instruction name to extract imm_b from.
 *
 * @return imm_b represented as a 32 bit signed integer.
 **********************************************************************************************/
int32_t rv32i_decode::get_imm_b(uint32_t insn)
{
  int32_t imm_b = (insn & 0x00000001) & 0;    //lsb is 0
  imm_b |= (insn & 0x00000f00) >> (8 - 1);    //shift right
  imm_b |= (insn & 0x7e000000) >> (25 - 5);   //shift right
  imm_b |= (insn & 0x00000080) << (11 - 7);   //shift left
  imm_b |= (insn & 0x80000000) >> (31 - 12);  //shift right

  if (insn & 0x80000000)
    imm_b |= 0xffffe000;  //extend sign for msb

  return imm_b;
}



/**
 * @brief extract and return the imm_s field from the given instruction as 32 bit signed integer.
 *
 * @param insn instruction name to extract imm_s from.
 *
 * @return imm_s respresented as a 32 bit signed integer.
 **********************************************************************************************/
int32_t rv32i_decode::get_imm_s(uint32_t insn)
{
  int32_t imm_s = (insn & 0xfe000000) >> (25 - 5);  //shift right
  imm_s |= (insn & 0x00000f80) >> (7 - 0);	    //shift right

  if (insn & 0x80000000)
    imm_s |= 0xfffff000;  //sign-extend on the left for msb

  return imm_s;
}



/**
 * @brief extract and return the imm_j field from the given instruction as 32 bit signed integer.
 *
 * @param insn instruction name to exract imm_j from.
 *
 * @return imm_j represented as 32 bit signed integer.
 **********************************************************************************************/
int32_t rv32i_decode::get_imm_j(uint32_t insn)
{
  int32_t imm_j = (insn & 0x00000001) & 0;    //lsb is 0
  imm_j |= (insn & 0x7fe00000) >> (21 - 1);   //right shift
  imm_j |= (insn & 0x00100000) >> (20 - 11);  //right shift
  imm_j |= (insn & 0x000ff000);		      //right shift
  imm_j |= (insn & 0x80000000) >> (31 - 20);  //right shift

  if (insn & 0x80000000)
    imm_j |= 0xffe00000;  //extend sign left for msb

  return imm_j;
}

