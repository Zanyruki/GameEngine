#pragma once
#include "DatabaseObject.h"
#include "..\EventSystem\VariableNamedProperties.h"

namespace gh
{
	class Event : public DatabaseObject
	{
	public:
		Event( tinyxml2::XMLNode* eventNode );
		Event( const std::string& eventName );
		void fireEvent();
		void fireCallbackEvents();
		void addProperty( XMLNode* argNode );

		template< typename T_Datatype >
		void addProperty( const std::string& propertyName, const T_Datatype& data )
		{
			m_VNP.SetProperty( propertyName, data );
		}

		std::string getEventName();
	private:
		
		VariableNamedProperties m_VNP;
		std::string m_eventName;
		float m_chance;
		std::vector< std::string > m_eventCallbackNames;
	};
}