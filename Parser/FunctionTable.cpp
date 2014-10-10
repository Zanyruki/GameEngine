#include "FunctionTable.h"
#include "../Utility/ScriptUtil.h"

namespace gh
{
	VariableBase* FunctionTable::executeFunction( const std::string functionName, std::vector< MathematicExpression* > argList )
	{
		auto found = m_functionMap.find(functionName);
		if (found != m_functionMap.end())
		{
			VariableBase* scriptFunctionResult = found->second->executeFunction( m_functionOwner, argList );
			return scriptFunctionResult;

		}
		return nullptr;
	}


	VariableBase* FunctionTable::executeFunction(const std::string functionName, const std::string& args)
	{
		std::vector< MathematicExpression* > argsList;
		parseFunctionArgs(args, argsList);
		return executeFunction(functionName, argsList);
	}


	DataType FunctionTable::getReturnType( const std::string& functionName )
	{
		ScriptFunctionBase* function = m_functionMap[functionName];
		return function->getReturnType();
	}
	
}