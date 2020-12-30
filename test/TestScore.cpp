#include "../../../../devtools/Catch/single_include/catch2/catch.hpp"
#include "../core/Score.h"
#include "../core/Enums.h"
#include "../core/RuleSet.h"
#include "../core/Fight.h"
#include "../core/Calculator.h"

#include <iostream>

using namespace Ipponboard;
auto first = FighterEnum::First;
auto second = FighterEnum::Second;

//FIXME: address golden score mode
bool IsScoreLess(RuleSet const& rules, Score const& score)
{
	Calculator calc(rules);
	return calc.CompareScore(score, false) > 0;
}

TEST_CASE("rules <=2013: shido rules for fights")
{
	//FIXME: fix this test!
	/*
	auto empty = Points();
	auto shido = Points().Add(Point::Shido);
	auto twoShido = Points().Add(Point::Shido).Add(Point::Shido);
	auto yukoWithShido = Points().Add(Point::Yuko).Add(Point::Shido);
	auto yukoWithTwoShido = Points().Add(Point::Yuko).Add(Point::Shido).Add(Point::Shido);
	auto wazaari = Points().Add(Point::Wazaari);
	auto wazaariWithShido = Points().Add(Point::Wazaari).Add(Point::Shido);
	auto wazaariWithTwoShido = Points().Add(Point::Wazaari).Add(Point::Shido).Add(Point::Shido);
	auto wazaariWithThreeShido = Points().Add(Point::Wazaari).Add(Point::Shido).Add(Point::Shido).Add(Point::Shido);

	auto rules2013 = std::make_shared<Ipponboard::Rules2013>();
	REQUIRE_FALSE(IsScoreLess(rules2013, shido, shido));
	REQUIRE(IsScoreLess(rules2013, shido, empty));
	REQUIRE(IsScoreLess(rules2013, twoShido, shido));
	REQUIRE(IsScoreLess(rules2013, shido, yukoWithTwoShido));
	REQUIRE(IsScoreLess(rules2013, twoShido, yukoWithTwoShido));
	REQUIRE(IsScoreLess(rules2013, empty, yukoWithTwoShido));
	REQUIRE(IsScoreLess(rules2013, wazaariWithShido, wazaari));
	REQUIRE(IsScoreLess(rules2013, wazaariWithTwoShido, wazaari));
	REQUIRE(IsScoreLess(rules2013, wazaariWithTwoShido, wazaariWithShido));
	REQUIRE(IsScoreLess(rules2013, wazaariWithThreeShido, wazaariWithShido));
	REQUIRE(IsScoreLess(rules2013, wazaariWithThreeShido, wazaariWithTwoShido));

	// classic rules
	auto classicRules = std::make_shared<Ipponboard::ClassicRules>();
	REQUIRE_FALSE(IsScoreLess(classicRules, shido, empty));
	REQUIRE_FALSE(IsScoreLess(classicRules, shido, shido));
	REQUIRE(IsScoreLess(classicRules, twoShido, yukoWithShido));
	REQUIRE(IsScoreLess(classicRules, shido, yukoWithTwoShido));
	REQUIRE(IsScoreLess(classicRules, twoShido, yukoWithTwoShido));
	REQUIRE(IsScoreLess(classicRules, empty, yukoWithTwoShido));
	*/

	FAIL();
}
TEST_CASE("empty score means no Hansookumake")
{
	std::vector<RuleSet> ruleSets{
		RuleSet::Create(RuleSet::Type::Classic),
		RuleSet::Create(RuleSet::Type::Rules2013),
		RuleSet::Create(RuleSet::Type::Rules2017),
		RuleSet::Create(RuleSet::Type::Rules2018)
	};

	Score s;
	for (auto const& r : ruleSets)
	{
		auto calc = Calculator(r);
		REQUIRE_FALSE(calc.HasHansokumake(s, Second));
		REQUIRE_FALSE(calc.HasHansokumake(s, First));
	}
}

