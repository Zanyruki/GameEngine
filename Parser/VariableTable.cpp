#include "VariableTable.h"
#include "..\Utility\VariableUtility.h"

namespace gh
{
	VariableTable::VariableTable()
	{
	}

	VariableBase* VariableTable::getVariable( const char* varName )
	{
		return getVariable( std::string( varName ) );
	}


	VariableBase* VariableTable::getVariable( const std::string& varName )
	{
		auto found = m_variableMap.find( varName );
		if( found != m_variableMap.end() )
		{
			return found->second;
		}
		return nullptr;
	}


	bool VariableTable::getVariableAsBool( const std::string& varName )
	{
		auto found = m_variableMap.find( varName );
		if( found != m_variableMap.end() )
		{
			if( found->second->getDataType() == BOOL_DATA_TYPE )
			{
				Variable< bool >* typedVar = reinterpret_cast< Variable< bool >* >( found->second );
				return typedVar->getValue();
			}
		}

		return false;
	}


	int VariableTable::getVariableAsInt( const std::string& varName )
	{
		auto found = m_variableMap.find( varName );
		if( found != m_variableMap.end() )
		{
			if( found->second->getDataType() == INT_DATA_TYPE )
			{
				Variable< int >* typedVar = reinterpret_cast< Variable< int >* >( found->second );
				return typedVar->getValue();
			}
		}

		return 0;
	}


	float VariableTable::getVariableAsFloat( const std::string& varName )
	{
		auto found = m_variableMap.find( varName );
		if( found != m_variableMap.end() )
		{
			if( found->second->getDataType() == FLOAT_DATA_TYPE )
			{
				Variable< float >* typedVar = reinterpret_cast< Variable< float >* >( found->second );
				return typedVar->getValue();
			}
		}

		return 0;
	}


	std::string VariableTable::getVariableAsString( const std::string& varName )
	{
		auto found = m_variableMap.find( varName );
		if( found != m_variableMap.end() )
		{
			if( found->second->getDataType() == STRING_DATA_TYPE )
			{
				Variable< std::string >* typedVar = reinterpret_cast< Variable< std::string >* >( found->second );
				return typedVar->getValue();
			}
		}

		return 0;
	}
	void VariableTable::setVariables( XMLNode* variablesNode )
	{
		if( variablesNode != nullptr )
		{
			for( auto var = variablesNode->FirstChildElement( "Var" ); var; var = var->NextSiblingElement("Var") )
			{

				if( validateXMLAttributes( var, "name,value", ""))
				{
					//trace( getRGBAXMLAttribute( child, "maxHealth", 0.f ) );
					VariableBase* varBase = parseVariableNode( var );
					setVariable( varBase );
					//std::string key = getStringXMLAttribute( child, "key", "0" );
					//m_tileTypeList[ key ] = c;
				}
			}
		}
	}


	

	void VariableTable::setVariable( VariableBase* var )
	{
		if( var != nullptr )
		{
			m_variableMap[ var->m_variableName ] = var;
		}
	}


	void VariableTable::setBoolVariable( const std::string& variableName, bool value )
	{
		Variable<bool>* boolInt = new Variable<bool>( variableName, value, BOOL_DATA_TYPE );
		setVariable( boolInt );
	}


	void VariableTable::setIntVariable( const std::string& variableName, int value )
	{
		Variable<int>* varInt = new Variable<int>( variableName, value, INT_DATA_TYPE );
		setVariable( varInt );
	}


	void VariableTable::setFloatVariable( const std::string& variableName, float value )
	{
		Variable<float>* varFloat = new Variable<float>( variableName, value, FLOAT_DATA_TYPE );
		setVariable( varFloat );
	}


	void VariableTable::setStringVariable( const std::string& variableName, const std::string& value )
	{
		Variable<std::string>* varString = new Variable<std::string>( variableName, value, STRING_DATA_TYPE );
		setVariable( varString );
	}

}