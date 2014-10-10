#include "Event.h"
#include "..\Utility\Utility.h"
#include "..\Utility\VariableUtility.h"
#include "..\EventSystem\EventSystem.h"
#include "ScoreBasedDatabase.h"

namespace gh
{

	Event::Event( const std::string& eventName )
		: m_eventName( eventName )
	{
		
	}


	Event::Event( tinyxml2::XMLNode* eventNode )
	{
		std::string name = getStringXMLAttribute( eventNode, "name", "noname" );
		m_variableList->setStringVariable( "name", name );
		m_eventName = getStringXMLAttribute( eventNode, "eventType", "noEvent" );
		m_chance = getFloatXMLAttribute( eventNode, "eventChance", 1.0f );
		XMLNode* arguementsNode = eventNode->FirstChildElement( "Arguments" );
		if( arguementsNode != nullptr )
		{
			for( XMLNode* argNode = arguementsNode->FirstChildElement( "Arg" ); argNode; argNode = argNode->NextSiblingElement( "Arg" ) )
			{
				addProperty( argNode );
			}

			//CallbackEvents
			XMLNode* eventsNode = eventNode->FirstChildElement( "Events" );
			if( eventsNode != nullptr )
			{
				for( XMLNode* eventNode = eventsNode->FirstChildElement( "Event" ); eventNode; eventNode = eventNode->NextSiblingElement( "Event" ) )
				{
					std::string eventName = getStringXMLAttribute( eventNode, "name", "noname" );
					m_eventCallbackNames.push_back( eventName );
				}
			}
			
		}
	}


	void Event::fireEvent()
	{
		FireEvent( m_eventName, m_VNP );
	}

	void Event::fireCallbackEvents()
	{
		Event* callbackEvent = nullptr;
		for( auto eventIter = m_eventCallbackNames.begin(); eventIter != m_eventCallbackNames.end(); ++eventIter )
		{
			callbackEvent = static_cast< Event*>( getObjectByName( "Event", *eventIter ) );
			callbackEvent->fireEvent();
		}
	}


	std::string Event::getEventName()
	{
		return m_eventName;
	}


	void Event::addProperty( XMLNode* argNode )
	{
		std::string dataName = getStringXMLAttribute( argNode, "name", "noname" );
		std::string dataTypeString = getStringXMLAttribute( argNode, "dataType", "string" );
		DataType dataType = getDataTypeFromString( dataTypeString );
		int tempInt;
		std::string tempString;
		float tempFloat;
		bool tempBool;
		switch( dataType )
		{
		case INT_DATA_TYPE:
			tempInt = getIntXMLAttribute( argNode, "value", 0 );
			m_VNP.SetProperty( dataName, tempInt );
			break;
		case STRING_DATA_TYPE:
			tempString = getStringXMLAttribute( argNode, "value", "" );
			m_VNP.SetProperty( dataName, tempString );
			break;
		case BOOL_DATA_TYPE:
			tempBool = getBoolXMLAttribute( argNode, "value", false );
			m_VNP.SetProperty( dataName, tempBool );
			break;
		case FLOAT_DATA_TYPE:
			tempFloat = getFloatXMLAttribute( argNode, "value", 0.f );
			m_VNP.SetProperty( dataName, tempFloat );
			break;
		}
	}
}