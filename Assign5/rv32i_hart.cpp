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

#include "rv32i_hart.h"

using namespace std;

/**
 * @brief reset() is responsible for resetting the rv32i object and
 *        registerfile.
 *
 **************************************************************************/
void rv32i_hart::reset()
{
  pc = 0;
  regs.reset();
  insn_counter = 0;
  halt = false;
  halt_reason = "none";
}



/**
 * @brief dump() is responsible for dumping the entire state of the hart.
 *
 * @param hdr, header name to prefix each dump line.
 *
 **************************************************************************/
void rv32i_hart::dump(const std::string &hdr) const
{
  regs.dump(hdr);
  std::cout << " pc " << hex::to_hex32(pc) << endl;
}



/**
 * @brief tick() is responsible for telling the simulator how to execute an
 *        instruction.
 *
 * @param hdr, header name to prefix each dump line.
 *
 **************************************************************************/
void rv32i_hart::tick(const std::string &hdr)
{
  uint32_t insn;

  if (halt)
    return;
  else
  {
    insn_counter++;

    if(show_registers)
      dump(hdr);

    insn = mem.get32(pc);

    if(show_instructions)
    {
      cout << hdr << hex::to_hex32(pc) << ": " << hex::to_hex32(insn) << "  ";
      exec(insn, &std::cout);
    }
    else
      exec(insn, nullptr);
  }
}



/**
 * @brief exec() is responsible for executinga given rv32i instruction.
 *
 * @param insn, instruction to be executed.
 * @param pos, output stream
 *
 **************************************************************************/
void rv32i_hart::exec(uint32_t insn, std::ostream* pos)
{
  uint32_t opcode = get_opcode(insn);
  uint32_t funct3 = get_funct3(insn);
  uint32_t funct7 = get_funct7(insn);

  switch(opcode)
  {
    default:           exec_illegal_insn(insn, pos); return;

    case opcode_lui:   exec_lui(insn, pos); return;
    case opcode_auipc: exec_auipc(insn, pos); return;
    case opcode_rtype:
       switch(funct3)
       {
         default:          exec_illegal_insn(insn, pos); return;
         case funct3_add:
            switch(funct7)
            {
              default:          exec_illegal_insn(insn, pos); return;
              case funct7_add:  return exec_add(insn, pos); return;
              case funct7_sub:  return exec_sub(insn, pos); return;
            }
         case funct3_sll:  exec_sll(insn, pos); return;
         case funct3_slt:  exec_slt(insn, pos); return;
         case funct3_sltu: exec_sltu(insn, pos); return;
         case funct3_xor:  exec_xor(insn, pos); return;
         case funct3_srx:
            switch(funct7)
            {
              default:         exec_illegal_insn(insn, pos); return;
              case funct7_srl: exec_srl(insn, pos); return;
              case funct7_sra: exec_sra(insn, pos); return;
            }
         case funct3_or:   exec_or(insn, pos); return;
         case funct3_and:  exec_and(insn, pos); return;
       }
    case opcode_stype:
       switch(funct3)
       {
         default:        exec_illegal_insn(insn, pos); return;
         case funct3_sb: exec_sb(insn, pos); return;
         case funct3_sh: exec_sh(insn, pos); return;
         case funct3_sw: exec_sw(insn, pos); return;
       }
    case opcode_alu_imm:
       switch(funct3)
       {
         default:          exec_illegal_insn(insn, pos); return;
         case funct3_sll:  exec_slli(insn, pos); return;
         case funct3_add:  exec_addi(insn, pos); return;
         case funct3_slt:  exec_slti(insn, pos); return;
         case funct3_sltu: exec_sltiu(insn, pos); return;
         case funct3_xor:  exec_xori(insn, pos); return;
         case funct3_or:   exec_ori(insn, pos); return;
         case funct3_and:  exec_andi(insn, pos); return;
         case funct3_srx:
            switch(funct7)
            {
              default:         exec_illegal_insn(insn, pos); return;
              case funct7_srl: exec_srli(insn, pos); return;
              case funct7_sra: exec_srai(insn, pos); return;
            }
       }
    case opcode_load_imm:
       switch(funct3)
       {
         default:         exec_illegal_insn(insn, pos); return;
         case funct3_lb:  exec_lb(insn, pos); return;
         case funct3_lh:  exec_lh(insn, pos); return;
         case funct3_lw:  exec_lw(insn, pos); return;
         case funct3_lbu: exec_lbu(insn, pos); return;
         case funct3_lhu: exec_lhu(insn, pos); return;
       }
    case opcode_btype:
       switch(funct3)
       {
         default:          exec_illegal_insn(insn, pos); return;
         case funct3_beq:  exec_beq(insn, pos); return;
         case funct3_bne:  exec_bne(insn, pos); return;
         case funct3_blt:  exec_blt(insn, pos); return;
         case funct3_bge:  exec_bge(insn, pos); return;
         case funct3_bltu: exec_bltu(insn, pos); return;
         case funct3_bgeu: exec_bgeu(insn, pos); return;
       }
    case opcode_jal:  exec_jal(insn, pos); return;
    case opcode_jalr: exec_jalr(insn, pos); return;
    case opcode_system:
       switch(insn)
       {
         default:
            switch(funct3)
            {
              default:             exec_illegal_insn(insn, pos); return;
              case funct3_csrrs:   exec_csrrs(insn, pos); return;
            }
         case insn_ebreak: exec_ebreak(insn, pos); return;
       }
  }
}



