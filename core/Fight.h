// Copyright 2010-2013 Florian Muecke. All rights reserved.
// http://www.ipponboard.info (ipponboardinfo at googlemail dot com)
//
// THIS FILE IS PART OF THE IPPONBOARD PROJECT.
// IT MAY NOT BE DISTRIBUTED TO OR SHARED WITH THE PUBLIC IN ANY FORM!
//

#ifndef __CORE_FIGHT_H
#define __CORE_FIGHT_H

#include "Score.h"
#include "Enums.h"
#include "Calculator.h"
#include "RuleSet.h"

#include "QString"
#include <memory>

namespace Ipponboard
{
struct SimpleFighter
{
	QString name;
	QString club;
};

class Fight
{
public:
	Fight();

	Fight(RuleSet const& rules, Score const& score)
		: calc{ rules }
		, current_score{ score }
	{
	}

	const RuleSet& GetRuleSet() const { return calc.GetRuleSet(); }

	Score const& GetScore() const
	{
		return current_score;
	}

	//FIXME: remove write access
	Score& GetScore()
	{
		return current_score;
	}

	SimpleFighter const& GetFighter(FighterEnum fighter) const
	{
		return fighters[static_cast<int>(fighter)];
	}

	SimpleFighter GetFighter(FighterEnum fighter)
	{
		return fighters[static_cast<int>(fighter)];
	}

	void AddPoint(FighterEnum whos, Ipponboard::Point point);
	void RemovePoint(FighterEnum whos, Ipponboard::Point point);
	void SetValue(FighterEnum whos, Ipponboard::Point point, int value);
	bool IsAwaseteIppon(FighterEnum whos) const;
	bool IsAlmostAwaseteIppon(FighterEnum whos) const;
	bool IsShidoMatchPoint(FighterEnum whos) const;
	bool HasHansokumake(FighterEnum who) const;
	int CompareScore() const;
	bool IsLeading(FighterEnum who) const;

	bool IsGoldenScore() const { return _isGoldenScore; }
	void SetGoldenScore(bool val) { _isGoldenScore = val; }
	void SetRules(RuleSet rules);
	void SetAutoAdjustPoints(bool autoAdjust);
	void SetCountSubscores(bool countSubscores);

	int GetSecondsElapsed() const;
	void SetSecondsElapsed(int s);
	bool SetElapsedFromTotalTime(QString s);
	int GetRoundSeconds() const;
	void SetRoundTime(int secs);
	QString GetTotalTimeElapsedString() const;
	QString GetTimeRemainingString() const;

public:
	// returns remaining seconds
	int GetRemainingTime() const;
	int GetGoldenScoreTime() const;
	bool HasWon(FighterEnum who) const;
	int GetScoreValue(FighterEnum whos) const;

	SimpleFighter fighters[2];
	QString weight;
	bool is_saved { false };

private:
	bool _isGoldenScore { false };
	Calculator calc;
	Score current_score;
	int seconds_elapsed { 0 };
	int round_time_seconds { 0 };
};
} // namespace Ipponboard

#endif // __CORE_FIGHT_H
