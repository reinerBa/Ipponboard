﻿// Copyright 2018 Florian Muecke. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE.txt file.

#include "../util/debug.h"
#include "MainWindowBase.h"

#include "View.h"
#include "DonationManager.h"
#include "../core/Controller.h"
#include "../core/Enums.h"
#include "../base/versioninfo.h"
#include "../base/SettingsDlg.h"
#ifdef _WITH_GAMEPAD_
#include "../gamepad/gamepad.h"
#endif
#include "../core/Rules.h"
#include "../util/path_helpers.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QInputDialog>
#include <QMessageBox>
#include <QSettings>
#include <QUrl>
#include <QTimer>
#include <QStyle>

// must be included at last, because of Xlib.h conflicts with QT!
#include "../util/screen_helpers.h"

#ifdef _WITH_GAMEPAD_
using namespace FMlib;
#endif
using namespace Ipponboard;
using Point = Score::Point;

MainWindowBase::MainWindowBase(QWidget* parent)
	: QMainWindow(parent)
	, m_pPrimaryView()
	, m_pSecondaryView()
    , m_pController(new Controller())
	, m_fighterManager()
	, m_Language("en")
	, m_MatLabel("  Ipponboard   ")
	, m_weights()
	, m_FighterNameFont("Calibri", 12, QFont::Bold, false)
	, m_secondScreenNo(0)
	, m_secondScreenSize()
    , m_secondScreenPos()
	, m_controllerCfg()
#ifdef _WITH_GAMEPAD_
	, m_pGamepad(new Gamepad)
#endif
{
    TRACE(2, "MainWindowBase::MainWindowBase()");
}

MainWindowBase::~MainWindowBase()
{
    TRACE(2, "MainWindowBase::~MainWindowBase()");
}

void MainWindowBase::Init()
{
    TRACE(2, "MainWindowBase::Init()");
    setWindowTitle(
		QCoreApplication::applicationName() + " v" +
		QCoreApplication::applicationVersion());

	setWindowFlags(Qt::Window);
	//setWindowState(Qt::WindowMaximized);
	// instead, center window
    this->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, this->size(), ScreenHelpers::getInstance()->getScreenGeometry()));

	load_fighters();

	// Setup views
	m_pPrimaryView.reset(
        new View(m_pController->GetIController(), Edition(), View::eTypePrimary));

	attach_primary_view();

	m_pSecondaryView.reset(
        new View(m_pController->GetIController(), Edition(), View::eTypeSecondary));

	// clear data
	m_pController->ClearFightsAndResetTimers();

	// Load settings
	read_settings();

	change_lang(true);

#ifdef _WITH_GAMEPAD_
	// Init gamepad
	QTimer* m_pTimer = new QTimer;
	connect(m_pTimer, SIGNAL(timeout()), this, SLOT(EvaluateInput()));
	m_pTimer->start(75);
#endif

	update_statebar();

	m_pController->RegisterView(m_pPrimaryView.get());
	m_pController->RegisterView(m_pSecondaryView.get());
	m_pController->RegisterView(static_cast<IView*>(this));
	m_pController->RegisterView(static_cast<IGoldenScoreView*>(this));
}

QString MainWindowBase::GetConfigFileName() const
{
    TRACE(2, "MainWindowBase::GetConfigFileName()");
    return "Ipponboard.ini";
}

QString MainWindowBase::GetFighterFileName() const
{
    TRACE(2, "MainWindowBase::GetFighterFileName()");
    return QString("Fighters%1.csv").arg(EditionNameShort());
}

void MainWindowBase::UpdateView()
{
    TRACE(4, "MainWindowBase::UpdateView()");
    update_views();
}

void MainWindowBase::changeEvent(QEvent* e)
{
    TRACE(2, "MainWindowBase::changeEvent(e=%s)", DebugHelpers::QEventToString(e).toUtf8().data());
    QMainWindow::changeEvent(e);

	switch (e->type())
	{
	case QEvent::LanguageChange:
		retranslate_Ui();
		break;

	default:
		break;
	}
}

void MainWindowBase::closeEvent(QCloseEvent* event)
{
    TRACE(2, "MainWindowBase::closeEvent()");
    write_settings();
	save_fighters();

	if (m_pSecondaryView)
	{
		m_pSecondaryView->close();
	}

	event->accept();
}