/**
 * @brief exec_illegal_insn() is responsible for checking if an
 *        intruction is illegal.
 *
 * @param insn, instruction to be executed.
 * @param pos, output stream.
 *
 **************************************************************************/
void rv32i_hart::exec_illegal_insn(uint32_t insn, std::ostream* pos)
{
  (void)insn;
  if(pos)
    *pos << render_illegal_insn(insn);

  halt = true;
  halt_reason = "Illegal instruction";
}



/**
 * @brief exec_lui() is responsible for executing lui instruction.
 *
 * @param insn, instruction to be executed.
 * @param pos, output stream.
 *
 **************************************************************************/
void rv32i_hart::exec_lui(uint32_t insn, std::ostream* pos)
{
  uint32_t rd = get_rd(insn);
  int32_t imm_u = get_imm_u(insn);

  if(pos)
  {
    std::string s = render_lui(insn);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(imm_u) << std::endl;
  }
  regs.set(rd, imm_u);
  pc += 4;
}



/**
 * @brief exec_auipc() is responsible for executing auipc instructions.
 *
 * @param insn, instruction to be executed.
 * @param pos, output stream.
 *
 **************************************************************************/
void rv32i_hart::exec_auipc(uint32_t insn, std::ostream* pos)
{
  uint32_t rd = get_rd(insn);
  int32_t imm_u = get_imm_u(insn);
  int32_t val = imm_u + pc;

  if(pos)
  {
    std::string s = render_auipc(insn);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(pc) << " + " << hex::to_hex0x32(imm_u) << " = " << hex::
            to_hex0x32(val) << std::endl;
  }
  regs.set(rd, val);
  pc += 4;
}



/**
 * @brief exec_jal() is responsible for executing jal instructions.
 *
 * @param insn, instruction to be executed.
 * @param pos, output stream.
 *
 **************************************************************************/
void rv32i_hart::exec_jal(uint32_t insn, std::ostream* pos)
{
  uint32_t rd = get_rd(insn);
  int32_t imm_j = get_imm_j(insn);
  int32_t val = pc + imm_j;

  if(pos)
  {
    std::string s = render_jal(pc, insn);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(pc + 4) << ",  pc = " << hex::
            to_hex0x32(pc) << " + " << hex::to_hex0x32(imm_j) << " = " << hex::to_hex0x32(val) << std::endl;
  }
  regs.set(rd, pc + 4);
  pc = val;
}



/**
 * @brief exec_jalr() is responsible for executing jalr instructions.
 *
 * @param insn, instruction to be executed.
 * @param pos output stream.
 *
 **************************************************************************/
void rv32i_hart::exec_jalr(uint32_t insn, std::ostream* pos)
{
  uint32_t rd = get_rd(insn);
  int32_t imm_i = get_imm_i(insn);
  uint32_t rs1 = get_rs1(insn);
  int32_t val = (regs.get(rs1) + imm_i) & 0xfffffffe;

  if(pos)
  {
    std::string s = render_jalr(insn);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(pc + 4) << ",  pc = (" << hex::to_hex0x32(imm_i) << " + " << hex::
            to_hex0x32(regs.get(rs1)) << ") & 0xfffffffe = " << hex::to_hex0x32(val) << std::endl;
  }
  regs.set(rd, pc + 4);
  pc = val;
}



/**
 * @brief exec_beq() is responsible for executing beq instructions.
 *
 * @param insn, instruction to be executed.
 * @param pos output stream.
 *
 **************************************************************************/
