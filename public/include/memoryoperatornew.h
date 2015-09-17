//-------------------------------------------
// author : alterhz
// date : 2015-9-17
// description : 

#ifndef _MEMORYOPERATORNEW_H_
#define _MEMORYOPERATORNEW_H_

void * operator new(unsigned int size, const char *file, int line);

void operator delete(void *p);

void operator delete [](void *p);

void operator delete(void *p, const char *file, int line);

void operator delete [](void *p, const char *file, int line);

#endif