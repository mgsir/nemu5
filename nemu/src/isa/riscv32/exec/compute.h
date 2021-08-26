static inline def_EHelper(lui) {
  rtl_li(s, ddest, id_src1->imm); // low bit was setten as zero
  print_asm_template2(lui);
}

static inline def_EHelper(addi){
  rtl_addi(s,ddest,dsrc1,id_src2->imm );
  print_asm_template3(addi);
}


static inline def_EHelper(add){
  rtl_add(s,ddest,dsrc1,dsrc2);
  print_asm_template3(add);
}

static inline def_EHelper(sub){
  rtl_sub(s,ddest,dsrc1,dsrc2);
  print_asm_template3(sub);
}
static inline def_EHelper(auipc){
  rtl_addi(s,ddest,&cpu.pc,id_src1->imm);
  print_asm_template2(auipc);
}




static inline def_EHelper(sll){
  rtl_shl(s,ddest,dsrc1,dsrc2);
  print_asm_template3(sll);
}

static inline def_EHelper(slli){
  rtl_shli(s,ddest,dsrc1,id_src2->imm);
  print_asm_template3(slli);
}

static inline def_EHelper(srl){
  rtl_shr(s,ddest,dsrc1,dsrc2);
  print_asm_template3(srl);
}

static inline def_EHelper(srli){
  rtl_shri(s,ddest,dsrc1,id_src2->imm);
  print_asm_template3(srli);
}


static inline def_EHelper(sra){
  rtl_sar(s,ddest,dsrc1,dsrc2);
  print_asm_template3(sra);
}

static inline def_EHelper(srai){
  rtl_sari(s,ddest,dsrc1,id_src2->imm);
  print_asm_template3(srai);
}

static inline def_EHelper(sltiu){
  // rtlreg_t temp = id_src2->imm;
  // rtl_zext(s,&temp,&temp,12);
  // cpu.gpr[*ddest]._32 = (cpu.gpr[*dsrc1]._32 < temp);
  rtl_setrelopi(s,RELOP_LTU,ddest,dsrc1,id_src2->imm);
  print_asm_template3(sltiu);
}

static inline def_EHelper(or){
  rtl_or(s,ddest,dsrc1,dsrc2);
  print_asm_template3(or);
}

static inline def_EHelper(ori){
  rtl_ori(s,ddest,dsrc1,id_src2->imm);
  print_asm_template3(ori);
}

static inline def_EHelper(xor){
  rtl_xor(s,ddest,dsrc1,dsrc2);
  print_asm_template3(xor);
}

static inline def_EHelper(xori){
  rtl_xori(s,ddest,dsrc1,id_src2->imm);
  print_asm_template3(xori);
}

static inline def_EHelper(slt){
  rtl_setrelop(s,RELOP_LT,ddest,dsrc1,dsrc2);
  print_asm_template3(slt);
}


static inline def_EHelper(sltu){
  rtl_setrelop(s,RELOP_LTU,ddest,dsrc1,dsrc2);
  print_asm_template3(sltu);
}


static inline def_EHelper(and){
  rtl_and(s,ddest,dsrc1,dsrc2);
  print_asm_template3(and);
}

static inline def_EHelper(andi)
{
  rtl_andi(s,ddest,dsrc1,id_src2->imm);
  print_asm_template3(andi);
}


static inline def_EHelper(mul)
{
  rtl_imul_lo(s,ddest,dsrc1,dsrc2);
  print_asm_template3(mul);
}

static inline def_EHelper(mulu)
{
  rtl_mul_lo(s,ddest,dsrc1,dsrc2);
  print_asm_template3(mulu);
}

static inline def_EHelper(mulh)
{
  rtl_imul_hi(s,ddest,dsrc1,dsrc2);
  print_asm_template3(mulh);
}

static inline def_EHelper(mulhu)
{
  rtl_mul_hi(s,ddest,dsrc1,dsrc2);
  print_asm_template3(mulhu);
}

static inline def_EHelper(mulhsu)
{
  *ddest = (((int64_t)(sword_t)(*dsrc1) * (uint64_t)(dsrc2)) >> 32);
  print_asm_template3(mulhsu);
}



static inline def_EHelper(div)
{
  rtl_idiv_q(s,ddest,dsrc1,dsrc2);
  print_asm_template3(div);
}

static inline def_EHelper(divu)
{
  rtl_div_q(s,ddest,dsrc1,dsrc2);
  print_asm_template3(divu);
}

static inline def_EHelper(remu)
{
  rtl_div_r(s,ddest,dsrc1,dsrc2);
  print_asm_template3(remu);
}

static inline def_EHelper(rem)
{
  rtl_idiv_r(s,ddest,dsrc1,dsrc2);
  print_asm_template3(rem);
}