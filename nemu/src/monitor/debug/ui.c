#include <isa.h>
#include "expr.h"
#include "watchpoint.h"

#include <memory/paddr.h>
#include <cpu/exec.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>



void cpu_exec(uint64_t);
int is_batch_mode();

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
 //if(args == NULL) printf("NULL\n");
  cpu_exec(-1);
  return 0;
}


static int cmd_q(char *args) {
  return -1;
}

static int cmd_help(char *args);
static int cmd_si(char *args);
static int cmd_info(char *args);
static int cmd_x(char *args);
static int cmd_p(char *args);
static int cmd_w(char *args);
static int cmd_d(char *args);


static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  {"si","Let the process suspended after N instructions are executed in one step",cmd_si},
  {"info","Print information of registers or watchpoint",cmd_info},
  {"x","Scan memory",cmd_x},
  {"p","Support mathmatic expression",cmd_p},
  {"w","Add a watchpoint",cmd_w},
  {"d","Delete signed watchpoint",cmd_d}
  /* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

static int cmd_si(char *args)
{

  if(args == NULL)
  {
    cpu_exec(1);
    // isa_exec_once();
    // print_asm();
    return 0;
  }

  uint32_t n = atoi(args);
  for(int i = 0; i < n; ++i ) { 
    // printf("%d---",++exec_count);
    cpu_exec(1);
    // print_asm();
    }

  return 0;
}

static void watchpoints_display()
{
    WP *temp_wp = wp;
    printf("------------------watchpoints_display-----------------\n");
        while(temp_wp)
        {
            printf("id:%d\n",temp_wp->NO+1);
            printf("exp_str:%s\n",temp_wp->exp);
            temp_wp = temp_wp->next;
        }
        printf("total num:%d\n",wp_size);

    printf("------------------watchpoints_display-----------------\n");
}

static int cmd_info(char *args)
{
  if(strcmp(args,"r") == 0) isa_reg_display();
  if(strcmp(args,"w") == 0) watchpoints_display(); 
  return 0;
}

static int cmd_x(char *args)
{
  char *token = " ";

  char *strN = strtok(args,token);
  uint32_t N = atoi(strN);
  char *str_expr =  strtok(NULL,token);
  
  paddr_t expr = (paddr_t)strtol(str_expr,NULL,16); 

  printf("\n--------------------Memory Scan---------------------\n");
  for(uint32_t i = 1; i <= N; ++i)
  {
    printf("%x:0x%08x\n",expr,paddr_read(expr,4));
    expr += 4;
  }

  printf("--------------------Memory Scan---------------------\n");

  return 0;
}

static int cmd_p(char *args)
{
    bool success = false;
    word_t result =  expr(args,&success);
    if(success){
        printf("0x%08x\n",result);
    }else{
        printf("failure\n");
    }

    return 0;
}

static int cmd_w(char *args)
{

    WP * temp_wp = wp;
    ++wp_size;
    bool scuccess = 0;

    if(wp == NULL) {
        wp = new_wp();
        strcpy(wp->exp,args);
        wp->info = expr(args,&scuccess);
    }else{
        while(temp_wp->next != NULL)  { temp_wp = temp_wp->next;}

        temp_wp->next = new_wp();
        strcpy(temp_wp->next->exp,args);
        temp_wp->next->info = expr(args,&scuccess);
    }

    if(scuccess == 0){printf("expr(%s)failed\n", args);}
    return 0;
}

static int cmd_d(char *args)
{
    int id = strtoul(args,NULL,10);

    if(id <= 0) return 0;
    free_wp(id,&wp);
    --wp_size;
    return 0;
}


void ui_mainloop() {
  if (is_batch_mode()) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }

    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
