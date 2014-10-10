#include "MathematicExpression.h"
#include "..\Utility\Utility.h"
#include "Variable.h"
#include "..\Parser\VariableTableManager.h"

namespace gh
{
	const int NO_OPERATION_PRIORITY = 100;

	void MathematicExpression::initializePriorityMap()
	{
		if( !s_bIsPriorityMapSet )
		{
			

			s_operationHashMap['='] = 1;
			s_operationHashMap['>'] = 1;
			s_operationHashMap['<'] = 1;
			s_operationHashMap['+'] = 1;
			s_operationHashMap['-'] = 1;
			s_operationHashMap['*'] = 1;
			s_operationHashMap['!'] = 1;
			s_operationHashMap['&'] = 1;
			s_operationHashMap['|'] = 1;
			s_operationHashMap['/'] = 1;
			s_operationHashMap['('] = 2;
			s_operationHashMap[')'] = 2;

			s_operationPriority["+="] = 0;
			s_operationPriority["||"] = 1;
			s_operationPriority["&&"] = 2;
			s_operationPriority["=="] = 3;
			s_operationPriority[">="] = 3;
			s_operationPriority["<="] = 3;
			s_operationPriority["<"] = 3;
			s_operationPriority[">"] = 3;
			
			s_operationPriority["+"] = 4;
			s_operationPriority["-"] = 4;
			s_operationPriority["*"] = 5;
			s_operationPriority["/"] = 5;
			s_operationPriority["!"] = 6;
			s_operationPriority["("] = 7;
			//s_operationPriority["("] = 4;

			s_operationTypeMap["=="] = equalEqual_operation;
			s_operationTypeMap[">="] = greaterThanOrEqual_operation;
			s_operationTypeMap["<="] = lessThanOrEqual_operation;
			s_operationTypeMap["<"] = lessThan_operation;
			s_operationTypeMap[">"] = greaterThan_operation;
			s_operationTypeMap["+"] = addition_operation;
			s_operationTypeMap["-"] = subtraction_operation;
			s_operationTypeMap["*"] = multiplication_operation;
			s_operationTypeMap["/"] = division_operation;
			s_operationTypeMap["!"] = not_operation;
			s_operationTypeMap["&&"] = and_operation;
			s_operationTypeMap["||"] = or_operation;
			s_operationTypeMap["+="] = plusEqual_operation;

			s_operationReternPreference[ equalEqual_operation ] = BOOL_DATA_TYPE;
			s_operationReternPreference[ greaterThanOrEqual_operation ] = BOOL_DATA_TYPE;
			s_operationReternPreference[ lessThanOrEqual_operation ] = BOOL_DATA_TYPE;
			s_operationReternPreference[ lessThan_operation ] = BOOL_DATA_TYPE;
			s_operationReternPreference[ greaterThan_operation ] = BOOL_DATA_TYPE;
			s_operationReternPreference[ addition_operation ] = NULL_DATA_TYPE;
			s_operationReternPreference[ subtraction_operation ] = NULL_DATA_TYPE;
			s_operationReternPreference[ multiplication_operation ] = NULL_DATA_TYPE;
			s_operationReternPreference[ division_operation ] = NULL_DATA_TYPE;
			s_operationReternPreference[ not_operation ] = BOOL_DATA_TYPE;
			s_operationReternPreference[ and_operation ] = BOOL_DATA_TYPE;
			s_operationReternPreference[ or_operation ] = BOOL_DATA_TYPE;
			s_operationReternPreference[ plusEqual_operation ] = NULL_DATA_TYPE;
			

			s_bIsPriorityMapSet = true;
		}
	}


	bool MathematicExpression::isOperation( char character )
	{
		int hashValue = s_operationHashMap[ character ];

		return hashValue > 0;

	}


	MathematicExpression::MathematicExpression( const std::string& expression )
	{
		initializePriorityMap();
		if( expression == "" )
		{
			Variable< std::string >* stringVar = new Variable< std::string >( "$nodeValue", "", STRING_DATA_TYPE );
			m_rootNode = new ExpressionNode( stringVar, STRING_DATA_TYPE );
		}
		else
		{
			m_rootNode = parseExpressionString( expression );
		}
		
	}


