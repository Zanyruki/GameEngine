#pragma once
#include <string>
#include "ExpressionNode.h"
#include "VariableTable.h"
#include <map>

namespace gh
{
	class MathematicExpression
	{
	public:
		MathematicExpression(const std::string& expression);

		void setLocalVariableTable(const std::string& localTableName);
		bool evaluateAsBool();
		int evaluateAsInt(Variable<int>* intVar = nullptr);
		float evaluateAsFloat();
		std::string evaluateAsString();

		int evaluate(int type)
		{
			return evaluateAsInt();
		}

		bool evaluate(bool type)
		{
			return evaluateAsBool();
		}

		float evaluate(float type)
		{
			return evaluateAsFloat();
		}

		std::string evaluate(const std::string& type)
		{
			return evaluateAsString();
		}

		template< typename T_ReturnType >
		T_ReturnType evaluateBasedOffTemplate(Variable< T_ReturnType >* variableType )
		{
			switch (variableType->m_dataType)
			{
			case INT_DATA_TYPE :
				return evaluateAsInt();
				break;
			case STRING_DATA_TYPE :
				return evaluateAsString();
				berak;
			case FLOAT_DATA_TYPE :
				return evaluateAsFloat();
				break;
			case BOOL_DATA_TYPE :
				return evaluateAsBool();
				break;
			}
			return variableType->m_value;
		}

		template< typename T_ReturnType >
		T_ReturnType evaluateBasedOffTemplate(T_ReturnType type)
		{
			return evaluate(type);
		}

		DataType getPreferredDataType();

		VariableBase* evaluateBasedOffPreference();


		ExpressionNode* m_rootNode;

		static std::map< std::string, OperationType > s_operationTypeMap;
	private:
		ExpressionNode* parseExpressionString( const std::string& expression );
		ExpressionNode* parseExpressionConstantOrVariable( const std::string& dataString );
		bool isOperation( char character );
		static void initializePriorityMap();
		static bool s_bIsPriorityMapSet;
		static std::map< std::string, int > s_operationPriority;
		
		static std::map< OperationType, DataType > MathematicExpression::s_operationReternPreference;
		static int* s_operationHashMap;

	};
}