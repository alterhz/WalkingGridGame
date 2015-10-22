// 战斗英雄

#ifndef _HERO_H_
#define _HERO_H_


class IFightObj
{
public:
	IFightObj();
	virtual ~IFightObj();

public:
	virtual void Walk() = 0;

private:
	int m_nIndexId;
};

// 小兵
class CDogFace : public IFightObj
{
public:
	CDogFace();
	~CDogFace();

};

// 英雄
class CHero : public IFightObj
{
public:
	CHero();
	~CHero();

public:

};

// 将领（司令官）
class CSirdar : public IFightObj
{
public:
	CSirdar();
	~CSirdar();


};



#endif