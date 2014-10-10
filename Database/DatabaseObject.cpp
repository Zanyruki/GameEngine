#include "DatabaseObject.h"
#include "..\EventSystem\EventSystem.h"

namespace gh
{
	DatabaseObject::DatabaseObject()
		: m_bIsActive( true )
	{
		m_variableList = new VariableTable();
	}

	bool DatabaseObject::isActive()
	{
		return m_bIsActive;
	}

	bool DatabaseObject::hasProperty( const std::string& propertyName )
	{
		
		return ( m_variableList->getVariable( propertyName ) != nullptr );
	}

	DatabaseObject::~DatabaseObject()
	{

	}


	std::string DatabaseObject::getName()
	{
		std::string name = m_variableList->getVariableAsString( "name" );
		return name;
	}
	void DatabaseObject::addProperties( XMLNode* node )
	{
		m_variableList->setVariables( node );
	}


	void mergeObjectLists( ObjectList& destinationList, const ObjectList& sourceList )
	{
		
	}


	bool isObjectInObjectList( DatabaseObject* object, const ObjectList& list )
	{
		for( auto iter = list.begin(); iter != list.end(); ++iter )
		{
			if( object == *iter )
			{
				return true;
			}
		}

		return false;
	}


	void DatabaseObject::callback()
	{
		FireEvent( m_variableList->getVariableAsString( "name") + "Callback" );
	}
}