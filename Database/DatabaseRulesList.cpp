#include "DatabaseRulesList.h"
#include "..\Utility\Utility.h"
#include <algorithm>

namespace gh
{
	DatabaseRulesList::DatabaseRulesList()
	{}


	DatabaseRulesList::DatabaseRulesList( XMLNode* ruleNode )
	{
		m_name = getStringXMLAttribute(ruleNode, "name", "noname" );
		
	}


	void DatabaseRulesList::addRule( DatabaseRule* rule )
	{
		m_databaseRules.push_back( rule );
	}


	std::string DatabaseRulesList::getBestMatch( DatabaseQuery* query, DatabaseRule* bestMatch )
	{
		int highestScore = 0;
		bestMatch = nullptr;
		for( auto ruleIter = m_databaseRules.begin(); ruleIter != m_databaseRules.end(); ++ruleIter )
		{
			if( (*ruleIter)->doesRulePass( query ) && (*ruleIter)->getScore() > highestScore )
			{
				bestMatch = *ruleIter;
				highestScore = bestMatch->getScore();
			}
		}
		if( bestMatch != nullptr )
		{
			return bestMatch->getResponseName();
		}
		return "nomatch";
	}


	void DatabaseRulesList::getTopMatches( DatabaseQuery* query, int maxNumber, std::vector< std::string >& outList )
	{
		std::vector< ScoredRule > ruleList;
		int scoreToBeat = 0;
		for( auto ruleIter = m_databaseRules.begin(); ruleIter != m_databaseRules.end(); ++ruleIter )
		{
			if( (*ruleIter)->doesRulePass( query ) && (*ruleIter)->getScore() > scoreToBeat )
			{
				ruleList.push_back( ScoredRule( (*ruleIter )->getScore(), *ruleIter ) );
				//bestMatch = *ruleIter;
				//highestScore = bestMatch->getScore();
			}
		}
		std::sort( ruleList.begin(), ruleList.end() );
		int limit = std::min( maxNumber, (int)ruleList.size() );
		for( int i = 0; i < limit; ++i )
		{
			outList.push_back( ruleList[ i ].m_rule->getResponseName() );
		}

	}
}