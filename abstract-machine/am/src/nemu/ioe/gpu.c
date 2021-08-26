#include <am.h>
#include <nemu.h>

#define W 400
#define H 300
#define SYNC_ADDR (VGACTL_ADDR + 4)

void __am_gpu_init() {

  // int i;
  // int w = inw(VGACTL_ADDR+2);
  // int h = inw(VGACTL_ADDR);
  // for (i = 0; i < w * h; i ++) fb[i] = i;
  // outl(SYNC_ADDR, 1);
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = 400, .height = 300,
    .vmemsz = 0
  };
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
 uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;

  uint32_t x = ctl->x; 
  uint32_t y = ctl->y;
  uint32_t h = ctl->h;
  uint32_t w = ctl->w;
  uint32_t *pix = ctl->pixels;

  for(int i  = 0; i < h; ++i)
  {
    for(int j = 0; j < w; ++j)
    {
      if(y+i < H && x+j < W)
      {
        fb[(W*(i+y)) + j + x ] = pix[w*i+j];
      }
    }
  }


  if (ctl->sync) {
    outl(SYNC_ADDR, 1);
  }
 
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
