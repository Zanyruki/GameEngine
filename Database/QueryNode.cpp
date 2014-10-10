#include "QueryNode.h"

namespace gh
{

	QueryNode::QueryNode()
		: m_leftChild( nullptr )
		, m_rightChild( nullptr )
	{

	}


	QueryNode::QueryNode( const Query& query )
		: m_query( query )
		, m_nodeType( constant_type )
	{

	}


	QueryNode::QueryNode( OperationType opType, QueryNode* leftChild, QueryNode* rightChild )
		: m_operationType( opType )
		, m_leftChild( leftChild )
		, m_rightChild( rightChild )
		, m_nodeType( operation_type )
	{

	}


	void QueryNode::evaluateQuery( ObjectList& outList )
	{
		if( m_nodeType == operation_type )
		{
			switch( m_operationType )
			{
			case and_operation :
				m_leftChild->evaluateQuery( outList );
				m_rightChild->evaluateQueryIntersection( outList );
				break;
			case or_operation :
				m_leftChild->evaluateQuery( outList );
				m_rightChild->evaluateQueryUnion( outList, m_leftChild->m_query );
				break;
			}
		}
		else if( m_nodeType == constant_type )
		{
			getThePropertyDatabase().populateObjectListFromQuery( m_query, outList );
		}
		
	}


	void QueryNode::evaluateQueryIntersection( ObjectList& outList )
	{
		if( m_nodeType == constant_type )
		{
			ObjectList newList;
			populateObjectListFromDatabaseQuery( m_query, newList, outList );
			outList = newList;
		}

		
	}


	
	void QueryNode::evaluateQueryUnion( ObjectList& outList, const Query& otherQuery )
	{
		if( m_nodeType == constant_type )
		{
			ObjectList newList;
			ObjectList listToAdd;
			populateObjectListFromDatabaseQuery( m_query, newList );
			for( auto newIter = newList.begin(); newIter != newList.end(); ++newIter )
			{
				DatabaseObject* object = *newIter;
				if( ! isObjectInObjectList( object, outList) )
				{
					listToAdd.push_back( object );
				}
			}

			outList.insert( outList.end(), listToAdd.begin(), listToAdd.end() );
			
		}
	}


	void evaluateQueryCompliment( ObjectList& outList );
}