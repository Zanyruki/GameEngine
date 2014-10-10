#include "RuleCriteria.h"
#include "ScoreBasedDatabase.h"
#include "../Utility/ScriptUtil.h"

namespace gh
{

	RuleCriteria::RuleCriteria( const std::string& varName, const std::string& operation, const std::string& valueExpression, int score )
		: m_variableOrFunctionName( varName )
		, m_value( valueExpression )
		, m_scoreValue( score )
	{
		m_operation = MathematicExpression::s_operationTypeMap[ operation ];
		setCriteriaType();
	}

	void RuleCriteria::setCriteriaType()
	{
		
		int dollarIndex = m_variableOrFunctionName.find('$');
		if( dollarIndex != std::string::npos )
		{
			int dotIndex = m_variableOrFunctionName.find('.');
			if( dotIndex != std::string::npos )
			{
				m_objectName = m_variableOrFunctionName.substr( 0, dotIndex );
			}
			else
			{
				reportError( "ERROR: Object used but no . to seperate object and variable name" );
			}
			
			m_variableOrFunctionName = m_variableOrFunctionName.substr( dotIndex + 1 );
			//Check to see if it is a funciton call
			int parinthesisIndex = m_variableOrFunctionName.find( '(' );
			if( parinthesisIndex != std::string::npos )
			{
				m_payloadType = FunctionPayload;
				int endParinthesisIndex = m_variableOrFunctionName.find( ')' );
				std::string argList = m_variableOrFunctionName.substr( parinthesisIndex + 1, endParinthesisIndex - parinthesisIndex - 1 );
				argList = trim( argList );
				parseFunctionArgs( argList, m_functionArgs );
				m_variableOrFunctionName = m_variableOrFunctionName.substr(0, parinthesisIndex);
			}
			else
			{
				m_payloadType = VariablePayload;
			}

			//Check to see if the object is needed from the Database
			parinthesisIndex = m_objectName.find( '(' );
			if( parinthesisIndex != std::string::npos )
			{
				m_criteriaType = DatabaseType;
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
				m_criteriaType = QueryType;
				//m_variableName = varName;
			}

		}
		else
		{
			m_criteriaType = VariableType;
		}
	}

	void RuleCriteria::setExpressionVariableTables(DatabaseQuery* query)
	{
		m_value;
	}


	bool RuleCriteria::isCriteriaMet( DatabaseQuery* query )
	{
		DataType dataType;
		VariableBase* var;
		DatabaseObject* object = nullptr;
		setExpressionVariableTables( query );
		if( m_criteriaType == DatabaseType )
		{
			object = getObjectByName( m_databaseCategoryName, m_objectName );
			if( object == nullptr )
			{
				return false;
			}

		}
		else if( m_criteriaType == QueryType )
		{
			object = query->getObject( m_objectName );

			if( object == nullptr )
			{
				return false;
#ifdef DEBUG
				reportError( "Query Object not found: " + m_objectName +". In Criteria " );
#endif
			}

		}

		if( m_criteriaType == VariableType )
		{
			var = query->getVariable( m_variableOrFunctionName );
		}
		else if( m_payloadType == VariablePayload )
		{
			var = object->m_variableList->getVariable( m_variableOrFunctionName );
		}
		else
		{
			var = object->m_functionTable->executeFunction( m_variableOrFunctionName, m_functionArgs );
		}
		bool criteriaMet = false;
		if( var != nullptr )
		{
			dataType = var->getDataType();
			switch( dataType )
			{
			case INT_DATA_TYPE:
				criteriaMet = evaluateCriteriaInt( static_cast< Variable<int>*>( var ), query );
#if 0
				
				if( criteriaMet )
				{
					trace( "Equel ints" );
				}
				else
				{
					trace( "Not equal ints" );
				}
#endif
				break;
			case BOOL_DATA_TYPE:
				criteriaMet = evaluateCriteriaBool( static_cast< Variable<bool>*>( var ), query );
				break;
			case FLOAT_DATA_TYPE:
				criteriaMet = evaluateCriteriaFloat( static_cast< Variable<float>*>( var ), query );
				break;
			case STRING_DATA_TYPE:
				criteriaMet = evaluateCriteriaString( static_cast< Variable<std::string>*>( var ), query );
#if 0
				if( criteriaMet )
				{
					trace( "Equel strings" );
				}
				else
				{
					trace( "Not equal strings" );
				}
#endif
				break;
			}
		}
		else
		{
			//trace("Var don't exist mayne");
			dataType = m_value.getPreferredDataType();
			switch( dataType )
			{
			case INT_DATA_TYPE:
				criteriaMet = evaluateBoolOperation( (int)0, m_value.evaluateAsInt(), m_operation );
				trace( "Int var not found: " << m_variableOrFunctionName );
				break;
			case BOOL_DATA_TYPE:
				//criteriaMet = evaluateBoolOperation( false, m_value.evaluateAsBool(), m_operation );
				trace("Bool var not found: " << m_variableOrFunctionName);
				break;
			case FLOAT_DATA_TYPE:
				criteriaMet = evaluateBoolOperation( 0.f, m_value.evaluateAsFloat(), m_operation );
				break;
			case STRING_DATA_TYPE:
				criteriaMet = evaluateBoolOperation( std::string(""), m_value.evaluateAsString(), m_operation );
				//trace("String var not found: " << m_variableOrFunctionName);
				break;
			}
		}
		return criteriaMet;
	}


	bool RuleCriteria::evaluateCriteriaInt( Variable<int>* variable, DatabaseQuery* query )
	{
		int value = m_value.evaluateAsInt();
		trace( "value as int: " << value );
		return evaluateBoolOperation( variable->getValue(), value, m_operation );
	}


	bool RuleCriteria::evaluateCriteriaBool( Variable<bool>* variable, DatabaseQuery* query )
	{
		bool value = m_value.evaluateAsBool();
		return evaluateBoolOperation( variable->getValue(), value, m_operation );
	}


	bool RuleCriteria::evaluateCriteriaFloat( Variable<float>* variable,DatabaseQuery* query )
	{
		float value = m_value.evaluateAsFloat();
		return evaluateBoolOperation( variable->getValue(), value, m_operation );
	}


	bool RuleCriteria::evaluateCriteriaString( Variable<std::string>* variable, DatabaseQuery* query )
	{
		std::string value = m_value.evaluateAsString();
#if 0
		trace( "Value: " << value );
		trace( "Var: " << variable->getValue() );
#endif
		return evaluateBoolOperation( variable->getValue(), value, m_operation );
	}
}
