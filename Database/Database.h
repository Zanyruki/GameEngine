#pragma once
#include <vector>
#include <string>
#include "DatabaseObject.h"
#include "Query.h"
#include "..\Utility\Utility.h"

namespace gh
{
	

	typedef std::vector< DatabaseObject* > ObjectList;
	
	class Database
	{
	public :
		Database();
		
		void populateObjectListFromQuery( const Query& query, ObjectList& outList, const ObjectList& inputList = m_objectList );
		void populateObjectListOppositeFromQuery( const Query& query, ObjectList& outList, const ObjectList& inputList = m_objectList );

		void sortObjectListAscendingByProperty( ObjectList& listToSort, const std::string& property );
		void sortObjectListDescendingByProperty( ObjectList& listToSort, const std::string& property );

		DatabaseObject* getObjectBasedOnScore();
		DatabaseObject* getObjectFromQuery( const Query& query, const ObjectList& inputList = m_objectList );
		DatabaseObject* getObjectFromQueryWithLowestAttribute( const Query& query, const std::string& propertyToSortBy, const ObjectList& inputList = m_objectList );
		DatabaseObject* getObjectFromQueryWithHighestAttribute( const Query& query, const std::string& propertyToSortBy, const ObjectList& inputList = m_objectList );
		DatabaseObject* getObjectFromMultipleQueries( const std::string& statement, const QueryList& listOfQueries );
		DatabaseObject* getObjectFromMultipleQueriesWithHighestAttribute( const std::string& statement, const QueryList& listOfQueries );
		DatabaseObject* getObjectFromMultipleQueriesWithLowestAttribute( const std::string& statement, const QueryList& listOfQueries );

		DatabaseObject* getRandomObjectWithProperty( const std::string& propertyName );
		DatabaseObject* getRandomObjectWithoutProperty( const std::string& propertyName );

		template< typename T_Datatype >
		void getAllObjectsWithPropertyThatMatchesValue( ObjectList& outList, const std::string& propertyName, T_Datatype valueToMatch, DataType dataType, OperationType operation = equalEqual_operation )
		{
			getAllObjectsWithPropertyThatMatchesValue( outList, m_objectList, propertyName, valueToMatch, dataType, operation );
		}


		template< typename T_Datatype >
		void getAllObjectsWithPropertyThatMatchesValue( ObjectList& outList, const ObjectList& inputList, const std::string& propertyName, T_Datatype valueToMatch, DataType dataType, OperationType operation = equalEqual_operation )
		{
			ObjectList objectsWithProperty;
			if( getAllObjectsWithProperty( propertyName, objectsWithProperty, inputList ) )
			{
				T_Datatype tempVar;
				for( auto iter = objectsWithProperty.begin(); iter != objectsWithProperty.end(); ++iter )
				{
					if( (*iter)->m_variableList->getVariable( propertyName, tempVar, dataType) )
					{	
						if( evaluateBoolOperation( tempVar, valueToMatch, operation ) )
						{
							outList.push_back( *iter );
						}

					}
				}
			}
		}
		template<>
		void getAllObjectsWithPropertyThatMatchesValue( ObjectList& outList, const ObjectList& inputList, const std::string& propertyName, char* valueToMatch, DataType dataType, OperationType operation )
		{
			std::string stringValue = valueToMatch;
			getAllObjectsWithPropertyThatMatchesValue( outList, inputList, propertyName, stringValue, dataType, operation );
		}
		template<>
		void getAllObjectsWithPropertyThatMatchesValue( ObjectList& outList, const ObjectList& inputList, const std::string& propertyName, const char* valueToMatch, DataType dataType, OperationType operation )
		{
			std::string stringValue = valueToMatch;
			getAllObjectsWithPropertyThatMatchesValue( outList, inputList, propertyName, stringValue, dataType, operation );
		}


