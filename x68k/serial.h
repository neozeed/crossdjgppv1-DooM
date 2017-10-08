#ifndef __SERIAL_H__
#define __SERIAL_H__

#define SER_DEBUG_ERR 1
#define SER_DEBUG_WARN 2
#define SER_DEBUG_INFO 3

extern void SER_puts(char *s);
extern void SER_debug(char *s, char mode);
extern void SER_cls();

#endif