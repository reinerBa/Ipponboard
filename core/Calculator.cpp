#include "Calculator.h"

using namespace Ipponboard;

int Calculator::GetScoreValue(const Score &score, FighterEnum who, bool isGoldenScoreMode) const
{
	const auto other = Ipponboard::OtherFighter(who);

	if (HasWon(score, who, isGoldenScoreMode))
	{
		if (HasIppon(score, who))
		{
			return ScoreValues::Ippon;
		}

		// Only the fight deciding point is taken into account!
		if (Wazaaris(score, who) > Wazaaris(score, other))
		{
			return ScoreValues::Wazaari;
		}

		if (Yukos(score, who) > Yukos(score, other))
		{
			return ScoreValues::Yuko;
		}

		if ((!_ruleSet.ShidoAddsPoint || isGoldenScoreMode) && Shidos(score, who) < Shidos(score, other))
		{
			return ScoreValues::Shido;
		}

		//TODO: Hantei!
	}
	else
	{
		if (!HasWon(score, other, isGoldenScoreMode))
		{
			return ScoreValues::Hikewake;
		}
		else if (_ruleSet.CountSubscores)
		{
			// Special rule for Jugendliga
			if (Wazaaris(score, who) > Wazaaris(score, other))
			{
				return ScoreValues::Wazaari;
			}
			else if (Yukos(score, who) > Yukos(score, other))
			{
				return ScoreValues::Yuko;
			}
		}
	}

	return ScoreValues::Defeat;
}

void Calculator::SetAutoAdjustPoints(bool autoAdjust)
{
	_ruleSet.AlwaysAutoAdjustPoints = autoAdjust;
}

void Calculator::SetCountSubscores(bool countSubscores)
{
	_ruleSet.CountSubscores = countSubscores;
}

bool Calculator::HasHansokumake(const Score &score, Calculator::FighterEnum who) const
{
	return score.Hansokumake(who) || score.Shido(who) == _ruleSet.MaxShidoCount;
}

bool Calculator::HasIppon(Score const& score, Calculator::FighterEnum who) const
{
	return score.Ippons(who) || _ruleSet.AwaseteIppon && score.Wazaari(who) == _ruleSet.MaxWazaariCount;
}

int Calculator::Yukos(Score const& score, Calculator::FighterEnum whos) const
{
	return _ruleSet.HasYuko ? score.Yuko(whos) : 0;
}

int Calculator::Wazaaris(Score const& score, Calculator::FighterEnum whos) const
{
	return score.Wazaari(whos);
}

int Calculator::Shidos(Score const& score, Calculator::FighterEnum whos) const
{
	return score.Shido(whos);
}

bool Calculator::HasWon(Score const& score, Calculator::FighterEnum who, bool isGoldenScoreMode) const
{
	auto result = CompareScore(score, isGoldenScoreMode);

	//TODO: return "result!=0" ?
	if (who == FighterEnum::First && result < 0 || who == FighterEnum::Second && result > 0)
	{
		return true;
	}

	return false;
}

void Calculator::assureFullCorrectScore(Score& score)
{
	assureSingleCorrectScore(score, FighterEnum::First);
	assureSingleCorrectScore(score, FighterEnum::Second);
}