		template< typename T_Datatype >
		DatabaseObject* getObjectWithPropertyThatMatchesValue( const std::string& propertyName, T_Datatype valueToMatch, DataType dataType, OperationType operation = equalEqual_operation )
		{
			ObjectList objectsWithProperty;
			if( getAllObjectsWithProperty( propertyName, objectsWithProperty ) )
			{
				T_Datatype tempVar;
				ObjectList matchingList;
				for( auto iter = objectsWithProperty.begin(); iter != objectsWithProperty.end(); ++iter )
				{
					if( (*iter)->m_variableList->getVariable( propertyName, tempVar, dataType) )
					{	
						if( evaluateBoolOperation( tempVar, valueToMatch, operation ) )
						{
							matchingList.push_back( *iter );
						}
						
					}
				}

				if( matchingList.size() > 0 )
				{
					return matchingList.front();
				}
				else
				{
					return NULL;
				}
			}
			else
			{
				return NULL;
			}
		}
		template<>
		DatabaseObject* getObjectWithPropertyThatMatchesValue( const std::string& propertyName, char* valueToMatch, DataType dataType, OperationType operation )
		{
			std::string stringValue = valueToMatch;
			return getObjectWithPropertyThatMatchesValue( propertyName, stringValue, dataType, operation );
		}
		template<>
		DatabaseObject* getObjectWithPropertyThatMatchesValue( const std::string& propertyName, const char* valueToMatch, DataType dataType, OperationType operation )
		{
			std::string stringValue = valueToMatch;
			return getObjectWithPropertyThatMatchesValue( propertyName, stringValue, dataType, operation );
		}


		template< typename T_Datatype >
		void getAllObjectsWithPropertyThatDoesNotMatchValue( ObjectList& outList, const ObjectList& inputList, const std::string& propertyName, T_Datatype valueToMatch, DataType dataType, OperationType operation = equalEqual_operation )
		{
			ObjectList objectsWithProperty;
			if( getAllObjectsWithProperty( propertyName, objectsWithProperty, inputList ) )
			{
				T_Datatype tempVar;
				for( auto iter = objectsWithProperty.begin(); iter != objectsWithProperty.end(); ++iter )
				{
					if( (*iter)->m_variableList->getVariable( propertyName, tempVar, dataType) )
					{	
						if( !evaluateBoolOperation( tempVar, valueToMatch, operation ) )
						{
							outList.push_back( *iter );
						}

					}
				}
			}
		}

		template< typename T_Datatype >
		void getAllObjectsWithPropertyThatDoesNotMatchValue( ObjectList& outList, const std::string& propertyName, T_Datatype valueToMatch, DataType dataType, OperationType operation = equalEqual_operation )
		{
			getAllObjectsWithPropertyThatDoesNotMatchValue( outList, m_objectList, propertyName, valueToMatch, dataType, operation );
		}

		bool getAllObjectsWithProperty( const std::string& propertyName, ObjectList& outObjectList, const ObjectList& inputList = m_objectList );
		bool getAllObjectsWithoutProperty( const std::string& propertyName, ObjectList& outObjectList, const ObjectList& inputList = m_objectList );
		
		friend void addDatabaseObject( DatabaseObject* object );

	protected:
		void parseQueryList( const std::string& queryString, const QueryList& queryList, ObjectList& outObjectList );
		void addDatabaseObject( DatabaseObject* object );

		static ObjectList m_objectList;
	};

	
	Database& getThePropertyDatabase();
	
	
	
	DatabaseObject* getObjectFromDatabaseQuery( const Query& query );
	DatabaseObject* getObjectFromDatabaseWithProperty( const std::string& propertyName );
	DatabaseObject* getObjectFromDatabaseWithoutProperty( const std::string& propertyName );

	void populateObjectListFromDatabaseQuery( const Query& query, ObjectList& outList );
	void populateObjectListFromDatabaseQuery( const Query& query, ObjectList& outList, const ObjectList& inputList );

	void populateObjectListFromDatabaseThatOpposesQuery( const std::string& propertyName );

	template< typename T_Datatype >
	DatabaseObject* getObjectFromDatabaseWithPropertyThatMatchesValue( const std::string& propertyName, T_Datatype valueToMatch, DataType dataType )
	{
		return getThePropertyDatabase().getObjectWithPropertyThatMatchesValue( propertyName, valueToMatch, dataType );
	}
}