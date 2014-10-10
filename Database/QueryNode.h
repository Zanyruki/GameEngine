#pragma once
#include <string>
#include "..\Parser\ExpressionNode.h"
#include "DatabaseObject.h"
#include "Database.h"
namespace gh
{

class QueryNode //: public ExpressionNode
{
public :
	QueryNode();
	QueryNode( const Query& query );
	QueryNode( OperationType opType, QueryNode* leftChild, QueryNode* rightChild );

	void evaluateQuery( ObjectList& outList );
	void evaluateQueryIntersection( ObjectList& outList );
	void evaluateQueryUnion( ObjectList& outList, const Query& otherQuery );
	void evaluateQueryCompliment( ObjectList& outList );

	NodeType m_nodeType;
	DataType m_dataType;
	OperationType m_operationType;
	DataType m_constantType;
	std::string m_variableName;
	std::string m_tableName;
	Query m_query;
	//MathematicExpression* m_myExpression;
	QueryNode* m_leftChild;
	QueryNode* m_rightChild;
private:
};

}