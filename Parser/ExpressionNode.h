#pragma once
#ifndef EXPRESSIONNODE_H
#define EXPRESSIONNODE_H

#include "VariableTable.h"


namespace gh
{
	class MathematicExpression;
	class DatabaseObject;

	enum OperationType
	{
		addition_operation,
		subtraction_operation,
		multiplication_operation,
		division_operation,
		lessThan_operation,
		greaterThan_operation,
		equalEqual_operation,
		greaterThanOrEqual_operation,
		lessThanOrEqual_operation,
		not_operation,
		or_operation,
		and_operation,
		plusEqual_operation,
		negative_operation,
		no_operation
	};

	enum NodeType
	{
		constant_type,
		variable_type,
		operation_type,
		function_type
	};

	enum PayloadSourceType
	{
		variableTable_type,
		database_type
	};

	class ExpressionNode
	{
	public:
		ExpressionNode();
		ExpressionNode( OperationType opType, ExpressionNode* leftChild, ExpressionNode* rightChild, DataType operationResult = NULL_DATA_TYPE );
		ExpressionNode( const std::string& tableName );
		ExpressionNode( const std::string& databaseName, const std::string& databaseKey );
		ExpressionNode( VariableBase* var, DataType constType );

		void setVariableName(const std::string& variableName);
		void setFunctionName(const std::string& functionName, const std::string& args);
		DataType getPreferredType();
		bool getValueAsBool();
		int getValueAsInt();
		float getValueAsFloat();
		std::string getValueAsString();

		bool evaluateAsBool();
		int evaluateAsInt();
		float evaluateAsFloat();
		std::string evaluateAsString();

		template< typename T_DataType >
		void getValue( T_DataType& outValue );

		
		
		

		NodeType m_nodeType;
		DataType m_dataType;
		OperationType m_operationType;
		DataType m_constantType;
		PayloadSourceType m_sourceType;
		std::string m_variableName;
		std::string m_args;
		std::string m_tableName;
		std::string m_databaseName;
		std::string m_databaseKey;

		VariableBase* m_constant;
		//MathematicExpression* m_myExpression;
		ExpressionNode* m_leftChild;
		ExpressionNode* m_rightChild;
	private:
		VariableTable* getTable();
		DatabaseObject* getDatabaseObject();
	};

	namespace{
	// Global methods
	bool evaluateAnd( bool leftHandSide, bool rightHandSide )
	{
		return leftHandSide && rightHandSide;
	}

	bool evaluateOr( bool leftHandSide, bool rightHandSide )
	{
		return leftHandSide || rightHandSide;
	}

	bool evaluateNot( bool rightHandSide )
	{
		return !rightHandSide;
	}

	template< typename T_DataType >
	bool evaluateEqualsEquals( const T_DataType& leftHandSide, const T_DataType& rightHandSide )
	{
		return leftHandSide == rightHandSide;
	}

	template< typename T_DataType >
	bool evaluateGreaterThanOrEquals( const T_DataType& leftHandSide, const T_DataType& rightHandSide )
	{
		return leftHandSide >= rightHandSide;
	}

	template< typename T_DataType >
	bool evaluateLessThanOrEquals( const T_DataType& leftHandSide, const T_DataType& rightHandSide )
	{
		return leftHandSide <= rightHandSide;
	}

	template< typename T_DataType >
	bool evaluateGreaterThan( const T_DataType& leftHandSide, const T_DataType& rightHandSide )
	{
		return leftHandSide > rightHandSide;
	}

	template< typename T_DataType >
	bool evaluateLessThan( const T_DataType& leftHandSide, const T_DataType& rightHandSide )
	{
		return leftHandSide < rightHandSide;
	}

	template< typename T_DataType >
	T_DataType evaluateAddition( const T_DataType& leftHandSide, const T_DataType& rightHandSide )
	{
		return leftHandSide + rightHandSide;
	}

	template< typename T_DataType >
	T_DataType evaluateSubtraction( const T_DataType& leftHandSide, const T_DataType& rightHandSide )
	{
		return leftHandSide - rightHandSide;
	}

	template< typename T_DataType >
	T_DataType evaluateMultiplication( const T_DataType& leftHandSide, const T_DataType& rightHandSide )
	{
		return leftHandSide * rightHandSide;
	}

	template< typename T_DataType >
	T_DataType evaluateDivision( const T_DataType& leftHandSide, const T_DataType& rightHandSide )
	{
		return leftHandSide / rightHandSide;
	}

	}

	template< typename T_Datatype >
	static bool evaluateBoolOperation( const T_Datatype& lhs, const T_Datatype& rhs, OperationType op )
	{
		bool returnValue = false;
		switch( op )
		{
		case equalEqual_operation :
			returnValue = evaluateEqualsEquals( lhs, rhs );
			break;
		case greaterThan_operation :
			returnValue = evaluateGreaterThan( lhs, rhs );
			break;
		case greaterThanOrEqual_operation :
			returnValue = evaluateGreaterThanOrEquals( lhs, rhs );
			break;
		case lessThan_operation :
			returnValue = evaluateLessThan( lhs, rhs );
			break;
		case lessThanOrEqual_operation :
			returnValue = evaluateLessThanOrEquals( lhs, rhs );
			break;
		}

		return returnValue;
	}
}
#endif