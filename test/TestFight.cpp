#include "../../../../devtools/Catch/single_include/catch2/catch.hpp"
#include "../core/Score.h"
#include "../core/Enums.h"
#include "../core/Fight.h"
#include "../core/RuleSet.h"

#include "../core/Score.cpp"
#include "../core/Fight.cpp"
#include "../core/RuleSet.cpp"
#include "../core/Calculator.cpp"

using Ipponboard::Fight;
using Points = Ipponboard::Score;
using Ipponboard::FighterEnum;
//using Point = Ipponboard::Point;

TEST_CASE("Fighter with less Shidos wins if points are equal (rules 2013)")
{
	auto first = FighterEnum::First;
	auto second = FighterEnum::Second;

	Fight fight;
	fight.SetRules(RuleSet::Create(RuleSet::Type::Rules2013));

	auto& score = fight.GetScore();
	score.SetValue(first, Point::Yuko, 2);
	score.SetValue(second, Point::Yuko, 2);
	REQUIRE(fight.CompareScore() == 0);

	score.Increment(first, Point::Shido);
	score.Increment(second, Point::Shido);
	REQUIRE(fight.CompareScore() == 0);

	score.SetValue(first, Point::Shido, 3);
	score.SetValue(second, Point::Shido, 3);
	REQUIRE(fight.CompareScore() == 0);

	score.SetValue(first, Point::Shido, 0);
	REQUIRE_FALSE(fight.HasWon(second));
	REQUIRE(fight.HasWon(first));
	REQUIRE(fight.CompareScore() < 0);

	score.Clear();
	score.SetValue(first, Point::Yuko, 2);
	score.SetValue(second, Point::Yuko, 2);
	score.SetValue(first, Point::Shido, 1);
	score.SetValue(second, Point::Shido, 3);
	REQUIRE_FALSE(fight.HasWon(second));
	REQUIRE(fight.HasWon(first));
	REQUIRE(fight.CompareScore() < 0);
}

TEST_CASE("rules <= 2013: Validate score points (subscore)")
{
	FAIL();
	/*auto first = FighterEnum::First;
	auto second = FighterEnum::Second;

	auto emptyScore = Points();
	auto shidoScore = Points().Add(Point::Shido);
	auto yukoScore = Points().Add(Point::Yuko);
	auto yukoWithShidoScore = Points(yukoScore).Add(Point::Shido);
	auto twoYukoScore = Points().Add(Point::Yuko).Add(Point::Yuko);
	auto twoYukoWithShidoScore = Points(twoYukoScore).Add(Point::Shido);
	auto twoYukoWithTwoShidoScore = Points(twoYukoScore).Add(Point::Shido).Add(Point::Shido);

	auto IpponScore = Points().Add(Point::Ippon);

	Fight f1 { emptyScore, shidoScore };
	f1.rules = std::make_shared<Ipponboard::Rules2013>();
	REQUIRE(f1.GetScorePoints(first) == 1);
	REQUIRE(f1.GetScorePoints(second) == 0);

	Fight f2 { yukoScore, yukoWithShidoScore };
	f2.rules = std::make_shared<Ipponboard::Rules2013>();
	REQUIRE(f2.GetScorePoints(first) == 1);
	REQUIRE(f2.GetScorePoints(second) == 0);

	// Hikewake
	Fight f3 { twoYukoScore, twoYukoScore };
	f3.rules = std::make_shared<Ipponboard::Rules2013>();
	REQUIRE(f3.GetScorePoints(first) == 0);
	REQUIRE(f3.GetScorePoints(second) == 0);

	Fight f4 { yukoScore, twoYukoScore };
	f4.rules = std::make_shared<Ipponboard::Rules2013>();
	REQUIRE(f4.GetScorePoints(first) == 0);
	REQUIRE(f4.GetScorePoints(second) == 5);

	Fight f5 { twoYukoWithShidoScore, twoYukoWithTwoShidoScore };
	f5.rules = std::make_shared<Ipponboard::Rules2013>();
	REQUIRE(f5.GetScorePoints(first) == 1);
	REQUIRE(f5.GetScorePoints(second) == 0);
	*/
}

TEST_CASE("TimeRemaining accounts for golden score")
{
	Fight f;
	f.SetRoundTime(240);
	f.SetGoldenScore(true);
	f.SetSecondsElapsed(-65);
	REQUIRE(f.GetTimeRemainingString().toStdString() == "-1:05");
}

TEST_CASE("TimeFaught accounts for golden score")
{
	Fight f;
	f.SetRoundTime(240);
	f.SetGoldenScore(true);
	f.SetSecondsElapsed(-65);
	REQUIRE(f.GetTotalTimeElapsedString().toStdString() == "5:05");
}

TEST_CASE("time string with Golden Score is propertly converted")
{
	Fight f;
	f.SetRoundTime(240);
	f.SetGoldenScore(true);
	f.SetElapsedFromTotalTime("5:23");
	REQUIRE(f.GetTotalTimeElapsedString().toStdString() == "5:23");

	f.SetElapsedFromTotalTime("14:20");
	REQUIRE(f.GetTotalTimeElapsedString().toStdString() == "14:20");
}

TEST_CASE("rules2017: score points will return 1 for shido won in golden score only")
{
	Fight f;
	f.SetRules(RuleSet::Create(RuleSet::Type::Rules2017));
	REQUIRE(f.GetScoreValue(FighterEnum::First) == 0);

	f.GetScore().SetValue(FighterEnum::First, Point::Shido, 1);
	REQUIRE(f.GetScoreValue(FighterEnum::First) == 0);
	REQUIRE(f.GetScoreValue(FighterEnum::Second) == 0);

	f.GetScore().SetValue(FighterEnum::First, Point::Shido, 2);
	REQUIRE(f.GetScoreValue(FighterEnum::First) == 0);
	REQUIRE(f.GetScoreValue(FighterEnum::Second) == 0);

	f.SetGoldenScore(true);
	REQUIRE(f.GetScoreValue(FighterEnum::First) == 0);
	REQUIRE(f.GetScoreValue(FighterEnum::Second) == 1);
}

TEST_CASE("rules2017: no one has won if points are equal and shidos aren't in golden score")
{
	auto first = FighterEnum::First;
	auto second = FighterEnum::Second;

	Fight fight;
	fight.SetRules(RuleSet::Create(RuleSet::Type::Rules2017));
	REQUIRE_FALSE(fight.IsGoldenScore());

	auto& score = fight.GetScore();
	score.SetValue(first, Point::Yuko, 1);
	score.SetValue(first, Point::Shido, 1);
	score.SetValue(second, Point::Yuko, 1);

	REQUIRE_FALSE(fight.HasWon(second));
	REQUIRE_FALSE(fight.HasWon(first));

	REQUIRE(fight.CompareScore() == 0);
}

//TEST_CASE("Setting negative seconds enabled golden score")
//{
//    Fight f;
//    f.SetSecondsElapsed(-5);
//    REQUIRE(f.IsGoldenScore());

//    f.SetSecondsElapsed(5);
//    REQUIRE_FALSE(f.IsGoldenScore());
//}