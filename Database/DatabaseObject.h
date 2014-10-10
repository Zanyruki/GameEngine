#pragma once
#include <string>
#include <vector>
#include <map>
#include "../Parser/VariableTable.h"
#include "../Parser/FunctionTable.h"

namespace gh
{
	class DatabaseObject
	{
	public:
		DatabaseObject();
		~DatabaseObject();
		bool hasProperty( const std::string& propertyName );
		bool isActive();
		void addProperties( XMLNode* node );
		virtual std::string getName();
		virtual void callback();

		VariableTable* m_variableList;
		FunctionTable* m_functionTable;
	protected:
		
		bool m_bIsActive;
	};

typedef std::vector< DatabaseObject* > ObjectList;

bool isObjectInObjectList( DatabaseObject* object, const ObjectList& list );
void mergeObjectLists( ObjectList& destinationList, const ObjectList& sourceList );
}