#include <stdio.h>
#include <sys/types.h>
#include <pc.h>
/* #include <go32.h>
 */


typedef struct {
  unsigned long size_of_this_structure_in_bytes;
  unsigned long linear_address_of_primary_screen;
  unsigned long linear_address_of_secondary_screen;
  unsigned long linear_address_of_transfer_buffer;
  unsigned long size_of_transfer_buffer; /* >= 4k */
  unsigned long pid;
  unsigned char master_interrupt_controller_base;
  unsigned char slave_interrupt_controller_base;
  unsigned short selector_for_linear_memory;
  unsigned long linear_address_of_stub_info_structure;
  unsigned long linear_address_of_original_psp;
  unsigned short run_mode;
  unsigned short run_mode_info;
} Go32_Info_Block;

#include <dos.h>

#include <dpmi.h>




static union REGS r;
static struct SREGS s;



/* DPMI functions ******************************************************************************/

static unsigned char wrapper_intcommon[] = {
0x1e,						/* push    ds                  */
0x06,						/* push    es                  */
0x0f, 0xa0,					/* push    fs                  */
0x0f, 0xa8,					/* push    gs                  */
0x60,						/* pusha                       */
0x66, 0xb8, 0x34, 0x12,				/* mov     ax,0x1234           */
0x8e, 0xd8,					/* mov     ds,ax               */
0x8e, 0xc0,					/* mov     es,ax               */
0x8e, 0xe0,					/* mov     fs,ax               */
0x8e, 0xe8,					/* mov     gs,ax               */
0xe8, 0x00, 0x00, 0x00, 0x00,			/* call    _rmih               */
0x61,						/* popa                        */
0x0f, 0xa9,					/* pop     gs                  */
0x0f, 0xa1,					/* pop     fs                  */
0x07,						/* pop     es                  */
0x1f						/* pop     ds                  */
};

static unsigned char wrapper_intiret[] = {
0xcf						/* iret                        */
};

static unsigned char wrapper_intchain[] = {
0x2e, 0xff, 0x2d, 0x00, 0x00, 0x00, 0x00,	/* jmp     cs:[_old_int+39]    */
0xcf,						/* iret                        */
0x78, 0x56, 0x34, 0x12,
0xcd, 0xab
};


static unsigned char wrapper_common[] = {
0x66, 0x06,				/* push    es                  */
0x66, 0x1e,				/* push    ds                  */
0x66, 0x06,				/* push    es                  */
0x66, 0x1f,				/* pop     ds                  */
0x56,					/* push    esi                 */
0x57,					/* push    edi                 */
0xe8, 0x00, 0x00, 0x00, 0x00,		/* call    _rmcb               */
0x5f,					/* pop     edi                 */
0x5e,					/* pop     esi                 */
0x66, 0x1f,				/* pop     ds                  */
0x66, 0x07,				/* pop     es                  */
0xfc,					/* cld                         */
0x66, 0x8b, 0x06,			/* mov     ax,[esi]            */
0x66, 0x26, 0x89, 0x47, 0x2a,		/* mov     es:[edi+42],ax      */
0x66, 0x8b, 0x46, 0x02,			/* mov     ax,[esi+2]          */
0x66, 0x26, 0x89, 0x47, 0x2c,		/* mov     es:[edi+44],ax      */
};

static unsigned char wrapper_retf[] = {
0x66, 0x26, 0x83, 0x47, 0x2e, 0x04,	/* add     es:[edi+46],0x4     */
0xcf					/* iret                        */
};

static unsigned char wrapper_iret[] = {
0x66, 0x8b, 0x46, 0x04,			/* mov     ax,[esi+4]          */
0x66, 0x26, 0x89, 0x47, 0x20,		/* mov     es:[edi+32],ax      */
0x66, 0x26, 0x83, 0x47, 0x2e, 0x06,	/* add     es:[edi+46],0x6     */
0xcf					/* iret                        */
};


int _go32_dpmi_allocate_dos_memory(_go32_dpmi_seginfo *info)
{
  r.x.ax = 0x0100;
  r.x.bx = info->size;
  int86(0x31, &r, &r);
  if (r.x.flags & 1)
  {
    info->size = r.x.bx;
    return r.x.ax;
  }
  else
  {
    info->rm_segment = r.x.ax;
    info->pm_selector = r.x.dx;
    return 0;
  }
}

int _go32_dpmi_free_dos_memory(_go32_dpmi_seginfo *info)
{
  r.x.ax = 0x0101;
  r.x.dx = info->pm_selector;
  int86(0x31, &r, &r);
  if (r.x.flags & 1)
  {
    return r.x.ax;
  }
  else
  {
    return 0;
  }
}

int _go32_dpmi_resize_dos_memory(_go32_dpmi_seginfo *info)
{
  r.x.ax = 0x0102;
  r.x.bx = info->size;
  r.x.dx = info->pm_selector;
  int86(0x31, &r, &r);
  if (r.x.flags & 1)
  {
    info->size = r.x.bx;
    return r.x.ax;
  }
  else
  {
    return 0;
  }
}

