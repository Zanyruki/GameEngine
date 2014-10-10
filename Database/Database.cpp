#include "Database.h"
#include "QueryExpression.h"


namespace gh
{
	Database::Database()
	{

	}

	DatabaseObject* Database::getObjectFromQuery( const Query& query, const ObjectList& inputList )
	{
		if( query.m_propertyValue != nullptr )
		{
			ObjectList objectList;
			// Have to initialize vars here because declaring them in switch case causes compile error
			Variable<int>* intVar = nullptr;
			Variable<float>* floatVar = nullptr;
			Variable<bool>* boolVar = nullptr;
			Variable<std::string>* stringVar = nullptr;
			switch( query.m_propertyValue->m_dataType )
			{
			case BOOL_DATA_TYPE :
				boolVar = static_cast< Variable<bool>* >( query.m_propertyValue );
				getAllObjectsWithPropertyThatMatchesValue( objectList, inputList, query.m_propertyName, boolVar->getValue(), BOOL_DATA_TYPE, query.m_opType );
				break;
			case INT_DATA_TYPE :
				intVar = static_cast< Variable<int>* >( query.m_propertyValue );
				getAllObjectsWithPropertyThatMatchesValue( objectList, inputList, query.m_propertyName, intVar->getValue(), INT_DATA_TYPE, query.m_opType );
				break;
			case FLOAT_DATA_TYPE :
				floatVar = static_cast< Variable<float>* >( query.m_propertyValue );
				getAllObjectsWithPropertyThatMatchesValue( objectList, inputList, query.m_propertyName, floatVar->getValue(), FLOAT_DATA_TYPE, query.m_opType );
				break;
			case STRING_DATA_TYPE :
				stringVar = static_cast< Variable<std::string>* >( query.m_propertyValue );
				getAllObjectsWithPropertyThatMatchesValue( objectList, inputList, query.m_propertyName, stringVar->getValue(), STRING_DATA_TYPE, query.m_opType );
				break;
			}
			
			if( objectList.size() > 0 )
			{
				return objectList[0];
			}
			return nullptr; //TODO
		}
		else
		{
			if( query.m_bContainsProperty )
			{
				return getRandomObjectWithProperty( query.m_propertyName );
			}
			else
			{
				return getRandomObjectWithoutProperty( query.m_propertyName );
			}
		}
	}


	DatabaseObject* Database::getObjectFromQueryWithLowestAttribute( const Query& query, const std::string& propertyToSortBy, const ObjectList& inputList )
	{
		ObjectList list;
		populateObjectListFromQuery( query, list, inputList );
		sortObjectListDescendingByProperty( list, propertyToSortBy );
		if( list.empty() )
		{
			return nullptr;
		}

		return list.front();
		
	}


	DatabaseObject* Database::getObjectFromQueryWithHighestAttribute( const Query& query, const std::string& propertyToSortBy, const ObjectList& inputList )
	{
		ObjectList list;
		populateObjectListFromQuery( query, list, inputList );
		sortObjectListDescendingByProperty( list, propertyToSortBy );
		if( list.empty() )
		{
			return nullptr;
		}

		return list.back();
	}


