static inline def_EHelper(jal){
  rtl_addi(s,ddest,&cpu.pc,4);
  rtl_addi(s,&cpu.pc,&cpu.pc,id_src1->imm);
  rtl_j(s,cpu.pc);

  print_asm_template2(jal);
}

static inline def_EHelper(jalr){

  word_t t = cpu.pc + 4;
  word_t temp = cpu.gpr[id_src1->reg]._32 + id_src2->imm;
  temp = temp & 0xfffffffe;
  rtl_li(s,&cpu.pc,temp);
  rtl_li(s,ddest,t);
  // printf("name %s \n",reg_name(ddest));
  rtl_j(s,cpu.pc);
  print_asm_template3(jalr);
}

static inline def_EHelper(bne){
  rtlreg_t temp;
  rtl_setrelop(s,RELOP_NE,&temp,dsrc1,dsrc2);
  if(temp!=0)
  {
    rtl_addi(s,&cpu.pc,&cpu.pc,id_dest->imm);
    rtl_j(s,cpu.pc);
  }
  print_asm_template3(bne);
}



static inline def_EHelper(beq){
  rtlreg_t temp;
  rtl_setrelop(s,RELOP_EQ,&temp,dsrc1,dsrc2);
  if(temp!=0)
  {
    rtl_addi(s,&cpu.pc,&cpu.pc,id_dest->imm);
    rtl_j(s,cpu.pc);
  }
  print_asm_template3(beq);
}

static inline def_EHelper(blt){
  rtlreg_t temp;
  rtl_setrelop(s,RELOP_LT,&temp,dsrc1,dsrc2);
  if(temp!=0)
  {
    rtl_addi(s,&cpu.pc,&cpu.pc,id_dest->imm);
    rtl_j(s,cpu.pc);
  }
  print_asm_template3(blt);
}

static inline def_EHelper(bge){
  rtlreg_t temp;
  rtl_setrelop(s,RELOP_GE,&temp,dsrc1,dsrc2);
  if(temp!=0)
  {
    rtl_addi(s,&cpu.pc,&cpu.pc,id_dest->imm);
    rtl_j(s,cpu.pc);
  }
  print_asm_template3(bge);
}

static inline def_EHelper(bltu){
  rtlreg_t temp;
  rtl_setrelop(s,RELOP_LTU,&temp,dsrc1,dsrc2);
  if(temp!=0)
  {
    rtl_addi(s,&cpu.pc,&cpu.pc,id_dest->imm);
    rtl_j(s,cpu.pc);
  }
  print_asm_template3(bltu);
}

static inline def_EHelper(bgeu){
  rtlreg_t temp;
  rtl_setrelop(s,RELOP_GEU,&temp,dsrc1,dsrc2);
  if(temp!=0)
  {
    rtl_addi(s,&cpu.pc,&cpu.pc,id_dest->imm);
    rtl_j(s,cpu.pc);
  }
  print_asm_template3(bgeu);
}