void MainWindowBase::keyPressEvent(QKeyEvent* event)
{
    TRACE(2, "MainWindowBase::keyPressEvent()");
    const bool isCtrlPressed = event->modifiers().testFlag(Qt::ControlModifier);

	switch (event->key())
	{
	case Qt::Key_Space:
        m_pController->DoAction(eAction_Hajime_Mate, FighterEnum::Nobody);
		qDebug() << "Action [ Hajime/Mate ] was triggered by keyboard";
		break;

	case Qt::Key_Backspace:
		if (isCtrlPressed)
		{
            m_pController->DoAction(eAction_ResetAll, FighterEnum::Nobody);
			qDebug() << "Action [ Reset ] was triggered by keyboard";
		}

		break;

	case Qt::Key_Left:
		{
			if (eState_Holding == m_pController->GetCurrentState() &&
                    FighterEnum::First != m_pController->GetLead())
			{
                m_pController->DoAction(eAction_SetOsaekomi,
                                        FighterEnum::First);
			}
			else
			{
                m_pController->DoAction(eAction_OsaeKomi_Toketa,
                                        FighterEnum::First);
			}

			qDebug() << "Action [ Osaekomi/Toketa for fighter1 ] was triggered by keyboard";
		}

		break;

	case Qt::Key_Right:
		{
			if (eState_Holding == m_pController->GetCurrentState() &&
                    FighterEnum::Second != m_pController->GetLead())
			{
                m_pController->DoAction(eAction_SetOsaekomi,
                                        FighterEnum::Second);
			}
			else
			{
                m_pController->DoAction(eAction_OsaeKomi_Toketa,
                                        FighterEnum::Second);
			}

			qDebug() << "Action [ Osaekomi/Toketa for fighter2 ] was triggered by keyboard";
		}

		break;

	case Qt::Key_Down:
		//if (isCtrlPressed)
		{
            m_pController->DoAction(eAction_ResetOsaeKomi,
                                    FighterEnum::Nobody,
									true);
			qDebug() << "Action [ Reset Osaekomi ] was triggered by keyboard";
		}
		break;

	case Qt::Key_F5:
        m_pController->DoAction(eAction_Ippon,
                                FighterEnum::First,
								isCtrlPressed);
		qDebug() << "Action [ Ippon for fighter1, revoke="
				 << isCtrlPressed
				 << "] was triggered by keyboard";
		break;

	case Qt::Key_F6:
        m_pController->DoAction(eAction_Wazaari,
                                FighterEnum::First,
								isCtrlPressed);
		qDebug() << "Action [ Wazaari for fighter1, revoke="
				 << isCtrlPressed
				 << "] was triggered by keyboard";
		break;

	case Qt::Key_F7:
        m_pController->DoAction(eAction_Yuko,
                                FighterEnum::First,
								isCtrlPressed);
		qDebug() << "Action [ Yuko for fighter1, revoke="
				 << isCtrlPressed
				 << "] was triggered by keyboard";
		break;

	case Qt::Key_F8:
        m_pController->DoAction(eAction_Shido,
                                FighterEnum::First,
								isCtrlPressed);
		qDebug() << "Action [ Shido for fighter1, revoke="
				 << isCtrlPressed
				 << "] was triggered by keyboard";
		break;

	case Qt::Key_F9:
        m_pController->DoAction(eAction_Ippon,
                                FighterEnum::Second,
								isCtrlPressed);
		qDebug() << "Action [ Ippon for fighter2, revoke="
				 << isCtrlPressed
				 << "] was triggered by keyboard";
		break;

	case Qt::Key_F10:
        m_pController->DoAction(eAction_Wazaari,
                                FighterEnum::Second,
								isCtrlPressed);
		qDebug() << "Action [ Wazaari for fighter2, revoke="
				 << isCtrlPressed
				 << "] was triggered by keyboard";
		break;

	case Qt::Key_F11:
        m_pController->DoAction(eAction_Yuko,
                                FighterEnum::Second,
								isCtrlPressed);
		qDebug() << "Action [ Yuko for fighter2, revoke="
				 << isCtrlPressed
				 << "] was triggered by keyboard";
		break;

	case Qt::Key_F12:
        m_pController->DoAction(eAction_Shido,
                                FighterEnum::Second,
								isCtrlPressed);
		qDebug() << "Action [ Shido for fighter2, revoke="
				 << isCtrlPressed
				 << "] was triggered by keyboard";
		break;

	default:
		QMainWindow::keyPressEvent(event);
		break;
	}
}

void MainWindowBase::on_actionAbout_Ipponboard_triggered()
{
    TRACE(2, "MainWindowBase::on_actionAbout_Ipponboard_triggered()");
    QMessageBox::about(
		this,
		tr("About %1").arg(QCoreApplication::applicationName()),
		QString("<h3>%1 v%2</h3>"
				"<p>Build: %3, Revision: %4</p>"
 				"<p>&copy; 2010-%5 Florian M&uuml;cke. All rights reserved.<br>For third party licenses see the User Manual.</p>"
				//"<p><a href=\"https://ipponboard.koe-judo.de\">ipponboard.koe-judo.de</a> and <a href=\"https://github.com/fmuecke/Ipponboard\">github.com/fmuecke/Ipponboard</a></p>"
				//"<p>Read how <a href=\"https://github.com/fmuecke/Ipponboard/blob/main/CONTRIBUTING.md\">you can contribute</a> and help Ipponboard improve. "
				//"Please keep Ipponboard alive with <a href=\"%6\">a little donation.</a></p>"
				"<p>This program is provided AS IS with NO WARRANTY OF ANY KIND, "
				"INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A "
				"PARTICULAR PURPOSE.</p>"
			   ).arg(QCoreApplication::applicationName(),
					 QCoreApplication::applicationVersion(),
					 VersionInfo::Date,
					 VersionInfo::Revision,
					 VersionInfo::CopyrightYear,
					 DonationManager::DonationUrl));
}

void MainWindowBase::on_actionUser_Manual_triggered()
{
    TRACE(2, "MainWindowBase::on_actionUser_Manual_triggered()");
    QUrl url(QCoreApplication::applicationDirPath() + tr("/User-Manual.html"));
	QDesktopServices::openUrl(url);
}

void MainWindowBase::on_actionAutoAdjustPoints_toggled(bool checked)
{
    TRACE(2, "MainWindowBase::on_actionAutoAdjustPoints_toggled(checked=%d)",checked);
    m_pController->SetAutoAdjustPoints(checked);
}

