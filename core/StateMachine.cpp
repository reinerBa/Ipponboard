﻿// Copyright 2018 Florian Muecke. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE.txt file.

#include "../util/debug.h"
#include "StateMachine.h"
#include "Controller.h"

#include <QTimer>

using namespace Ipponboard;
using Point = Score::Point;

//---------------------------------------------------------
void IpponboardSM_::add_point(HoldTimeEvent const& evt)
//---------------------------------------------------------
{
    TRACE(2, "IpponboardSM_::add_point()");
    auto pRules = m_pCore->GetRules();

	if (m_pCore->is_auto_adjust())
	{

		if (pRules->GetOsaekomiValue(Point::Yuko) == evt.secs)
		{
			Score_(evt.tori).Add(Point::Yuko);
		}
		else if (pRules->GetOsaekomiValue(Point::Wazaari) == evt.secs)
		{
			Score_(evt.tori).Remove(Point::Yuko);
			Score_(evt.tori).Add(Point::Wazaari);
		}
		else if (pRules->GetOsaekomiValue(Point::Ippon) == evt.secs)
		{
			Score_(evt.tori).Remove(Point::Wazaari);
			Score_(evt.tori).Add(Point::Ippon);
		}
	}
}

//---------------------------------------------------------
bool IpponboardSM_::can_add_wazaari(Wazaari const& evt)
//---------------------------------------------------------
{
    TRACE(2, "IpponboardSM_::can_add_wazaari()");
    return m_pCore->GetRules()->IsOption_AwaseteIppon() ||
		   Score_(evt.tori).Wazaari() < m_pCore->GetRules()->GetMaxWazaariCount();
}

//---------------------------------------------------------
bool IpponboardSM_::wazaari_is_match_point(Wazaari const& evt)
//---------------------------------------------------------
{
    TRACE(2, "IpponboardSM_::wazaari_is_match_point()");
    return m_pCore->GetRules()->IsOption_AwaseteIppon()
		   && Score_(evt.tori).Wazaari() == m_pCore->GetRules()->GetMaxWazaariCount() - 1;
}

//---------------------------------------------------------
bool IpponboardSM_::has_max_wazaari(RevokeWazaari const& evt)
//---------------------------------------------------------
{
    TRACE(2, "IpponboardSM_::has_max_wazaari()");
    return m_pCore->GetRules()->IsAwaseteIppon(Score_(evt.tori));
}

//---------------------------------------------------------
bool IpponboardSM_::has_IpponTime(HoldTimeEvent const& evt)
//---------------------------------------------------------
{
    TRACE(2, "IpponboardSM_::has_IpponTime()");
    return m_pCore->GetRules()->GetOsaekomiValue(Point::Ippon) == evt.secs;
}

//---------------------------------------------------------
bool IpponboardSM_::has_WazaariTime(HoldTimeEvent const& evt)
//---------------------------------------------------------
{
    TRACE(2, "IpponboardSM_::has_WazaariTime()");
    return m_pCore->GetRules()->GetOsaekomiValue(Point::Wazaari) == evt.secs;
}

//---------------------------------------------------------
bool IpponboardSM_::has_AwaseteTime(HoldTimeEvent const& evt)
//---------------------------------------------------------
{
    TRACE(2, "IpponboardSM_::has_AwaseteTime()");
    if (m_pCore->GetRules()->IsOption_AwaseteIppon() && 0 != Score_(evt.tori).Wazaari())
	{
		return m_pCore->GetRules()->GetOsaekomiValue(Point::Wazaari) == evt.secs;
	}

	return false;
}

//---------------------------------------------------------
bool IpponboardSM_::has_YukoTime(HoldTimeEvent const& evt)
//---------------------------------------------------------
{
    TRACE(2, "IpponboardSM_::has_YukoTime()");
    return m_pCore->GetRules()->GetOsaekomiValue(Point::Yuko) == evt.secs;
}

//---------------------------------------------------------
bool IpponboardSM_::is_sonomama(Osaekomi_Toketa const& /*evt*/)
//---------------------------------------------------------
{
    TRACE(2, "IpponboardSM_::is_sonomama()");
    return m_pCore->is_sonomama();
}

