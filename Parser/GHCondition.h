#pragma once
#include <string>
#include "MathematicExpression.h"

namespace gh
{
	class GHCondition
	{
	public:
		GHCondition( const std::string& conditionString );
		void setLocalVariableTable( const std::string& localTableName );
		bool evaluate();
		std::string m_string;
		MathematicExpression m_conditionalExpression;
	};
}