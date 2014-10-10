#include "DatabaseAction.h"
#include "ScoreBasedDatabase.h"
#include "..\Utility\Utility.h"
#include "..\Utility\VariableUtility.h"
#include "..\Parser\Variable.h"
#include "..\Parser\VariableTableManager.h"


namespace gh
{
	using namespace tinyxml2;

	DatabaseAction::DatabaseAction( XMLNode* actionNode, const std::string& databaseResponseName, const std::string& fileName )
		: m_expression( nullptr )
	{
		m_variableName = getStringXMLAttribute( actionNode, "variable", "" );
		/*int tableIndex = tempVar.find_first_of( '.' );
		if( tableIndex == std::string::npos )
		{
			reportError("Failed: Variable in SetVariable does not have the format $<objectName>.<variableName>. In Response node <" +databaseResponseName + "> in file \"" + fileName + "\".");
		}
		else
		{
			m_objectName = tempVar.substr( 0, tableIndex );
			m_variableName = tempVar.substr( tableIndex + 1 );
		}*/
		setActionType();

		std::string expressionString = getStringXMLAttribute( actionNode, "value", m_variableName );
		m_expression = new MathematicExpression( expressionString );
		std::string newVarType = getStringXMLAttribute( actionNode, "type", "string" );
		m_variableDataType = getDataTypeFromString( newVarType );
	}


	DatabaseAction::DatabaseAction(const std::string& memoryVariableName, const std::string& responseNodeName, const std::string& fileName)
		: m_variableName( memoryVariableName )
	{
		setActionType();
		m_expression = new MathematicExpression("+= 1");
		m_variableDataType = INT_DATA_TYPE;
	}

	void DatabaseAction::setActionType()
	{
		int dollarIndex = m_variableName.find('$');
		if( dollarIndex != std::string::npos )
		{
			int dotIndex = m_variableName.find('.');
			if( dotIndex != std::string::npos )
			{
				m_objectName = m_variableName.substr( 0, dotIndex );
			}
			else
			{
				reportError( "ERROR: Object used but no . to seperate object and variable name" );
			}

			m_variableName = m_variableName.substr( dotIndex + 1 );
			int parinthesisIndex = m_objectName.find( '(' );
			if( parinthesisIndex != std::string::npos )
			{
				m_actionType = DatabaseType;
				m_databaseCategoryName = "";

				if( dollarIndex == std::string::npos )
				{
					m_databaseCategoryName = m_objectName.substr( 0, parinthesisIndex );
				}
				else
				{
					m_databaseCategoryName = m_objectName.substr( dollarIndex + 1 , parinthesisIndex - 1 );
				}

				int endParinthesisIndex = m_objectName.find(')');
#ifdef DEBUG
				if( endParinthesisIndex == std::string::npos )
				{
					reportError( "ERROR: closing parinthesis not found" );
				}
#endif
				m_objectName = m_objectName.substr( parinthesisIndex + 1, endParinthesisIndex - parinthesisIndex - 1 );
				m_objectName = trim( m_objectName );
			}
			else
			{
				m_actionType = QueryType;
				//m_variableName = varName;
			}

		}
		else
		{
			reportError( "ERROR: You aren't setting an object. No dollar sign." );
		}
	}


	void DatabaseAction::doAction( DatabaseQuery* query )
	{
		VariableTable* varTable = nullptr;
		DatabaseObject* object = nullptr;
		if( m_actionType == QueryType )
		{
			object = query->getObject( m_objectName );
		}
		else if( m_actionType == DatabaseType )
		{
			object = getObjectByName( m_databaseCategoryName, m_objectName );
		}
		
		varTable = object->m_variableList;

		VariableBase* var = varTable->getVariable( m_variableName );
		if( var != nullptr )
		{
			setNewValue( var );
		}
		else
		{
			createNewVariable( varTable );
		}
		

	}


	void DatabaseAction::setNewValue( VariableBase* var )
	{
		DataType type = var->getDataType();
		if( type == FLOAT_DATA_TYPE )
		{
			Variable<float>* floatVar = reinterpret_cast< Variable< float >* >( var );
			floatVar->m_value = m_expression->evaluateAsFloat();
		}
		else if( type == INT_DATA_TYPE )
		{
			Variable< int >* intVar = reinterpret_cast< Variable< int >* >( var );
			intVar->m_value = m_expression->evaluateAsInt( intVar );
		}
		else if( type == STRING_DATA_TYPE )
		{
			Variable< std::string >* stringVar = reinterpret_cast< Variable< std::string >* >( var );
			stringVar->m_value = m_expression->evaluateAsString();
		}
		else if( type == BOOL_DATA_TYPE )
		{
			Variable< bool >* boolVar = reinterpret_cast< Variable< bool >* >( var );
			boolVar->m_value = m_expression->evaluateAsBool();
		}
	}


	void DatabaseAction::createNewVariable( VariableTable* localTable )
	{
		if( m_variableDataType == BOOL_DATA_TYPE )
		{
			bool value = m_expression->evaluateAsBool();
			Variable< bool >* boolVar = new Variable< bool >( m_variableName, value, BOOL_DATA_TYPE );
			localTable->setVariable( boolVar );
		}
		else if( m_variableDataType == INT_DATA_TYPE )
		{
			int value = m_expression->evaluateAsInt();
			localTable->setIntVariable(m_variableName, value);
		}
		else if( m_variableDataType == FLOAT_DATA_TYPE )
		{
			float value = m_expression->evaluateAsFloat();
			Variable< float >* floatVar = new Variable< float >( m_variableName, value, FLOAT_DATA_TYPE );
			localTable->setVariable( floatVar );
		}
		else if( m_variableDataType == STRING_DATA_TYPE )
		{
			std::string value = m_expression->evaluateAsString();
			Variable< std::string >* stringVar = new Variable< std::string >( m_variableName, value, STRING_DATA_TYPE );
			localTable->setVariable( stringVar );
		}

	}
}