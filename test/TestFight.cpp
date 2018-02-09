#include "../../../../devtools/Catch/single_include/catch.hpp"
#include "../core/Score.h"
#include "../core/Enums.h"
#include "../core/Fight.h"
#include "../core/Rules.h"

#include "../core/Score.cpp"
#include "../core/Fight.cpp"
#include "../core/Rules.cpp"
#include "../core/Calculator.cpp"

using Ipponboard::Fight;
using Points = Ipponboard::Score;
using Ipponboard::FighterEnum;
using Point = Ipponboard::Point;

TEST_CASE("Fighter with less Shidos wins if points are equal (rules 2013)")
{
	FAIL();
	/*	auto points = Points().Add(Point::Yuko).Add(Point::Yuko);
	auto pointsWithShido = Points(score).Add(Point::Shido);
	auto pointsWithThreeShido = Points(score).Add(Point::Shido).Add(Point::Shido).Add(Point::Shido);

	Fight fight { points, points };
	fight.rules = std::make_shared<Ipponboard::Rules2013>();

	auto first = FighterEnum::First;
	auto second = FighterEnum::Second;

	REQUIRE_FALSE(fight.HasWon(second));
	REQUIRE_FALSE(fight.HasWon(first));

	fight.GetScore(first) = pointsWithShido;
	fight.GetScore(second) = pointsWithShido;
	REQUIRE_FALSE(fight.HasWon(second));
	REQUIRE_FALSE(fight.HasWon(first));

	fight.GetScore(first) = pointsWithThreeShido;
	fight.GetScore(second) = pointsWithThreeShido;
	REQUIRE_FALSE(fight.HasWon(second));
	REQUIRE_FALSE(fight.HasWon(first));

	fight.GetScore(first) = points;
	fight.GetScore(second) = pointsWithThreeShido;
	REQUIRE_FALSE(fight.HasWon(second));
	REQUIRE(fight.HasWon(first));

	fight.GetScore(first) = pointsWithShido;
	fight.GetScore(second) = pointsWithThreeShido;
	REQUIRE_FALSE(fight.HasWon(second));
	REQUIRE(fight.HasWon(first));
	*/
}

TEST_CASE("Validate score points (subscore)")
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
	FAIL();
	/*Fight f;
	f.rules = std::make_shared<Ipponboard::Rules2017>();

	REQUIRE(f.GetScorePoints(FighterEnum::First) == 0);

	f.GetScore(FighterEnum::First).Add(Point::Shido);
	REQUIRE(f.GetScorePoints(FighterEnum::First) == 0);
	REQUIRE(f.GetScorePoints(FighterEnum::Second) == 0);

	f.GetScore(FighterEnum::First).Add(Point::Shido);
	REQUIRE(f.GetScorePoints(FighterEnum::First) == 0);
	REQUIRE(f.GetScorePoints(FighterEnum::Second) == 0);

	f.SetGoldenScore(true);
	REQUIRE(f.GetScorePoints(FighterEnum::First) == 0);
	REQUIRE(f.GetScorePoints(FighterEnum::Second) == 1);
	*/
}

TEST_CASE("rules2017: no one has won if points are equal and shidos aren't in golden score")
{
	FAIL();
	/*auto score1 = Points().Add(Point::Yuko).Add(Point::Shido);
	auto score2 = Points().Add(Point::Yuko);
	Fight fight { score1, score2 };
	fight.rules = std::make_shared<Ipponboard::Rules2017>();

	auto first = FighterEnum::First;
	auto second = FighterEnum::Second;

	REQUIRE_FALSE(fight.HasWon(second));
	REQUIRE_FALSE(fight.HasWon(first));
	*/
}

//TEST_CASE("Setting negative seconds enabled golden score")
//{
//    Fight f;
//    f.SetSecondsElapsed(-5);
//    REQUIRE(f.IsGoldenScore());

//    f.SetSecondsElapsed(5);
//    REQUIRE_FALSE(f.IsGoldenScore());
//}
