#include <isa.h>
#include<stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory/vaddr.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

int skip_$_type = 0;
enum {
  TK_NOTYPE = 256, TK_EQ, TK_NUM, TK_DEREF, TK_0X,TK_$, TK_LEQ, TK_NEQ,TK_AND, TK_REG
  /* TODO: Add more token types */

};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  // {"[EePp][ADCBSDPadcbsdp][XIPxip]*",TK_REG}, // register and depend on ISA
  {"\\$",TK_$}, //'$' must match index 0
  {"[$][0]",TK_REG}, 
  {"[rsgtpa][0-9acp][0-9]*",TK_REG}, //  depend on ISA(rsicv-32)  and can not support register '$0'
  {" +", TK_NOTYPE},    // spaces
  {"\\+", '+'},         // plus
  {"\\-", '-'},
  {"\\*",'*'},
  {"\\/",'/'},
  {"\\(",'('},
  {"\\)",')'},
  {"==", TK_EQ},        // equal
  {"0[xX]",TK_0X},
  {"<=",TK_LEQ}, // less and euqal
  {"!=",TK_NEQ}, // not equal
  {"&&",TK_AND},
  {"[0-9a-fA-F]*",TK_NUM},    // number
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )
#define TOK_STR_LEN 30000
#define TOK_SIZE 1000

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }

}

typedef struct token {
  int type;
  char str[TOK_STR_LEN+1] ;
} Token;

static Token tokens[TOK_SIZE] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    if(skip_$_type == 1) { i = 1; skip_$_type = 0;}
    else  i = 0;
    for (; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;
        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
        struct token temp;
        memset(temp.str,0,sizeof(temp.str));

        switch (rules[i].token_type)
        {
        case TK_NOTYPE:
          temp.type = TK_NOTYPE;
          assert(substr_len <= TOK_STR_LEN);
          strncpy(temp.str, e + position - substr_len, substr_len);
          break;
        case TK_0X:
          temp.type = TK_0X;
          strcpy(temp.str,"0x");
          break;
        case TK_REG:
          temp.type = TK_REG ;
          assert(substr_len <= TOK_STR_LEN);
          strncpy(temp.str, e + position - substr_len, substr_len);;
          break;
        case TK_$:
          temp.type = TK_$;
          strcpy(temp.str,"$");
          break;
        case TK_NEQ:
          temp.type = TK_NEQ;
          strcpy(temp.str,"!=");
          break;
        case TK_AND:
          temp.type = TK_AND;
          strcpy(temp.str,"&&");
          break;
        case TK_EQ:
          temp.type = TK_EQ;
          strcpy(temp.str, "==");
          break;
        case '+':
          temp.type = '+';
          strcpy(temp.str, "+");
          break;
        case '-':
          temp.type = '-';
          strcpy(temp.str, "-");
          break;
        case '*':
          temp.type = '*';
          strcpy(temp.str, "*");
          break;
        case '/':
          temp.type = '/';
          strcpy(temp.str, "/");
          break;
        case '(':
          temp.type = '(';
          strcpy(temp.str, "(");
          break;
        case ')':
          temp.type = ')';
          strcpy(temp.str, ")");
          break;
      case TK_NUM:
          temp.type = TK_NUM;
          assert(substr_len <= TOK_STR_LEN);
          strncpy(temp.str, e + position - substr_len, substr_len);
          break;
        }

        tokens[nr_token] = temp;
        ++nr_token;
        if(temp.type == TK_$) skip_$_type = 1;
        break;
      }


    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}



bool check_parentheses(uint32_t p, uint32_t q)
{
  if(tokens[p].type != '(' || tokens[q].type != ')') return false;

  uint32_t _lock = 1;
  for(u_int32_t i = p+1; i < q; ++i)
  {
    if(tokens[i].type == '(') ++ _lock;
    else if(tokens[i].type == ')') -- _lock;
    if(_lock == 0) return false;
  }

  --_lock;
  return _lock == 0 ? true : false;
}

uint32_t find_main_operator(uint32_t p, uint32_t q)
{

  uint32_t mainop_pos = p;
  char mainop = ' ';
  uint32_t _lock = 0;

  for(u_int32_t i = p; i <= q; ++i)
  {
    if(_lock == 0) {
      if(tokens[i].type == '(')  ++ _lock ;
      else {
        if( (tokens[i].type == '+' || tokens[i].type == '-') && (mainop == '*' || mainop == '/' || mainop == ' ')) // low power
        {
          mainop = tokens[i].type;
          mainop_pos = i;
      //   printf("--%c--",mainop);
          // return i;
        }
        else if((tokens[i].type == '*' || tokens[i].type == '/') && mainop == ' ')
        {
          mainop = tokens[i].type;
          mainop_pos = i;
     //     printf("--%c--",mainop);
        }
      }
    }else{
      if(tokens[i].type == ')') -- _lock; 
      else if(tokens[i].type == '(') ++ _lock;
    }
  }

  return mainop_pos;
}

