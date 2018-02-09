#ifndef BASE__ICONTROLLERCORE_H_
#define BASE__ICONTROLLERCORE_H_

#include "Enums.h"
#include "Score.h"

#include <QString>
#include <memory>


namespace Ipponboard
{

// forwards
class IView;
class Fight;

class IControllerCore
{

	friend class IpponboardSM_;

private:
	virtual void start_timer(ETimer) = 0;
	virtual void stop_timer(ETimer) = 0;
	virtual void save_fight() = 0;
	virtual void reset_fight() = 0;
	virtual void reset_timer(ETimer) = 0;
	virtual int get_time(ETimer) const = 0;
	virtual bool is_sonomama() const = 0;
	virtual bool is_golden_score() const = 0;
	virtual bool is_option(Ipponboard::EOption option) const = 0;
	virtual bool is_auto_adjust() const = 0;
	virtual Fight& CurrentMatch() = 0;
};

}
#endif  // BASE__ICONTROLLERCORE_H_
