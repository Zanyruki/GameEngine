#pragma once
#include "Variable.h"
#include <map>

namespace gh
{
	class VariableTable
	{
	public:
		VariableTable();
		VariableBase* getVariable( const std::string& varName );
		VariableBase* getVariable( const char* varName );

		template < typename T_DataType >
		bool getVariable( const std::string& varName, T_DataType& outVariable, DataType dataType )
		{
			auto found = m_variableMap.find( varName );
			if( found != m_variableMap.end() )
			{
				if( found->second->getDataType() == dataType )
				{
					Variable< T_DataType >* typedVar = reinterpret_cast< Variable< T_DataType >* >( found->second );
					outVariable = typedVar->getValue();
					return true;
				}
			}

			return false;
		}

		bool getVariableAsBool( const std::string& varName );
		int getVariableAsInt( const std::string& varName );
		float getVariableAsFloat( const std::string& varName );
		std::string getVariableAsString( const std::string& varName );

		/*template < typename T_DataType >
		bool getVariable( const char* varName, T_DataType& outVariable, DataType dataType )
		{
			return getVariable( std::string( varName ), outVariable, dataType );
		}*/

		void setVariables( XMLNode* variablesNode );
		void setVariable( VariableBase* var );
		void setBoolVariable( const std::string& variableName, bool value );
		void setIntVariable( const std::string& variableName, int value );
		void setFloatVariable( const std::string& variableName, float value );
		void setStringVariable( const std::string& variableName, const std::string& value );

		std::map< std::string, VariableBase * > m_variableMap;

	private:
		

	};
}