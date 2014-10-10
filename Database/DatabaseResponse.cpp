#include "DatabaseResponse.h"
#include "..\Utility\Utility.h"
#include "ScoreBasedDatabase.h"
#include "Event.h"
#include "..\EventSystem\EventSystem.h"

namespace gh
{
	DatabaseResponse::DatabaseResponse( XMLNode* node, const std::string& fileName )
		: m_activeResponseOptionIndex( 0 )
		, m_fileName( fileName )
		, m_totalResponseWeight( 0 )
		, m_hasCallback( false )
	{
		m_name = getStringXMLAttribute( node, "name", "noname" );
		std::string objectName;
		for( XMLNode* objectNode = node->FirstChildElement( "Object" ); objectNode; objectNode = objectNode->NextSiblingElement( "Object" ) )
		{
			objectName = getStringXMLAttribute( objectNode, "name", "noname" );
			int weight = getIntXMLAttribute( objectNode, "weight", 1 );
			m_totalResponseWeight += weight;
			ResponseOption newOption( objectName, weight );
			for( XMLNode* actionNode = objectNode->FirstChildElement( "SetVariable" ); actionNode; actionNode = actionNode->NextSiblingElement( "SetVariable" ) )
			{
				DatabaseAction* action = new DatabaseAction( actionNode, m_name, m_fileName );
				newOption.addAction( action );
			}
			for( XMLNode* memoryNode = objectNode->FirstChildElement( "Remember" ); memoryNode; memoryNode = memoryNode->NextSiblingElement( "Remember" ) )
			{
				//TODO
				std::string variableName = getStringXMLAttribute(memoryNode, "variable", "none");
				DatabaseAction* action = new DatabaseAction(variableName, m_name, m_fileName);
				newOption.addAction( action );
			}

			std::string eventName;
			for( XMLNode* eventNode = objectNode->FirstChildElement( "Event" ); eventNode; eventNode = eventNode->NextSiblingElement( "Event" ) )
			{
				//TODO
				eventName = getStringXMLAttribute( eventNode, "name", "nomatch" );
				newOption.m_eventNames.push_back( eventName );
			}
			
			m_responseOptions.push_back( newOption );
		}
	}


	void DatabaseResponse::activate( DatabaseQuery* query )
	{
		int randomNumber = rand() % m_totalResponseWeight;
		int sum = 0;
		m_activeResponseOptionIndex = 0;
		for( auto optionIter = m_responseOptions.begin(); optionIter != m_responseOptions.end(); ++optionIter )
		{
			sum += optionIter->m_weight;
			if( sum > randomNumber )
			{
				break;
			}
			++m_activeResponseOptionIndex;
				
		}
		ResponseOption currentOption = m_responseOptions[ m_activeResponseOptionIndex ];
		m_hasCallback = currentOption.m_callbackEvent != nullptr;

		for( auto iter = currentOption.m_actions.begin(); iter != currentOption.m_actions.end(); ++iter ) // Sets all the variables in any setVariables were in the response
		{
			(*iter)->doAction( query );
		}

		for( auto eventIter = currentOption.m_eventNames.begin(); eventIter != currentOption.m_eventNames.end(); ++eventIter )
		{
			Event* event = static_cast< Event* >( getObjectByName( "Event", *eventIter ) );
			event->fireEvent();
		}
	}


	const std::string& DatabaseResponse::getName()
	{
		return m_name;
	}

	const std::string& DatabaseResponse::getObjectName()
	{
		return m_responseOptions[ m_activeResponseOptionIndex ].m_objectName;
	}


	void DatabaseResponse::callbackFunction( const VariableNamedProperties& VNP )
	{
		trace( "callbackFunction:" << m_name );
		ResponseOption option = m_responseOptions[ m_activeResponseOptionIndex ];
		Unregister( this, option.m_objectName + "Callback" );
		if( option.m_callbackEvent != nullptr )
		{
			option.m_callbackEvent->fireEvent();
		}
	}


	bool DatabaseResponse::hasCallback()
	{
		return m_hasCallback;
	}
}