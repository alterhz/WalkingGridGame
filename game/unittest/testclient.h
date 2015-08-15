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

class CTestClient : public INetClient, public Singleton<CTestClient>
{
public:
	CTestClient();
	~CTestClient();

	bool Init(INetConnector *pNetConnector);

	bool SendMessage(unsigned short wProtocolId, google::protobuf::Message *pMessage);

	void Disconnect();

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
	INetConnector *m_pNetConnector;
	INetSocket *m_pNetSocket;
};


#endif