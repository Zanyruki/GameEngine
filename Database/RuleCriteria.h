#pragma once
#include <string>
#include "DatabaseQuery.h"
#include "..\Parser\MathematicExpression.h"
#include "..\Parser\Variable.h"

namespace gh
{
	enum ScriptSourceType
	{
		VariableType,
		QueryType,
		DatabaseType,
	};

	enum ScriptPayloadType
	{
		VariablePayload,
		FunctionPayload
	};

	class RuleCriteria
	{
	public:
		RuleCriteria( const std::string& varName, const std::string& operation, const std::string& valueExpression, int score );
		bool isCriteriaMet( DatabaseQuery* query );
		bool evaluateCriteriaInt( Variable<int>* variable, DatabaseQuery* query );
		bool evaluateCriteriaBool( Variable<bool>* variable, DatabaseQuery* query );
		bool evaluateCriteriaFloat( Variable<float>* variable, DatabaseQuery* query );
		bool evaluateCriteriaString( Variable<std::string>* variable, DatabaseQuery* query );
		int getScoreValue();
		void setExpressionVariableTables(DatabaseQuery* query);
	private:
		void setCriteriaType();
		ScriptSourceType m_criteriaType;
		ScriptPayloadType m_payloadType;
		std::string m_objectName;
		std::string m_databaseCategoryName;
		std::string m_variableOrFunctionName;
		std::vector< MathematicExpression* > m_functionArgs;
		OperationType m_operation;
		MathematicExpression m_value;
		int m_scoreValue;
	};

}