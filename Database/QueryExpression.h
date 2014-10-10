#pragma once
#include "QueryNode.h"
#include "DatabaseObject.h"
#include <map>

namespace gh
{

class QueryExpression
{
public :
	QueryExpression( const std::string& expression, const QueryList& listOfQueries );
	bool isOperation( char character );
	void evaluateQuery( ObjectList& outList );
private:
	static void initializePriorityMap();
	QueryNode* parseQueryString( const std::string& expression );
	QueryNode* parseQueryConstantOrVariable( const std::string& dataString );
	static bool s_bIsPriorityMapSet;
	static std::map< std::string, int > s_operationPriority;
	static std::map< std::string, OperationType > s_operationTypeMap;
	QueryNode* m_rootNode;
	QueryList m_listOfQueries;
	static int* s_operationHashMap;
};

}