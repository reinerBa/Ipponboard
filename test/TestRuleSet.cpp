#include "../../../../devtools/Catch/single_include/catch.hpp"
#include "../core/Score.h"
#include "../core/Enums.h"
#include "../core/RuleSet.h"
//#include "../core/Fight.h"

#include <iostream>

using namespace Ipponboard;
using Catch::Matchers::Equals;

TEST_CASE("Rules factory creates correct rule object")
{
	auto assureCorrectCreation = [](RuleSet::Type t)->void
	{
		auto r = RuleSet::Create(t);
		REQUIRE_THAT(r.GetName().toStdString(), Equals(RuleSet::GetName(t).toStdString()));
	};

	assureCorrectCreation(RuleSet::Type::Classic);
	assureCorrectCreation(RuleSet::Type::Rules2013);
	assureCorrectCreation(RuleSet::Type::Classic);
	assureCorrectCreation(RuleSet::Type::Rules2017U15);
	assureCorrectCreation(RuleSet::Type::Rules2017);
	assureCorrectCreation(RuleSet::Type::Rules2018);
	assureCorrectCreation(RuleSet::Type::Default);
}

TEST_CASE("Default rules are IJF-2018")
{
	auto r = RuleSet::Create("");
	REQUIRE_THAT(r.GetName().toStdString(), Equals(RuleSet::GetName(RuleSet::Type::Rules2018).toStdString()));
}
