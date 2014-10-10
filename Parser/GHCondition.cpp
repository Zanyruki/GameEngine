#include "GHCondition.h"

namespace gh
{
	GHCondition::GHCondition( const std::string& conditionString )
		: m_string( conditionString )
		, m_conditionalExpression( conditionString )
	{	
	}

	void GHCondition::setLocalVariableTable( const std::string& localTableName )
	{
		m_conditionalExpression.setLocalVariableTable( localTableName );
	}


	bool GHCondition::evaluate()
	{
		return m_conditionalExpression.evaluateAsBool();
	}
}