	void Database::populateObjectListFromQuery( const Query& query, ObjectList& outList, const ObjectList& inputList )
	{
		if( query.m_propertyValue != nullptr )
		{
			//getAllObjectsWithPropertyThatMatchesValue( outList, inputList, query.m_propertyName, query.m_propertyValue, query.m_queryValueType, query.m_opType );
			Variable<int>* intVar = nullptr;
			Variable<float>* floatVar = nullptr;
			Variable<bool>* boolVar = nullptr;
			Variable<std::string>* stringVar = nullptr;
			switch( query.m_propertyValue->m_dataType )
			{
			case BOOL_DATA_TYPE :
				boolVar = static_cast< Variable<bool>* >( query.m_propertyValue );
				getAllObjectsWithPropertyThatMatchesValue( outList, inputList, query.m_propertyName, boolVar->getValue(), BOOL_DATA_TYPE, query.m_opType );
				break;
			case INT_DATA_TYPE :
				intVar = static_cast< Variable<int>* >( query.m_propertyValue );
				getAllObjectsWithPropertyThatMatchesValue( outList, inputList, query.m_propertyName, intVar->getValue(), INT_DATA_TYPE, query.m_opType );
				break;
			case FLOAT_DATA_TYPE :
				floatVar = static_cast< Variable<float>* >( query.m_propertyValue );
				getAllObjectsWithPropertyThatMatchesValue( outList, inputList, query.m_propertyName, floatVar->getValue(), FLOAT_DATA_TYPE, query.m_opType );
				break;
			case STRING_DATA_TYPE :
				stringVar = static_cast< Variable<std::string>* >( query.m_propertyValue );
				getAllObjectsWithPropertyThatMatchesValue( outList, inputList, query.m_propertyName, stringVar->getValue(), STRING_DATA_TYPE, query.m_opType );
				break;
			}
		}
		else
		{
			if( query.m_bContainsProperty )
			{
				//return getRandomObjectWithProperty( query.m_propertyName );
				getAllObjectsWithProperty( query.m_propertyName, outList, inputList );
			}
			else
			{
				//return getRandomObjectWithoutProperty( query.m_propertyName );
				getAllObjectsWithoutProperty( query.m_propertyName, outList, inputList );
			}
		}
	}


	void Database::populateObjectListOppositeFromQuery( const Query& query, ObjectList& outList, const ObjectList& inputList )
	{
		if( query.m_propertyValue != nullptr )
		{
			getAllObjectsWithPropertyThatDoesNotMatchValue( outList, inputList, query.m_propertyName, query.m_propertyValue, query.m_queryValueType, query.m_opType );
		}
		else
		{
			if( query.m_bContainsProperty )
			{
				//return getRandomObjectWithProperty( query.m_propertyName );
				getAllObjectsWithoutProperty( query.m_propertyName, outList, inputList );
			}
			else
			{
				//return getRandomObjectWithoutProperty( query.m_propertyName );
				getAllObjectsWithProperty( query.m_propertyName, outList, inputList );
			}
		}
	}


	DatabaseObject* Database::getObjectFromMultipleQueries( const std::string& statement, const QueryList& listOfQueries )
	{
		ObjectList list;
		parseQueryList( statement, listOfQueries, list );
		if( list.size() > 0)
		{
			return list[0];
		}

		return nullptr;
		
	}


	bool Database::getAllObjectsWithProperty( const std::string& propertyName, ObjectList& outObjectList, const ObjectList& inputList )
	{
		for( auto iter = m_objectList.begin(); iter != m_objectList.end(); ++iter )
		{
			if( (*iter)->hasProperty( propertyName ) )
			{
				outObjectList.push_back( *iter );
			}
		}

		return outObjectList.size() > 0;
	}


	bool Database::getAllObjectsWithoutProperty( const std::string& propertyName, ObjectList& outObjectList, const ObjectList& inputList )
	{
		for( auto iter = m_objectList.begin(); iter != m_objectList.end(); ++iter )
		{
			if( !(*iter)->hasProperty( propertyName ) )
			{
				outObjectList.push_back( *iter );
			}
		}

		return outObjectList.size() > 0;
	}

	DatabaseObject* Database::getRandomObjectWithProperty( const std::string& propertyName )
	{
		ObjectList objectsWithProperty;
		if(getAllObjectsWithProperty( propertyName, objectsWithProperty ) )
		{
			return objectsWithProperty.front();
		}
		else
		{
			return NULL;
		}
		
	}


	DatabaseObject* Database::getRandomObjectWithoutProperty( const std::string& propertyName )
	{
		ObjectList objectsWithoutProperty;
		if( getAllObjectsWithoutProperty( propertyName, objectsWithoutProperty ) )
		{
			return objectsWithoutProperty.front();
		}
		else
		{
			return NULL;
		}
	}

	/*****************
	* Sorting Methods
	*****************/