//---------------------------------------------------------
bool IpponboardSM_::shido_is_match_point(Shido const& evt)
//---------------------------------------------------------
{
    TRACE(2, "IpponboardSM_::shido_is_match_point()");
    if (Score_(evt.tori).Shido() == m_pCore->GetRules()->GetMaxShidoCount())
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

//---------------------------------------------------------
bool IpponboardSM_::can_take_shido(Shido const& evt)
//---------------------------------------------------------
{
    TRACE(2, "IpponboardSM_::can_take_shido()");
    return Score_(evt.tori).Shido() <= m_pCore->GetRules()->GetMaxShidoCount();
}

void IpponboardSM_::reset(Reset const& /*evt*/)
{
    TRACE(2, "IpponboardSM_::reset()");
    m_pCore->reset_fight();
}

void IpponboardSM_::save(Finish const& /*evt*/)
{
    TRACE(2, "IpponboardSM_::save()");
    m_pCore->save_fight();
}

void IpponboardSM_::stop_timer(Osaekomi_Toketa const& /*evt*/)
{
    TRACE(2, "IpponboardSM_::stop_timer()");
    m_pCore->stop_timer(eTimer_Hold);
}

void IpponboardSM_::stop_timer(TimeEndedEvent const& /*evt*/)
{
    TRACE(2, "IpponboardSM_::stop_timer()");
    m_pCore->stop_timer(eTimer_Main);
}

void IpponboardSM_::stop_timer(Finish const& /*evt*/)
{
    TRACE(2, "IpponboardSM_::stop_timer()");
    // Finish will be created if current fight should be saved
	// --> stop timers
	m_pCore->stop_timer(eTimer_Hold);
	m_pCore->stop_timer(eTimer_Main);   // will save main time
	m_pCore->save_fight();
}

void IpponboardSM_::stop_timer(Hajime_Mate const& /*evt*/)
{
    TRACE(2, "IpponboardSM_::stop_timer()");
    m_pCore->stop_timer(ETimer(Hajime_Mate::type));
}

void IpponboardSM_::start_timer(Hajime_Mate const& /*evt*/)
{
    TRACE(2, "IpponboardSM_::start_timer()");
    m_pCore->reset_timer(eTimer_Hold);
	m_pCore->start_timer(eTimer_Main);
}

void IpponboardSM_::start_timer(Osaekomi_Toketa const& /*evt*/)
{
    TRACE(2, "IpponboardSM_::start_timer()");
    //m_pCore->reset_timer( eTimer_Hold );
	m_pCore->start_timer(eTimer_Hold);
}

void IpponboardSM_::add_point(PointEvent<ippon_type> const& evt)
{
    TRACE(2, "IpponboardSM_::add_point()");
    Score_(evt.tori).Add(Point::Ippon);
	m_pCore->stop_timer(eTimer_Main);
	m_pCore->stop_timer(eTimer_Hold);
}

void IpponboardSM_::add_point(PointEvent<shido_type> const& evt)
{
    TRACE(2, "IpponboardSM_::add_point()");
    auto pRules = m_pCore->GetRules();

	if (m_pCore->is_auto_adjust())
	{
		FighterEnum uke = GetUkeFromTori(evt.tori);

		auto maxShidoCount = pRules->GetMaxShidoCount();

		if (maxShidoCount == Score_(evt.tori).Shido())
		{
			Score_(uke).Add(Point::Ippon);
		}
		else
		{
			if (pRules->IsOption_ShidoAddsPoint())
			{
				if (maxShidoCount > 2 && 3 == Score_(evt.tori).Shido())
				{
					Score_(uke).Remove(Point::Wazaari);
					Score_(uke).Add(Point::Ippon);
				}
				else if (maxShidoCount > 1 && 2 == Score_(evt.tori).Shido())
				{
					Score_(uke).Remove(Point::Yuko);
					Score_(uke).Add(Point::Wazaari);
				}
				else if (maxShidoCount > 0 && 1 == Score_(evt.tori).Shido())
				{
					Score_(uke).Add(Point::Yuko);
				}
			}
		}
	}

	Score_(evt.tori).Add(Point::Shido);
}

void IpponboardSM_::add_point(PointEvent<revoke_shido_hm_type> const& evt)
{
    TRACE(2, "IpponboardSM_::add_point()");
    FighterEnum uke = GetUkeFromTori(evt.tori);

	if (Score_(evt.tori).Hansokumake())
	{
		Score_(uke).Remove(Point::Ippon);
		Score_(evt.tori).Remove(Point::Hansokumake);
	}
	else
	{
		auto pRules = m_pCore->GetRules();

		if (m_pCore->is_auto_adjust())
		{
			auto maxShidoCount = pRules->GetMaxShidoCount();

			if (maxShidoCount + 1 == Score_(evt.tori).Shido())
			{
				Score_(uke).Remove(Point::Ippon);
			}
			else
			{
				if (maxShidoCount > 2 && 4 == Score_(evt.tori).Shido())
				{
					Score_(uke).Remove(Point::Ippon);
					Score_(uke).Add(Point::Wazaari);
				}
				else if (maxShidoCount > 1 && 3 == Score_(evt.tori).Shido())
				{
					Score_(uke).Remove(Point::Wazaari);
					Score_(uke).Add(Point::Yuko);
				}
				else if (maxShidoCount > 0 && 2 == Score_(evt.tori).Shido())
				{
					Score_(uke).Remove(Point::Yuko);
				}
			}
		}

		Score_(evt.tori).Remove(Point::Shido);
	}
}

void IpponboardSM_::add_point(PointEvent<hansokumake_type> const& evt)
{
    TRACE(2, "IpponboardSM_::add_point()");
    FighterEnum uke = GetUkeFromTori(evt.tori);
	Score_(uke).Add(Point::Ippon);
	Score_(evt.tori).Add(Point::Hansokumake);

	m_pCore->stop_timer(eTimer_Main);
	m_pCore->stop_timer(eTimer_Hold);
}

void IpponboardSM_::yoshi(Osaekomi_Toketa const& /*evt*/)
{
    TRACE(2, "IpponboardSM_::yoshi()");
    m_pCore->start_timer(eTimer_Main);
	m_pCore->start_timer(eTimer_Hold);
}