void Calculator::assureSingleCorrectScore(Score& score, FighterEnum whos)
{
	// ippon
	if (score.GetValue(whos, Point::Ippon) > _ruleSet.MaxIpponCount)
		score.SetValue(whos, Point::Ippon, _ruleSet.MaxIpponCount);

	if (score.GetValue(whos, Point::Ippon) < 0)
		score.SetValue(whos, Point::Ippon, 0);

	// waza-ari
	if (score.GetValue(whos, Point::Wazaari) > _ruleSet.MaxWazaariCount)
		score.SetValue(whos, Point::Wazaari, _ruleSet.MaxWazaariCount);

	if (score.GetValue(whos, Point::Wazaari) < 0)
		score.SetValue(whos, Point::Wazaari, 0);

	// yuko
	if (score.GetValue(whos, Point::Yuko) < 0)
		score.SetValue(whos, Point::Yuko, 0);

	// shido
	if (score.GetValue(whos, Point::Shido) > _ruleSet.MaxShidoCount)
		score.SetValue(whos, Point::Shido, _ruleSet.MaxShidoCount);

	if (score.GetValue(whos, Point::Shido) < 0)
		score.SetValue(whos, Point::Shido, 0);

	// hansokumake
	if (score.GetValue(whos, Point::Hansokumake) > 1)
		score.SetValue(whos, Point::Hansokumake, 1);

	if (score.GetValue(whos, Point::Hansokumake) < 0)
		score.SetValue(whos, Point::Hansokumake, 0);

	// if hansokumake is set, there can not be all shidos be set (other way round is possible)
	if (score.GetValue(whos, Point::Hansokumake) == 1 &&
		score.GetValue(whos, Point::Shido) == _ruleSet.MaxShidoCount)
		score.SetValue(whos, Point::Shido, _ruleSet.MaxShidoCount - 1);
}

void Calculator::AddPoint(Score& score, FighterEnum whos, Point point)
{
	auto curVal = score.GetValue(whos, point);
	auto other = Ipponboard::OtherFighter(whos);

	// auto adjusting for penalties
	if (point == Point::Hansokumake)
	{
		score.Increment(other, Point::Ippon);
	}
	else if (point == Point::Shido)
	{
		if (_ruleSet.AlwaysAutoAdjustPoints)
		{
			if (_ruleSet.ShidoAddsPoint)
			{
				if (_ruleSet.MaxShidoCount - 1 == curVal)
				{
					score.Decrement(other, Point::Wazaari);
					score.Increment(other, Point::Ippon);
				}
				else if (_ruleSet.MaxShidoCount - 2 == curVal)
				{
					score.Decrement(other, Point::Yuko);
					score.Increment(other, Point::Wazaari);
				}
				else if (_ruleSet.MaxShidoCount - 3 == curVal)
				{
					score.Increment(other, Point::Yuko);
				}
			}
			else
			{
				if (IsShidoMatchPoint(score, whos))
				{
					score.Increment(other, Point::Ippon);
				}
			}
		}
	}

	score.Increment(whos, point);
	assureFullCorrectScore(score);
}

void Calculator::RemovePoint(Score& score, FighterEnum whos, Point point)
{
	const auto curVal = score.GetValue(whos, point);
	const auto other = Ipponboard::OtherFighter(whos);

	if (point == Point::Hansokumake)
	{
		score.Decrement(other, Point::Ippon);

		// no direct Hansokumake? -> disqualification by single shidos
		if (curVal == 0 && score.GetValue(whos, Point::Shido) > 0)
		{
			RemovePoint(score, whos, Point::Shido);
		}
	}
	else if (point == Point::Shido)
	{
		if (_ruleSet.AlwaysAutoAdjustPoints)
		{
			if (_ruleSet.MaxShidoCount == curVal)
			{
				score.Decrement(other, Point::Ippon);
				if (_ruleSet.ShidoAddsPoint)
				{
					score.Increment(other, Point::Wazaari);
				}
			}
			else if (_ruleSet.MaxShidoCount - 1 == curVal && _ruleSet.ShidoAddsPoint)
			{
				score.Decrement(other, Point::Wazaari);
				score.Increment(other, Point::Yuko);
			}
			else if (_ruleSet.MaxShidoCount - 2 == curVal && _ruleSet.ShidoAddsPoint)
			{
				score.Decrement(other, Point::Yuko);
			}
		}
	}

	score.Decrement(whos, point);
	assureFullCorrectScore(score);
}

