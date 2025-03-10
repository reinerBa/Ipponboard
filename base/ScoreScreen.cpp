// Copyright 2018 Florian Muecke. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE.txt file.

#include "../util/debug.h"
#include "ScoreScreen.h"
#include "ui_ScoreScreen.h"

using namespace Ipponboard;

ScoreScreen::ScoreScreen(QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::ScoreScreen)
{
    TRACE(2, "ScoreScreen::ScoreScreen()");
    ui->setupUi(this);

	ui->club_first->SetColor(Qt::white, Qt::blue);
	ui->score_first->SetColor(Qt::white, Qt::blue);
	ui->logo_first->SetBgColor(Qt::blue);
	ui->text_dummy1_first->SetColor(Qt::white, Qt::blue);
	ui->text_dummy2_first->SetColor(Qt::white, Qt::blue);
	ui->text_dummy1_first->SetText("");
	ui->text_dummy2_first->SetText("");

	ui->club_second->SetColor(Qt::black, Qt::white);
	ui->score_second->SetColor(Qt::black, Qt::white);
	ui->logo_second->SetBgColor(Qt::white);
	ui->text_dummy1_second->SetColor(Qt::black, Qt::white);
	ui->text_dummy2_second->SetColor(Qt::black, Qt::white);
	ui->text_dummy1_second->SetText("");
	ui->text_dummy2_second->SetText("");

    SetTextFont(QFont("Arial", 12, QFont::Normal, false));
    SetDigitFont(QFont("Arial", 12, QFont::Bold, false));

	//ui->text_club_home->setAlignment(Qt::AlignRight);
	//ui->text_club_guest->setAlignment(Qt::AlignLeft);
}

ScoreScreen::~ScoreScreen()
{
    TRACE(2, "ScoreScreen::~ScoreScreen()");
    delete ui;
}

//---------------------------------------------------------
void ScoreScreen::SetClubs(const QString& first, const QString& second)
//---------------------------------------------------------
{
    TRACE(2, "ScoreScreen::SetClubs(first=%s, second=%s)", first.toUtf8().data(), second.toUtf8().data());
    ui->club_first->SetText(first);
	ui->club_second->SetText(second);
}

//---------------------------------------------------------
void ScoreScreen::SetLogos(const QString& fileNameFirst,
						   const QString& fileNameSecond)
//---------------------------------------------------------
{
    TRACE(2, "ScoreScreen::SetLogos(fileNameFirst=%s, fileNameSecond=%s)", fileNameFirst.toUtf8().data(), fileNameSecond.toUtf8().data());
    ui->logo_first->UpdateImage(fileNameFirst);
	ui->logo_second->UpdateImage(fileNameSecond);
}

//---------------------------------------------------------
void ScoreScreen::SetScore(int scoreFirst, int scoreSecond)
//---------------------------------------------------------
{
    TRACE(2, "ScoreScreen::SetScore()");
    ui->score_first->SetText(QString::number(scoreFirst));
	ui->score_second->SetText(QString::number(scoreSecond));
}

//---------------------------------------------------------
void ScoreScreen::SetTextFont(const QFont& font)
//---------------------------------------------------------
{
    TRACE(2, "ScoreScreen::SetTextFont()");
    m_textFont = font;

	ui->club_first->setFont(m_textFont);
	ui->club_second->setFont(m_textFont);
}

//---------------------------------------------------------
void ScoreScreen::SetDigitFont(const QFont& font)
//---------------------------------------------------------
{
    TRACE(2, "ScoreScreen::SetDigitFont()");
    m_digitFont = font;

	ui->score_first->setFont(m_digitFont);
	ui->score_second->setFont(m_digitFont);
}

//---------------------------------------------------------
void ScoreScreen::changeEvent(QEvent* e)
//---------------------------------------------------------
{
    TRACE(2, "ScoreScreen::changeEvent(e=%s)", DebugHelpers::QEventToString(e).toUtf8().data());
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


//---------------------------------------------------------
void ScoreScreen::SetTextColorFirst(const QColor& color, const QColor& bgColor)
//---------------------------------------------------------
{
    TRACE(2, "ScoreScreen::SetTextColorFirst()");
    ui->logo_first->SetBgColor(bgColor);
	ui->club_first->SetColor(color, bgColor);
	ui->score_first->SetColor(color, bgColor);
	ui->text_dummy1_first->SetColor(color, bgColor);
	ui->text_dummy2_first->SetColor(color, bgColor);

	update();
}

//---------------------------------------------------------
void ScoreScreen::SetTextColorSecond(const QColor& color, const QColor& bgColor)
//---------------------------------------------------------
{
    TRACE(2, "ScoreScreen::SetTextColorSecond()");
    ui->logo_second->SetBgColor(bgColor);
	ui->club_second->SetColor(color, bgColor);
	ui->score_second->SetColor(color, bgColor);
	ui->text_dummy1_second->SetColor(color, bgColor);
	ui->text_dummy2_second->SetColor(color, bgColor);

	update();
}

