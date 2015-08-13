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
		typedef void (T::*CallBack)(P1, P2);
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

				((*dg.pObj).*dg.callback)(p1, p2);
			}
		}

	private:
		MapDelegate m_mapHandler;
	};
}

#endif // _OBSERVER_H_
