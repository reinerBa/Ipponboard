#include "Rules.h"
#include "RuleSet.h"
#include "Fight.h"

using namespace Ipponboard;

const char* const Rules2018::StaticName = "IJF-2018";
const char* const Rules2017::StaticName = "IJF-2017";
const char* const Rules2017U15::StaticName = "IJF-2017 U15";
const char* const Rules2013::StaticName = "IJF-2013";
const char* const ClassicRules::StaticName = "Classic";

AbstractRules::AbstractRules()
{
}


ClassicRules::ClassicRules()
{
	_options.Name = ClassicRules::StaticName;
	_options.GoldenScoreIsOpenEnd = false;
	_options.ShidoAddsPoint = true;

	//TODO: assure those (via tests)
	//		_options.OsaekomiValue_Ippon = 25;
	//		_options.OsaekomiValue_Wazaari = 20;
	//		_options.OsaekomiValue_Yuko = 15;
}

Rules2013::Rules2013()
{
	_options.Name = Rules2013::StaticName;
	_options.OsaekomiValue_Ippon = 20;
	_options.OsaekomiValue_Wazaari = 15;
	_options.OsaekomiValue_Yuko = 10;
}

Rules2017::Rules2017()
{
	_options.Name = Rules2017::StaticName;
	_options.ShidoScoreCounts = false;
	_options.HasYuko = false;
	_options.AwaseteIppon = false;
	_options.MaxShidoCount = 2;
	_options.MaxWazaariCount = 999;
	_options.OsaekomiValue_Ippon = 20;
	_options.OsaekomiValue_Wazaari = 10;
	_options.OsaekomiValue_Yuko = -1;
}

Rules2017U15::Rules2017U15()
{
	_options.Name = Rules2017U15::StaticName;
	_options.ShidoScoreCounts = false;
	_options.HasYuko = false;
	_options.AwaseteIppon = false;
	_options.MaxShidoCount = 3;
	_options.MaxWazaariCount = 999;
	_options.OsaekomiValue_Ippon = 20;
	_options.OsaekomiValue_Wazaari = 10;
	_options.OsaekomiValue_Yuko = -1;
}

Rules2018::Rules2018()
{
	_options.Name = Rules2018::StaticName;
	_options.ShidoScoreCounts = false;
	_options.HasYuko = false;
	_options.AwaseteIppon = true; // important
	_options.MaxShidoCount = 2;
	_options.MaxWazaariCount = 2; // Important
	_options.OsaekomiValue_Ippon = 20;
	_options.OsaekomiValue_Wazaari = 10;
	_options.OsaekomiValue_Yuko = -1;
}

std::shared_ptr<AbstractRules> RulesFactory::Create(QString name)
{
	if (name == ClassicRules::StaticName)
	{
		return std::make_shared<ClassicRules>();
	}

	if (name == Rules2013::StaticName)
	{
		return std::make_shared<Rules2013>();
	}

	if (name == Rules2017U15::StaticName)
	{
		return std::make_shared<Rules2017U15>();
	}

	if (name == Rules2017::StaticName)
	{
		return std::make_shared<Rules2017>();
	}

	if (name == Rules2018::StaticName)
	{
		return std::make_shared<Rules2018>();
	}

	// default
	return std::make_shared<Rules2018>();
}

QStringList RulesFactory::GetNames()
{
	auto result = QStringList();

	result.push_back(Rules2018::StaticName);
	result.push_back(Rules2017::StaticName);
	result.push_back(Rules2017U15::StaticName);
	result.push_back(Rules2013::StaticName);
	result.push_back(ClassicRules::StaticName);

	return result;
}

QString RulesFactory::GetDefaultName()
{
	return Rules2018::StaticName;
}
