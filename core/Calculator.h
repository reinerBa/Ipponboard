#pragma once

#include "Enums.h"
#include "ScoreValues.h"
#include "Score.h"
#include "RuleSet.h"
namespace Ipponboard
{
class Calculator
{
	using FighterEnum = Ipponboard::FighterEnum;
	using Point = Ipponboard::Point;
	using Score = Ipponboard::Score;

public:
	Calculator() {} //FIXME: remove, should not be needed!

	Calculator(RuleSet ruleSet) : _ruleSet{ ruleSet }
	{
	}

	void AddPoint(Score& score, FighterEnum whos, Point point);
	void RemovePoint(Score& score, FighterEnum whos, Point point);
	void SetPointValue(Score& score, FighterEnum whos, Point point, int value);

	bool HasHansokumake(Score const& score, FighterEnum who) const;
	bool HasIppon(Score const& score, FighterEnum who) const;
	int Yukos(Score const& score, FighterEnum whos) const;
	int Wazaaris(Score const& score, FighterEnum whos) const;
	int Shidos(Score const& score, FighterEnum whos) const;
	bool HasWon(Score const& score, FighterEnum who, bool isGoldenScoreMode) const;
	bool IsAwaseteIppon(Score const& s, FighterEnum whos) const;
	bool IsAlmostAwaseteIppon(Score const& s, FighterEnum whos) const;
	bool IsShidoMatchPoint(Score const& score, FighterEnum whos) const;
	bool IsLeading(Score const& score, FighterEnum who, bool isGoldenScoreMode) const;

	int CompareScore(Score const& score, bool isGoldenScoreMode) const;
	int GetScoreValue(Score const& score, FighterEnum whos, bool isGoldenScoreMode) const;

	const RuleSet& GetRuleSet() const { return _ruleSet; }
	void SetAutoAdjustPoints(bool autoAdjust);
	void SetCountSubscores(bool countSubscores);

private:
	void assureFullCorrectScore(Score& score);
	void assureSingleCorrectScore(Score& score, FighterEnum whos);
	RuleSet _ruleSet {};
};

} // ns