	void mergeSortInt( ObjectList& listToSort, const std::string& property )
	{
		int middle = listToSort.size() / 2;
		if( middle > 0 )
		{
			ObjectList frontHalf;
			ObjectList backHalf;
			frontHalf.insert( frontHalf.end(), listToSort.begin(), listToSort.begin() + middle );
			backHalf.insert( backHalf.end(), listToSort.begin() + middle, listToSort.end() );
			mergeSortInt( frontHalf, property );
			mergeSortInt( backHalf, property );
			ObjectList newList;
			int frontIndex = 0;
			int backIndex = 0;
			for( int i = 0; i < listToSort.size(); ++i )
			{
				if( frontIndex >= frontHalf.size() )
				{
					newList.push_back( backHalf[backIndex++] );
				}
				else if( backIndex >= frontHalf.size() )
				{
					newList.push_back( frontHalf[frontIndex++] );
				}
				else
				{
					DatabaseObject* frontObject = frontHalf[ frontIndex ];
					int frontValue = frontObject->m_variableList->getVariableAsInt( property );
					DatabaseObject* backObject = backHalf[ backIndex ];
					int backValue = backObject->m_variableList->getVariableAsInt( property );
					if( frontValue <= backValue )
					{
						newList.push_back( frontObject );
						++frontIndex;
					}
					else
					{
						newList.push_back( backObject );
						++backIndex;
					}

				}
				
			}
			listToSort = newList;
		}
		
	}


	void Database::sortObjectListDescendingByProperty( ObjectList& listToSort, const std::string& property )
	{
		ObjectList noPropertyList;

		
		for( auto iter = listToSort.begin(); iter != listToSort.end(); )
		{
			if( ! (*iter)->hasProperty(property) )
			{
				noPropertyList.push_back( *iter );
				iter = listToSort.erase( iter );
			}
			else
			{
				++iter;
			}
		}
		
		VariableBase* propertyVariable = listToSort.front()->m_variableList->getVariable( property );
		switch( propertyVariable->m_dataType )
		{
		case INT_DATA_TYPE :
			mergeSortInt( listToSort, property );
			break;
		}
		
	}


	/*DatabaseObject* getObjectWithPropertyThatMatchesValue( const std::string& propertyName, T_Datatype valueToMatch )
	{

	}*/

	//******************************************************
	//Private methods
	//******************************************************

	void Database::addDatabaseObject( DatabaseObject* object )
	{
		m_objectList.push_back( object );
	}


	void Database::parseQueryList( const std::string& queryString, const QueryList& queryList, ObjectList& outObjectList )
	{
		QueryExpression parsedQuery( queryString, queryList );
		parsedQuery.evaluateQuery( outObjectList );
	}

	//*******************************************************
	// Global Database Methods
	//*******************************************************
	Database& getThePropertyDatabase()
	{
		static Database theDatabase;
		return theDatabase;
	}

	void addDatabaseObject( DatabaseObject* object )
	{
		getThePropertyDatabase().addDatabaseObject( object );
	}

	void populateObjectListFromDatabaseQuery( const Query& query, ObjectList& outList, const ObjectList& inputList )
	{
		getThePropertyDatabase().populateObjectListFromQuery( query, outList, inputList );
	}

	void populateObjectListFromDatabaseQuery( const Query& query, ObjectList& outList )
	{
		getThePropertyDatabase().populateObjectListFromQuery( query, outList );
	}


	DatabaseObject* getObjectFromDatabaseQuery( const Query& query )
	{
		return getThePropertyDatabase().getObjectFromQuery( query );
	}

	DatabaseObject* getObjectFromDatabaseWithProperty( const std::string& propertyName )
	{
		return getThePropertyDatabase().getRandomObjectWithProperty( propertyName );
	}

	DatabaseObject* getObjectFromDatabaseWithoutProperty( const std::string& propertyName )
	{
		return getThePropertyDatabase().getRandomObjectWithoutProperty( propertyName );
	}

	ObjectList Database::m_objectList;
}