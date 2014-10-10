#pragma once
#include "DatabaseObject.h"
#include "..\Parser\VariableBase.h"
#include "..\Parser\ExpressionNode.h"

namespace gh
{

class Query
{
public:
	Query(){}

	Query( bool hasProp, const std::string& propName )
		: m_bContainsProperty( hasProp )
		, m_propertyName( propName )
		, m_propertyValue( nullptr )
		, m_queryValueType( NULL_DATA_TYPE )
		, m_opType( no_operation )
	{}


	template< typename T_Datatype >
	Query( bool hasProp, const std::string& propName, T_Datatype valueToMatch, DataType valueType, OperationType opType )
		: m_bContainsProperty( hasProp )
		, m_propertyName( propName )
		, m_propertyValue( nullptr )
		, m_queryValueType( valueType )
		, m_opType( opType )
	{
		m_propertyValue = new Variable< T_Datatype >( "valueToMatch", valueToMatch, valueType );
	}

	bool m_bContainsProperty;
	std::string m_propertyName;
	VariableBase* m_propertyValue;
	DataType m_queryValueType;
	OperationType m_opType;
};

typedef std::vector< Query > QueryList;

}