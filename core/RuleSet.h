#pragma once

#include "Enums.h"
#include <QString>
namespace Ipponboard
{
struct RuleSet
{
	RuleSet() {}  //FIXME: remove default constructor

	RuleSet(const char* name) : Name{name}
	{}

	QString Name {"default"} ;

	bool AlwaysAutoAdjustPoints { false };
	bool CountSubscores { false };

	// second shido will result in yuko beeing added, 3rd will give waza-ari
	bool ShidoAddsPoint { false };
	bool ShidoScoreCounts { true };
	bool AwaseteIppon { true };
	bool HasYuko { true };
	bool GoldenScoreIsOpenEnd { true };

	int MaxIpponCount { 1 };
	int MaxShidoCount { 3 }; // is actually one less than total (4th shido = hansokumake)
	int MaxWazaariCount { 2 };
	int OsaekomiValue_Ippon { 25 };
	int OsaekomiValue_Wazaari { 20 };
	int OsaekomiValue_Yuko { 15 };

	int GetOsaekomiValue(Ipponboard::Point p) const
	{
		switch (p)
		{
		case Ipponboard::Point::Ippon: return OsaekomiValue_Ippon;
		case Ipponboard::Point::Wazaari: return OsaekomiValue_Wazaari;
		case Ipponboard::Point::Yuko: return OsaekomiValue_Yuko;
		default: return -1;
		}
	}
};
}