void MainWindowBase::on_actionVisit_Project_Homepage_triggered()
{
    TRACE(2, "MainWindowBase::on_actionVisit_Project_Homepage_triggered()");
    QUrl url("https://ipponboard.koe-judo.de"); // TODO
	QDesktopServices::openUrl(url);
}

void MainWindowBase::on_actionOnline_Feedback_triggered()
{
    TRACE(2, "MainWindowBase::on_actionOnline_Feedback_triggered()");
    QUrl url("https://github.com/fmuecke/Ipponboard/issues");
	QDesktopServices::openUrl(url);
}

void MainWindowBase::change_lang(bool beQuiet)
{
    TRACE(2, "MainWindowBase::change_lang(beQuiet=%d)", beQuiet);
    ui_check_language_items();

	if (!beQuiet)
	{
		QMessageBox::information(this, QCoreApplication::applicationName(),
								 tr("Please restart the application so that the change can take effect."));
	}
}

void MainWindowBase::on_actionLang_Deutsch_triggered(bool val)
{
    TRACE(2, "MainWindowBase::on_actionLang_Deutsch_triggered(val=%d)", val);
    if (val)
	{
		m_Language = "de";
		change_lang();
	}
}

void MainWindowBase::on_actionLang_English_triggered(bool val)
{
    TRACE(2, "MainWindowBase::on_actionLang_English_triggered(val=%d)", val);
    if (val)
	{
		m_Language = "en";
		change_lang();
	}
}


void MainWindowBase::on_actionLang_Dutch_triggered(bool val)
{
    TRACE(2, "MainWindowBase::on_actionLang_Dutch_triggered(val=%d)", val);
    if (val)
	{
		m_Language = "nl";
		change_lang();
	}
}

void MainWindowBase::on_actionRulesClassic_triggered(bool checked)
{
    TRACE(2, "MainWindowBase::on_actionRulesClassic_triggered(checked=%d)", checked);
    if (checked)
	{
		m_pController->SetRules(std::make_shared<ClassicRules>());
		ui_check_rules_items();
	}
}

void MainWindowBase::on_actionRules2013_triggered(bool checked)
{
    TRACE(2, "MainWindowBase::on_actionRules2013_triggered(checked=%d)", checked);
    if (checked)
	{
		m_pController->SetRules(std::make_shared<Rules2013>());
		ui_check_rules_items();
	}
}

void MainWindowBase::on_actionRules2017_triggered(bool checked)
{
    TRACE(2, "MainWindowBase::on_actionRules2017_triggered(checked=%d)", checked);
    if (checked)
	{
		m_pController->SetRules(std::make_shared<Rules2017>());
		ui_check_rules_items();
	}
}

void MainWindowBase::on_actionRules2017U15_triggered(bool checked)
{
    TRACE(2, "MainWindowBase::on_actionRules2017U15_triggered(checked=%d)", checked);
    if (checked)
	{
		m_pController->SetRules(std::make_shared<Rules2017U15>());
		ui_check_rules_items();
	}
}

void MainWindowBase::on_actionRules2018_triggered(bool checked)
{
    TRACE(2, "MainWindowBase::on_actionRules2018_triggered(checked=%d)", checked);
    if (checked)
	{
		m_pController->SetRules(std::make_shared<Rules2018>());
		ui_check_rules_items();
	}
}

