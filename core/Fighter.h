// Copyright 2010-2018 Florian Muecke. All rights reserved.
// http://www.ipponboard.info (ipponboardinfo at googlemail dot com)
//
// THIS FILE IS PART OF THE IPPONBOARD PROJECT.
// IT MAY NOT BE DISTRIBUTED TO OR SHARED WITH THE PUBLIC IN ANY FORM!
//

#ifndef FIGHTER_H
#define FIGHTER_H

#include <QString>

namespace Ipponboard
{

class Fighter
{
public:
	Fighter(QString const& firstName, QString const& lastName);

	bool operator==(Fighter const& other) const;
	bool operator<(Fighter const& other) const;

	QString first_name;
	QString last_name;
	QString club;
	QString weight;
	QString category;
	//QString nation;
};

}  // namespace Ipponboard

#endif // FIGHTER_H
