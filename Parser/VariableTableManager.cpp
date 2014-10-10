#include "VariableTableManager.h"

namespace gh
{
	VariableTable* VariableTableManager::getTable( const std::string& tableName )
	{
		if( tableName == "local" )
		{
			return m_currentLocalTable;
		}

		auto found = m_variableTables.find( tableName );
		if( found != m_variableTables.end() )
		{
			return m_variableTables[ tableName ];
		}

		return nullptr;
	}


	void VariableTableManager::addTable( const std::string& tableName, VariableTable* table )
	{
		if( table != nullptr )
		{
			m_variableTables[ tableName ] = table;
		}
	}


	void VariableTableManager::clearAllTables()
	{
		for (auto tableIter = m_variableTables.begin(); tableIter != m_variableTables.end();)
		{
			tableIter = m_variableTables.erase(tableIter);
		}
	}

	void VariableTableManager::setLocalTable( const std::string& tableName )
	{
		m_currentLocalTable = m_variableTables[ tableName ];
	}


	void VariableTableManager::setLocalTable( VariableTable* localTable )
	{
		m_currentLocalTable = localTable;
	}


	VariableTableManager& getTheVariableTableManager()
	{
		static VariableTableManager theVariableTableManager;
		return theVariableTableManager;
	}
}