#include <am.h>
#include <nemu.h>
#include <klib.h>

static  uint32_t origin_tv_uses;
static  uint32_t origin_tv_sec;

void __am_timer_init() {
  origin_tv_uses = inl(RTC_ADDR);
   origin_tv_sec = inl(RTC_ADDR+4);
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
  uptime->us = (inl(RTC_ADDR+4)-origin_tv_sec) * 1000000 + (inl(RTC_ADDR)-origin_tv_uses + 500);
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}
