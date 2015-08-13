//-------------------------------------------
// author : alterhz
// date : 6/17/2015
// description : 

#include <iostream>
#include "app.h"
#include "debug.h"
#include "thread.h"

int main(int argc, char* argv[])
{
	CApp::getMe().Run();

	LOGPrint("Have run.");

	while (true)
	{
		char szIn[1024] = {0};
		std::cin >> szIn;

		if (std::string("exit") == szIn)
		{
			// 停止
			CApp::getMe().Stop();
		}
	}

	return 0;
}



#include <tchar.h>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <map>

using namespace std;

//namespace Observer2
//{
//	template<typename P1, typename P2>
//	class IRunCallBack
//	{
//	public:
//		virtual void OnInit() = 0;
//		virtual void DoExcute(int nProtoId, P1 p1, P2 p2) = 0;
//	};
//
//	template<typename P1, typename P2>
//	class IDispatcherManager
//	{
//	public:
//		bool AddDispatcher(IRunCallBack<P1, P2> *p)
//		{
//			if (nullptr == p)
//			{
//				return false;
//			}
//
//			p->OnInit();
//			m_vtRunCallBack.push_back(p);
//
//			return true;
//		}
//
//		void RunExcute(int nProtoId, P1 p1, P2 p2)
//		{
//			for (IRunCallBack<P1, P2> *pRunCallBack : m_vtRunCallBack)
//			{
//				pRunCallBack->DoExcute(nProtoId, p1, p2);
//			}
//		}
//
//	private:
//		std::vector<IRunCallBack<P1, P2> *> m_vtRunCallBack;
//	};
//
//	template<typename T, typename P1, typename P2>
//	class IDispatcher : public IRunCallBack<P1, P2>
//	{
//		typedef void (T::*CallBack)(P1, P2);
//		struct Delegate
//		{
//			T* pObj;
//			CallBack callback;
//		};
//
//		typedef std::map<int, Delegate> MapDelegate;
//
//	public:
//		void RegMessage(int nProtoId, CallBack pCallBack, T* pObj)
//		{
//			Delegate dg;
//			dg.callback = pCallBack;
//			dg.pObj = pObj;
//
//			m_mapHandler.insert(std::make_pair(nProtoId, dg));
//		}
//
//	private:
//		virtual void DoExcute(int nProtoId, P1 p1, P2 p2)
//		{
//			MapDelegate::iterator itDelegate = m_mapHandler.find(nProtoId);
//			if (itDelegate != m_mapHandler.end())
//			{
//				Delegate dg = itDelegate->second;
//
//				((*dg.pObj).*dg.callback)(p1, p2);
//			}
//		}
//
//	private:
//		MapDelegate m_mapHandler;
//	};
//
//}
//
//class CLoginDispatch : public Observer2::IDispatcher<CLoginDispatch, int, int>
//{
//public:
//	virtual void OnInit()
//	{
//		RegMessage(1001, &CLoginDispatch::func, this);
//		RegMessage(1002, &CLoginDispatch::OnHeartBeat, this); 
//		RegMessage(1003, &CLoginDispatch::OnJoinTeam, this);
//	}
//
//	void func(int x, int y)
//	{
//		std::cout << "func:" << x << y << std::endl;
//	}
//
//	void OnHeartBeat(int x, int y)
//	{
//		std::cout << "heartbeat:" << x << y << std::endl;
//	}
//
//	void OnJoinTeam(int x, int y)
//	{
//		std::cout << "join team" << std::endl;
//	}
//};
//
//class CGameDispatch : public Observer2::IDispatcher<CGameDispatch, int, int>
//{
//public:
//	virtual void OnInit()
//	{
//		RegMessage(1001, &CGameDispatch::func2, this);
//		RegMessage(1002, &CGameDispatch::OnHeartBeat2, this);
//	}
//
//	void func2(int x, int y)
//	{
//		std::cout << "B::func:" << x << y << std::endl;
//	}
//
//	void OnHeartBeat2(int x, int y)
//	{
//		std::cout << "B::heartbeat:" << x << y << std::endl;
//	}
//};

//#include "observer.h"
//#include "logindispatcher.h"
//
//int main()
//{
//	Observer2::IDispatcherManager<const char *, int> gDM;
//
//	CLoginDispatcher loginDispatcher;
//
//	gDM.AddDispatcher(&loginDispatcher);
//
//	
//	// 接收到消息，并分发
//	char *p = "hello world";
//	gDM.RunExcute(1001, p, 3);
//
//	return 0;
//}

//template<typename Handler>
//class I
//{
//	struct F2
//	{
//		Handler h;
//		I* o;
//	};
//public:
//	void Add(Handler h, I* o)
//	{
//		F2 f2;
//		f2.h = h;
//		f2.o = o;
//		m_vtHandler.push_back(f2);
//	}
//
//	void Run(int x, int y)
//	{
//		for (F2 h : m_vtHandler)
//		{
//			((*h.o).*h.h)(x, y);
//		}
//	}
//
//	std::vector<F2> m_vtHandler;
//};
//
//class A : public I<void (A::*)(int, int)>
//{
//public:
//
//	void Init()
//	{
//		Add(&A::func, this);
//	}
//
//	void func(int x, int y)
//	{
//		std::cout << x << y << std::endl;
//	}
//};
