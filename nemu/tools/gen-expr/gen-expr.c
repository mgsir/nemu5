#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536] = {};
static char code_buf[65536 + 128] = {}; // a little larger than `buf`
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

static char str_num[100] = {};
static char ops[4][2] = {"+","-","*","/"};
static int iterator_num = 0;

static char * gen_rand_op()
{
    return ops[rand() % 4];
}

static char *gen_num()
{
  uint32_t num = rand() % 2+1;
  memset(str_num,0,sizeof(str_num));
  char nums[9][2] = {"1","2","3","4","5","6","7","8","9"};
  for(uint32_t i = 0; i < num; ++i)
  {
    strcat(str_num,nums[rand()%9]);
  }
  return str_num;
}

static inline void gen_rand_expr() {
  iterator_num++;
  if(iterator_num > 100) return;
  if(strlen(buf) > 65535) return;

  switch (rand() % 3)
  {
  case 0:
    strcat(buf,gen_num());
    break;
  case 1:
    strcat(buf,"(");
    gen_rand_expr();
    strcat(buf,")");
    break; 
  default: 
    gen_rand_expr(); 
    strcat(buf,gen_rand_op());
    gen_rand_expr();
    break;
  }

}

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    
    memset(buf,0,sizeof(buf));
    iterator_num = 0;

    gen_rand_expr();

    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -o /tmp/.expr");
    if (ret != 0) continue;

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    fscanf(fp, "%d", &result);
    pclose(fp);

    printf("%u %s\n", result, buf );
  
  }
  return 0;
}