uint32_t eval(uint32_t p,  uint32_t q)
{ 
 
  if(p > q)  return 0;
  else if(p == q) return (uint32_t)strtol(tokens[p].str,NULL,10);
  else if(check_parentheses(p,q) == true) 
  {
      /*
    printf("p:%u,q:%u\n",p,q);
    for (int i = p; i <= q; ++i)
    {
      printf("%s", tokens[i].str);
    }
    */
     return eval(p + 1, q - 1);
  }
  else
  {
    uint32_t pos = find_main_operator(p,q);
   // printf("sign: %c num:%d\n",tokens[pos].type,pos);
    //printf("p:%u,q:%u\n",p,q);
    /*
    for(int i = p; i <= q; ++i)
    {
      printf("%s",tokens[i].str);
    }
    printf("\n");
    */
    switch (tokens[pos].type)
    {
      case '+':
        return eval(p,pos-1) + eval(pos+1,q);
      case '-':
        return eval(p,pos-1) - eval(pos+1,q);
      case '*':
        return eval(p,pos-1) * eval(pos+1,q);
      case '/':
        assert(eval(pos+1,q) != 0);
        return eval(p,pos-1) / eval(pos+1,q);
      case TK_NEQ:
        return eval(p,pos-1) != eval(pos+1,q);
      case TK_AND:
        return eval(p,pos-1) && eval(pos+1,q);
      case TK_DEREF:
        return paddr_read(eval(pos+1,q),4);
      default: assert(0);
    }

  }
  return 0;
}


bool type_compare(int pre_type)
{
    if(pre_type == TK_NUM) return false;
    else return true;
}

void _ui32tostr(uint32_t num, char *buf, int base)
{

    char temp[30];
    memset(temp,0,sizeof(temp));
    int cnt = 0;
    if(num == 0)
    {
      buf[0] = '0';
      return;
    }

    while(num)
     {
         temp[cnt++] = (num%base) + '0';
         num /= base;
     }

    for(int i = 0; i < cnt; ++i)
    {
        buf[i] = temp[cnt-i-1];
    }
    return ;
}



void _dealwith_sepcial_sign(int type)
{
 for(uint32_t i = 0; i < nr_token;++i) 
  {
    if(tokens[i].type == TK_0X && type == TK_0X)
    {
        assert(tokens[i+1].type == TK_NUM);
        uint32_t decimal_number = (uint32_t)strtol(tokens[i+1].str,NULL,16);
        memset(tokens[i+1].str,0,sizeof(tokens[i+1].str));
        _ui32tostr(decimal_number,tokens[i+1].str,10);
        for(int j = i; j < nr_token - 1; ++j)
        {
            tokens[j]  = tokens[j+1];
        }
        --nr_token;
        --i;
    }
    else if(tokens[i].type == TK_$ && type == TK_$)
    {
        assert(tokens[i+1].type == TK_REG);

        bool scuccess = false;
        uint32_t reg_val = (uint32_t) isa_reg_str2val(tokens[i+1].str, &scuccess);

        memset(tokens[i+1].str,0,sizeof(tokens[i+1].str));
        _ui32tostr(reg_val,tokens[i+1].str,10);

        for(int j = i; j < nr_token - 1; ++j)
        {
            tokens[j]  = tokens[j+1];
        }
        --nr_token;
        --i;
    }else if(tokens[i].type == TK_NOTYPE && type == TK_NOTYPE){
        for(int j = i; j < nr_token - 1; ++j)
        {
            tokens[j]  = tokens[j+1];
        }
        --nr_token;
        --i;
    }

  }
}

word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  *success = true;

  /* deal with ' ' case */
  _dealwith_sepcial_sign(TK_NOTYPE);

  for(uint32_t i = 0; i < nr_token; ++i)
  {
    if(tokens[i].type == '*' && (i == 0 || type_compare(tokens[i-1].type)))
    {
        tokens[i].type = TK_DEREF;
    }
  }

  
  /* deal with '0x' case */
  _dealwith_sepcial_sign(TK_0X);
  /* deal with '$' case */
  _dealwith_sepcial_sign(TK_$);

  return eval(0,nr_token-1);
}


void check_regex()
{
  FILE * fpr;
  FILE * fpw;
  assert((fpr = fopen("/home/mg/cs/nemu/tools/gen-expr/input","r")) != NULL);
  assert((fpw = fopen("/home/mg/cs/nemu/tools/gen-expr/ouput","w")) != NULL);

  char buffer[1025];

  while(true)
  {
    memset(buffer, 0, sizeof(buffer));
    if(fgets(buffer, 1024, fpr) == 0) break;
    char *ans;
    ans = strtok(buffer, " ");
    char *e = strtok(NULL, " ");
    bool success = false;
    if(e[strlen(e) - 1] == '\n')e[strlen(e)-1] = '\0';

    uint32_t result = expr(e, &success);

    printf("result: %d\n",result);
    if(success)
    {
      fprintf(fpw,"%s %s %u\n",ans,e,result );
    }else fprintf(fpw,"%s %s ileagel\n",ans,e);
  }

}

