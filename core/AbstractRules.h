#pragma once

#include "Score.h"
#include "Calculator.h"
#include "RuleSet.h"

namespace Ipponboard
{
class Fight;

class AbstractRules
{
public:
	AbstractRules();

	virtual const char* Name() const = 0;

	const RuleSet& Get() const { return _options; } //FIXME: encapsulate

	template<typename T>
	bool IsOfType() const { return dynamic_cast<const T*>(this) != nullptr; }

protected:
	RuleSet _options;
};

} // namespace
