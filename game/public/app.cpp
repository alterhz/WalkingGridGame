#include "app.h"

#include "debug.h"
#include "clientmanager.h"
#include "testclient.h"
#include "configread.h"

#include "memoryleakinfo.h"

CApp::CApp()
	: m_pNetService(nullptr)
	, m_pNetAcceptor(nullptr)
	, m_pTimerManager(nullptr)
	, m_pEventManager(nullptr)
	, m_nRunTimerId(0)
{
	m_pNetService = CreateNetService();
}

CApp::~CApp()
{
	DestoryNetService(m_pNetService);
	m_pNetService = nullptr;
}

bool CApp::Run()
{
	return CreateThread(*this);
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
	if (m_pTimerManager)
	{
		return m_pTimerManager->SetTimer(pTimerEvent, nInterval);
	}

	return INVALID_TIMER_ID;
}

void CApp::KillTimer(int nTimerId)
{
	if (m_pTimerManager)
	{
		return m_pTimerManager->KillTimer(nTimerId);
	}
}

bool CApp::InitNet()
{
	if (nullptr == m_pNetService)
	{
		return false;
	}

	m_pTimerManager = m_pNetService->CreateTimerManager();

	if (nullptr == m_pTimerManager)
	{
		return false;
	}

	m_nRunTimerId = m_pTimerManager->SetTimer(this, 5000);

	m_pNetAcceptor = m_pNetService->CreateListener();
	if (nullptr == m_pNetAcceptor)
	{
		return false;
	}

	// 初始化协议
	CClientManager::getMe().InitProto();

	unsigned short wPort = 8000;
	m_pNetAcceptor->Listen(CClientManager::instance(), wPort);

	LOGPrint("正在网络监听" + wPort + "端口。");

	{
		// Test客户端连接
		INetConnector *pNetConnector = m_pNetService->CreateConnector();
		if (pNetConnector)
		{
			CTestClient::getMe().Init(pNetConnector);
		}
	}

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

bool CApp::OnInit()
{
	new char[333];

	CConfigReadManager::getMe().LoadConfigData();

	InitDb();

	InitNet();

	return true;
}

bool CApp::OnThreadRun()
{
	if (nullptr == m_pNetService)
	{
		return true;
	}

	if (!OnInit())
	{
		return false;
	}

	m_pNetService->Run();

	LOGPrint("主逻辑线程停止。");

	exit(0);

	return true;
}

bool CApp::OnTimerEvent(int nTimerId)
{
	if (nTimerId == m_nRunTimerId)
	{
		LOGPrint("\n------------------- 内存泄露信息 ---------------------");
		// 打印内存泄露情况
		VtMemoryLeak vtMemoryLeak = GetMemoryLeak();
		for (InfoMemoryLeak infoMemoryLeak : vtMemoryLeak)
		{
			LOGPrint("onlyIndex:" + infoMemoryLeak.nOnlyIndex + ",leakmemory:" + infoMemoryLeak.nLeakMemory
				+ "[" + infoMemoryLeak.strFileName.c_str() + "-" + infoMemoryLeak.nFileLine + "]");
		}
		LOGPrint("-----------------------------------------------------\n");
	}

	//static int n = 0;
	//LOGPrint("Timer[" + nTimerId + "]触发" + (++n) + "次");

	//if (n >= 5)
	//{
	//	m_pTimerManager->KillTimer(m_nRunTimerId);
	//	m_nRunTimerId = 0;
	//}

	return true;
}

void CApp::Stop()
{
	delete m_pNetAcceptor;
	m_pNetAcceptor = nullptr;

	delete m_pTimerManager;
	m_pTimerManager = nullptr;

	if (m_pEventManager)
	{
		m_pEventManager->Stop();
	}

	delete m_pEventManager;
	m_pEventManager = nullptr;

	if (m_pNetService)
	{
		m_pNetService->Stop();
	}
}

