#ifndef CLUBPARSER_H
#define CLUBPARSER_H

#include "Club.h"
#include "../util/json.hpp"
#include "../util/tinytoml/include/toml/toml.h"
#include "../util/qt_helpers.hpp"

#include <QSettings>
#include <QStringList>
#include <iostream>

namespace ClubParser
{

namespace Tags
{
static const char* Address = "Address";
static const char* LogoFile = "LogoFile";
}

static Ipponboard::ClubList ParseJsonFile(const char* filePath)
{
	Ipponboard::ClubList clubs;

	auto const& jsonClubs = fm::Json::ReadFile(filePath);

	for (auto const & jsonClub : jsonClubs)
	{
		Ipponboard::Club club;
		club.name = fm::qt::from_utf8_str(jsonClub["name"].asString());
		club.address = fm::qt::from_utf8_str(jsonClub["address"].asString());
		club.logoFile = fm::qt::from_utf8_str(jsonClub["image"].asString());

		clubs.push_back(club);
	}

	return clubs;
}

static Ipponboard::ClubList ParseIniFile_UNUSED(const char* filePath)
{
	QSettings settings(filePath, QSettings::IniFormat);
	settings.setIniCodec("UTF-8");

	Ipponboard::ClubList clubs;
	for (auto const& group : settings.childGroups())
	{
		settings.beginGroup(group);
		Ipponboard::Club club;
		club.name = group;
		if (settings.contains(Tags::Address))
		{
			club.address = settings.value(Tags::Address).toString();
		}

		if (settings.contains(Tags::LogoFile))
		{
			club.logoFile = settings.value(Tags::LogoFile).toString();
		}
		settings.endGroup();

		clubs.push_back(club);
	}

	return clubs;
}

static void ToIniFile_UNUSED(const char* filePath, Ipponboard::ClubList const& clubs)
{
	QSettings settings(filePath, QSettings::IniFormat);
	settings.setIniCodec("UTF-8");

	settings.clear();
	for (auto const& club : clubs)
	{
		settings.beginGroup(club.name);
		settings.setValue(Tags::Address, club.address);
		settings.setValue(Tags::LogoFile, club.logoFile);
		settings.endGroup();
	}
}

static Ipponboard::ClubList ParseTomlFile(const char* filePath)
{
	Ipponboard::ClubList clubs;

	auto root = toml::parseFile(filePath);

	if (!root.valid())
	{
		throw std::runtime_error(root.errorReason);
	}

	if (root.value.is<toml::Table>())
	{
		for (auto const& val : root.value.as<toml::Table>())
		{
			Ipponboard::Club club;

			club.name = fm::qt::from_utf8_str(val.first);
			club.address = fm::qt::from_utf8_str(val.second.get<std::string>(Tags::Address));
			club.logoFile = fm::qt::from_utf8_str(val.second.get<std::string>(Tags::LogoFile));

			clubs.emplace_back(club);
		}
	}
	else
	{
		throw std::runtime_error("unexpected format");
	}

	return clubs;
}

static void ToTomlFile(const char* filePath, Ipponboard::ClubList const& clubs)
{
	toml::Value root((toml::Table()));
	toml::Value* top = &root;

	for (auto const& club : clubs)
	{
		auto table = top->setChild(fm::qt::to_utf8_str(club.name).c_str(), toml::Table());

		table->setChild(Tags::Address, fm::qt::to_utf8_str(club.address));
		table->setChild(Tags::LogoFile, fm::qt::to_utf8_str(club.logoFile));
	}

	std::ofstream of(filePath, std::ofstream::out | std::ofstream::trunc | std::ofstream::binary);
	if (of.is_open())
	{
//			if (writeBom)
//			{
//				t << (char)0xEF << (char)0xBB << (char)0xBF;
//			}

		root.writeFormatted(&of, toml::FORMAT_INDENT);
		//of << root;

		of.close();
	}
}

static void ToJsonFile_UNUSED(const char* filePath, Ipponboard::ClubList const& clubs)
{
	fm::Json::Value jsonClubs;

	for (Ipponboard::Club const & club : clubs)
	{
		fm::Json::Value jsonClub;
		jsonClub["name"] = fm::qt::to_utf8_str(club.name);
		jsonClub["address"] = fm::qt::to_utf8_str(club.address);
		jsonClub["image"] = fm::qt::to_utf8_str(club.logoFile);

		jsonClubs.append(jsonClub);
	}

	fm::Json::WriteFile(filePath, jsonClubs);
}

}

#endif // CLUBPARSER_H
