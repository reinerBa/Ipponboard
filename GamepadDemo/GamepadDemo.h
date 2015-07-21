#ifndef GAMEPADDEMO__GAMEPADDEMO_H_
#define GAMEPADDEMO__GAMEPADDEMO_H_

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QLabel>
#include <memory>

namespace Ui
{
class GamepadDemo;
}

namespace FMlib
{
class Gamepad;
}

typedef std::shared_ptr<FMlib::Gamepad> PGamepad;

class GamepadDemo : public QMainWindow
{
	Q_OBJECT
public:
	GamepadDemo(QWidget* parent = 0);
	~GamepadDemo();

protected:
	void changeEvent(QEvent* e);
	void UpdateButtonState(unsigned button) const;
	void UpdateCapabilities();

private slots:
	void GetData();

private:
	Ui::GamepadDemo* ui;
	QLabel* m_pSBarText;
	QTimer* m_pTimer;
	PGamepad m_pGamepad;
};

#endif  // GAMEPADDEMO__GAMEPADDEMO_H_
