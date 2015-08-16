
#ifndef _ACTION_H_
#define _ACTION_H_

#include <vector>

struct Vector2
{
	int x;
	int y;
	bool valid;

	Vector2()
		: valid(false)
	{ }
	Vector2(int _x, int _y)
		: x(_x)
		, y(_y) 
		, valid(true)
	{ }
};

typedef std::vector<Vector2> VtPath;
typedef unsigned int MSTIME;

enum EActionType
{
	EActionType_Null = 0,
	EActionType_Stand,
	EActionType_Run,
	EActionType_Dead,
};

class IAction
{
public:
	virtual ~IAction() {}

public:
	virtual Vector2 GetCurrentPosition(MSTIME msNow) const = 0;
	virtual EActionType GetCurrentActionType() const { return EActionType_Null; }
};

class CStandAction : public IAction
{
public:
	CStandAction(Vector2 &v2) 
		: m_v2CurrentPosition(v2)
	{}
	~CStandAction() {}

public:
	virtual Vector2 GetCurrentPosition(MSTIME msNow) const;
	virtual EActionType GetCurrentActionType() const { return EActionType_Stand; }

private:
	Vector2 m_v2CurrentPosition;
};

class CRunAction : public IAction
{
public:
	CRunAction(VtPath &vtPath, double fRunSpeed, MSTIME msNow);
	~CRunAction() {}

public:
	virtual Vector2 GetCurrentPosition(MSTIME msNow) const;
	virtual EActionType GetCurrentActionType() const { return EActionType_Run; }

private:
	VtPath m_vtRunPath;	//行走路径
	MSTIME m_msTimeBegin;	//开始时间
	double m_fRunSpeed;	//行走速度
};

class CDeadAction : public IAction
{
public:
	CDeadAction(Vector2 &v2) 
		: m_v2CurrentPosition(v2)
	{}
	~CDeadAction() {}

public:
	virtual Vector2 GetCurrentPosition(MSTIME msNow) const;
	virtual EActionType GetCurrentActionType() const { return EActionType_Dead; }

private:
	Vector2 m_v2CurrentPosition;
};


#endif