static void CheckFinalShido(RuleSet& r)
{
	auto const isGoldenScore = false;
	Calculator calc(r);
	Score s;
	s.SetValue(First, Point::Shido, r.MaxShidoCount);
	REQUIRE(calc.HasHansokumake(s, First));
	REQUIRE(calc.CompareScore(s, isGoldenScore) > 0);

	s.Clear();
	s.SetValue(Second, Point::Shido, r.MaxShidoCount);
	REQUIRE(calc.HasHansokumake(s, Second));
	REQUIRE(calc.CompareScore(s, isGoldenScore) < 0);

	s.Clear();
	s.SetValue(First, Point::Shido, r.MaxShidoCount);
	s.SetValue(Second, Point::Shido, r.MaxShidoCount);
	REQUIRE(calc.HasHansokumake(s, First));
	REQUIRE(calc.HasHansokumake(s, Second));
	REQUIRE(calc.CompareScore(s, isGoldenScore) == 0);

	s.Clear();
	s.SetValue(First, Point::Shido, r.MaxShidoCount);
	s.SetValue(Second, Point::Hansokumake, 1);
	REQUIRE(calc.HasHansokumake(s, First));
	REQUIRE(calc.HasHansokumake(s, Second));
	REQUIRE(calc.CompareScore(s, isGoldenScore) == 0);
}

TEST_CASE("maxShido equals Hansokumake")
{
	CheckFinalShido(RuleSet::Create(RuleSet::Type::Classic));
	CheckFinalShido(RuleSet::Create(RuleSet::Type::Rules2013));
	CheckFinalShido(RuleSet::Create(RuleSet::Type::Rules2017));
	//FIXME: not sure why failing: CheckFinalShido(Calculator(RuleSet::Create(RuleSet::Type::Rules2017U15)), 3);
	CheckFinalShido(RuleSet::Create(RuleSet::Type::Rules2018));
}

TEST_CASE("4th shido equals hansokumake")
{
	REQUIRE(RuleSet::Create(RuleSet::Type::Classic).MaxShidoCount == 4);
	REQUIRE(RuleSet::Create(RuleSet::Type::Rules2013).MaxShidoCount == 4);
}

TEST_CASE("rules 2017+: 3rd shido equals hansokumake")
{
	REQUIRE(RuleSet::Create(RuleSet::Type::Rules2017).MaxShidoCount == 3);
	//FIXME: not sure why failing: REQUIRE(RuleSet::Create(RuleSet::Type::Rules2017U15).MaxShidoCount == 3);
	REQUIRE(RuleSet::Create(RuleSet::Type::Rules2018).MaxShidoCount == 3);
}

TEST_CASE("Each fighter can have Hansokumake")
{
	std::vector<RuleSet> ruleSets{
		RuleSet::Create(RuleSet::Type::Classic),
		RuleSet::Create(RuleSet::Type::Rules2013),
		RuleSet::Create(RuleSet::Type::Rules2017),
		RuleSet::Create(RuleSet::Type::Rules2018)
	};

	auto const isGoldenScore = false;
	for (auto const& r : ruleSets)
	{
		Score score;
		auto calc = Calculator(r);
		REQUIRE(calc.CompareScore(score, isGoldenScore) == 0);

		score.Clear();
		score.SetValue(first, Point::Hansokumake, 1);
		REQUIRE(calc.CompareScore(score, isGoldenScore) > 0);

		score.Clear();
		score.SetValue(second, Point::Hansokumake, 1);
		REQUIRE(calc.CompareScore(score, isGoldenScore) < 0);

		score.SetValue(first, Point::Hansokumake, 1);
		REQUIRE(calc.CompareScore(score, isGoldenScore) == 0);
	}
}

TEST_CASE("empty score has no awasette ippon")
{
	std::vector<RuleSet> ruleSets{
		RuleSet::Create(RuleSet::Type::Classic),
		RuleSet::Create(RuleSet::Type::Rules2013),
		RuleSet::Create(RuleSet::Type::Rules2017),
		RuleSet::Create(RuleSet::Type::Rules2018)
	};

	Score score;
	for (auto const& r : ruleSets)
	{
		auto calc = Calculator(r);
		REQUIRE_FALSE(calc.IsAwaseteIppon(score, FighterEnum::First));
		REQUIRE_FALSE(calc.IsAwaseteIppon(score, FighterEnum::Second));
	}
}

