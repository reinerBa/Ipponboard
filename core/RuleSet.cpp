#include "RuleSet.h"

using namespace Ipponboard;
using Type = Ipponboard::RuleSet::Type;

QString RuleSet::GetName(Type t)
{
	switch(t)
	{
	case Type::Custom:		 return "Custom";
	case Type::Classic:		 return "Classic";
	case Type::Rules2013:	 return "IJF-2013";
	case Type::Rules2017U15: return "IJF-2017 U15";
	case Type::Rules2017:	 return "IJF-2017";
	default:				 return "IJF-2018";
	}
}

Type RuleSet::GetType(QString name)
{
	if (name == RuleSet::GetName(Type::Custom))
	{
		return Type::Custom;
	}
	if (name == RuleSet::GetName(Type::Classic))
	{
		return Type::Classic;
	}
	if (name == RuleSet::GetName(Type::Rules2013))
	{
		return Type::Rules2013;
	}
	if (name == RuleSet::GetName(Type::Rules2017U15))
	{
		return Type::Rules2017U15;
	}
	if (name == RuleSet::GetName(Type::Rules2017))
	{
		return Type::Rules2017;
	}

	return Type::Rules2018;
}

QStringList RuleSet::GetNames()
{
	auto result = QStringList();

	result.push_back(GetName(Type::Rules2018));
	result.push_back(GetName(Type::Rules2017));
	result.push_back(GetName(Type::Rules2017U15));
	result.push_back(GetName(Type::Rules2013));
	result.push_back(GetName(Type::Classic));

	return result;
}

QString RuleSet::GetName() const { return GetName(_type); }

bool RuleSet::IsType(Type t) const { return t == _type; }

RuleSet RuleSet::Create(Type t)
{
	RuleSet r { t };

	switch(t)
	{
	case Type::Classic:
	{
		r.GoldenScoreIsOpenEnd = false;
		r.ShidoAddsPoint = true;
		//TODO: assure those (via tests)
		//		r.OsaekomiValue_Ippon = 25;
		//		r.OsaekomiValue_Wazaari = 20;
		//		r.OsaekomiValue_Yuko = 15;
		return std::move(r);
		}
	case Type::Rules2013:
		{
			r.OsaekomiValue_Ippon = 20;
			r.OsaekomiValue_Wazaari = 15;
			r.OsaekomiValue_Yuko = 10;
			return std::move(r);
		}
		case Type::Rules2017U15:
		{
			r.ShidoScoreCounts = false;
			r.HasYuko = false;
			r.AwaseteIppon = false;
			r.MaxShidoCount = 4;
			r.MaxWazaariCount = 999;
			r.OsaekomiValue_Ippon = 20;
			r.OsaekomiValue_Wazaari = 10;
			r.OsaekomiValue_Yuko = -1;
			return std::move(r);
		}
		case Type::Rules2017:
		{
			r.ShidoScoreCounts = false;
			r.HasYuko = false;
			r.AwaseteIppon = false;
			r.MaxShidoCount = 3;
			r.MaxWazaariCount = 999;
			r.OsaekomiValue_Ippon = 20;
			r.OsaekomiValue_Wazaari = 10;
			r.OsaekomiValue_Yuko = -1;
			return std::move(r);
		}
		default:
		{
			r.ShidoScoreCounts = false;
			r.HasYuko = false;
			r.AwaseteIppon = true; // important
			r.MaxShidoCount = 3;
			r.MaxWazaariCount = 2; // important
			r.OsaekomiValue_Ippon = 20;
			r.OsaekomiValue_Wazaari = 10;
			r.OsaekomiValue_Yuko = -1;
			return std::move(r);
		}
	}
}

RuleSet RuleSet::Create(QString name)
{
	return std::move(RuleSet::Create(RuleSet::GetType(name)));
}
