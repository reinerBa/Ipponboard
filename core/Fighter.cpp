// Copyright 2010-2013 Florian Muecke. All rights reserved.
// http://www.ipponboard.info (ipponboardinfo at googlemail dot com)
//
// THIS FILE IS PART OF THE IPPONBOARD PROJECT.
// IT MAY NOT BE DISTRIBUTED TO OR SHARED WITH THE PUBLIC IN ANY FORM!
//

#include "Fighter.h"

using namespace Ipponboard;

Fighter::Fighter(QString const& firstName,
				 QString const& lastName)
	: first_name(firstName)
	, last_name(lastName)
	, club()
	, weight()
	, category()
{
}

bool Fighter::operator==(const Fighter &other) const
{
	return this == &other ||
			(first_name == other.first_name &&
			 last_name == other.last_name &&
			 club == other.club &&
			 weight == other.weight &&
			 category == other.category);
}

bool Fighter::operator<(const Fighter &other) const
{
	if (first_name < other.first_name)
		return true;

	if (other.first_name < first_name)
		return false;

	if (last_name < other.last_name)
		return true;

	if (other.last_name < last_name)
		return false;

	if (club < other.club)
		return true;

	if (other.club < club)
		return false;

	if (weight < other.weight)
		return true;

	if (other.weight < weight)
		return false;

	if (category < other.category)
		return true;

	if (other.category < category)
		return false;

	// equal
	return false;
}
