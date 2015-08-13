//-------------------------------------------
// author : alterhz
// date : 8/13/2015
// description : 

#ifndef _OBSERVER_H_
#define _OBSERVER_H_

#include <vector>
#include <map>

namespace Observer2
{
	template<typename P1, typename P2>
	class IRunCallBack
	{
	public:
		virtual void OnInit() = 0;
		virtual void DoExcute(int nProtoId, P1 p1, P2 p2) = 0;
	};

	template<typename P1, typename P2>
	class IDispatcherManager
	{
	public:
		~IDispatcherManager()
		{
			for (IRunCallBack<P1, P2> *pDel : m_vtRunCallBack)
			{
				delete pDel;
			}
			m_vtRunCallBack.clear();
		}

		bool AddDispatcher(IRunCallBack<P1, P2> *p)
		{
			if (nullptr == p)
			{
				return false;
			}

			p->OnInit();
			m_vtRunCallBack.push_back(p);

			return true;
		}

		void RunExcute(int nProtoId, P1 p1, P2 p2)
		{
			for (IRunCallBack<P1, P2> *pRunCallBack : m_vtRunCallBack)
			{
				pRunCallBack->DoExcute(nProtoId, p1, p2);
			}
		}

	private:
		std::vector<IRunCallBack<P1, P2> *> m_vtRunCallBack;
	};

	template<typename T, typename P1, typename P2>
	class IDispatcher : public IRunCallBack<P1, P2>
	{
		typedef void (T::*CallBack)(int, P1, P2);
		struct Delegate
		{
			T* pObj;
			CallBack callback;
		};

		typedef std::map<int, Delegate> MapDelegate;

	public:
		void RegMessage(int nProtoId, CallBack pCallBack, T* pObj)
		{
			Delegate dg;
			dg.callback = pCallBack;
			dg.pObj = pObj;

			m_mapHandler.insert(std::make_pair(nProtoId, dg));
		}

	private:
		virtual void DoExcute(int nProtoId, P1 p1, P2 p2)
		{
			MapDelegate::iterator itDelegate = m_mapHandler.find(nProtoId);
			if (itDelegate != m_mapHandler.end())
			{
				Delegate dg = itDelegate->second;

				((*dg.pObj).*dg.callback)(nProtoId, p1, p2);
			}
		}

	private:
		MapDelegate m_mapHandler;
	};
}

namespace Observer3
{
	template<typename P1, typename P2, typename P3>
	class IRunCallBack
	{
	public:
		virtual void OnInit() = 0;
		virtual void DoExcute(int nProtoId, P1 p1, P2 p2, P3 p3) = 0;
	};

	template<typename P1, typename P2, typename P3>
	class IDispatcherManager
	{
	public:
		~IDispatcherManager()
		{
			for (IRunCallBack<P1, P2, P3> *pDel : m_vtRunCallBack)
			{
				delete pDel;
			}
			m_vtRunCallBack.clear();
		}

		bool AddDispatcher(IRunCallBack<P1, P2, P3> *p)
		{
			if (nullptr == p)
			{
				return false;
			}

			p->OnInit();
			m_vtRunCallBack.push_back(p);

			return true;
		}

		void RunExcute(int nProtoId, P1 p1, P2 p2, P3 p3)
		{
			for (IRunCallBack<P1, P2, P3> *pRunCallBack : m_vtRunCallBack)
			{
				pRunCallBack->DoExcute(nProtoId, p1, p2, p3);
			}
		}

	private:
		std::vector<IRunCallBack<P1, P2, P3> *> m_vtRunCallBack;
	};

	template<typename T, typename P1, typename P2, typename P3>
	class IDispatcher : public IRunCallBack<P1, P2, P3>
	{
		typedef void (T::*CallBack)(int, P1, P2, P3);
		struct Delegate
		{
			T* pObj;
			CallBack callback;
		};

		typedef std::map<int, Delegate> MapDelegate;

	public:
		void RegMessage(int nProtoId, CallBack pCallBack, T* pObj)
		{
			Delegate dg;
			dg.callback = pCallBack;
			dg.pObj = pObj;

			m_mapHandler.insert(std::make_pair(nProtoId, dg));
		}

	private:
		virtual void DoExcute(int nProtoId, P1 p1, P2 p2, P3 p3)
		{
			MapDelegate::iterator itDelegate = m_mapHandler.find(nProtoId);
			if (itDelegate != m_mapHandler.end())
			{
				Delegate dg = itDelegate->second;

				((*dg.pObj).*dg.callback)(nProtoId, p1, p2, p3);
			}
		}

	private:
		MapDelegate m_mapHandler;
	};
}

#endif // _OBSERVER_H_
