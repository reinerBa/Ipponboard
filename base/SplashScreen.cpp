﻿// Copyright 2018 Florian Muecke. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE.txt file.

#include "../util/debug.h"
#include "DonationManager.h"
#include "SplashScreen.h"
#include "ui_SplashScreen.h"
#include "versioninfo.h"

#include <algorithm>
using namespace std;

SplashScreen::SplashScreen(Data const& data, QWidget* parent)
	: QDialog(parent)
	, ui(new Ui::SplashScreen)
{
    TRACE(2, "SplashScreen::SplashScreen()");
    ui->setupUi(this);

//	const int days_left = data.date.daysTo(QDate::currentDate());
//	ui->label_valid->setText(
//			"- " + QString::number( days_left ) +
//			" " + tr("days left") + " -");

	ui->textBrowser_eula->setHtml(data.text);
	ui->label_info->setText(data.info);
	ui->commandLinkButton_donate->setText(DonationManager::GetDonationLabel());

	/*std::vector<QCommandLinkButton*> buttons =
	{
	    ui->commandLinkButton_donate,
	    ui->commandLinkButton_startSingleVersion,
	    ui->commandLinkButton_startTeamVersion
	};

	buttons.erase(
	        remove_if(
	              begin(buttons),
	              end(buttons),
	              [](QCommandLinkButton* b) { return !b->isEnabled(); }),
	        end(buttons));

	auto r = qrand() % buttons.size();
	buttons[r]->setFocus();
	buttons[r]->setDefault(true);

	//ui->commandLinkButton_donate->setFocus();
	*/

	setWindowFlags(Qt::Window);
}

SplashScreen::~SplashScreen()
{
    TRACE(2, "SplashScreen::~SplashScreen()");
    delete ui;
}

void SplashScreen::SetImageStyleSheet(QString const& /*text*/)
{
    TRACE(2, "SplashScreen::SetImageStyleSheet()");
    //"image: url(:/res/images/logo.png);"
	//ui->widget_image->setStyleSheet(text);
}

void SplashScreen::changeEvent(QEvent* e)
{
    TRACE(4, "SplashScreen::changeEvent(e=%s)", DebugHelpers::QEventToString(e).toUtf8().data());
    QWidget::changeEvent(e);

	switch (e->type())
	{
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;

	default:
		break;
	}
}

void SplashScreen::on_commandLinkButton_startSingleVersion_pressed()
{
    TRACE(2, "SplashScreen::on_commandLinkButton_startSingleVersion_pressed()");
    accept();
}

void SplashScreen::on_commandLinkButton_startTeamVersion_pressed()
{
    TRACE(2, "SplashScreen::on_commandLinkButton_startTeamVersion_pressed()");
    done(QDialog::Accepted + 1);
}

void SplashScreen::on_commandLinkButton_donate_pressed()
{
    TRACE(2, "SplashScreen::on_commandLinkButton_donate_pressed()");
    DonationManager::OpenUrl();
}

//void SplashScreen::on_commandLinkButton_cancel_pressed()
//{
//  TRACE(2, "SplashScreen::on_commandLinkButton_cancel_pressed()");
//	reject();
//}
