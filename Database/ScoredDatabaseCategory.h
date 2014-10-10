#pragma once
#include "DatabaseObject.h"
#include "DatabaseRulesList.h"
#include "DatabaseResponse.h"
#include <map>

namespace gh
{
	typedef std::map< std::string, DatabaseObject* > ObjectMap;
	typedef std::map< std::string, DatabaseResponse* > ResponseMap;
	class ScoredDatabaseCategory
	{
	public:
		ScoredDatabaseCategory();
		void addObject( DatabaseObject* object );
		void addResponse( DatabaseResponse* newResponse );
		void addRule( DatabaseRule* rule );
		DatabaseObject* getBestMatch( DatabaseQuery* query );
		DatabaseObject* getObjectByName( const std::string& nameKey );
		void getTopMatches( DatabaseQuery* query, int maxNumber, ObjectList& outList );
	private:
		ObjectMap m_objectMap;
		DatabaseRulesList m_rulesList;
		ResponseMap m_responses;
	};
}