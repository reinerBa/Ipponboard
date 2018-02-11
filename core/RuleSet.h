#pragma once

#include "Enums.h"
#include <QString>
#include <QStringList>

namespace Ipponboard
{
	class RuleSet
	{
	public:
		enum class Type
		{
			Custom = -1,
			Classic = 0,
			Rules2013 = 1,
			Rules2017 = 2,
			Rules2017U15 = 3,
			Rules2018 = 4,
			Default = Type::Rules2018
		};

		RuleSet() {}
		RuleSet(Type t) : _type{t} {}

		static RuleSet Create(Type t);
		static RuleSet Create(QString name);
		static QString GetName(Type t);
		static Type GetType(QString name);
		static QStringList GetNames();
		QString GetName() const;
		bool IsType(Type t) const;

		bool AlwaysAutoAdjustPoints { false };
		bool CountSubscores { false };

		// second shido will result in yuko beeing added, 3rd will give waza-ari
		bool ShidoAddsPoint { false };
		bool ShidoScoreCounts { true };
		bool AwaseteIppon { true };
		bool HasYuko { true };
		bool GoldenScoreIsOpenEnd { true };

		int MaxIpponCount { 1 };
		int MaxShidoCount { 4 }; // Max Shido equals Hansokumake
		int MaxWazaariCount { 2 };
		int OsaekomiValue_Ippon { 25 };
		int OsaekomiValue_Wazaari { 20 };
		int OsaekomiValue_Yuko { 15 };

	private:
		Type _type { Type::Custom };
	};
}
