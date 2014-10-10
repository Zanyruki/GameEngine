#include "DatabaseRule.h"
#include "..\Utility\Utility.h"

using namespace tinyxml2;
namespace gh
{
	DatabaseRule::DatabaseRule( XMLNode* ruleNode )
		: m_ruleScore( 0 )
	{
		m_ruleName = getStringXMLAttribute( ruleNode, "name", "noRule" );
		XMLNode* criteriaListNode = ruleNode->FirstChildElement( "Criteria" );
		for( XMLNode* criteriaNode = criteriaListNode->FirstChildElement( "Criterion" ); criteriaNode; criteriaNode = criteriaNode->NextSiblingElement("Criterion") )
		{
			addCriteria( criteriaNode );
		}
		XMLNode* responseNode = ruleNode->FirstChildElement( "Response" );
		m_responseName = getStringXMLAttribute( responseNode, "name", "noname" );
	}


	void DatabaseRule::addCriteria( XMLNode* criteriaNode )
	{
		std::string varName = getStringXMLAttribute( criteriaNode, "entry", "" );
		std::string operation = getStringXMLAttribute( criteriaNode, "operation", "==" );
		std::string value = getStringXMLAttribute( criteriaNode, "value", "" );
		int score = getIntXMLAttribute( criteriaNode, "score", 1 );
		RuleCriteria* newCriteria = new RuleCriteria( varName, operation, value, score );
		m_allCriteria.push_back( newCriteria );
		m_ruleScore += score;
	}


	bool DatabaseRule::doesRulePass( DatabaseQuery* query )
	{
		for( auto criteriaIter = m_allCriteria.begin(); criteriaIter != m_allCriteria.end(); ++criteriaIter )
		{
			RuleCriteria* criteria = *criteriaIter;
			if( !criteria->isCriteriaMet( query ) )
			{
				return false;
			}
		}

		return true;
	}


	int DatabaseRule::getScore()
	{
		return m_ruleScore;
	}

	const std::string& DatabaseRule::getName()
	{
		return m_ruleName;
	}


	const std::string& DatabaseRule::getResponseName()
	{
		return m_responseName;
	}


	const std::string& DatabaseRule::getFileName()
	{
		return m_fileName;
	}
}