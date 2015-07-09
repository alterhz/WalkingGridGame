#ifndef _BOOST_HEADER_
#define _BOOST_HEADER_

#define BOOST_ALL_NO_LIB
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/locale.hpp>
#include <boost/locale/encoding.hpp>
#include <boost/timer.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include <iostream>
#include <list>
#include <map>
#include <vector>
#include <math.h>

#ifndef NS_IO

#define NS_IO		dgs
#define NS_IO_Header	namespace dgs {
#define NS_IO_Footer	}

#endif	//NS_IO

NS_IO_Header

#define MAX_RECV_BUFFER_LENGTH		(0x2000)
#define MAX_SEND_BUFFER_LENGTH		(0x2000)

typedef unsigned short USHORT;

class INetSocket;

class IParent
{
public:
	virtual ~IParent() {}

public:
	virtual void OnRelease(INetSocket *pNetSocket) { }
};

NS_IO_Footer

// end
#endif
