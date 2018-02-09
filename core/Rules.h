#ifndef RULESET_H
#define RULESET_H

#include "AbstractRules.h"

#include <QString>
#include <QStringList>
#include <memory>

namespace Ipponboard {

class ClassicRules : public AbstractRules
{
public:
	ClassicRules();

	static const char* const StaticName;
	virtual const char* Name() const final { return StaticName; }
};

class Rules2013 : public AbstractRules
{
public:
	Rules2013();

	static const char* const StaticName;
	virtual const char* Name() const final { return StaticName; }

};

class Rules2017 : public AbstractRules
{
public:
	Rules2017();

	static const char* const StaticName;
	virtual const char* Name() const final { return StaticName; }
};

class Rules2017U15 : public AbstractRules
{
public:
	Rules2017U15();

	static const char* const StaticName;
	virtual const char* Name() const final { return StaticName; }
};

class Rules2018 : public AbstractRules
{
public:
	Rules2018();

	static const char* const StaticName;
	virtual const char* Name() const final { return StaticName; }
};

class RulesFactory
{
public:
	static std::shared_ptr<AbstractRules> Create(QString name);
	static QStringList GetNames();
	static QString GetDefaultName();
};
} // namespace
#endif // RULESET_H
