#pragma once
#include "DatabaseRule.h"
#include <vector>
#include "..\Utility\tinyxml2.h"
using namespace tinyxml2;
namespace gh
{
	typedef std::vector< DatabaseRule* > RuleList;

	struct ScoredRule
	{
		ScoredRule( int score, DatabaseRule* rule )
			: m_score( score )
			, m_rule( rule )
		{}

		bool operator<( const ScoredRule& other ) const
		{
			return m_score < other.m_score;
		}

		int m_score;
		DatabaseRule* m_rule;
	};

	class DatabaseRulesList
	{
	public :
		DatabaseRulesList();
		DatabaseRulesList( XMLNode* ruleNode );
		void addRule( DatabaseRule* rule );
		std::string getBestMatch( DatabaseQuery* query, DatabaseRule* bestMatch );
		void getTopMatches( DatabaseQuery* query, int maxNumber, std::vector< std::string >& outList );
		std::string m_name;

	private:
		RuleList m_databaseRules;
		std::string m_responseName;
		
	};
}