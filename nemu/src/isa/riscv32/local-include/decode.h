#include <cpu/exec.h>
#include "rtl.h"


int mode_switch = 2; // if mode_switch == 0 , it means bit width equales to 12 , when mode_swithc == 1 then bit width equales to 20 ,otherwise it dont need sign  extension

// decode operand helper
#define def_DopHelper(name) \
  void concat(decode_op_, name) (DecodeExecState *s, Operand *op, word_t val, bool load_val)

static inline def_DopHelper(i) {
  op->type = OP_TYPE_IMM;
 
  if( (val & 0x00100000) && (mode_switch ==1) ){
    val = val | 0xfff00000;
    // printf("123");
  }
  else if((val & 0x800) && !mode_switch){
    val = val | 0xfffff000;
    // printf("123");
  }
  else if((val & 0x1000) && mode_switch == 3){
    val =  val |  0xffffe000;
  }

  op->imm = val;
  // printf(" : 0x%08x\n",(int)val);
  print_Dop(op->str, OP_STR_SIZE, "%d", op->imm);
}

static inline def_DopHelper(r) {
  op->type = OP_TYPE_REG;
  op->reg = val;
  op->preg = &reg_l(val);

  print_Dop(op->str, OP_STR_SIZE, "%s", reg_name(op->reg));
}

static inline def_DHelper(I) {
  mode_switch = 0;
  decode_op_r(s, id_src1, s->isa.instr.i.rs1, true);
  decode_op_i(s, id_src2, s->isa.instr.i.simm11_0, true);
  decode_op_r(s, id_dest, s->isa.instr.i.rd, false);
}

static inline def_DHelper(U) {
  mode_switch = 2;
  decode_op_i(s, id_src1, s->isa.instr.u.imm31_12 << 12, true);
  decode_op_r(s, id_dest, s->isa.instr.u.rd, false);

  print_Dop(id_src1->str, OP_STR_SIZE, "0x%x", s->isa.instr.u.imm31_12);
}

static inline def_DHelper(S) {
  decode_op_r(s, id_src1, s->isa.instr.s.rs1, true);
  sword_t simm = (s->isa.instr.s.simm11_5 << 5) | s->isa.instr.s.imm4_0;
  decode_op_i(s, id_src2, simm, true);
  decode_op_r(s, id_dest, s->isa.instr.s.rs2, true);
}

static inline def_DHelper(J) {
  mode_switch = 1;
  sword_t simm = (s->isa.instr.j.imm19_12 << 12 ) | (s->isa.instr.j.imm11 << 11) | (s->isa.instr.j.imm10_1 << 1) | (s->isa.instr.j.imm20 << 20); 
  decode_op_i(s, id_src1,simm , true);
  decode_op_r(s, id_dest, s->isa.instr.j.rd, false);
}

static inline def_DHelper(R){
  decode_op_r(s,id_src1,s->isa.instr.r.rs1,true);
  decode_op_r(s,id_src2,s->isa.instr.r.rs2,true);
  decode_op_r(s,id_dest,s->isa.instr.r.rd,false);
}

static inline def_DHelper(B){
  mode_switch = 3;
  decode_op_r(s,id_src1,s->isa.instr.b.rs1,true);
  decode_op_r(s,id_src2,s->isa.instr.b.rs2,true);
  sword_t simm = ( s->isa.instr.b.imm4_1 << 1) | (s->isa.instr.b.imm10_5 << 5) | (s->isa.instr.b.imm11 << 11) | (s->isa.instr.b.imm12 << 12) ; 
  // printf("0x%08x\n",simm);
  decode_op_i(s,id_dest,simm, true);
}
