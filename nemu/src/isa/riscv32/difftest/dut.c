#include <isa.h>
#include <monitor/difftest.h>
#include "../local-include/reg.h"
#include "difftest.h"



bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {

  
  for(int i = 0; i < 32; ++i)
  {
    if(cpu.gpr[i]._32 != ref_r->gpr[i]._32) 
    {
    
      printf("diff: %s\n",reg_name(i));
      printf("nemu value : 0x%08x  qemu value : 0x%08x\n",cpu.gpr[i]._32,ref_r->gpr[i]._32);
      return false;
    }
  }
  if(ref_r->pc!= cpu.pc)
  {
    printf("diff: %s","pc\n");
    printf("nemu value : 0x%08x  qemu value : 0x%08x\n",cpu.pc,ref_r->pc);
    return false;
  }
  

  return true;
}

void isa_difftest_attach() {
}
