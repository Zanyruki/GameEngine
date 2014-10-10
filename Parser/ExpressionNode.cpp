#include "ExpressionNode.h"
#include "MathematicExpression.h"
#include "VariableTableManager.h"
#include "..\Database\ScoreBasedDatabase.h"
#include "..\Database\DatabaseObject.h"

namespace gh
{


ExpressionNode::ExpressionNode()
{
}


ExpressionNode::ExpressionNode( OperationType opType, ExpressionNode* leftChild, ExpressionNode* rightChild, DataType operationResult )
	: m_nodeType( operation_type )
	, m_operationType( opType )
	, m_constantType( operationResult )
	, m_leftChild( leftChild )
	, m_rightChild( rightChild )
{
}


ExpressionNode::ExpressionNode( const std::string& tableName )
	: m_sourceType(variableTable_type)
	, m_tableName( tableName )
{
	trace("VariableTypeMade");
}


ExpressionNode::ExpressionNode(const std::string& databaseName, const std::string& databaseKey)
	: m_sourceType(database_type)
	, m_databaseName(databaseName)
	, m_databaseKey(databaseKey)
{
	trace("DatabaseTypeMade");
}

ExpressionNode::ExpressionNode( VariableBase* var, DataType constType )
	: m_nodeType( constant_type )
	, m_constant( var )
	, m_constantType( constType )
{
	trace("ConstantTypeMade");
}


void ExpressionNode::setVariableName(const std::string& variableName)
{
	m_variableName = variableName;
	m_nodeType = variable_type;
}


void ExpressionNode::setFunctionName(const std::string& functionName, const std::string& args)
{
	m_variableName = functionName; //TODO get better name
	m_args = args;
	m_nodeType = function_type;
}


VariableTable* ExpressionNode::getTable()
{		
	return getTheVariableTableManager().getTable( m_tableName );
}

DatabaseObject* ExpressionNode::getDatabaseObject()
{
	return getObjectByName(m_databaseName, m_databaseKey);
}


DataType ExpressionNode::getPreferredType()
{
	if( m_nodeType == operation_type )
	{
		if( m_constantType == NULL_DATA_TYPE )
		{
			if( m_leftChild != nullptr )
			{
				return m_leftChild->getPreferredType();
			}
			else
			{
				return m_rightChild->getPreferredType();
			}
		}
		else
		{
			return m_constantType;
		}
	}
	else if( m_nodeType == variable_type )
	{
		VariableBase* varBase = nullptr;
		if (m_sourceType == variableTable_type)
		{
			VariableTable* table = getTable();
			if( table != nullptr )
			{
				varBase = table->getVariable( m_variableName );
				
			}
		}
		else
		{
			DatabaseObject* object = getDatabaseObject();
			if (object != nullptr)
			{
				varBase = object->m_variableList->getVariable(m_variableName);
			}
		}
		if (varBase != nullptr)
		{
			return varBase->getDataType();
		}
		return NULL_DATA_TYPE;
	}
	else if(m_nodeType == function_type)
	{
		ScriptFunctionBase* function = nullptr;
		if (m_sourceType == database_type)
		{
			DatabaseObject* object = getDatabaseObject();
			if (object != nullptr)
			{
				return object->m_functionTable->getReturnType(m_variableName);
			}
		}
	}
	else
	{
		return m_constantType;
	}
}


bool ExpressionNode::evaluateAsBool()
{
	DataType prefferedType = getPreferredType();
	if( prefferedType == BOOL_DATA_TYPE )
	{
		return getValueAsBool();
	}
	else if( prefferedType == INT_DATA_TYPE )
	{
		int intValue = getValueAsInt();
		return intValue != 0;
	}
	else if( prefferedType == FLOAT_DATA_TYPE )
	{
		float floatValue = getValueAsFloat();
		return floatValue != 0.f;
	}
	else if( prefferedType == STRING_DATA_TYPE )
	{
		std::string stringValue = getValueAsString();
		return stringValue == "true";
	}
	return false;
}


int ExpressionNode::evaluateAsInt()
{
	DataType prefferedType = getPreferredType();
	if( prefferedType == INT_DATA_TYPE )
	{
		return getValueAsInt();
	}
	else if( prefferedType == BOOL_DATA_TYPE )
	{
		return (int)getValueAsBool();
	}
	else if( prefferedType == FLOAT_DATA_TYPE )
	{
		return (int)getValueAsFloat();
	}
	else if( prefferedType == STRING_DATA_TYPE )
	{
		return atoi( getValueAsString().c_str() );
	}
	return 0;
}


float ExpressionNode::evaluateAsFloat()
{
	DataType prefferedType = getPreferredType();
	if( prefferedType == FLOAT_DATA_TYPE )
	{
		return getValueAsFloat();
	}
	else if( prefferedType == BOOL_DATA_TYPE )
	{
		return (float)getValueAsBool();
	}
	else if( prefferedType == INT_DATA_TYPE )
	{
		return (float)getValueAsInt();
	}
	else if( prefferedType == STRING_DATA_TYPE );
	{
		return (float)atof( getValueAsString().c_str() );
	}

	return 0.f;
}


std::string ExpressionNode::evaluateAsString()
{
	DataType prefferedType = getPreferredType();
	if( prefferedType == STRING_DATA_TYPE )
	{
		return getValueAsString();
	}
	else if( prefferedType == BOOL_DATA_TYPE )
	{
		if( getValueAsBool() )
		{
			return "true";
		}
		return "false";
	}
	else if( prefferedType == INT_DATA_TYPE )
	{
		std::string returnString;
		convertToString( getValueAsInt(), returnString );
		return returnString;
	}
	else if( prefferedType == FLOAT_DATA_TYPE )
	{
		std::string returnString;
		convertToString( getValueAsFloat(), returnString );
		return returnString;
	}
	return "";
}


bool ExpressionNode::getValueAsBool()
{
	if( m_nodeType == constant_type )
	{
		Variable< bool >* boolVar = reinterpret_cast< Variable< bool >* >( m_constant );
		if( boolVar != nullptr )
		{
			return boolVar->m_value;
		}
	}
	else if ( m_nodeType == variable_type )
	{
		VariableTable* varTable = nullptr;
		if (m_sourceType == variableTable_type)
		{
			varTable = getTable();
		}
		else
		{
			DatabaseObject* object = getDatabaseObject();
			if (object != nullptr)
			{
				varTable = object->m_variableList;
			}
		}
		
		if( varTable != nullptr )
		{
			bool returnBool;
			if( varTable->getVariable( m_variableName, returnBool, BOOL_DATA_TYPE ) )
			{
				return returnBool;
			}
			
		}
	}
	else if (m_nodeType == function_type)
	{
		if (m_sourceType == database_type)
		{
			DatabaseObject* object = getDatabaseObject();
			if (object != nullptr)
			{
				Variable< bool >* boolVar = static_cast< Variable<bool>* >(object->m_functionTable->executeFunction(m_variableName, m_args));
				if (boolVar != nullptr)
				{
					return boolVar->getValue();
				}
			}
		}
	}
	else
	{
		if( m_operationType == equalEqual_operation )
		{
			if( m_leftChild != nullptr )
			{
				DataType childrenType = m_leftChild->getPreferredType();
				switch( childrenType )
				{
				case BOOL_DATA_TYPE:
					return evaluateEqualsEquals( m_leftChild->evaluateAsBool(), m_rightChild->evaluateAsBool() );
					break;
				case INT_DATA_TYPE:
					return evaluateEqualsEquals( m_leftChild->evaluateAsInt(), m_rightChild->evaluateAsInt() );
					break;
				case FLOAT_DATA_TYPE:
					return evaluateEqualsEquals( m_leftChild->evaluateAsFloat(), m_rightChild->evaluateAsFloat() );
					break;
				case STRING_DATA_TYPE:
					return evaluateEqualsEquals( m_leftChild->evaluateAsString(), m_rightChild->evaluateAsString() );
					break;
				}
			}
		}
		else if( m_operationType == greaterThanOrEqual_operation )
		{
			if( m_leftChild != nullptr )
			{
				DataType childrenType = m_leftChild->getPreferredType();
				switch( childrenType )
				{
				case BOOL_DATA_TYPE:
					return evaluateGreaterThanOrEquals( m_leftChild->evaluateAsBool(), m_rightChild->evaluateAsBool() );
					break;
				case INT_DATA_TYPE:
					return evaluateGreaterThanOrEquals( m_leftChild->evaluateAsInt(), m_rightChild->evaluateAsInt() );
					break;
				case FLOAT_DATA_TYPE:
					return evaluateGreaterThanOrEquals( m_leftChild->evaluateAsFloat(), m_rightChild->evaluateAsFloat() );
					break;
				case STRING_DATA_TYPE:
					return evaluateGreaterThanOrEquals( m_leftChild->evaluateAsString(), m_rightChild->evaluateAsString() );
					break;
				}
			}
		}
		else if( m_operationType == greaterThan_operation )
		{
			if( m_leftChild != nullptr )
			{
				DataType childrenType = m_leftChild->getPreferredType();
				switch( childrenType )
				{
				case BOOL_DATA_TYPE:
					return evaluateGreaterThan( m_leftChild->evaluateAsBool(), m_rightChild->evaluateAsBool() );
					break;
				case INT_DATA_TYPE:
					return evaluateGreaterThan( m_leftChild->evaluateAsInt(), m_rightChild->evaluateAsInt() );
					break;
				case FLOAT_DATA_TYPE:
					return evaluateGreaterThan( m_leftChild->evaluateAsFloat(), m_rightChild->evaluateAsFloat() );
					break;
				case STRING_DATA_TYPE:
					return evaluateGreaterThan( m_leftChild->evaluateAsString(), m_rightChild->evaluateAsString() );
					break;
				}
			}
		}
		else if( m_operationType == lessThan_operation )
		{
			if( m_leftChild != nullptr )
			{
				DataType childrenType = m_leftChild->getPreferredType();
				switch( childrenType )
				{
				case BOOL_DATA_TYPE:
					return evaluateLessThan( m_leftChild->evaluateAsBool(), m_rightChild->evaluateAsBool() );
					break;
				case INT_DATA_TYPE:
					return evaluateLessThan( m_leftChild->evaluateAsInt(), m_rightChild->evaluateAsInt() );
					break;
				case FLOAT_DATA_TYPE:
					return evaluateLessThan( m_leftChild->evaluateAsFloat(), m_rightChild->evaluateAsFloat() );
					break;
				case STRING_DATA_TYPE:
					return evaluateLessThan( m_leftChild->evaluateAsString(), m_rightChild->evaluateAsString() );
					break;
				}
			}
		}
		else if( m_operationType == lessThanOrEqual_operation )
		{
			if( m_leftChild != nullptr )
			{
				DataType childrenType = m_leftChild->getPreferredType();
				switch( childrenType )
				{
				case BOOL_DATA_TYPE:
					return evaluateLessThanOrEquals( m_leftChild->evaluateAsBool(), m_rightChild->evaluateAsBool() );
					break;
				case INT_DATA_TYPE:
					return evaluateLessThanOrEquals( m_leftChild->evaluateAsInt(), m_rightChild->evaluateAsInt() );
					break;
				case FLOAT_DATA_TYPE:
					return evaluateLessThanOrEquals( m_leftChild->evaluateAsFloat(), m_rightChild->evaluateAsFloat() );
					break;
				case STRING_DATA_TYPE:
					return evaluateLessThanOrEquals( m_leftChild->evaluateAsString(), m_rightChild->evaluateAsString() );
					break;
				}
			}
		}
		else if( m_operationType == and_operation )
		{
			return evaluateAnd( m_leftChild->evaluateAsBool(), m_rightChild->evaluateAsBool() );
		}
		else if( m_operationType == or_operation )
		{
			return evaluateOr( m_leftChild->evaluateAsBool(), m_rightChild->evaluateAsBool() );
		}
		else if( m_operationType == not_operation )
		{
			return evaluateNot( m_rightChild->evaluateAsBool() );
		}
	}

	return false;
}


int ExpressionNode::getValueAsInt()
{
	if( m_nodeType == constant_type )
	{
		Variable< int >* intVar = reinterpret_cast< Variable< int >* >( m_constant );
		if( intVar != nullptr )
		{
			return intVar->m_value;
		}
	}
	else if ( m_nodeType == variable_type )
	{
		VariableTable* varTable = nullptr;
		if (m_sourceType == variableTable_type)
		{
			varTable = getTable();
		}
		else
		{
			DatabaseObject* object = getDatabaseObject();
			if (object != nullptr)
			{
				varTable = object->m_variableList;
			}
		}

		if( varTable != nullptr )
		{
			int returnInt;
			if( varTable->getVariable( m_variableName, returnInt, INT_DATA_TYPE ) )
			{
				return returnInt;
			}
		}
	}
	else if (m_nodeType == function_type)
	{
		if (m_sourceType == database_type)
		{
			DatabaseObject* object = getDatabaseObject();
			if (object != nullptr)
			{
				Variable< int >* intVar = static_cast< Variable<int>* >(object->m_functionTable->executeFunction(m_variableName, m_args));
				if (intVar != nullptr)
				{
					return intVar->getValue();
				}
			}
		}
	}
	else
	{
		if( m_operationType == addition_operation )
		{
			if( m_leftChild != nullptr )
			{
				return evaluateAddition( m_leftChild->evaluateAsInt(), m_rightChild->evaluateAsInt() );
			}
		}
		else if( m_operationType == multiplication_operation )
		{
			if( m_leftChild != nullptr )
			{
				return evaluateMultiplication( m_leftChild->evaluateAsInt(), m_rightChild->evaluateAsInt() );
			}
		}
		else if( m_operationType == subtraction_operation )
		{
			if( m_leftChild != nullptr )
			{
				return evaluateSubtraction( m_leftChild->evaluateAsInt(), m_rightChild->evaluateAsInt() );
			}
		}
		else if( m_operationType == division_operation )
		{
			if( m_leftChild != nullptr )
			{
				return evaluateDivision( m_leftChild->evaluateAsInt(), m_rightChild->evaluateAsInt() );
			}
		}
		else if( m_operationType == plusEqual_operation )
		{
			if( m_leftChild != nullptr )
			{
				return evaluateAddition( m_leftChild->evaluateAsInt(), m_rightChild->evaluateAsInt() );
			}
			else
			{
				return evaluateAddition( 0, m_rightChild->evaluateAsInt() );
			}
		}
	}
	return 0;
}


float ExpressionNode::getValueAsFloat()
{
	if( m_nodeType == constant_type )
	{
		Variable< float >* floatVar = reinterpret_cast< Variable< float >* >( m_constant );
		if( floatVar != nullptr )
		{
			return floatVar->m_value;
		}
	}
	else if ( m_nodeType == variable_type )
	{
		VariableTable* varTable = nullptr;
		if (m_sourceType == variableTable_type)
		{
			varTable = getTable();
		}
		else
		{
			DatabaseObject* object = getDatabaseObject();
			if (object != nullptr)
			{
				varTable = object->m_variableList;
			}
		}

		if( varTable != nullptr )
		{
			float returnFloat;
			if( varTable->getVariable( m_variableName, returnFloat, FLOAT_DATA_TYPE ) )
			{
				return returnFloat;
			}
		}
	}
	else if (m_nodeType == function_type)
	{
		if (m_sourceType == database_type)
		{
			DatabaseObject* object = getDatabaseObject();
			if (object != nullptr)
			{
				Variable< float >* floatVar = static_cast< Variable<float>* >(object->m_functionTable->executeFunction(m_variableName, m_args));
				if (floatVar != nullptr)
				{
					return floatVar->getValue();
				}
			}
		}
	}
	else
	{
		if( m_operationType == addition_operation )
		{
			if( m_leftChild != nullptr )
			{
				return evaluateAddition( m_leftChild->evaluateAsFloat(), m_rightChild->evaluateAsFloat() );
			}
		}
		else if( m_operationType == multiplication_operation )
		{
			if( m_leftChild != nullptr )
			{
				return evaluateMultiplication( m_leftChild->evaluateAsFloat(), m_rightChild->evaluateAsFloat() );
			}
		}
		else if( m_operationType == subtraction_operation )
		{
			if( m_leftChild != nullptr )
			{
				return evaluateSubtraction( m_leftChild->evaluateAsFloat(), m_rightChild->evaluateAsFloat() );
			}
		}
		else if( m_operationType == division_operation )
		{
			if( m_leftChild != nullptr )
			{
				return evaluateDivision( m_leftChild->evaluateAsFloat(), m_rightChild->evaluateAsFloat() );
			}
		}
	}
	return 0.f;
}


std::string ExpressionNode::getValueAsString()
{
	if( m_nodeType == constant_type )
	{
		Variable< std::string >* stringVar = reinterpret_cast< Variable< std::string >* >( m_constant );
		if( stringVar != nullptr )
		{
			return stringVar->m_value;
		}
	}
	else if ( m_nodeType == variable_type )
	{
		VariableTable* varTable = nullptr;
		if (m_sourceType == variableTable_type)
		{
			varTable = getTable();
		}
		else
		{
			DatabaseObject* object = getDatabaseObject();
			if (object != nullptr)
			{
				varTable = object->m_variableList;
			}
		}

		if( varTable != nullptr )
		{
			std::string returnString;
			if( varTable->getVariable( m_variableName, returnString, STRING_DATA_TYPE ) )
			{
				return returnString;
			}
		}
	}
	else if (m_nodeType == function_type)
	{
		if (m_sourceType == database_type)
		{
			DatabaseObject* object = getDatabaseObject();
			if (object != nullptr)
			{
				Variable< std::string>* stringVar = static_cast< Variable<std::string>* >( object->m_functionTable->executeFunction(m_variableName, m_args) );
				if (stringVar != nullptr)
				{
					return stringVar->getValue();
				}
			}
		}
	}
	else
	{
		if( m_operationType == addition_operation )
		{
			if( m_leftChild != nullptr )
			{
				return evaluateAddition( m_leftChild->evaluateAsString(), m_rightChild->evaluateAsString() );
			}
		}
	}
	return "";
}


}
