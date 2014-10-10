#pragma once
#include "RuleCriteria.h"
#include <string>
#include "..\Utility\tinyxml2.h"
using namespace tinyxml2;
namespace gh
{
	typedef std::vector< RuleCriteria* > CriteriaList;
	class DatabaseRule
	{
	public :
		DatabaseRule( XMLNode* ruleNode );
		bool doesRulePass( DatabaseQuery* query );
		int getScore();
		const std::string& getName();
		const std::string& getResponseName();
		const std::string& getFileName();
	private:
		void addCriteria( XMLNode* criteriaNode );
		CriteriaList m_allCriteria;
		std::string m_ruleName;
		std::string m_responseName;
		std::string m_fileName;
		int m_ruleScore;

	};
}