	ExpressionNode* MathematicExpression::parseExpressionString( const std::string& expression )
	{
		std::string expressionString = trim( expression );
		
		if( expressionString.size() > 0 && expressionString[0] == '(' && expressionString[ expressionString.size() - 1 ] == ')' )
		{
			return parseExpressionString( expressionString.substr( 1, expressionString.size() - 2 ) );
		}
		if( expression.size() > 0 && expressionString[0] == '\'' && expressionString[ expressionString.size() - 1 ] == '\'' )
		{
			expressionString = expressionString.substr( 1, expressionString.size() - 2 );
			return parseExpressionConstantOrVariable(expressionString);
		}
		int parinthesisCount = 0;
		int currentLeftChildEndIndex = 0;
		int currentRightChildBeginIndex = 0;
		int lowestLeftChildEndIndex = 0;
		int lowestRightChildBeginIndex = 0;
		int lowestPriority = NO_OPERATION_PRIORITY;
		std::string lowestPriorityOperatorString;
		ExpressionNode* node = nullptr;
		for( int charIndex = 0; charIndex < (int)expressionString.size(); ++charIndex )
		{
			char currentChar = expressionString[ charIndex ];
			
			if( parinthesisCount == 0 )
			{	
				if( currentChar == '(' )
				{
					++parinthesisCount;
				}
				else if( isOperation( currentChar ) && charIndex != expressionString.size() - 1 )
				{
					std::string operationString = "";
					operationString += currentChar;
					char nextChar = expressionString[ charIndex + 1 ];
					currentLeftChildEndIndex = charIndex;
					currentRightChildBeginIndex = charIndex + 1;

					if( isOperation( nextChar ) )
					{
						++charIndex;
						operationString += expressionString[ charIndex ];
						++currentRightChildBeginIndex;
					}

					int currentPriority = s_operationPriority[ operationString ];
					if( currentPriority < lowestPriority )
					{
						lowestPriority = currentPriority;
						lowestLeftChildEndIndex = currentLeftChildEndIndex;
						lowestRightChildBeginIndex = currentRightChildBeginIndex;
						lowestPriorityOperatorString = operationString;
					}
				}
				else if( currentChar == ')')
				{
					//TODO THROW ERROR
				}
			}
			else
			{
				if( currentChar == '(' )
				{
					++parinthesisCount;
				}
				else if( currentChar == ')' )
				{
					--parinthesisCount;
				}
			}

		}

		if( lowestPriority != NO_OPERATION_PRIORITY )
		{
			OperationType opType = s_operationTypeMap[ lowestPriorityOperatorString ];
			/*if( opType == not_operation )
			{

			}*/
			std::string leftChildString = expressionString.substr( 0, lowestLeftChildEndIndex );
			std::string righChildString = expressionString.substr( lowestRightChildBeginIndex );
			ExpressionNode* leftChild = parseExpressionString( leftChildString );
			ExpressionNode* rightChild = parseExpressionString( righChildString );

			ExpressionNode* operationNode = new ExpressionNode( opType, leftChild, rightChild, s_operationReternPreference[ opType ] );
			return operationNode;
		}

		return parseExpressionConstantOrVariable( expressionString );
	}


