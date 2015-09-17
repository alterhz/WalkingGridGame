//-------------------------------------------
// author : alterhz
// date : 9/9/2015
// description : 

#ifndef _ZONE_H_
#define _ZONE_H_

#include "memoryleak.h"
#include <map>

class CZone
{
public:
	CZone();
	~CZone();

public:

private:

};


class CZoneManager
{
	typedef std::map<int, CZone> MapZone;
public:
	CZoneManager();
	~CZoneManager();

public:
	bool Init();

private:
	MapZone m_mapZone;
};

#endif