TEST_CASE("one wazaari is no awasette ippon")
{
	std::vector<RuleSet> ruleSets{
		RuleSet::Create(RuleSet::Type::Classic),
		RuleSet::Create(RuleSet::Type::Rules2013),
		RuleSet::Create(RuleSet::Type::Rules2017),
		RuleSet::Create(RuleSet::Type::Rules2018)
	};

	Score score;
	score.SetValue(FighterEnum::First, Point::Wazaari, 1);
	for (auto const& r : ruleSets)
	{
		auto calc = Calculator(r);
		REQUIRE_FALSE(calc.IsAwaseteIppon(score, FighterEnum::First));
		REQUIRE_FALSE(calc.IsAwaseteIppon(score, FighterEnum::Second));
	}

	score.Clear();
	score.SetValue(FighterEnum::Second, Point::Wazaari, 1);
	for (auto const& r : ruleSets)
	{
		auto calc = Calculator(r);
		REQUIRE_FALSE(calc.IsAwaseteIppon(score, FighterEnum::First));
		REQUIRE_FALSE(calc.IsAwaseteIppon(score, FighterEnum::Second));
	}
}

TEST_CASE("two wazaari is awasete ippon")
{
	std::vector<RuleSet> ruleSets{
		RuleSet::Create(RuleSet::Type::Classic),
		RuleSet::Create(RuleSet::Type::Rules2013),
		RuleSet::Create(RuleSet::Type::Rules2018)
	};

	auto calc = Calculator(RuleSet::Create(RuleSet::Type::Classic));
	Score score;
	score.SetValue(FighterEnum::First, Point::Wazaari, 2);
	for (auto const& r : ruleSets)
	{
		auto calc = Calculator(r);
		REQUIRE(calc.IsAwaseteIppon(score, FighterEnum::First));
		REQUIRE_FALSE(calc.IsAwaseteIppon(score, FighterEnum::Second));
	}

	score.Clear();
	score.SetValue(FighterEnum::Second, Point::Wazaari, 2);
	for (auto const& r : ruleSets)
	{
		auto calc = Calculator(r);
		REQUIRE_FALSE(calc.IsAwaseteIppon(score, FighterEnum::First));
		REQUIRE(calc.IsAwaseteIppon(score, FighterEnum::Second));
	}
}

TEST_CASE("rules 2017: no awasete ippon")
{
	auto calc = Calculator(RuleSet::Create(RuleSet::Type::Rules2017));
	Score score;
	REQUIRE_FALSE(calc.IsAwaseteIppon(score, FighterEnum::First));
	REQUIRE_FALSE(calc.IsAwaseteIppon(score, FighterEnum::Second));

	score.SetValue(FighterEnum::First, Point::Wazaari, 2);
	REQUIRE_FALSE(calc.IsAwaseteIppon(score, FighterEnum::First));
	REQUIRE_FALSE(calc.IsAwaseteIppon(score, FighterEnum::Second));

	score.SetValue(FighterEnum::Second, Point::Wazaari, 2);
	REQUIRE_FALSE(calc.IsAwaseteIppon(score, FighterEnum::First));
	REQUIRE_FALSE(calc.IsAwaseteIppon(score, FighterEnum::Second));

	score.SetValue(FighterEnum::First, Point::Wazaari, 20);
	REQUIRE_FALSE(calc.IsAwaseteIppon(score, FighterEnum::First));
	REQUIRE_FALSE(calc.IsAwaseteIppon(score, FighterEnum::Second));

	score.SetValue(FighterEnum::Second, Point::Wazaari, 20);
	REQUIRE_FALSE(calc.IsAwaseteIppon(score, FighterEnum::First));
	REQUIRE_FALSE(calc.IsAwaseteIppon(score, FighterEnum::Second));
}

TEST_CASE("rules 2017: only first shido does not count")
{
	auto rules = RuleSet::Create(RuleSet::Type::Rules2017);
	Fight f;
	f.SetRules(rules);
	f.SetGoldenScore(false);

	REQUIRE(f.CompareScore() == 0);

	f.AddPoint(First, Point::Shido);
	REQUIRE_FALSE(f.CompareScore() < 0);

	f.AddPoint(First, Point::Shido);
	REQUIRE_FALSE(f.CompareScore() < 0);  // as yuko has to be added manually
}

TEST_CASE("rules 2017: first shido does count in golden score")
{
	auto rules = RuleSet::Create(RuleSet::Type::Rules2017);

	Fight f;
	f.SetRules(rules);
	f.SetGoldenScore(true);

	REQUIRE(f.CompareScore() == 0);

	f.AddPoint(First, Point::Shido);
	REQUIRE(f.CompareScore() > 0);

	f.AddPoint(First, Point::Shido);
	REQUIRE(f.CompareScore() > 0);
}