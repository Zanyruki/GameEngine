#pragma once
#include "..\Utility\tinyxml2.h"
#include "..\Parser\MathematicExpression.h"
#include <string>
#include "DatabaseQuery.h"
#include "..\Utility\tinyxml2.h"
#include "RuleCriteria.h"

namespace gh
{
	using namespace tinyxml2;


	class DatabaseAction
	{
	public:
		DatabaseAction( XMLNode* actionNode, const std::string& responseNodeName, const std::string& fileName ); //Used for SetVariableNodes
		DatabaseAction(const std::string& memoryVariableName, const std::string& responseNodeName, const std::string& fileName); //Used for memoryNodes instead of SetVariable nodes
		void doAction( DatabaseQuery* query );

		std::string m_objectName;
		std::string m_variableName;
		std::string m_databaseCategoryName;
		MathematicExpression* m_expression;
		DataType m_variableDataType;
	private:
		void setActionType();
		void createNewVariable( VariableTable* localTable );
		void setNewValue( VariableBase* var );

		ScriptSourceType m_actionType;
	};
}