void Calculator::SetPointValue(Score& score, FighterEnum whos, Ipponboard::Point point, int value)
{
	if (value >= 0)
	{
		score.SetValue(whos, point, value);
		assureFullCorrectScore(score);
	}
}

bool Calculator::IsAwaseteIppon(Score const& score, FighterEnum whos) const
{
	return _ruleSet.AwaseteIppon && score.Wazaari(whos) >= _ruleSet.MaxWazaariCount;
}

bool Calculator::IsAlmostAwaseteIppon(const Score &score, FighterEnum whos) const
{
	return _ruleSet.AwaseteIppon && score.Wazaari(whos) == _ruleSet.MaxWazaariCount - 1;
}

bool Calculator::IsShidoMatchPoint(Score const& score, FighterEnum whos) const
{
	return score.Shido(whos) == _ruleSet.MaxShidoCount - 1;  //TODO: add a test for the correct behaviour!
}

bool Calculator::IsLeading(const Calculator::Score &score, Calculator::FighterEnum who, bool /*isGoldenScoreMode*/) const
{
	//FIXME: replace with CompareScore!
	//FIXME: correct Golden Score handling!

	FighterEnum leader = FighterEnum::None;

	// determine who has the lead
	if (score.Wazaari(FighterEnum::First) > score.Wazaari(FighterEnum::Second))
	{
		leader = FighterEnum::First;
	}
	else if (score.Wazaari(FighterEnum::First) < score.Wazaari(FighterEnum::Second))
	{
		leader = FighterEnum::Second;
	}
	else  // GetScore_(eFirst).Wazaari() == GetScore_(eSecond).Wazaari()
	{
		if (score.Yuko(FighterEnum::First) > score.Yuko(FighterEnum::Second))
		{
			leader = FighterEnum::First;
		}
		else if (score.Yuko(FighterEnum::First) < score.Yuko(FighterEnum::Second))
		{
			leader = FighterEnum::Second;
		}
		else  // GetScore_(eFirst).Yuko() == GetScore_(eSecond).Yuko()
		{
			if (score.Shido(FighterEnum::First) < score.Shido(FighterEnum::Second) &&
					score.Shido(FighterEnum::Second) > 1)  // no "koka"!
			{
				leader = FighterEnum::First;
			}
			else if (score.Shido(FighterEnum::First) > score.Shido(FighterEnum::Second) &&
					 score.Shido(FighterEnum::First) > 1)
			{
				leader = FighterEnum::Second;
			}
			else
			{
				// equal ==> golden score in single tournament (Hantai is no more)
			}
		}
	}

	return leader == who;
}

int Calculator::CompareScore(Score const& score, bool isGoldenScoreMode) const
{
	auto first = Ipponboard::FighterEnum::First;
	auto second = Ipponboard::FighterEnum::Second;

	if (score.Hansokumake(first) < score.Hansokumake(second))
	{
		return -1;
	}

	if (score.Hansokumake(first) > score.Hansokumake(second))
	{
		return 1;
	}

	if (score.Ippons(first) > score.Ippons(second))
	{
		return -1;
	}

	if (score.Ippons(first) < score.Ippons(second))
	{
		return 1;
	}

	if (score.Wazaari(first) > score.Wazaari(second))
	{
		return -1;
	}

	if (score.Wazaari(first) < score.Wazaari(second))
	{
		return 1;
	}

	if (score.Yuko(first) > score.Yuko(second))
	{
		return -1;
	}

	if (score.Yuko(first) < score.Yuko(second))
	{
		return 1;
	}

	// shidos are not compared as they result in concrete points
	if (!_ruleSet.ShidoAddsPoint && (_ruleSet.ShidoScoreCounts || isGoldenScoreMode))
	{
		if (score.Shido(first) < score.Shido(second))
		{
			return -1;
		}

		if (score.Shido(first) > score.Shido(second))
		{
			return 1;
		}

		// FIXME: ignore first shido in case of koka
	}

	return 0;
}