int _go32_dpmi_get_real_mode_interrupt_vector(int vector, _go32_dpmi_seginfo *info)
{
  r.x.ax = 0x0200;
  r.h.bl = vector;
  int86(0x31, &r, &r);
  info->rm_segment = r.x.cx;
  info->rm_offset = r.x.dx;
  return 0;
}

int _go32_dpmi_set_real_mode_interrupt_vector(int vector, _go32_dpmi_seginfo *info)
{
  r.x.ax = 0x0201;
  r.h.bl = vector;
  r.x.cx = info->rm_segment;
  r.x.dx = info->rm_offset;
  int86(0x31, &r, &r);
  return 0;
}

int _go32_dpmi_get_protected_mode_interrupt_vector(int vector, _go32_dpmi_seginfo *info)
{
  r.x.ax = 0x0204;
  r.h.bl = vector;
  int86(0x31, &r, &r);
  info->pm_selector = r.x.cx;
  info->pm_offset = r.x.dx;
  return 0;
}

int _go32_dpmi_set_protected_mode_interrupt_vector(int vector, _go32_dpmi_seginfo *info)
{
  r.x.ax = 0x0205;
  r.h.bl = vector;
  r.x.cx = info->pm_selector;
  r.x.dx = info->pm_offset;
  int86(0x31, &r, &r);
  if (r.x.flags & 1)
  {
    return r.x.ax;
  }
  else
  {
    return 0;
  }
}

int _go32_dpmi_chain_protected_mode_interrupt_vector(int vector, _go32_dpmi_seginfo *info)
{
  unsigned char *wrapper = (unsigned char *)malloc(sizeof(wrapper_intcommon) + sizeof(wrapper_intchain));
  if (wrapper == 0)
    return 0x8015;

  r.x.ax = 0x0204;
  r.h.bl = vector;
  int86(0x31, &r, &r);

  memcpy(wrapper, wrapper_intcommon, sizeof(wrapper_intcommon));
  memcpy(wrapper+sizeof(wrapper_intcommon), wrapper_intchain, sizeof(wrapper_intchain));
  *(short *)(wrapper+9) = _go32_my_ds();
  *(long *)(wrapper+20) = info->pm_offset - (int)wrapper - 24;
  *(long *)(wrapper+sizeof(wrapper_intcommon)+3) = (long)wrapper+sizeof(wrapper_intcommon)+8;
  *(long *)(wrapper+sizeof(wrapper_intcommon)+8) = r.x.dx;
  *(short *)(wrapper+sizeof(wrapper_intcommon)+12) = r.x.cx;

  r.x.ax = 0x0205;
  r.h.bl = vector;
  r.x.cx = _go32_my_cs();
  r.x.dx = (int)wrapper;
  int86(0x31, &r, &r);
  return 0;
}

int _go32_dpmi_allocate_iret_wrapper(_go32_dpmi_seginfo *info)
{
  unsigned char *wrapper = (unsigned char *)malloc(sizeof(wrapper_intcommon) + sizeof(wrapper_intiret));
  if (wrapper == 0)
    return 0x8015;

  memcpy(wrapper, wrapper_intcommon, sizeof(wrapper_intcommon));
  memcpy(wrapper+sizeof(wrapper_intcommon), wrapper_intiret, sizeof(wrapper_intiret));
  *(long *)(wrapper+20) = info->pm_offset - (int)wrapper - 24;

  info->pm_offset = (int)wrapper;
  return 0;
}

int _go32_dpmi_free_iret_wrapper(_go32_dpmi_seginfo *info)
{
  free(info->pm_offset);
  return 0;
}

int _go32_dpmi_simulate_int(int vector, _go32_dpmi_registers *regs)
{
  r.h.bl = vector;
  r.h.bh = 0;
  r.x.cx = 0;
  r.x.di = (int)regs;
  if (vector == 0x21 && regs->x.ax == 0x4b00)
  {
    r.x.ax = 0xff0a;
    int86(0x21, &r, &r);
  }
  else
  {
    r.x.ax = 0x0300;
    int86(0x31, &r, &r);
  }
  if (r.x.flags & 1)
  {
    return r.x.ax;
  }
  else
  {
    return 0;
  }
}

int _go32_dpmi_simulate_fcall(_go32_dpmi_registers *regs)
{
  r.x.ax = 0x0301;
  r.h.bh = 0;
  r.x.cx = 0;
  r.x.di = (int)regs;
  int86(0x31, &r, &r);
  if (r.x.flags & 1)
  {
    return r.x.ax;
  }
  else
  {
    return 0;
  }
}

int _go32_dpmi_simulate_fcall_iret(_go32_dpmi_registers *regs)
{
  r.x.ax = 0x0302;
  r.h.bh = 0;
  r.x.cx = 0;
  r.x.di = (int)regs;
  int86(0x31, &r, &r);
  if (r.x.flags & 1)
  {
    return r.x.ax;
  }
  else
  {
    return 0;
  }
}

