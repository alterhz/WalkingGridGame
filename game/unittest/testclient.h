//-------------------------------------------
// author : alterhz
// date : 8/15/2015
// description : 

#ifndef _TESTCLIENT_H_
#define _TESTCLIENT_H_

#include "netio.h"
#include "msginc.h"
#include "utilityinc.h"
using namespace NS_IO;

#include <ctime>

class CTestClient : public INetClient
{
public:
	CTestClient();
	~CTestClient();

	bool Init(INetConnector *pNetConnector);

	bool SendMessage(unsigned short wProtocolId, google::protobuf::Message *pMessage);

	void Disconnect();

public:
	// 准备
	bool SendPrepare();
	// 进入场景
	bool SendEnterGround();
	// 获取场景数据
	bool SendGetGroundInfo();
	// 准备完毕
	bool SendPrepareFinish();
	// 攻击完毕
	bool SendBattleFightEnd();

protected:
	// 网络连接事件
	virtual bool OnConnected(INetSocket *pNetSocket);
	// 网络断开事件(此函数调用后INetSocket对象销毁)
	virtual bool OnDisconnect();
	// 接收数据
	virtual bool OnRecvPacket(const char *pPacket, unsigned short wLength);
	// 错误异常
	virtual bool OnError(int nErrCode);


private:
	int m_nId;
	INetConnector *m_pNetConnector;
	INetSocket *m_pNetSocket;

	std::clock_t m_tBegin;

	int m_nCountryIndexId;
};

class CTestClientManager : public Singleton<CTestClientManager>
{
public:
	CTestClientManager();
	~CTestClientManager();

public:
	void Init(NS_IO::INetService *pNetService);


private:
	CTestClient *m_pTestClientA;
	CTestClient *m_pTestClientB;
};


#endif