void MainWindowBase::write_settings()
{
    TRACE(2, "MainWindowBase::write_settings()");
    QString iniFile(
		QString::fromStdString(
            fm::GetSettingsFilePath(GetConfigFileName().toLatin1())));

	QSettings settings(iniFile, QSettings::IniFormat, this);
	//TODO: settings.setIniCodec("UTF-8");

	settings.beginGroup(str_tag_Main);
	{
		settings.remove("");
		settings.setValue(str_tag_Version, VersionInfo::VersionStr);
		settings.setValue(str_tag_Language, m_Language);
		settings.setValue(str_tag_size, size());
		settings.setValue(str_tag_pos, pos());
		settings.setValue(str_tag_SecondScreen, m_secondScreenNo);
        settings.setValue(str_tag_SecondScreenSize, m_secondScreenSize);
        settings.setValue(str_tag_SecondScreenPos, m_secondScreenPos);
	}
	settings.endGroup();

	// write the edition specific settings
	write_specific_settings(settings);

	settings.beginGroup(str_tag_Fonts);
	{
		settings.remove("");
		settings.setValue(str_tag_TextFont1, m_pPrimaryView->GetInfoHeaderFont().toString());
		settings.setValue(str_tag_FighterNameFont, m_FighterNameFont.toString());
		settings.setValue(str_tag_DigitFont, m_pPrimaryView->GetDigitFont().toString());
	}
	settings.endGroup();

	settings.beginGroup(str_tag_Colors);
	{
		settings.remove("");
		settings.setValue(str_tag_InfoTextColor, m_pPrimaryView->GetInfoTextColor());
		settings.setValue(str_tag_InfoTextBgColor, m_pPrimaryView->GetInfoTextBgColor());
		settings.setValue(str_tag_TextColorFirst, m_pPrimaryView->GetTextColorFirst());
		settings.setValue(str_tag_TextBgColorFirst, m_pPrimaryView->GetTextBgColorFirst());
		settings.setValue(str_tag_TextColorSecond, m_pPrimaryView->GetTextColorSecond());
		settings.setValue(str_tag_TextBgColorSecond, m_pPrimaryView->GetTextBgColorSecond());
		//settings.setValue(str_tag_MainClockColorRunning, m_pPrimaryView->GetMainClockColor1());
		//settings.setValue(str_tag_MainClockColorStopped, m_pPrimaryView->GetMainClockColor2());
	}
	settings.endGroup();

	settings.beginGroup(str_tag_Input);
	{
		settings.remove("");
		settings.setValue(str_tag_buttonHajimeMate, m_controllerCfg.button_hajime_mate);
		settings.setValue(str_tag_buttonNext, m_controllerCfg.button_next);
		settings.setValue(str_tag_buttonPrev, m_controllerCfg.button_prev);
		settings.setValue(str_tag_buttonPause, m_controllerCfg.button_pause);
		settings.setValue(str_tag_buttonReset, m_controllerCfg.button_reset);
		settings.setValue(str_tag_buttonReset2, m_controllerCfg.button_reset_2);
		settings.setValue(str_tag_buttonResetHoldFirst, m_controllerCfg.button_reset_hold_first);
		settings.setValue(str_tag_buttonResetHoldSecond, m_controllerCfg.button_reset_hold_second);
		settings.setValue(str_tag_buttonFirstHolding, m_controllerCfg.button_osaekomi_toketa_first);
		settings.setValue(str_tag_buttonSecondHolding, m_controllerCfg.button_osaekomi_toketa_second);
		settings.setValue(str_tag_buttonHansokumakeFirst, m_controllerCfg.button_hansokumake_first);
		settings.setValue(str_tag_buttonHansokumakeSecond, m_controllerCfg.button_hansokumake_second);
		settings.setValue(str_tag_invertX, m_controllerCfg.axis_inverted_X);
		settings.setValue(str_tag_invertY, m_controllerCfg.axis_inverted_Y);
		settings.setValue(str_tag_invertR, m_controllerCfg.axis_inverted_R);
		settings.setValue(str_tag_invertZ, m_controllerCfg.axis_inverted_Z);
	}
	settings.endGroup();

	settings.beginGroup(str_tag_Sounds);
	{
		settings.remove("");
		settings.setValue(str_tag_sound_time_ends, m_pController->GetGongFile());
	}
	settings.endGroup();

	settings.beginGroup(str_tag_Options);
	{
		settings.remove("");
		settings.setValue(str_tag_autoAdjustPoints, m_pController->IsAutoAdjustPoints());
	}
	settings.endGroup();
}

