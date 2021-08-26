#include <cpu/exec.h>
#include "../local-include/decode.h"
#include "all-instr.h"

static inline void set_width(DecodeExecState *s, int width) {
  if (width != 0) s->width = width;
}

static inline def_EHelper(load) {
  switch (s->isa.instr.i.funct3) {
    EXW  (2, ld, 4)
    EXW  (0b000,lds,1)
    EXW  (0b001,lds,2)
    EXW  (0b100,ld,1)
    EXW  (0b101,ld,2)
    default: exec_inv(s);
  }
}

static inline def_EHelper(store) {
  switch (s->isa.instr.s.funct3) {
    EXW  (2, st, 4)
    EXW  (0b000, st, 1)
    EXW  (0b001, st, 2)
    default: exec_inv(s);
  }
}

static inline def_EHelper(compute)
{
  if (s->isa.instr.r.funct7 == 0b0000000 || s->isa.instr.r.funct7 == 0b0100000)
  {
    switch (s->isa.instr.r.funct3)
    {
    case 0b000:
      switch (s->isa.instr.r.funct7)
      {
        EX(0b0000000, add)
        EX(0b0100000, sub)
      }
      break;
      EX(0b001, sll)
      EX(0b010, slt)
      EX(0b011, sltu)
      EX(0b100, xor)
    case 0b101:
      switch (s->isa.instr.r.funct7)
      {
        EX(0b0000000, srl)
        EX(0b0100000, sra)
      }
      break;
      EX(0b110, or)
      EX(0b111, and)
    default:
      exec_inv(s);
    }
  }
  else if(s->isa.instr.r.funct7 == 0b0000001)
  {
    switch (s->isa.instr.r.funct3)
    {
      EX(0b000, mul)
      EX(0b001, mulh)
      EX(0b010, mulhsu)
      EX(0b011, mulhu)
      EX(0b100, div)
      EX(0b101, divu)
      EX(0b110, rem)
      EX(0b111, remu)
    }
  }

}

static inline def_EHelper(computei)
{
  switch(s->isa.instr.i.funct3)
  {
    EX(0b000,addi)
    EX(0b001,slli)
    case 0b101:
    switch (0xfc0 & s->isa.instr.i.simm11_0 )
    {
      EX(0b000000000000,srli)
      EX(0b010000000000,srai)
    }
    break;
    EX(0b011,sltiu)
    EX(0b100,xori)
    EX(0b110,ori)
    EX(0b111,andi)
    break;
  }
}

static inline def_EHelper(branch)
{
  
  switch(s->isa.instr.b.funct3)
  {
    EX(0b001,bne)
    EX(0b000,beq)
    EX(0b100,blt)
    EX(0b101,bge)
    EX(0b110,bltu)
    EX(0b111,bgeu)
  }
}

static inline void fetch_decode_exec(DecodeExecState *s) {
  s->isa.instr.val = instr_fetch(&s->seq_pc, 4);
  Assert(s->isa.instr.i.opcode1_0 == 0x3, "Invalid instruction");
  switch (s->isa.instr.i.opcode6_2) {
    IDEX (0b00000, I, load)
    IDEX (0b01000, S, store)
    IDEX (0b01101, U, lui)
    IDEX (0b00100,I,computei)
    IDEX (0b00101,U,auipc)
    IDEX (0b11011,J,jal)
    IDEX (0b11001,I,jalr)
    IDEX (0b01100,R,compute)
    IDEX (0b11000,B,branch)

    EX   (0b11010, nemu_trap)
    default: exec_inv(s);
  }
}

static inline void reset_zero() {
  reg_l(0) = 0;
}

vaddr_t isa_exec_once() {
  DecodeExecState s;
  s.is_jmp = 0;
  s.seq_pc = cpu.pc;

  fetch_decode_exec(&s);
  update_pc(&s);

  reset_zero();

  return s.seq_pc;
}
