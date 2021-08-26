#include <klib.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  if(s == NULL) return 0;

  size_t len = 0;
  while(s[len] != '\0')
  {
    ++len;
  }

  return len;
}
char* strncpy(char* dst, const char* src, size_t n) {
  int i = 0;
  for( ; i < n && src[i] != '\0'; ++i) dst[i] = src[i];
  for(; i < n; ++i) dst[i] = '\0';

  return dst;
}

char *strcpy(char* dst,const char* src) {
  return strncpy(dst,src,strlen(src));
}



char* strcat(char* dst, const char* src) {
  int dst_len = strlen(dst);
  
  int i = 0;
  for(; i < strlen(src); ++i) dst[dst_len+i] = src[i];
  dst[dst_len+i] = '\0';
  return dst;
}

int strcmp(const char* s1, const char* s2) {
  
  size_t len_s1 = strlen(s1);
  size_t len_s2 = strlen(s2);
  size_t min_len  = len_s1 < len_s2 ?  len_s1 : len_s2;

  for(int i = 0; i < min_len; ++i)
  {
    if(s1[i] > s2[i]) return 1;
    else if(s1[i] < s2[i]) return -1;
  }

  if(len_s1 > len_s2) return 1;
  else if(len_s1 < len_s2) return -1; 

  return 0;
}

int strncmp(const char* s1, const char* s2, size_t n) {
  // uint32_t len_s1 = strlen(s1);
  // uint32_t len_s2 = strlen(s2);
  // int min_len  = len_s1 < len_s2 ?  len_s1 : len_s2;

  // min_len = min_len > n ? n : min_len;

  for(uint32_t i = 0; i < n; ++i)
  {
    if(s1[i] > s2[i]) return 1;
    else if(s1[i] < s2[i]) return -1;
  }

  return 0;
}

void* memset(void* v,int c,size_t n) {
  if(v == NULL) return NULL;
  int i  = 0;

  char * temp_v = v;
  for( ; i < n; ++i)
  {
    temp_v[i] = (unsigned char)c;
  }
 
 return v;
}

void* memmove(void* dst,const void* src,size_t n) {
  return NULL;
}

void* memcpy(void* out, const void* in, size_t n) {
  size_t i = 0;

  char * temp_out = out;
  const char * temp_in  = in;
  for( ; i < n ; ++i) temp_out[i] = temp_in[i];
  return out;
}

int memcmp(const void* s1, const void* s2, size_t n) {

  const unsigned char * temp_s1 = s1;
  const unsigned char * temp_s2 = s2;
  size_t i = 0;

  for(; i < n; ++i)
  {
    if(temp_s1[i] > temp_s2[i]) return 1;
    else if(temp_s1[i] < temp_s2[i]) return -1;
  }

 return 0;
}

#endif
