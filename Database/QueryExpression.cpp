#include "QueryExpression.h"

namespace gh
{
	const int NO_OPERATION_PRIORITY = 100;
	void QueryExpression::initializePriorityMap()
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

			/*s_operationReternPreference[ equalEqual_operation ] = BOOL_DATA_TYPE;
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
			s_operationReternPreference[ or_operation ] = BOOL_DATA_TYPE;*/


			s_bIsPriorityMapSet = true;
		}
	}


QueryExpression::QueryExpression( const std::string& expression,  const QueryList& listOfQueries )
	: m_listOfQueries( listOfQueries )
{
	initializePriorityMap();
	if( expression == "" )
	{
		m_rootNode = new QueryNode( );
	}
	else
	{
		m_rootNode = parseQueryString( expression );
	}
}

QueryNode* QueryExpression::parseQueryString( const std::string& expression )
{
	std::string expressionString = trim( expression );
	if( expressionString.size() > 0 && expressionString[0] == '(' && expressionString[ expressionString.size() - 1 ] == ')' )
	{
		return parseQueryString( expressionString.substr( 1, expressionString.size() - 2 ) );
	}
	if( expression.size() > 0 && expressionString[0] == '\'' && expressionString[ expressionString.size() - 1 ] == '\'' )
	{
		expressionString = expressionString.substr( 1, expressionString.size() - 2 );
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
		QueryNode* leftChild = parseQueryString( leftChildString );
		QueryNode* rightChild = parseQueryString( righChildString );

		QueryNode* operationNode = new QueryNode( opType, leftChild, rightChild );
		return operationNode;
	}

	return parseQueryConstantOrVariable( expressionString );
}


QueryNode* QueryExpression::parseQueryConstantOrVariable( const std::string& dataString )
{
	//TODO
	if( dataString[0] == 'q' || dataString[0] == 'Q' )
	{
		std::string number = dataString.substr( 1 );
		int listIndex = convertStringToInt( number );
		return new QueryNode( m_listOfQueries[ listIndex ] );
	}
	return nullptr;
}


bool QueryExpression::isOperation( char character )
{
	int hashValue = s_operationHashMap[ character ];

	return hashValue > 0;

}


void QueryExpression::evaluateQuery( ObjectList& outList )
{
	m_rootNode->evaluateQuery( outList );
}

std::map< std::string, OperationType > QueryExpression::s_operationTypeMap;
int* QueryExpression::s_operationHashMap = new int[256];
bool QueryExpression::s_bIsPriorityMapSet = false;
std::map< std::string, int > QueryExpression::s_operationPriority;

}