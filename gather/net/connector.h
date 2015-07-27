//-------------------------------------------
// author : alterhz
// date : 6/18/2015
// description : 

#ifndef _CONNECTOR_H_
#define _CONNECTOR_H_

#include "netio.h"
#include "msginc.h"
#include "event.h"
using namespace NS_IO;

class CConnectorTest : public INetClient, ITimerEvent
{
public:
	CConnectorTest();
	virtual ~CConnectorTest();

	void Init(INetConnector *pNetConnector);

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
	virtual bool OnError(int nErrCode) { return true; }

	// 返回true:继续；返回false:终止
	virtual bool OnTimerEvent(int nTimerId);
	// 删除
	virtual void OnKilled(int nTimerId);

private:
	INetConnector *m_pNetConnector;
	INetSocket *m_pNetSocket;
	int m_nTimerId;
};

#endif