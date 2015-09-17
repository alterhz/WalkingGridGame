//-------------------------------------------
// author : alterhz
// date : 2015-9-17
// description : 

#ifndef _MEMORYLEAK_H_
#define _MEMORYLEAK_H_

#include "memoryoperatornew.h"

//#undef new
#define new new(__FILE__, __LINE__)

#endif