void MainWindowBase::read_settings()
{
    TRACE(2, "MainWindowBase::read_settings()");
    QString iniFile(
		QString::fromStdString(
            fm::GetSettingsFilePath(GetConfigFileName().toLatin1())));

	QSettings settings(iniFile, QSettings::IniFormat, this);
	//TODO: settings.setIniCodec("UTF-8");

	// MainWindow

	settings.beginGroup(str_tag_Main);
	{
		QString langStr = QLocale::system().name();
		langStr.truncate(langStr.lastIndexOf('_'));
		m_Language = settings.value(str_tag_Language, langStr).toString();

		//resize(settings.value(str_tag_size, size()).toSize());
		move(settings.value(str_tag_pos, QPoint(200, 200)).toPoint());
		m_secondScreenNo = settings.value(str_tag_SecondScreen, 0).toInt();
        m_secondScreenSize = settings.value(str_tag_SecondScreenSize, QSize(0, 0)).toSize();
        m_secondScreenPos = settings.value(str_tag_SecondScreenPos, QPoint(0,0)).toPoint();
		update_statebar();
	}
	settings.endGroup();

	// read edition specific settings
	read_specific_settings(settings);

	//
	// Fonts
	//
	settings.beginGroup(str_tag_Fonts);
	{
		QFont font = m_pPrimaryView->GetInfoHeaderFont();
		font.fromString(settings.value(str_tag_TextFont1, font.toString()).toString());
		m_pPrimaryView->SetInfoHeaderFont(m_pPrimaryView->GetInfoHeaderFont());
		m_pSecondaryView->SetInfoHeaderFont(m_pPrimaryView->GetInfoHeaderFont());

		font = m_pPrimaryView->GetFighterNameFont();
		font.fromString(settings.value(str_tag_FighterNameFont, font.toString()).toString());
		update_fighter_name_font(font);

		font = m_pPrimaryView->GetDigitFont();
		font.fromString(settings.value(str_tag_DigitFont, font.toString()).toString());
		m_pPrimaryView->SetDigitFont(font);
		m_pSecondaryView->SetDigitFont(font);
		//m_pScoreScreen->SetDigitFont(font);
	}
	settings.endGroup();

	//
	// Colors
	//
	settings.beginGroup(str_tag_Colors);
	{
		QColor fgColor = m_pSecondaryView->GetInfoTextColor();
		QColor bgColor = m_pSecondaryView->GetInfoTextBgColor();

		if (settings.contains(str_tag_InfoTextColor))
			fgColor = settings.value(str_tag_InfoTextColor).value<QColor>();

		if (settings.contains(str_tag_InfoTextBgColor))
			bgColor = settings.value(str_tag_InfoTextBgColor).value<QColor>();

		update_info_text_color(fgColor, bgColor);

		fgColor = m_pSecondaryView->GetTextColorFirst();
		bgColor = m_pSecondaryView->GetTextBgColorFirst();

		if (settings.contains(str_tag_TextColorFirst))
			fgColor = settings.value(str_tag_TextColorFirst).value<QColor>();

		if (settings.contains(str_tag_TextBgColorFirst))
			bgColor = settings.value(str_tag_TextBgColorFirst).value<QColor>();

		update_text_color_first(fgColor, bgColor);

		fgColor = m_pPrimaryView->GetTextColorSecond();
		bgColor = m_pSecondaryView->GetTextBgColorSecond();

		if (settings.contains(str_tag_TextColorSecond))
			fgColor = settings.value(str_tag_TextColorSecond).value<QColor>();

		if (settings.contains(str_tag_TextBgColorSecond))
			bgColor = settings.value(str_tag_TextBgColorSecond).value<QColor>();

		update_text_color_second(fgColor, bgColor);

		//fgColor = m_pPrimaryView->GetMainClockColor1();
		//bgColor = m_pPrimaryView->GetMainClockColor2();
		//if( settings.contains(str_tag_MainClockColorRunning) )
		//	fgColor = settings.value(str_tag_MainClockColorRunning).value<QColor>();
		//if( settings.contains(str_tag_MainClockColorStopped) )
		//	bgColor = settings.value(str_tag_MainClockColorStopped).value<QColor>();
		//m_pPrimaryView->SetMainClockColor(fgColor, bgColor);
		//m_pSecondaryView->SetMainClockColor(fgColor, bgColor);
	}
	settings.endGroup();

#ifdef _WITH_GAMEPAD_
	settings.beginGroup(str_tag_Input);
	{
		m_controllerCfg.button_hajime_mate =
			settings.value(str_tag_buttonHajimeMate, Gamepad::eButton_pov_back).toInt();

		m_controllerCfg.button_next =
			settings.value(str_tag_buttonNext, Gamepad::eButton10).toInt();

		m_controllerCfg.button_prev =
			settings.value(str_tag_buttonPrev, Gamepad::eButton9).toInt();

		m_controllerCfg.button_pause =
			settings.value(str_tag_buttonPause, Gamepad::eButton2).toInt();

		m_controllerCfg.button_reset =
			settings.value(str_tag_buttonReset, Gamepad::eButton1).toInt();

		m_controllerCfg.button_reset_2 =
			settings.value(str_tag_buttonReset2, Gamepad::eButton4).toInt();

		m_controllerCfg.button_reset_hold_first =
			settings.value(str_tag_buttonResetHoldFirst, Gamepad::eButton6).toInt();

		m_controllerCfg.button_reset_hold_second =
			settings.value(str_tag_buttonResetHoldSecond, Gamepad::eButton8).toInt();

		m_controllerCfg.button_osaekomi_toketa_first =
			settings.value(str_tag_buttonFirstHolding, Gamepad::eButton5).toInt();

		m_controllerCfg.button_osaekomi_toketa_second =
			settings.value(str_tag_buttonSecondHolding, Gamepad::eButton7).toInt();

		m_controllerCfg.button_hansokumake_first =
			settings.value(str_tag_buttonHansokumakeFirst, Gamepad::eButton11).toInt();

		m_controllerCfg.button_hansokumake_second =
			settings.value(str_tag_buttonHansokumakeSecond, Gamepad::eButton12).toInt();

		m_controllerCfg.axis_inverted_X = settings.value(str_tag_invertX, false).toBool();
		m_controllerCfg.axis_inverted_Y = settings.value(str_tag_invertY, true).toBool();
		m_controllerCfg.axis_inverted_R = settings.value(str_tag_invertR, true).toBool();
		m_controllerCfg.axis_inverted_Z = settings.value(str_tag_invertZ, true).toBool();
		// apply settings to gamepad controller
		m_pGamepad->SetInverted(FMlib::Gamepad::eAxis_X, m_controllerCfg.axis_inverted_X);
		m_pGamepad->SetInverted(FMlib::Gamepad::eAxis_Y, m_controllerCfg.axis_inverted_Y);
		m_pGamepad->SetInverted(FMlib::Gamepad::eAxis_R, m_controllerCfg.axis_inverted_R);
		m_pGamepad->SetInverted(FMlib::Gamepad::eAxis_Z, m_controllerCfg.axis_inverted_Z);
	}
	settings.endGroup();
#endif

	settings.beginGroup(str_tag_Sounds);
	{
		m_pController->SetGongFile(settings.value(str_tag_sound_time_ends,
                                   "sounds/buzzer.wav").toString());
	}
	settings.endGroup();

	settings.beginGroup(str_tag_Options);
	{
		const auto isAutoAdjust = settings.value(str_tag_autoAdjustPoints, true).toBool();
		m_pController->SetAutoAdjustPoints(isAutoAdjust);
	}
	settings.endGroup();

	// update views
	update_views();
}

void MainWindowBase::load_fighters()
{
    TRACE(2, "MainWindowBase::load_fighters()");
    QString csvFile(
		QString::fromStdString(
            fm::GetSettingsFilePath(GetFighterFileName().toLatin1())));

	QString errorMsg;

	if (!QFile::exists(csvFile))
	{
		// silently ignore
		return;
	}

	if (!m_fighterManager.ImportFighters(csvFile, FighterManager::DefaultExportFormat(), errorMsg))
	{
		QMessageBox::critical(
			this,
			QCoreApplication::applicationName(),
			errorMsg);
	}
}

