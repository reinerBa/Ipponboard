#pragma once

#include "Enums.h"

namespace Ipponboard
{

class Score
{
public:
	Score()
	{
		Clear();
	}

	void Clear();

	int GetValue(FighterEnum whos, Point point) const;
	void SetValue(FighterEnum whos, Point point, int value);
	void Increment(FighterEnum whos, Point point);
	void Decrement(FighterEnum whos, Point point);

	// convenience functions
	int Ippons(FighterEnum whos) const;
	int Wazaari(FighterEnum whos) const;
	int Yuko(FighterEnum whos) const;
	int Shido(FighterEnum whos) const;

	bool Hansokumake(FighterEnum whos) const;

private:
	int* data(FighterEnum whos);
	int const* data(FighterEnum whos) const;
	int values1[static_cast<int>(Point::_MAX)];
	int values2[static_cast<int>(Point::_MAX)];
};

} // namespace ipponboard
