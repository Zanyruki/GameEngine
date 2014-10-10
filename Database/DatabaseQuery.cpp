#include "DatabaseQuery.h"
#include "..\Parser\VariableTableManager.h"

namespace gh
{
	DatabaseQuery::DatabaseQuery()
	{
		
	}

	
	void DatabaseQuery::setObject( const std::string& key, DatabaseObject* value )
	{
		m_gameObjects[ key ] = value;
		getTheVariableTableManager().addTable( key, value->m_variableList );
	}


	DatabaseObject* DatabaseQuery::getObject( const std::string key )
	{
		return m_gameObjects[ key ];
	}


	DataType DatabaseQuery::getVariableType( const std::string& key )
	{
		return m_variables[ key ]->getDataType();
	}


	VariableBase* DatabaseQuery::getVariable( const std::string& key )
	{
		return m_variables[ key ];
	}
}