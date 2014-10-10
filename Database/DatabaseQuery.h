#pragma once
#include "DatabaseObject.h"
#include "..\Parser\VariableBase.h"

namespace gh
{
	class DatabaseQuery
	{
	public:
		DatabaseQuery();
		void setObject( const std::string& key, DatabaseObject* value );
		DatabaseObject* getObject( const std::string key );

		template< typename T_Datatype >
		void setVariable( const std::string& key, T_Datatype value, DataType type )
		{
			Variable< T_Datatype >* var = new Variable< T_Datatype >( "value", value, type );
			m_variables[ key ] = var;
		}

		template< typename T_Datatype >
		void setVariable(const std::string& key, T_Datatype value)
		{
			Variable< T_Datatype >* var = new Variable< T_Datatype >("value", value);
			m_variables[key] = var;
		}


		template<>
		void setVariable( const std::string&key, const char* value, DataType type )
		{
			//Variable< std::string >* var = new Variable< std::string >( value );
			//m_variables[ key ] = var;
			std::string strVersion = value;
			Variable< std::string >* var = new Variable< std::string >( "value", strVersion, type );
			m_variables[ key ] = var;
		}

		template< typename T_Datatype >
		bool getVariable( const std::string& key, T_Datatype& outVar )
		{
			auto found = m_variables.find( key );
			VariableBase* varBase = nullptr;
			if( found != m_variables.end() )
			{
				varBase = found->second;
				Variable< T_Datatype >* typedVar = static_cast< Variable< T_Datatype >* >( varBase );
				outVar = typedVar->getValue();
				return true;
			}
			return false;
			
		}


		DataType getVariableType( const std::string& key );
		VariableBase* getVariable( const std::string& key );

	private:
		std::map< std::string, DatabaseObject* > m_gameObjects;
		std::map< std::string, VariableBase* > m_variables;
	};
}