int _go32_dpmi_allocate_real_mode_callback_retf(_go32_dpmi_seginfo *info, _go32_dpmi_registers *regs)
{
  unsigned char *wrapper = (unsigned char *)malloc(sizeof(wrapper_common) + sizeof(wrapper_retf));
  if (wrapper == 0)
    return 0x8015;

  memcpy(wrapper, wrapper_common, sizeof(wrapper_common));
  memcpy(wrapper+sizeof(wrapper_common), wrapper_retf, sizeof(wrapper_retf));
  *(long *)(wrapper+11) = info->pm_offset - (int)wrapper - 15;
  info->size = (int)wrapper;

  r.x.ax = 0x0303;
  r.x.si = (int)wrapper;
  r.x.di = (int)regs;
  s.ds = _go32_my_cs();
  s.es = _go32_my_ds();
  s.fs = 0;
  s.gs = 0;
  int86x(0x31, &r, &r, &s);
  if (r.x.flags & 1)
  {
    return r.x.ax;
  }
  else
  {
    info->rm_segment = r.x.cx;
    info->rm_offset = r.x.dx;
    return 0;
  }
}

int _go32_dpmi_allocate_real_mode_callback_iret(_go32_dpmi_seginfo *info, _go32_dpmi_registers *regs)
{
  unsigned char *wrapper = (unsigned char *)malloc(sizeof(wrapper_common) + sizeof(wrapper_iret));
  if (wrapper == 0)
    return 0x8015;

  memcpy(wrapper, wrapper_common, sizeof(wrapper_common));
  memcpy(wrapper+sizeof(wrapper_common), wrapper_iret, sizeof(wrapper_iret));
  *(long *)(wrapper+11) = info->pm_offset - (int)wrapper - 15;
  info->size = (int)wrapper;

  r.x.ax = 0x0303;
  r.x.si = (int)wrapper;
  r.x.di = (int)regs;
  s.ds = _go32_my_cs();
  s.es = _go32_my_ds();
  s.fs = 0;
  s.gs = 0;
  int86x(0x31, &r, &r, &s);
  if (r.x.flags & 1)
  {
    return r.x.ax;
  }
  else
  {
    info->rm_segment = r.x.cx;
    info->rm_offset = r.x.dx;
    return 0;
  }
}

int _go32_dpmi_free_real_mode_callback(_go32_dpmi_seginfo *info)
{
  free(info->size);
  r.x.ax = 0x0304;
  r.x.cx = info->rm_segment;
  r.x.dx = info->rm_offset;
  int86(0x31, &r, &r);
  if (r.x.flags & 1)
  {
    return r.x.ax;
  }
  else
  {
    return 0;
  }
}

int _go32_dpmi_get_free_memory_information(_go32_dpmi_meminfo *info)
{
  r.x.ax = 0x0500;
  r.x.di = (int)info;
  int86(0x31, &r, &r);
  return 0;
}

u_long _go32_dpmi_remaining_physical_memory()
{
  _go32_dpmi_meminfo info;
  _go32_dpmi_get_free_memory_information(&info);
  if (info.available_physical_pages)
    return info.available_physical_pages * 4096;
  return info.available_memory;
}

u_long _go32_dpmi_remaining_virtual_memory()
{
  _go32_dpmi_meminfo info;
  _go32_dpmi_get_free_memory_information(&info);
  return info.available_memory;
}






/* go32 functions ******************************************************************************/
u_short _go32_my_cs()
{
  asm("movw %cs,%ax");
}

u_short _go32_my_ds()
{
  asm("movw %ds,%ax");
}

u_short _go32_my_ss()
{
  asm("movw %ss,%ax");
}

/*infoblk.c*/
Go32_Info_Block _go32_info_block = { sizeof(Go32_Info_Block) };

u_short _go32_conventional_mem_selector()
{
  return _go32_info_block.selector_for_linear_memory;
}

static _go32_dpmi_registers regs;
static volatile u_long ctrl_break_count = 0;
static int ctrl_break_hooked = 0;
static _go32_dpmi_seginfo old_vector;
static _go32_dpmi_seginfo new_vector;

static ctrl_break_isr(_go32_dpmi_registers *regs)
{
  ctrl_break_count ++;
}

u_long _go32_was_ctrl_break_hit()
{
  u_long cnt;
  _go32_want_ctrl_break(1);
  cnt = ctrl_break_count;
  ctrl_break_count = 0;
  return cnt;
}

void _go32_want_ctrl_break(int yes)
{
  if (yes)
  {
    if (ctrl_break_hooked)
      return;
    _go32_dpmi_get_real_mode_interrupt_vector(0x1b, &old_vector);

    new_vector.pm_offset = (int)ctrl_break_isr;
    _go32_dpmi_allocate_real_mode_callback_iret(&new_vector, &regs);
    _go32_dpmi_set_real_mode_interrupt_vector(0x1b, &new_vector);
    ctrl_break_count = 0;
    ctrl_break_hooked = 1;
  }
  else
  {
    if (!ctrl_break_hooked)
      return;
    _go32_dpmi_set_real_mode_interrupt_vector(0x1b, &old_vector);
    _go32_dpmi_free_real_mode_callback(&new_vector);
    ctrl_break_count = 0;
    ctrl_break_hooked = 0;
  }
}
