#include "action.h"


Vector2 CStandAction::GetCurrentPosition(MSTIME msNow) const
{
	return m_v2CurrentPosition;
}


CRunAction::CRunAction(VtPath &vtPath, double fRunSpeed, MSTIME msNow) 
	: m_vtRunPath(vtPath) 
	, m_fRunSpeed(fRunSpeed)
	, m_msTimeBegin(msNow)
{
}

Vector2 CRunAction::GetCurrentPosition(MSTIME msNow) const
{
	int nPassTime = msNow - m_msTimeBegin;
	
	double fRunDistance = m_fRunSpeed * (nPassTime / 1000.0f);

	Vector2 v2CurrentPosition;

	return v2CurrentPosition;
}

Vector2 CDeadAction::GetCurrentPosition( MSTIME msNow ) const
{
	return m_v2CurrentPosition;
}
