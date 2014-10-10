#pragma once
#include "VariableDataType.h"
#include "FunctionOwner.h"
#include "MathematicExpression.h"
namespace gh
{
	class ScriptFunctionBase
	{
	public:
		virtual VariableBase* executeFunction(FunctionOwnerBase* functionOwner, const std::vector< MathematicExpression* >& argList )
		{
			return nullptr;
		}

		virtual DataType getReturnType()
		{
			return NULL_DATA_TYPE;
		}
	};
}