void rv32i_hart::exec_beq(uint32_t insn, std::ostream* pos)
{
  int32_t rs1 = get_rs1(insn);
  int32_t rs2 = get_rs2(insn);
  int32_t imm_b = get_imm_b(insn);
  int32_t t_addr;

  if((uint32_t)regs.get(rs1) == (uint32_t)regs.get(rs2))
    t_addr = pc + imm_b;
  else
    t_addr = pc + 4;

  if(pos)
  {
    std::string s = render_btype(pc, insn, "beq");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// pc += (" << hex::to_hex0x32(regs.get(rs1)) << " == " << hex::to_hex0x32(regs.get(rs2)) << " ? " << hex::
            to_hex0x32(imm_b) << " : 4) = " << hex::to_hex0x32(t_addr) << std::endl;
  }
  pc = t_addr;
}



/**
 * @brief exec_bne() is responsible for executing bne instructions.
 *
 * @param insn, instruction to be executed.
 * @param pos output stream.
 *
 **************************************************************************/
void rv32i_hart::exec_bne(uint32_t insn, std::ostream* pos)
{
  int32_t rs1 = get_rs1(insn);
  int32_t rs2 = get_rs2(insn);
  int32_t imm_b = get_imm_b(insn);
  int32_t t_addr;

  if(regs.get(rs1) != regs.get(rs2))
    t_addr = pc + imm_b;
  else
    t_addr = pc + 4;

  if(pos)
  {
    std::string s = render_btype(pc, insn, "bne");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// pc += (" << hex::to_hex0x32(regs.get(rs1)) << " != " << hex::to_hex0x32(regs.get(rs2)) << " ? " << hex::
            to_hex0x32(imm_b) << " : 4) = " << hex::to_hex0x32(t_addr) << std::endl;
  }
  pc = t_addr;
}



/**
 * @brief exec_blt() is responsible for executing blt instructions.
 *
 * @param insn, instruction to be executed.
 * @param pos output stream.
 *
 **************************************************************************/
void rv32i_hart::exec_blt(uint32_t insn, std::ostream* pos)
{
  int32_t rs1 = get_rs1(insn);
  int32_t rs2 = get_rs2(insn);
  int32_t imm_b = get_imm_b(insn);
  int32_t t_addr;

  if(regs.get(rs1) < regs.get(rs2))
    t_addr = pc + imm_b;
  else
    t_addr = pc + 4;

  if(pos)
  {
    std::string s = render_btype(pc, insn, "blt");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// pc += (" << hex::to_hex0x32(regs.get(rs1)) << " < " << hex::to_hex0x32(regs.get(rs2)) << " ? " << hex::
            to_hex0x32(imm_b) << " : 4) = " << hex::to_hex0x32(t_addr) << std::endl;
  }
  pc = t_addr;
}



/**
 * @brief exec_bge() is responsible for executing bge instructions.
 *
 * @param insn, instruction to be executed.
 * @param pos output stream.
 *
 **************************************************************************/
void rv32i_hart::exec_bge(uint32_t insn, std::ostream* pos)
{
  int32_t rs1 = get_rs1(insn);
  int32_t rs2 = get_rs2(insn);
  int32_t imm_b = get_imm_b(insn);
  int32_t t_addr;

  if(regs.get(rs1) >= regs.get(rs2))
    t_addr = pc + imm_b;
  else
    t_addr = pc + 4;

  if(pos)
  {
    std::string s = render_btype(pc, insn, "bge");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// pc += (" << hex::to_hex0x32(regs.get(rs1)) << " >= " << hex::to_hex0x32(regs.get(rs2)) << " ? " << hex::
            to_hex0x32(imm_b) << " : 4) = " << hex::to_hex0x32(t_addr) << std::endl;
  }
  pc = t_addr;
}



/**
 * @brief exec_bltu() is responsible for executing bltu instructions.
 *
 * @param insn, instruction to be executed.
 * @param pos output stream.
 *
 **************************************************************************/
void rv32i_hart::exec_bltu(uint32_t insn, std::ostream* pos)
{
  int32_t rs1 = get_rs1(insn);
  int32_t rs2 = get_rs2(insn);
  int32_t imm_b = get_imm_b(insn);
  int32_t t_addr;

  if((uint32_t)regs.get(rs1) < (uint32_t)regs.get(rs2))
    t_addr = pc + imm_b;
  else
    t_addr = pc + 4;

  if(pos)
  {
    std::string s = render_btype(pc, insn, "bltu");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// pc += (" << hex::to_hex0x32(regs.get(rs1)) << " <U " << hex::to_hex0x32(regs.get(rs2)) << " ? " << hex::
            to_hex0x32(imm_b) << " : 4) = " << hex::to_hex0x32(t_addr) << std::endl;
  }
  pc = t_addr;
}



/**
 * @brief exec_bgeu() is responsible for executing bgeu instructions.
 *
 * @param insn, instruction to be executed.
 * @param pos output stream.
 *
 **************************************************************************/