void MainWindowBase::save_fighters()
{
    TRACE(2, "MainWindowBase::save_fighters()");
    QString csvFile(
		QString::fromStdString(
            fm::GetSettingsFilePath(GetFighterFileName().toLatin1())));
	QString errorMsg;

	if (!m_fighterManager.ExportFighters(csvFile, FighterManager::DefaultExportFormat(), errorMsg))
	{
		QMessageBox::critical(
			this,
			QCoreApplication::applicationName(),
			errorMsg);
	}
}

void MainWindowBase::update_views()
{
    TRACE(4, "MainWindowBase::update_views()");
    m_pPrimaryView->UpdateView();
	m_pSecondaryView->UpdateView();
}

void MainWindowBase::on_actionTest_Gong_triggered()
{
    TRACE(2, "MainWindowBase::on_actionTest_Gong_triggered()");
    m_pController->Gong();
}

void MainWindowBase::on_actionShow_SecondaryView_triggered()
{
    TRACE(2, "MainWindowBase::on_actionShow_SecondaryView_triggered()");
    show_hide_view();

	if (m_pSecondaryView->isVisible())
	{
	}
}

void MainWindowBase::on_actionPreferences_triggered()
{
    TRACE(2, "MainWindowBase::on_actionPreferences_triggered()");
    SettingsDlg dlg(Edition(), this);
	dlg.SetInfoHeaderSettings(m_pPrimaryView->GetInfoHeaderFont(),
							  m_pPrimaryView->GetInfoTextColor(),
							  m_pPrimaryView->GetInfoTextBgColor());

	dlg.SetFighterNameFont(m_FighterNameFont);
	dlg.SetTextColorsFirst(m_pPrimaryView->GetTextColorFirst(), m_pPrimaryView->GetTextBgColorFirst());
	dlg.SetTextColorsSecond(m_pPrimaryView->GetTextColorSecond(), m_pPrimaryView->GetTextBgColorSecond());

    if (ScreenHelpers::getInstance()->numScreens() <= m_secondScreenNo)
        m_secondScreenNo = 0;
    dlg.SetScreensSettings(m_secondScreenNo, m_secondScreenSize, m_secondScreenPos);
#ifdef _WITH_GAMEPAD_
	dlg.SetControllerConfig(&m_controllerCfg);
#endif
	dlg.SetLabels(m_MatLabel, m_pController->GetHomeLabel(), m_pController->GetGuestLabel());
	dlg.SetGongFile(m_pController->GetGongFile());

	if (QDialog::Accepted == dlg.exec())
	{
		m_pPrimaryView->SetInfoHeaderFont(dlg.GetInfoHeaderFont());
		m_pSecondaryView->SetInfoHeaderFont(dlg.GetInfoHeaderFont());
		update_fighter_name_font(dlg.GetFighterNameFont());
		update_info_text_color(dlg.GetInfoTextColor(), dlg.GetInfoTextBgColor());
		update_text_color_first(dlg.GetTextColorFirst(), dlg.GetTextBgColorFirst());
		update_text_color_second(dlg.GetTextColorSecond(), dlg.GetTextBgColorSecond());

		m_secondScreenNo = dlg.GetSelectedScreen();
        m_secondScreenSize = dlg.GetSecondScreenSize();
        m_secondScreenPos = dlg.GetSecondScreenPos();

#ifdef _WITH_GAMEPAD_
		dlg.GetControllerConfig(&m_controllerCfg);
		// apply settings to gamepad
		m_pGamepad->SetInverted(FMlib::Gamepad::eAxis_X, m_controllerCfg.axis_inverted_X);
		m_pGamepad->SetInverted(FMlib::Gamepad::eAxis_Y, m_controllerCfg.axis_inverted_Y);
		m_pGamepad->SetInverted(FMlib::Gamepad::eAxis_R, m_controllerCfg.axis_inverted_R);
		m_pGamepad->SetInverted(FMlib::Gamepad::eAxis_Z, m_controllerCfg.axis_inverted_Z);
#endif
		m_MatLabel = dlg.GetMatLabel();
		m_pController->SetLabels(dlg.GetHomeLabel(), dlg.GetGuestLabel());

		m_pPrimaryView->SetMat(m_MatLabel);
		m_pSecondaryView->SetMat(m_MatLabel);
		m_pController->SetGongFile(dlg.GetGongFile());

		// save changes to file
		write_settings();

		update_statebar();
		update_views();
	}
}

void MainWindowBase::show_hide_view() const
{
    TRACE(2, "MainWindowBase::show_hide_view()");
    static bool isAlreadyCalled = false;

	if (isAlreadyCalled)
	{
		return;
	}

	isAlreadyCalled = true;  // prevents recursive calls (TODO: implement better)

	if (m_pSecondaryView->isHidden())
	{
        const int nScreens = ScreenHelpers::getInstance()->numScreens();

		if (nScreens > 0 && nScreens > m_secondScreenNo)
		{
            auto screenRes = ScreenHelpers::getInstance()->getScreenGeometry(m_secondScreenNo);
			m_pSecondaryView->move(QPoint(screenRes.x(), screenRes.y()));
		}

        if (m_secondScreenSize.isNull() || m_secondScreenPos.isNull())
		{
			m_pSecondaryView->showFullScreen();
		}
		else
		{
            m_pSecondaryView->move(m_secondScreenPos);
            m_pSecondaryView->resize(m_secondScreenSize);
			m_pSecondaryView->show();
		}
	}
	else
	{
		m_pSecondaryView->hide();
	}

	ui_check_show_secondary_view(!m_pSecondaryView->isHidden());
	isAlreadyCalled = false;
}

