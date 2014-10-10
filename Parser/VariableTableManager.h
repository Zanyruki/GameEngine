#pragma once
#include "VariableTable.h"

namespace gh
{
	class VariableTableManager
	{
	public:
		VariableTableManager()
		: m_currentLocalTable( nullptr)
		{};

		VariableTable* getTable( const std::string& tableName );
		void addTable( const std::string& tableName, VariableTable* table );
		void setLocalTable( const std::string& tableName );
		void setLocalTable( VariableTable* localTable );
		void clearAllTables();

	private:
		std::map< std::string, VariableTable* > m_variableTables;
		VariableTable* m_currentLocalTable;
	};

	VariableTableManager& getTheVariableTableManager();
}