void rv32i_hart::exec_bgeu(uint32_t insn, std::ostream* pos)
{
  int32_t rs1 = get_rs1(insn);
  int32_t rs2 = get_rs2(insn);
  int32_t imm_b = get_imm_b(insn);
  int32_t t_addr;

  if((uint32_t)regs.get(rs1) >= (uint32_t)regs.get(rs2))
    t_addr = pc + imm_b;
  else
    t_addr = pc + 4;

  if(pos)
  {
    std::string s = render_btype(pc, insn, "bgeu");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// pc += (" << hex::to_hex0x32(regs.get(rs1)) << " >=U " << hex::to_hex0x32(regs.get(rs2)) << " ? " << hex::
            to_hex0x32(imm_b) << " : 4) = " << hex::to_hex0x32(t_addr) << std::endl;
  }
  pc = t_addr;
}



/**
 * @brief exec_lb() is responsible for executing lb instructions.
 *
 * @param insn, instruction to be executed.
 * @param pos output stream.
 *
 **************************************************************************/
void rv32i_hart::exec_lb(uint32_t insn, std::ostream* pos)
{
  int32_t rs1 = get_rs1(insn);
  int32_t rd = get_rd(insn);
  int32_t imm_i = get_imm_i(insn);
  uint32_t t_addr = regs.get(rs1) + imm_i;
  uint32_t val = mem.get8(t_addr);
  int32_t num = 0x80;

  val = 0xff & (int32_t)val;

  if(num & val)
    val += 0xffffff00;

  if(pos)
  {
    std::string s = render_itype_load(insn, "lb");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = sx(m8(" << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::
            to_hex0x32(imm_i) << ")) = " << hex::to_hex0x32(val) << std::endl;
  }
  regs.set(rd, val);
  pc += 4;
}



/**
 * @brief exec_lh() is responsible for executing lh instructions.
 *
 * @param insn, instruction to be executed.
 * @param pos output stream.
 *
 **************************************************************************/
void rv32i_hart::exec_lh(uint32_t insn, std::ostream* pos)
{
  int32_t rs1 = get_rs1(insn);
  int32_t rd = get_rd(insn);
  int32_t imm_i = get_imm_i(insn);
  uint32_t t_addr = regs.get(rs1) + imm_i;
  uint32_t val = mem.get16(t_addr);

  val = 0xffff & (int32_t)val;
  int32_t num = 0x8000;

  if(num & val)
    val += 0xffff0000;

  if(pos)
  {
    std::string s = render_itype_load(insn, "lh");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = sx(m16(" << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::
            to_hex0x32(imm_i) << ")) = " << hex::to_hex0x32(val) << std::endl;
  }
  regs.set(rd, val);
  pc += 4;
}



/**
 * @brief exec_lw() is responsible for executing lw instructions.
 *
 * @param insn, instruction to be executed.
 * @param pos output stream.
 *
 **************************************************************************/
void rv32i_hart::exec_lw(uint32_t insn, std::ostream* pos)
{
  int32_t rs1 = get_rs1(insn);
  int32_t rd = get_rd(insn);
  int32_t imm_i = get_imm_i(insn);
  uint32_t t_addr = regs.get(rs1) + imm_i;
  uint32_t val = mem.get32(t_addr);

  if(pos)
  {
    std::string s = render_itype_load(insn, "lw");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = sx(m32(" << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::
            to_hex0x32(imm_i) << ")) = " << hex::to_hex0x32(val) << std::endl;
  }
  regs.set(rd, val);
  pc += 4;
}



/**
 * @brief exec_lbu() is responsible for executing lbu instructions.
 *
 * @param insn, instruction to be executed.
 * @param pos output stream.
 *
 **************************************************************************/
void rv32i_hart::exec_lbu(uint32_t insn, std::ostream* pos)
{
  int32_t rs1 = get_rs1(insn);
  int32_t rd = get_rd(insn);
  int32_t imm_i = get_imm_i(insn);
  uint32_t t_addr = regs.get(rs1) + imm_i;
  uint32_t val = mem.get8(t_addr) & 0x000000ff;

  if(pos)
  {
    std::string s = render_itype_load(insn, "lbu");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = zx(m8(" << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::
            to_hex0x32(imm_i) << ")) = " << hex::to_hex0x32(val) << std::endl;
  }
  regs.set(rd, val);
  pc += 4;
}



/**
 * @brief exec_lhu() is responsible for executing lhu instructions.
 *
 * @param insn, instruction to be executed.
 * @param pos output stream.
 *
 **************************************************************************/
void rv32i_hart::exec_lhu(uint32_t insn, std::ostream* pos)
{
  int32_t rs1 = get_rs1(insn);
  int32_t rd = get_rd(insn);
  int32_t imm_i = get_imm_i(insn);
  uint32_t t_addr = regs.get(rs1) + imm_i;
  uint32_t val = mem.get16(t_addr) & 0x0000ffff;

  if(pos)
  {
    std::string s = render_itype_load(insn, "lhu");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = zx(m16(" << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::
            to_hex0x32(imm_i) << ")) = " << hex::to_hex0x32(val) << std::endl;
  }
  regs.set(rd, val);
  pc += 4;
}



/**
 * @brief exec_sb() is responsible for executing sb instructions.
 *
 * @param insn, instruction to be executed.
 * @param pos output stream.
 *
 **************************************************************************/
void rv32i_hart::exec_sb(uint32_t insn, std::ostream* pos)
{
  int32_t rs1 = get_rs1(insn);
  int32_t rs2 = get_rs2(insn);
  int32_t imm_s = get_imm_s(insn);
  uint32_t t_addr = regs.get(rs1) + imm_s;
  uint32_t val = regs.get(rs2) & 0x000000ff;

  if(pos)
  {
    std::string s = render_stype(insn, "sb");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// m8(" << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm_s) << ") = " << hex::
            to_hex0x32(val) << std::endl;
  }
  mem.set8(t_addr, val);
  pc += 4;
}



/**
 * @brief exec_sh() is responsible for executing sh instructions.
 *
 * @param insn, instruction to be executed.
 * @param pos output stream.
 *
 **************************************************************************/
void rv32i_hart::exec_sh(uint32_t insn, std::ostream* pos)
{
  int32_t rs1 = get_rs1(insn);
  int32_t rs2 = get_rs2(insn);
  int32_t imm_s = get_imm_s(insn);
  uint32_t t_addr = regs.get(rs1) + imm_s;
  uint32_t val = regs.get(rs2) & 0x0000ffff;

  if(pos)
  {
    std::string s = render_stype(insn, "sh");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// m16(" << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm_s) << ") = " << hex::
            to_hex0x32(val) << std::endl;
  }
  mem.set16(t_addr, val);
  pc += 4;
}



/**
 * @brief exec_sw() is responsible for executing sw instructions.
 *
 * @param insn, instruction to be executed.
 * @param pos output stream.
 *
 **************************************************************************/
void rv32i_hart::exec_sw(uint32_t insn, std::ostream* pos)
{
  int32_t rs1 = get_rs1(insn);
  int32_t rs2 = get_rs2(insn);
  int32_t imm_s = get_imm_s(insn);
  uint32_t t_addr = regs.get(rs1) + imm_s;
  uint32_t val = regs.get(rs2);

  if(pos)
  {
    std::string s = render_stype(insn, "sw");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// m32(" << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm_s) << ") = " << hex::
            to_hex0x32(val) << std::endl;
  }
  mem.set32(t_addr, val);
  pc += 4;
}



/**
 * @brief exec_addi() is responsible for executing addi instructions.
 *
 * @param insn, instruction to be executed.
 * @param pos output stream.
 *
 **************************************************************************/
void rv32i_hart::exec_addi(uint32_t insn, std::ostream* pos)
{
  int32_t rs1 = get_rs1(insn);
  int32_t rd = get_rd(insn);
  int32_t imm_i = get_imm_i(insn);
  uint32_t sum = regs.get(rs1) + imm_i;

  if(pos)
  {
    std::string s = render_itype_alu(insn, "addi", imm_i);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::
            to_hex0x32(imm_i) << " = " << hex::to_hex0x32(sum) << std::endl; 
  }
  regs.set(rd, sum);
  pc += 4;
}



/**
 * @brief exec_slti() is responsible for executing slti instructions.
 *
 * @param insn, instruction to be executed.
 * @param pos output stream.
 *
 **************************************************************************/
void rv32i_hart::exec_slti(uint32_t insn, std::ostream* pos)
{
  int32_t rd = get_rd(insn);
  int32_t rs1 = get_rs1(insn);
  int32_t imm_i = get_imm_i(insn);

  int32_t val = (regs.get(rs1) < imm_i) ? 1 : 0;

  if(pos)
  {
    std::string s = render_itype_alu(insn, "slti", imm_i);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = (" << hex::to_hex0x32(regs.get(rs1)) << " < " <<
            std::dec << imm_i << ") ? 1 : 0 = " << hex::to_hex0x32(val) << std::endl;
  }
  regs.set(rd, val);
  pc += 4;
}



/**
 * @brief exec_sltiu() is responsible for executing sltiu instructions.
 *
 * @param insn, instruction to be executed.
 * @param pos output stream.
 *
 **************************************************************************/
void rv32i_hart::exec_sltiu(uint32_t insn, std::ostream* pos)
{
  int32_t rd = get_rd(insn);
  int32_t rs1 = get_rs1(insn);
  int32_t imm_i = get_imm_i(insn);

  int32_t val = ((uint32_t)regs.get(rs1) < (uint32_t)imm_i) ? 1 : 0;

  if(pos)
  {
    std::string s = render_itype_alu(insn, "sltiu", imm_i);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = (" << hex::to_hex0x32(regs.get(rs1)) << " <U " <<
            std::dec << imm_i << ") ? 1 : 0 = " << hex::to_hex0x32(val) << std::endl;
  }
  regs.set(rd, val);
  pc += 4;
}



/**
 * @brief exec_xori() is responsible for executing xori instructions.
 *
 * @param insn, instruction to be executed.
 * @param pos output stream.
 *
 **************************************************************************/
void rv32i_hart::exec_xori(uint32_t insn, std::ostream* pos)
{
  int32_t rd = get_rd(insn);
  int32_t rs1 = get_rs1(insn);
  int32_t imm_i = get_imm_i(insn);

  int32_t val = regs.get(rs1) ^ imm_i;

  if(pos)
  {
    std::string s = render_itype_alu(insn, "xori", imm_i);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " ^ " << hex::
            to_hex0x32(imm_i) << " = " << hex::to_hex0x32(val) << std::endl;
  }
  regs.set(rd, val);
  pc += 4;
}



/**
 * @brief exec_ori() is responsible for executing ori instructions.
 *
 * @param insn, instruction to be executed.
 * @param pos output stream.
 *
 **************************************************************************/
void rv32i_hart::exec_ori(uint32_t insn, std::ostream* pos)
{
  int32_t rd = get_rd(insn);
  int32_t rs1 = get_rs1(insn);
  int32_t imm_i = get_imm_i(insn);

  int32_t val = regs.get(rs1) | imm_i;

  if(pos)
  {
    std::string s = render_itype_alu(insn, "ori", imm_i);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " | " << hex::
            to_hex0x32(imm_i) << " = " << hex::to_hex0x32(val) << std::endl;
  }
  regs.set(rd, val);
  pc += 4;
}



/**
 * @brief exec_andi() is responsible for executing andi instructions.
 *
 * @param insn, instruction to be executed.
 * @param pos output stream.
 *
 **************************************************************************/
void rv32i_hart::exec_andi(uint32_t insn, std::ostream* pos)
{
  int32_t rd = get_rd(insn);
  int32_t rs1 = get_rs1(insn);
  int32_t imm_i = get_imm_i(insn);

  int32_t val = (regs.get(rs1) & imm_i);

  if(pos)
  {
    std::string s = render_itype_alu(insn, "andi", imm_i);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " & " << hex::
            to_hex0x32(imm_i) << " = " << hex::to_hex0x32(val) << std::endl;
  }
  regs.set(rd, val);
  pc += 4;
}



/**
 * @brief exec_slli() is responsible for executing slli instructions.
 *
 * @param insn, instruction to be executed.
 * @param pos output stream.
 *
 **************************************************************************/
void rv32i_hart::exec_slli(uint32_t insn, std::ostream* pos)
{
  int32_t rd = get_rd(insn);
  int32_t rs1 = get_rs1(insn);
  int32_t shift = get_imm_i(insn)%XLEN;

  int32_t val = regs.get(rs1) << shift;

  if(pos)
  {
    std::string s = render_itype_alu(insn, "slli", get_imm_i(insn));
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " << " <<
            std::dec << shift << " = " << hex::to_hex0x32(val) << std::endl;
  }
  regs.set(rd, val);
  pc += 4;
}



/**
 * @brief exec_srli() is responsible for executing srli instructions.
 *
 * @param insn, instruction to be executed.
 * @param pos output stream.
 *
 **************************************************************************/
void rv32i_hart::exec_srli(uint32_t insn, std::ostream* pos)
{
  int32_t rd = get_rd(insn);
  int32_t rs1 = get_rs1(insn);
  int32_t shift = get_imm_i(insn)%XLEN;

  int32_t val = (uint32_t)regs.get(rs1) >> shift;

  if(pos)
  {
    std::string s = render_itype_alu(insn, "srli", get_imm_i(insn));
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " >> " << 
            std::dec << shift << " = " << hex::to_hex0x32(val) << std::endl;
  }
  regs.set(rd, val);
  pc += 4;
}



/**
 * @brief exec_srai() is responsible for executing srai instructions.
 *
 * @param insn, instruction to be executed.
 * @param pos output stream.
 *
 **************************************************************************/
void rv32i_hart::exec_srai(uint32_t insn, std::ostream* pos)
{
  int32_t rd = get_rd(insn);
  int32_t rs1 = get_rs1(insn);
  int32_t shift = get_imm_i(insn)%XLEN;

  int32_t val = regs.get(rs1) >> shift;

  if(pos)
  {
    std::string s = render_itype_alu(insn, "srai", (int32_t)get_imm_i(insn)%XLEN);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " >> " << 
            std::dec << shift << " = " << hex::to_hex0x32(val) << std::endl;
  }
  regs.set(rd, val);
  pc += 4;
}



/**
 * @brief exec_add() is responsible for executing add instructions.
 *
 * @param insn, instruction to be executed.
 * @param pos output stream.
 *
 **************************************************************************/
void rv32i_hart::exec_add(uint32_t insn, std::ostream* pos)
{
  int32_t rd = get_rd(insn);
  int32_t rs1 = get_rs1(insn);
  int32_t rs2 = get_rs2(insn);

  int32_t val = regs.get(rs1) + regs.get(rs2);

  if(pos)
  {
    std::string s = render_rtype(insn, "add");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::
            to_hex0x32(regs.get(rs2)) << " = " << hex::to_hex0x32(val) << std::endl;
  }
  regs.set(rd, val);
  pc += 4;
}



/**
 * @brief exec_sub() is responsible for executing sub instructions.
 *
 * @param insn, instruction to be executed.
 * @param pos output stream.
 *
 **************************************************************************/
void rv32i_hart::exec_sub(uint32_t insn, std::ostream* pos)
{
  int32_t rd = get_rd(insn);
  int32_t rs1 = get_rs1(insn);
  int32_t rs2 = get_rs2(insn);

  int32_t val = regs.get(rs1) - regs.get(rs2);

  if(pos)
  {
    std::string s = render_rtype(insn, "sub");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " - " << hex::
            to_hex0x32(regs.get(rs2)) << " = " << hex::to_hex0x32(val) << std::endl;
  }
  regs.set(rd, val);
  pc += 4;
}



/**
 * @brief exec_sll() is responsible for executing sll instructions.
 *
 * @param insn, instruction to be executed.
 * @param pos output stream.
 *
 **************************************************************************/
void rv32i_hart::exec_sll(uint32_t insn, std::ostream* pos)
{
  int32_t rd = get_rd(insn);
  int32_t rs1 = get_rs1(insn);
  int32_t rs2 = get_rs2(insn);

  int32_t shamt = regs.get(rs2) & 0x01f;
  int32_t val = regs.get(rs1) << shamt;

  if(pos)
  {
    std::string s = render_rtype(insn, "sll");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " << " <<
            std::dec << shamt << " = " << hex::to_hex0x32(val) << std::endl;
  }
  regs.set(rd, val);
  pc += 4;
}



/**
 * @brief exec_slt() is responsible for executing slt instructions.
 *
 * @param insn, instruction to be executed.
 * @param pos output stream.
 *
 **************************************************************************/
void rv32i_hart::exec_slt(uint32_t insn, std::ostream* pos)
{
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  uint32_t rs2 = get_rs2(insn);

  int32_t val = (regs.get(rs1) < regs.get(rs2)) ? 1 : 0;

  if(pos)
  {
    std::string s = render_rtype(insn, "slt");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = (" << hex::to_hex0x32(regs.get(rs1)) << " < " << hex::
            to_hex0x32(regs.get(rs2)) << ") ? 1 : 0 = " << hex::to_hex0x32(val) << std::endl;
  }
  regs.set(rd, val);
  pc += 4;
}



/**
 * @brief exec_sltu() is responsible for executing sltu instructions.
 *
 * @param insn, instruction to be executed.
 * @param pos output stream.
 *
 **************************************************************************/
void rv32i_hart::exec_sltu(uint32_t insn, std::ostream* pos)
{
  int32_t rd = get_rd(insn);
  int32_t rs1 = get_rs1(insn);
  int32_t rs2 = get_rs2(insn);

  int32_t val = ((uint32_t)regs.get(rs1) < (uint32_t)regs.get(rs2)) ? 1 : 0;

  if(pos)
  {
    std::string s = render_rtype(insn, "sltu");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = (" << hex::to_hex0x32(regs.get(rs1)) << " <U " << hex::
            to_hex0x32(regs.get(rs2)) << ") ? 1 : 0 = " << hex::to_hex0x32(val) << std::endl; 
  }
  regs.set(rd, val);
  pc += 4;
}



/**
 * @brief exec_xor() is responsible for executing xor instructions.
 *
 * @param insn, instruction to be executed.
 * @param pos output stream.
 *
 **************************************************************************/
void rv32i_hart::exec_xor(uint32_t insn, std::ostream* pos)
{
  int32_t rd = get_rd(insn);
  int32_t rs1 = get_rs1(insn);
  int32_t rs2 = get_rs2(insn);

  int32_t val = regs.get(rs1) ^ regs.get(rs2);

  if(pos)
  {
    std::string s = render_rtype(insn, "xor");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " ^ " << hex::
            to_hex0x32(regs.get(rs2)) << " = " << hex::to_hex0x32(val) << std::endl;
  }
  regs.set(rd, val);
  pc += 4;
}



/**
 * @brief exec_srl() is responsible for executing srl instructions.
 *
 * @param insn, instruction to be executed.
 * @param pos output stream.
 *
 **************************************************************************/
void rv32i_hart::exec_srl(uint32_t insn, std::ostream* pos)
{
  int32_t rd = get_rd(insn);
  int32_t rs1 = get_rs1(insn);
  int32_t rs2 = get_rs2(insn);

  int32_t shift = regs.get(rs2) & 0x0000001f;
  int32_t val = (uint32_t)regs.get(rs1) >> shift;

  if(pos)
  {
    std::string s = render_rtype(insn, "srl");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " >> " <<
            std::dec << shift << " = " << hex::to_hex0x32(val) << std::endl;
  }
  regs.set(rd, val);
  pc += 4;
}



/**
 * @brief exec_sra() is responsible for executing sra instructions.
 *
 * @param insn, instruction to be executed.
 * @param pos output stream.
 *
 **************************************************************************/
void rv32i_hart::exec_sra(uint32_t insn, std::ostream* pos)
{
  int32_t rd = get_rd(insn);
  int32_t rs1 = get_rs1(insn);
  int32_t rs2 = get_rs2(insn);

  int32_t shift = regs.get(rs2) & 0x0000001f;
  int32_t val = regs.get(rs1) >> shift;

  if(pos)
  {
    std::string s = render_rtype(insn, "sra");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " >> " <<
            std::dec << shift << " = " << hex::to_hex0x32(val) << std::endl;
  }
  regs.set(rd, val);
  pc += 4;
}



/**
 * @brief exec_or() is responsible for executing or instructions.
 *
 * @param insn, instruction to be executed.
 * @param pos output stream.
 *
 **************************************************************************/
void rv32i_hart::exec_or(uint32_t insn, std::ostream* pos)
{
  int32_t rd = get_rd(insn);
  int32_t rs1 = get_rs1(insn);
  int32_t rs2 = get_rs2(insn);

  int32_t val = regs.get(rs1) | regs.get(rs2);

  if(pos)
  {
    std::string s = render_rtype(insn, "or");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " | " << hex::
            to_hex0x32(regs.get(rs2)) << " = " << hex::to_hex0x32(val) << std::endl; 
  }
  regs.set(rd, val);
  pc += 4;
}



/**
 * @brief exec_and() is responsible for executing and instructions.
 *
 * @param insn, instruction to be executed.
 * @param pos output stream.
 *
 **************************************************************************/
void rv32i_hart::exec_and(uint32_t insn, std::ostream* pos)
{
  int32_t rd = get_rd(insn);
  int32_t rs1 = get_rs1(insn);
  int32_t rs2 = get_rs2(insn);

  int32_t val = regs.get(rs1) & regs.get(rs2);

  if(pos)
  {
    std::string s = render_rtype(insn, "and");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " & " << hex::
            to_hex0x32(regs.get(rs2)) << " = " << hex::to_hex0x32(val) << std::endl;
  }
  regs.set(rd, val);
  pc += 4;
}



/**
 * @brief exec_ebreak() is responsible for executing ebreak instructions.
 *
 * @param insn, instruction to be executed.
 * @param pos output stream.
 *
 **************************************************************************/
void rv32i_hart::exec_ebreak(uint32_t insn, std::ostream* pos)
{
  if(pos)
  {
    std::string s = render_ebreak(insn);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// HALT" << std::endl;
  }
  halt = true;
  halt_reason = "EBREAK instruction";
}



/**
 * @brief exec_csrrs() is responsible for executing csrrs instructions.
 *
 * @param insn, instruction to be executed.
 * @param pos output stream.
 *
 **************************************************************************/
void rv32i_hart::exec_csrrs(uint32_t insn, std::ostream* pos)
{
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  int32_t csr = get_imm_i(insn) & 0x00000fff;

  if(csr != 0xf14 || rs1 != 0)
  {
    halt = true;
    halt_reason = "Illegal CSR in CRSS instruction";
  }

  if(pos)
  {
    std::string s = render_csrrx(insn, "csrrs");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << std::dec << mhartid << std::endl;
  }

  if(!halt)
  {
    regs.set(rd, mhartid);
    pc += 4;
  }
}
