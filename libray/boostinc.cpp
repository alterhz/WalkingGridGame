#include "boostinc.h"

#include <libs/system/src/error_code.cpp>

// thread
#ifdef _MSC_VER
namespace boost { void tss_cleanup_implemented(void) {}} 
#include <libs/thread/src/win32/thread.cpp>
#include <libs/thread/src/win32/tss_dll.cpp>
#include <libs/thread/src/win32/tss_pe.cpp>
#else
#include <libs/thread/src/pthread/thread.cpp>
#include <libs/thread/src/pthread/once.cpp>
#endif

#include <libs/locale/src/encoding/conv.hpp>
#include <libs/locale/src/encoding/codepage.cpp>

#include <libs/filesystem/src/path.cpp>

#include <libs/date_time/src/gregorian/date_generators.cpp>
#include <libs/date_time/src/gregorian/greg_month.cpp>
#include <libs/date_time/src/gregorian/greg_names.hpp>
#include <libs/date_time/src/gregorian/greg_weekday.cpp>
#include <libs/date_time/src/gregorian/gregorian_types.cpp>
