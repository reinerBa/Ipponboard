#include "StateMachine.h"
#include "Controller.h"
#include "RuleSet.h"

#include <QTimer>

using namespace Ipponboard;

//---------------------------------------------------------
void IpponboardSM_::add_point(HoldTimeEvent const& evt)
//---------------------------------------------------------
{
	if (m_pCore->is_auto_adjust() && evt.secs > 0)
	{
		if (CurrentRules().OsaekomiValue_Yuko == evt.secs)
		{
			m_pCore->CurrentMatch().AddPoint(evt.who, Point::Yuko);
		}
		else if (CurrentRules().OsaekomiValue_Wazaari == evt.secs)
		{
			m_pCore->CurrentMatch().RemovePoint(evt.who, Point::Yuko);
			m_pCore->CurrentMatch().AddPoint(evt.who, Point::Wazaari);
		}
		else if (CurrentRules().OsaekomiValue_Ippon == evt.secs)
		{
			m_pCore->CurrentMatch().RemovePoint(evt.who, Point::Wazaari);
			m_pCore->CurrentMatch().AddPoint(evt.who, Point::Ippon);
		}
	}
}

//---------------------------------------------------------
bool IpponboardSM_::wazaari_is_match_point(Wazaari const& evt)
//---------------------------------------------------------
{
	return CurrentMatch().IsAlmostAwaseteIppon(evt.who);
}

//---------------------------------------------------------
bool IpponboardSM_::has_IpponTime(HoldTimeEvent const& evt)
//---------------------------------------------------------
{
	return evt.secs > 0 && CurrentRules().OsaekomiValue_Ippon == evt.secs;
}

//---------------------------------------------------------
bool IpponboardSM_::has_WazaariTime(HoldTimeEvent const& evt)
//---------------------------------------------------------
{
	return evt.secs > 0 && CurrentRules().OsaekomiValue_Wazaari == evt.secs;
}

//---------------------------------------------------------
bool IpponboardSM_::has_AwaseteTime(HoldTimeEvent const& evt)
//---------------------------------------------------------
{
	if (evt.secs > 0
		&& CurrentRules().AwaseteIppon
		&& CurrentMatch().IsAlmostAwaseteIppon(evt.who))
	{
		return CurrentRules().OsaekomiValue_Wazaari == evt.secs;
	}

	return false;
}

//---------------------------------------------------------
bool IpponboardSM_::has_YukoTime(HoldTimeEvent const& evt)
//---------------------------------------------------------
{
	return evt.secs > 0 && CurrentRules().OsaekomiValue_Yuko == evt.secs;
}

//---------------------------------------------------------
bool IpponboardSM_::is_sonomama(Osaekomi_Toketa const& /*evt*/)
//---------------------------------------------------------
{
	return m_pCore->is_sonomama();
}

//---------------------------------------------------------
bool IpponboardSM_::shido_is_match_point(Shido const& evt)
//---------------------------------------------------------
{
	if (CurrentMatch().IsShidoMatchPoint(evt.who))
	{
		return true;
	}

	// Note: new 2013 IJF rule: no points for first three shido
//    if (m_pCore->GetRules()->IsShidosCountAsPoints())
//	{
//		FighterEnum uke = GetUkeFromTori(evt.tori);

//        if (Score_(evt.tori).Shido() == 2 && Score_(uke).Wazaari() == 1)
//		{
//			return true;
//		}
//	}

	return false;
}

const RuleSet &IpponboardSM_::CurrentRules() const
{
	return m_pCore->CurrentMatch().GetRuleSet();
}

Fight &IpponboardSM_::CurrentMatch()
{
	return m_pCore->CurrentMatch();
}

void IpponboardSM_::reset(Reset const& /*evt*/)
{
	m_pCore->reset_fight();
}

void IpponboardSM_::save(Finish const& /*evt*/)
{
	m_pCore->save_fight();
}

void IpponboardSM_::stop_timer(Osaekomi_Toketa const& /*evt*/)
{
	m_pCore->stop_timer(eTimer_Hold);
}

void IpponboardSM_::stop_timer(TimeEndedEvent const& /*evt*/)
{
	m_pCore->stop_timer(eTimer_Main);
}

void IpponboardSM_::stop_timer(Finish const& /*evt*/)
{
	// Finish will be created if current fight should be saved
	// --> stop timers
	m_pCore->stop_timer(eTimer_Hold);
	m_pCore->stop_timer(eTimer_Main);   // will save main time
	m_pCore->save_fight();
}

void IpponboardSM_::stop_timer(Hajime_Mate const& /*evt*/)
{
	m_pCore->stop_timer(ETimer(Hajime_Mate::type));
}

void IpponboardSM_::start_timer(Hajime_Mate const& /*evt*/)
{
	m_pCore->reset_timer(eTimer_Hold);
	m_pCore->start_timer(eTimer_Main);
}

void IpponboardSM_::start_timer(Osaekomi_Toketa const& /*evt*/)
{
	//m_pCore->reset_timer( eTimer_Hold );
	m_pCore->start_timer(eTimer_Hold);
}

void IpponboardSM_::add_point(PointEvent<ippon_type> const& evt)
{
	CurrentMatch().AddPoint(evt.who, Point::Ippon);
	m_pCore->stop_timer(eTimer_Main);
	m_pCore->stop_timer(eTimer_Hold);
}

void IpponboardSM_::add_point(PointEvent<shido_type> const& evt)
{
	CurrentMatch().AddPoint(evt.who, Point::Shido);
}

void IpponboardSM_::add_point(PointEvent<hansokumake_type> const& evt)
{
	CurrentMatch().AddPoint(evt.who, Point::Hansokumake);

	m_pCore->stop_timer(eTimer_Main);
	m_pCore->stop_timer(eTimer_Hold);
}

void IpponboardSM_::yoshi(Osaekomi_Toketa const& /*evt*/)
{
	m_pCore->start_timer(eTimer_Main);
	m_pCore->start_timer(eTimer_Hold);
}
