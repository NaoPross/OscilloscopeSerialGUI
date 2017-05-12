#ifndef __TEK_INTERFACE_H__
#define __TEK_INTERFACE_H__

#include <stdlib.h>

int tek_open();
int tek_close();

int tek_write(void *data, size_t size);
int tek_read(void *data, size_t size);

#endif // __TEK_INTERFACE_H__
