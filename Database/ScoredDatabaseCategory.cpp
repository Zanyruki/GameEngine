#include "ScoredDatabaseCategory.h"
#include "../EventSystem/EventSystem.h"

namespace gh
{
	ScoredDatabaseCategory::ScoredDatabaseCategory()
	{
	}


	void ScoredDatabaseCategory::addObject( DatabaseObject* object )
	{
		std::string name = object->m_variableList->getVariableAsString( "name" );
		m_objectMap[ name ] = object;
	}


	void ScoredDatabaseCategory::addResponse( DatabaseResponse* newResponse )
	{
		m_responses[ newResponse->getName() ] = newResponse;
	}


	void ScoredDatabaseCategory::addRule( DatabaseRule* rule )
	{
		m_rulesList.addRule( rule );
	}


	DatabaseObject* ScoredDatabaseCategory::getObjectByName( const std::string& nameKey )
	{
		auto found = m_objectMap.find( nameKey );
		if( found != m_objectMap.end() )
		{
			return found->second;
		}
		else
		{
			reportError( "ERROR: Object name \"" + nameKey + "\" not found." );
			return nullptr;
		}
	}


	DatabaseObject* ScoredDatabaseCategory::getBestMatch( DatabaseQuery* query )
	{
		DatabaseRule* rule = nullptr;
		std::string responseName = m_rulesList.getBestMatch( query, rule );
		if( responseName == "nomatch")
		{
			return nullptr;
		}
		DatabaseResponse* response = m_responses[ responseName ];
		if( response == nullptr )
		{
			reportError( "Error: Response \"" + responseName + "\" from rule \"" + rule->getName() + "\" in file \""
				+ rule->getFileName() + "\" does not have a response associated with it." );
		}
		response->activate( query );
		DatabaseObject* object = m_objectMap[ response->getObjectName() ];
#ifdef DEBUG
		if( object == nullptr )
		{
			reportError( "ERROR: objectName \"" + response->getObjectName() + "\" does not exist. Response: " + responseName );
		}
#endif
		//set callback
		if( response->hasCallback() )
		{
			std::string callbackName = response->getObjectName() + "Callback";
			Register( response, &DatabaseResponse::callbackFunction, callbackName );
		}
		
		return object;
	}


	void ScoredDatabaseCategory::getTopMatches( DatabaseQuery* query, int maxNumber, ObjectList& outList )
	{
		std::vector< std::string > responses;
		m_rulesList.getTopMatches( query, maxNumber, responses );
		DatabaseResponse* response;
		for( auto responseIter = responses.begin(); responseIter != responses.end(); ++responseIter )
		{
			response = m_responses[ (*responseIter) ];
			outList.push_back( m_objectMap[ response->getObjectName() ] );
		}
	}
}