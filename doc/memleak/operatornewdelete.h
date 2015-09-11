//-------------------------------------------
// author : alterhz
// date : 9/11/2015
// description : 

#ifndef _OPERATOR_NEW_DELETE_H_
#define _OPERATOR_NEW_DELETE_H_

void * operator new(unsigned int size, const char *file, int line);

void operator delete(void *p);

void operator delete [](void *p);

void operator delete(void *p, const char *file, int line);

void operator delete [](void *p, const char *file, int line);


#endif