#ifdef _WITH_GAMEPAD_
void MainWindowBase::EvaluateInput()
{
	if (Gamepad::eState_ok != m_pGamepad->GetState())
	{
		return;
	}

	m_pGamepad->ReadData();

	if (EvaluateSpecificInput(m_pGamepad.get()))
	{
		return;
	}

	if (m_pGamepad->WasPressed(Gamepad::EButton(m_controllerCfg.button_hajime_mate)))
	{
        m_pController->DoAction(eAction_Hajime_Mate, FighterEnum::Nobody);
	}
	else if (m_pGamepad->WasPressed(Gamepad::EButton(m_controllerCfg.button_reset_hold_first)))
	{
		m_pController->DoAction(eAction_ResetOsaeKomi, FighterEnum::First, true);
	}
	else if (m_pGamepad->WasPressed(Gamepad::EButton(m_controllerCfg.button_reset_hold_second)))
	{
		m_pController->DoAction(eAction_ResetOsaeKomi, FighterEnum::Second, true);
	}
	else if (m_pGamepad->WasPressed(Gamepad::EButton(m_controllerCfg.button_osaekomi_toketa_first)))
	{
		if (eState_Holding == m_pController->GetCurrentState() &&
				FighterEnum::First != m_pController->GetLead())
		{
			m_pController->DoAction(eAction_SetOsaekomi, FighterEnum::First);
		}
		else
		{
			m_pController->DoAction(eAction_OsaeKomi_Toketa, FighterEnum::First);
		}
	}
	else if (m_pGamepad->WasPressed(Gamepad::EButton(m_controllerCfg.button_osaekomi_toketa_second)))
	{
		if (eState_Holding == m_pController->GetCurrentState() &&
				FighterEnum::Second != m_pController->GetLead())
		{
			m_pController->DoAction(eAction_SetOsaekomi, FighterEnum::Second);
		}
		else
		{
			m_pController->DoAction(eAction_OsaeKomi_Toketa, FighterEnum::Second);
		}
	}
	// reset
	else if (
		m_pGamepad->IsPressed(Gamepad::EButton(m_controllerCfg.button_reset)) &&
		m_pGamepad->IsPressed(Gamepad::EButton(m_controllerCfg.button_reset_2)))
	{
        m_pController->DoAction(eAction_ResetAll, FighterEnum::Nobody);
	}

	// hansokumake fighter1
	else if (m_pGamepad->WasPressed(Gamepad::EButton(m_controllerCfg.button_hansokumake_first)))
	{
		const bool revoke(m_pController->GetScore(
							  FighterEnum::First, Point::Hansokumake) != 0);
		m_pController->DoAction(eAction_Hansokumake, FighterEnum::First, revoke);
	}
	// hansokumake fighter2
	else if (m_pGamepad->WasPressed(Gamepad::EButton(m_controllerCfg.button_hansokumake_second)))
	{
		const bool revoke(m_pController->GetScore(
							  FighterEnum::Second, Point::Hansokumake) != 0);
		m_pController->DoAction(eAction_Hansokumake, FighterEnum::Second, revoke);

	}
	else
	{
		// TODO: don't calc this every time...
		float sections[8][2] = {{0}};
		float angle = 360.0f;
		float deadSpace = 2.0f;
		float angle_adjustment = 5.0f;
		sections[0][0] = 360 - 45 / 2 + 1 + deadSpace + angle_adjustment;
		sections[0][1] = 45 / 2 + 1 - deadSpace + angle_adjustment;
		angle = sections[0][1];

		for (int i = 1; i < 8; ++i)
		{
			sections[i][0] = angle + 2 * deadSpace;
			sections[i][1] = angle + 45;
			angle = sections[i][1];
		}

		if (m_pGamepad->WasSectionEnteredXY(sections[0][0], sections[0][1]))
		{
			m_pController->DoAction(eAction_Ippon, FighterEnum::First);
		}
		else if (m_pGamepad->WasSectionEnteredXY(sections[1][0], sections[1][1]))
		{
			m_pController->DoAction(eAction_Wazaari, FighterEnum::First);
		}
		else if (m_pGamepad->WasSectionEnteredXY(sections[2][0], sections[2][1]))
		{
			m_pController->DoAction(eAction_Yuko, FighterEnum::First);
		}
		else if (m_pGamepad->WasSectionEnteredXY(sections[3][0], sections[3][1]))
		{
			m_pController->DoAction(eAction_Shido, FighterEnum::First, true);
		}
		else if (m_pGamepad->WasSectionEnteredXY(sections[4][0], sections[4][1]))
		{
			m_pController->DoAction(eAction_Ippon, FighterEnum::First, true);
		}
		else if (m_pGamepad->WasSectionEnteredXY(sections[5][0], sections[5][1]))
		{
			m_pController->DoAction(eAction_Wazaari, FighterEnum::First, true);
		}
		else if (m_pGamepad->WasSectionEnteredXY(sections[6][0], sections[6][1]))
		{
			m_pController->DoAction(eAction_Yuko, FighterEnum::First, true);
		}
		else if (m_pGamepad->WasSectionEnteredXY(sections[7][0], sections[7][1]))
		{
			m_pController->DoAction(eAction_Shido, FighterEnum::First);
		}

		// evaluate fighter2 actions
		else if (m_pGamepad->WasSectionEnteredRZ(sections[0][0], sections[0][1]))
		{
			m_pController->DoAction(eAction_Ippon, FighterEnum::Second);
		}
		else if (m_pGamepad->WasSectionEnteredRZ(sections[1][0], sections[1][1]))
		{
			m_pController->DoAction(eAction_Wazaari, FighterEnum::Second);
		}
		else if (m_pGamepad->WasSectionEnteredRZ(sections[2][0], sections[2][1]))
		{
			m_pController->DoAction(eAction_Yuko, FighterEnum::Second);
		}
		else if (m_pGamepad->WasSectionEnteredRZ(sections[3][0], sections[3][1]))
		{
			m_pController->DoAction(eAction_Shido, FighterEnum::Second, true);
		}
		else if (m_pGamepad->WasSectionEnteredRZ(sections[4][0], sections[4][1]))
		{
			m_pController->DoAction(eAction_Ippon, FighterEnum::Second, true);
		}
		else if (m_pGamepad->WasSectionEnteredRZ(sections[5][0], sections[5][1]))
		{
			m_pController->DoAction(eAction_Wazaari, FighterEnum::Second, true);
		}
		else if (m_pGamepad->WasSectionEnteredRZ(sections[6][0], sections[6][1]))
		{
			m_pController->DoAction(eAction_Yuko, FighterEnum::Second, true);
		}
		else if (m_pGamepad->WasSectionEnteredRZ(sections[7][0], sections[7][1]))
		{
			m_pController->DoAction(eAction_Shido, FighterEnum::Second);
		}
	}
}
#endif

