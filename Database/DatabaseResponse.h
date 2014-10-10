#pragma once
#include "..\Utility\tinyxml2.h"
#include <string>
#include <vector>
#include "DatabaseAction.h"
#include "Event.h"


using namespace tinyxml2;
namespace gh
{
	struct ResponseOption
	{
		ResponseOption( const std::string& objectName, int weight )
			: m_objectName( objectName )
			, m_weight( weight )
			, m_callbackEvent( nullptr )
		{

		}

		void addAction( DatabaseAction* action )
		{
			m_actions.push_back( action );
		}

		void addEvent( const std::string& eventName )
		{

		}

		void setCallbackEvent( Event* callback )
		{
			m_callbackEvent = callback;
		}

		std::string m_objectName;
		std::vector< DatabaseAction* > m_actions;
		std::vector< std::string > m_eventNames;
		Event* m_callbackEvent;
		int m_weight;
	};

	class DatabaseResponse
	{
	public:
		DatabaseResponse( XMLNode* node, const std::string& fileName );
		const std::string& getName();
		const std::string& getObjectName();
		void activate( DatabaseQuery* query );
		void callbackFunction( const VariableNamedProperties& VNP );
		bool hasCallback();
		
	private:
		std::string m_name;
		std::vector< ResponseOption >m_responseOptions;
		int m_totalResponseWeight;
		int m_activeResponseOptionIndex;
		std::string m_fileName;
		bool m_hasCallback;
		
	};

}