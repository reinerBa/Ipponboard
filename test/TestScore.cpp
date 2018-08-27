#include "../../../../devtools/Catch/single_include/catch2/catch.hpp"
#include "../core/Score.h"
#include "../core/Enums.h"
#include "../core/RuleSet.h"
#include "../core/Fight.h"
#include "../core/Calculator.h"

#include <iostream>

using namespace Ipponboard;

//FIXME: address golden score mode
bool IsScoreLess(RuleSet const& rules, Score const& score)
{
	Calculator calc(rules);
	return calc.CompareScore(score, false) > 0;
}

TEST_CASE("Shido rules for fights")
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
}

//TEST_CASE("4th shido sets hansokumake")
//{
//	Score one;
//	Score two;
//
//	one.Add(Point::Shido);
//	one.Add(Point::Shido);
//	one.Add(Point::Shido);
//	one.Add(Point::Shido);
//	two.Add(Hansokumake);
//
//	one.Hansokumake
//	REQUIRE_FALSE(one.IsLess(two));
//	REQUIRE_FALSE(two.IsLess(one));
//}

TEST_CASE("Each fighter can have Hansokumake")
{
	//FIXME: rewrite this test
	/*
	Score score;
	auto classicRules = std::make_shared<Ipponboard::ClassicRules>();
	auto rules2013 = std::make_shared<Ipponboard::Rules2013>();

	score.Add(Point::Hansokumake);

	REQUIRE(IsScoreLess(classicRules, score1, score2));
	REQUIRE_FALSE(IsScoreLess(classicRules, score2, score1));

	REQUIRE(IsScoreLess(rules2013, score1, score2));
	REQUIRE_FALSE(IsScoreLess(rules2013, score2, score1));

	score2.Add(Point::Hansokumake);

	REQUIRE_FALSE(IsScoreLess(classicRules, score1, score2));
	REQUIRE_FALSE(IsScoreLess(classicRules, score2, score1));

	REQUIRE_FALSE(IsScoreLess(rules2013, score1, score2));
	REQUIRE_FALSE(IsScoreLess(rules2013, score2, score1));
	*/
}

TEST_CASE("is awasette ippon")
{
	//FIXME: rewrite this test
	/*Score score;
	auto classicRules = std::make_shared<Ipponboard::ClassicRules>();
	auto rules2013 = std::make_shared<Ipponboard::Rules2013>();
	auto rules2017 = std::make_shared<Ipponboard::Rules2017>();
	auto rules2018 = std::make_shared<Ipponboard::Rules2018>();

	REQUIRE_FALSE(classicRules->IsAwaseteIppon(score));
	REQUIRE_FALSE(rules2013->IsAwaseteIppon(score));
	REQUIRE_FALSE(rules2017->IsAwaseteIppon(score));
	REQUIRE_FALSE(rules2018->IsAwaseteIppon(score));

	score.Add(Point::Wazaari);

	REQUIRE_FALSE(classicRules->IsAwaseteIppon(score));
	REQUIRE_FALSE(rules2013->IsAwaseteIppon(score));
	REQUIRE_FALSE(rules2017->IsAwaseteIppon(score));
	REQUIRE_FALSE(rules2018->IsAwaseteIppon(score));

	score.Add(Point::Wazaari);

	REQUIRE(classicRules->IsAwaseteIppon(score));
	REQUIRE(rules2013->IsAwaseteIppon(score));
	REQUIRE_FALSE(rules2017->IsAwaseteIppon(score));
	REQUIRE(rules2018->IsAwaseteIppon(score));
*/
	//TODO
	//FIXME
	/* not correct, yet
	score.Add(Score::Point::Wazaari);

	std::cout << "ippon: " << score.Ippon() << ", ";
	std::cout << "wazaari: " << score.Wazaari();
	REQUIRE(classicRules->IsAwaseteIppon(score));
	REQUIRE(rules2013->IsAwaseteIppon(score));
	REQUIRE_FALSE(rules2017->IsAwaseteIppon(score));
	REQUIRE(rules2018->IsAwaseteIppon(score));
	*/

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
