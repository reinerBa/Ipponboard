#include "Score.h"

using namespace Ipponboard;

int Score::GetValue(FighterEnum whos, Point point) const
{
	return data(whos)[static_cast<int>(point)];
}

void Score::SetValue(FighterEnum whos, Point point, int value)
{
	data(whos)[static_cast<int>(point)] = value;
}

void Score::Clear()
{
	for (auto i = 0; i < static_cast<int>(Point::_MAX); ++i)
	{
		values1[i] = 0;
		values2[i] = 0;
	}
}

void Score::Increment(FighterEnum whos, Point point)
{
	data(whos)[static_cast<int>(point)] += 1;
}

void Score::Decrement(FighterEnum whos, Point point)
{
	data(whos)[static_cast<int>(point)] -= 1;
}

int Score::Ippon(FighterEnum whos) const
{
	return GetValue(whos, Point::Ippon);
}

int Score::Wazaari(FighterEnum whos) const
{
	return GetValue(whos, Point::Wazaari);
}

int Score::Yuko(FighterEnum whos) const
{
	return GetValue(whos, Point::Yuko);
}

int Score::Shido(FighterEnum whos) const
{
	return GetValue(whos, Point::Shido);
}

bool Score::Hansokumake(FighterEnum whos) const
{
	return GetValue(whos, Point::Hansokumake) != 0;
}

int *Score::data(FighterEnum whos)
{
	return whos == First ? values1 : values2;
}

const int *Score::data(FighterEnum whos) const
{
	return whos == First ? values1 : values2;
}