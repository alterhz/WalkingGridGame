#include "app.h"

#include "debug.h"
#include "clientmanager.h"
#include "connector.h"

CApp::CApp()
	: m_pNetService(nullptr)
	, m_pEventManager(nullptr)
{
	m_pNetService = CreateNetService();
}

CApp::~CApp()
{
	DestoryNetService(m_pNetService);
	m_pNetService = nullptr;
}

bool CApp::InitNet()
{
	if (nullptr == m_pNetService)
	{
		return false;
	}

	INetAcceptor *pNetAcceptor = m_pNetService->CreateListener();
	if (nullptr == pNetAcceptor)
	{
		return false;
	}

	CClientManager *pClientManager = new CClientManager();
	if (nullptr == pClientManager)
	{
		return false;
	}

	unsigned short wPort = 8000;

	pNetAcceptor->Listen(pClientManager, wPort);

	LOGPrint("正在网络监听" + wPort + "端口。");

	INetConnector *pNetConnector = m_pNetService->CreateConnector();
	if (nullptr == pNetConnector)
	{
		return false;
	}

	CConnectorTest *pConnectorTest = new CConnectorTest();
	if (nullptr == pConnectorTest)
	{
		return false;
	}

	pConnectorTest->Init(pNetConnector);

	return true;
}

bool CApp::InitDb()
{
	if (nullptr == m_pNetService)
	{
		return false;
	}

	IEventManager *pEventManager = m_pNetService->CreateEventManager(6);

	if (nullptr == pEventManager)
	{
		return false;
	}

	m_pEventManager = pEventManager;

	return true;
}

bool CApp::DoTick()
{
	if (m_pNetService)
	{
		m_pNetService->Run();
	}

	return true;
}

void CApp::Stop()
{
	delete m_pEventManager;
	m_pEventManager = nullptr;

	if (m_pNetService)
	{
		// 关闭
	}
}

bool CApp::PostAsyncEvent(IAsyncEvent *pAsyncEvent)
{
	if (m_pEventManager)
	{
		return m_pEventManager->PostAsyncEvent(pAsyncEvent);
	}

	return false;
}

int CApp::SetTimer(ITimerEvent *pTimerEvent, int nInterval)
{
	if (m_pEventManager)
	{
		return m_pEventManager->SetTimer(pTimerEvent, nInterval);
	}

	return INVALID_TIMER_ID;
}

void CApp::KillTimer(int nTimerId)
{
	if (m_pEventManager)
	{
		return m_pEventManager->KillTimer(nTimerId);
	}
}

