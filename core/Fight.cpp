// Copyright 2010-2013 Florian Muecke. All rights reserved.
// http://www.ipponboard.info (ipponboardinfo at googlemail dot com)
//
// THIS FILE IS PART OF THE IPPONBOARD PROJECT.
// IT MAY NOT BE DISTRIBUTED TO OR SHARED WITH THE PUBLIC IN ANY FORM!
//

#include "Fight.h"

using namespace Ipponboard;

Fight::Fight()
	: weight("-")
	, calc()
{
	fighters[0] = SimpleFighter();
	fighters[1] = SimpleFighter();

	SetAutoAdjustPoints(false);
	SetCountSubscores(false);
	current_score.Clear();
}

void Fight::AddPoint(FighterEnum whos, Point point)
{
	calc.AddPoint(current_score, whos, point);
}

void Fight::RemovePoint(FighterEnum whos, Point point)
{
	calc.RemovePoint(current_score, whos, point);
}



bool Fight::IsShidoMatchPoint(FighterEnum whos) const
{
	return calc.IsShidoMatchPoint(current_score, whos);
}

bool Fight::HasHansokumake(FighterEnum who) const
{
	return calc.HasHansokumake(current_score, who);
}

int Fight::CompareScore() const
{
	return calc.CompareScore(current_score, IsGoldenScore());
}

bool Fight::IsLeading(FighterEnum who) const
{
	return calc.IsLeading(current_score, who, IsGoldenScore());
}

void Fight::SetValue(FighterEnum whos, Point point, int value)
{
	calc.SetPointValue(current_score, whos, point, value);
}

bool Fight::IsAwaseteIppon(FighterEnum whos) const
{
	return calc.IsAwaseteIppon(current_score, whos);
}

bool Fight::IsAlmostAwaseteIppon(FighterEnum whos) const
{
	return calc.IsAlmostAwaseteIppon(current_score, whos);
}

void Fight::SetRules(RuleSet rules)
{
	calc = Calculator{ rules };
	current_score.Clear();
}

void Fight::SetAutoAdjustPoints(bool autoAdjust)
{
	calc.SetAutoAdjustPoints(autoAdjust);
}

void Fight::SetCountSubscores(bool countSubscores)
{
	calc.SetCountSubscores(countSubscores);
}

int Fight::GetSecondsElapsed() const
{
	return seconds_elapsed;
}

void Fight::SetSecondsElapsed(int s)
{
	//SetGoldenScore(s < 0);
	seconds_elapsed = s;
}

int Fight::GetRoundSeconds() const { return round_time_seconds; }

void Fight::SetRoundTime(int secs) { round_time_seconds = secs; }

int Fight::GetRemainingTime() const
{
	if (IsGoldenScore())
	{
		return 0;
	}

	return round_time_seconds - seconds_elapsed;
}

int Fight::GetGoldenScoreTime() const
{
	if (IsGoldenScore() && seconds_elapsed < 0)
	{
		return -seconds_elapsed;
	}

	return 0;
}

bool Fight::HasWon(FighterEnum who) const
{
	return calc.HasWon(current_score, who, IsGoldenScore());
}

int Fight::GetScoreValue(FighterEnum whos) const
{
	return calc.GetScoreValue(current_score, whos, IsGoldenScore());
}


QString Fight::GetTotalTimeElapsedString() const
{
	// get time display
	auto elapsed = IsGoldenScore() ? -seconds_elapsed + round_time_seconds : seconds_elapsed;

	int minutes = elapsed / 60;
	int seconds = elapsed % 60;

	return QString("%1:%3%4").arg(
			   QString::number(minutes),
			   seconds < 10 ? "0" : "",
			   QString::number(seconds));
}

bool Fight::SetElapsedFromTotalTime(QString s)
{
	int secs = 0;
	auto parts = s.split(":");

	if (parts.size() > 1)
	{
		secs += parts.at(0).toUInt() * 60;
		secs += parts.at(1).toUInt();
	}
	else
	{
		return false;
	}

	if (secs > round_time_seconds)
	{
		secs = round_time_seconds - secs;
		SetGoldenScore(true);
	}
	else
	{
		SetGoldenScore(false);
	}

	SetSecondsElapsed(secs);

	return true;
}

QString Fight::GetTimeRemainingString() const
{
	// get time display
	auto isGoldenScore = seconds_elapsed < 0;

	auto time_remaining = isGoldenScore ? GetGoldenScoreTime() : GetRemainingTime();
	auto minutes = time_remaining / 60;
	auto seconds = time_remaining % 60;

	return QString("%1%2:%3%4").arg(
			   isGoldenScore ? "-" : "",
			   QString::number(minutes),
			   seconds < 10 ? "0" : "",
			   QString::number(seconds));
}