void MainWindowBase::update_info_text_color(const QColor& color, const QColor& bgColor)
{
    TRACE(2, "MainWindowBase::update_info_text_color()");
    m_pPrimaryView->SetInfoTextColor(color, bgColor);
	m_pSecondaryView->SetInfoTextColor(color, bgColor);
}

void MainWindowBase::update_text_color_first(const QColor& color, const QColor& bgColor)
{
    TRACE(2, "MainWindowBase::update_text_color_first()");
    m_pPrimaryView->SetTextColorFirst(color, bgColor);
	m_pSecondaryView->SetTextColorFirst(color, bgColor);
}

void MainWindowBase::update_text_color_second(const QColor& color, const QColor& bgColor)
{
    TRACE(2, "MainWindowBase::update_text_color_second()");
    m_pPrimaryView->SetTextColorSecond(color, bgColor);
	m_pSecondaryView->SetTextColorSecond(color, bgColor);
}

void MainWindowBase::update_fighter_name_font(const QFont& font)
{
    TRACE(2, "MainWindowBase::update_fighter_name_font()");
    m_FighterNameFont = font;
	m_pPrimaryView->SetFighterNameFont(font);
	m_pSecondaryView->SetFighterNameFont(font);
}

void MainWindowBase::on_button_reset_clicked()
{
    TRACE(2, "MainWindowBase::update_fighter_name_font()");
//	QMessageBox::StandardButton answer =
//		QMessageBox::question( this,
//							   tr("Reset"),
//							   tr("Really reset current fight?"),
//							   QMessageBox::No | QMessageBox::Yes );
//	if( QMessageBox::Yes == answer )
    m_pController->DoAction(eAction_ResetAll,
                            FighterEnum::Nobody,
							false);
}

void MainWindowBase::on_action_Info_Header_triggered(bool val)
{
    TRACE(2, "MainWindowBase::on_action_Info_Header_triggered(val=%d)", val);
    m_pPrimaryView->SetShowInfoHeader(val);
	m_pSecondaryView->SetShowInfoHeader(val);
}

void MainWindowBase::on_actionSet_Hold_Timer_triggered()
{
    TRACE(2, "MainWindowBase::on_actionSet_Hold_Timer_triggered()");
    bool ok(false);
	const int seconds = QInputDialog::getInt(
							this,
							tr("Set Value"),
							tr("Set value to (ss):"),
							0,	// value
							0,	// min
							59,	// max
							1,	// step
							&ok);

	if (ok)
	{
		m_pController->SetTimerValue(eTimer_Hold, QString::number(seconds));
	}
}

void MainWindowBase::on_actionSet_Main_Timer_triggered()
{
    TRACE(2, "MainWindowBase::on_actionSet_Main_Timer_triggered()");
    // Note: this is implemented in the view as well!

//	if( m_pController->GetCurrentState() == eState_SonoMama ||
//		m_pController->GetCurrentState() == eState_TimerStopped )
	{
		bool ok(false);
		const QString time = QInputDialog::getText(
								 this,
								 tr("Set Value"),
								 tr("Set value to (m:ss):"),
								 QLineEdit::Normal,
								 m_pController->GetTimeText(eTimer_Main),
								 &ok);

		if (ok)
		{
			m_pController->SetTimerValue(eTimer_Main, time);
		}
	}
}

void MainWindowBase::update_statebar()
{
    TRACE(2, "MainWindowBase::update_statebar()");
    qDebug() << "virtual function not implemented: " << __FUNCTION__;
}

void MainWindowBase::write_specific_settings(QSettings&)
{
    TRACE(2, "MainWindowBase::write_specific_settings()");
    qDebug() << "virtual function not implemented: " << __FUNCTION__;
}

void MainWindowBase::read_specific_settings(QSettings&)
{
    TRACE(2, "MainWindowBase::read_specific_settings()");
    qDebug() << "virtual function not implemented: " << __FUNCTION__;
}