	ExpressionNode* MathematicExpression::parseExpressionConstantOrVariable( const std::string& dataString )
	{
		if( dataString.size() > 0 )
		{
			char dataChar = dataString[0];
			ExpressionNode* variableNode = nullptr;
			if( dataChar == '$' )
			{
				
				int tableNameIndex = dataString.find_first_of(".");
				if( tableNameIndex != std::string::npos )
				{
					std::string tableName = dataString.substr( 0, tableNameIndex - 1 );
					std::string variableName = dataString.substr(tableNameIndex + 1);
					int tableParinthesisIndex = tableName.find_first_of('(');
					if (tableParinthesisIndex != std::string::npos) //Database type
					{
						std::string databaseName = tableName.substr(1, tableParinthesisIndex - 1);
						int endParinthesisIndex = tableName.find_last_of(')');
						std::string databaseKey = tableName.substr(tableParinthesisIndex + 1, endParinthesisIndex - tableParinthesisIndex - 1);
						variableNode = new ExpressionNode(databaseName, databaseKey);
					}
					else
					{
						variableNode = new ExpressionNode(tableName);
					}

					int functionParinthesisIndex = variableName.find_first_of('(');
					if (functionParinthesisIndex != std::string::npos)
					{
						//function stuff
						std::string functionName = variableName.substr(0, functionParinthesisIndex);
						int endParinthesisIndex = variableName.find_last_of(')');
						std::string args = variableName.substr(functionParinthesisIndex + 1, endParinthesisIndex - functionParinthesisIndex - 1);
						variableNode->setFunctionName(functionName, args);
					}
					else
					{
						variableNode->setVariableName(variableName);
					}
				}
				else
				{
					variableNode = new ExpressionNode( dataString, "local" );
				}
				
				return variableNode;
			}
			else if( isCharANumber( dataChar ) )
			{
				if( dataString.find('.') != std::string::npos )
				{
					float dataFloat = toFloat( dataString );
					Variable< float >* floatVar = new Variable< float >( "$nodeValue", dataFloat, FLOAT_DATA_TYPE );
					ExpressionNode* floatNode = new ExpressionNode( floatVar, FLOAT_DATA_TYPE );
					return floatNode;
				}
				else
				{
					int dataInt = toInt( dataString );
					Variable< int >* intVar = new Variable< int >( "$nodeValue", dataInt, INT_DATA_TYPE );
					ExpressionNode* intNode = new ExpressionNode( intVar, INT_DATA_TYPE );
					return intNode;
				}
			}
			else if( dataString == "true" )
			{
				Variable< bool >* boolVar = new Variable< bool >( "$nodeValue", true, BOOL_DATA_TYPE );
				ExpressionNode* boolNode = new ExpressionNode( boolVar, BOOL_DATA_TYPE );
				return boolNode;
			}
			else if( dataString == "false" )
			{
				Variable< bool >* boolVar = new Variable< bool >( "$nodeValue", false, BOOL_DATA_TYPE );
				ExpressionNode* boolNode = new ExpressionNode( boolVar, BOOL_DATA_TYPE );
				return boolNode;
			}
			else
			{
				Variable< std::string >* stringVar = new Variable< std::string >( "$nodeValue", dataString, STRING_DATA_TYPE );
				ExpressionNode* stringNode = new ExpressionNode( stringVar, STRING_DATA_TYPE );
				return stringNode;
			}
		}
		
		return nullptr;
	}


	void MathematicExpression::setLocalVariableTable( const std::string& localTableName )
	{
		getTheVariableTableManager().setLocalTable( localTableName );
	}


	bool MathematicExpression::evaluateAsBool()
	{
		if( m_rootNode != nullptr )
		{
			return m_rootNode->evaluateAsBool();
		}
		return false;
	}


	int MathematicExpression::evaluateAsInt( Variable<int>* intVar )
	{
		if( m_rootNode != nullptr )
		{
			if( intVar != nullptr )
			{
				if( m_rootNode->m_leftChild == nullptr )
				{
					ExpressionNode* leftNode = new ExpressionNode( intVar, INT_DATA_TYPE );
					m_rootNode->m_leftChild = leftNode;
				}
			}
			return m_rootNode->evaluateAsInt();
		}
		return false;
	}

	float MathematicExpression::evaluateAsFloat()
	{
		if( m_rootNode != nullptr )
		{
			return m_rootNode->evaluateAsFloat();
		}
		return false;
	}


	std::string MathematicExpression::evaluateAsString()
	{
		if( m_rootNode != nullptr )
		{
			return m_rootNode->evaluateAsString();
		}
		return false;
	}


	VariableBase* MathematicExpression::evaluateBasedOffPreference()
	{
		DataType preferredType = getPreferredDataType();
		if (preferredType == INT_DATA_TYPE)
		{
			Variable<int>* var = new Variable<int>("var", evaluateAsInt());
			return var;
		}
		else if (preferredType == FLOAT_DATA_TYPE)
		{
			Variable<float>* var = new Variable<float>(evaluateAsFloat());
			return var;
		}
		else if (preferredType == BOOL_DATA_TYPE)
		{
			Variable<bool>* var = new Variable<bool>(evaluateAsBool());
			return var;
		}
		else if (preferredType == STRING_DATA_TYPE)
		{
			Variable<std::string>* var = new Variable<std::string>(evaluateAsString());
			return var;
		}
		else
		{
			reportError("Something messed up in evaluateBasedOffPreference");
		}
	}


	DataType MathematicExpression::getPreferredDataType()
	{
		return m_rootNode->getPreferredType();
	}

	std::map< std::string, OperationType > MathematicExpression::s_operationTypeMap;
	int* MathematicExpression::s_operationHashMap = new int[256];
	bool MathematicExpression::s_bIsPriorityMapSet = false;
	std::map< std::string, int > MathematicExpression::s_operationPriority;
	std::map< OperationType, DataType > MathematicExpression::s_operationReternPreference;

}