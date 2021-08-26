#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {

  va_list ap;
  char c = *fmt;
  // char pre ;

  va_start(ap,fmt);
  int print_cnt = 0;
  int num;
  char *str;

  while(c)
  {
    if ( (fmt - 1) != NULL && *(fmt - 1) == '%')
    {

      switch (c)
      {
      case 'd':
        num = va_arg(ap, int);
        if (num == 0)
        {
          putch('0');
        }
        else
        {
          int num_len = 0;
          int arr_num[1000];
          while (num)
          {
            arr_num[num_len] = num % 10;
            num /= 10;
            ++num_len;
          }

          for (int i = num_len - 1; i >= 0; --i)
          {
            putch(arr_num[i] - 0 + '0');
          }
        }
        break;
      case 's':
        str = va_arg(ap, char *);
        for (int i = 0; i < strlen(str); ++i)
        {
            putch(str[i]);
        }
        break;
      default:
         putch(c);
      }
    }else{

        if(c != '%' ) putch(c);
    }
    c = *(++fmt);
  }

  va_end(ap);
  return print_cnt;
  // char buffer[100];
  // int size = sprintf(buffer,fmt);

  // for(int i = 0; i < strlen(buffer); ++i)
  // {
  //   putch(buffer[i]);
  // }

  // return size;
}


int vsprintf(char *out, const char *fmt, va_list ap) {
  return 0;
}


int sprintf(char *out, const char *fmt, ...) {
  va_list ap;
  char c = *fmt;

  va_start(ap,fmt);
  int print_cnt = 0;
  int num;
  char *str;

  while(c)
  {
    if ( (fmt - 1) != NULL && *(fmt - 1) == '%')
    {

      switch (c)
      {
      case 'd':
        num = va_arg(ap, int);
        if (num == 0)
        {
          out[print_cnt++] = '0';
        }
        else
        {
          int num_len = 0;
          int arr_num[1000];
          while (num)
          {
            arr_num[num_len] = num % 10;
            num /= 10;
            ++num_len;
          }

          for (int i = num_len - 1; i >= 0; --i)
          {
            out[print_cnt++] = arr_num[i] - 0 + '0';
          }
        }
        break;
      case 's':
        str = va_arg(ap, char *);
        for (int i = 0; i < strlen(str); ++i)
        {
          out[print_cnt++] = str[i];
        }
        break;
      default:
         out[print_cnt++] = c;
      }
    }else{

        if(c != '%' ) out[print_cnt++] = c;
    }
    c = *(++fmt);
  }

  va_end(ap);
  out[print_cnt] = '\0';
  return print_cnt;
}


int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  return 0;
}

#endif
