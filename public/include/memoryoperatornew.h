//-------------------------------------------
// author : alterhz
// date : 2015-9-17
// description : 

#ifndef _MEMORYOPERATORNEW_H_
#define _MEMORYOPERATORNEW_H_

#include <new>

void * operator new(std::size_t size, const char *file, int line);

void * operator new[](std::size_t size, const char *file, int line);

void operator delete(void *p, const char *file, int line);

void operator delete [](void *p, const char *file, int line);

#ifdef WIN32

void operator delete( void *ptr );

void operator delete[]( void *ptr );

#else

void operator delete( void *ptr ) noexcept;

void operator delete[]( void *ptr ) noexcept;

#endif // WIN32

#endif