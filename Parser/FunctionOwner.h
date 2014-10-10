#pragma once
#include "FunctionOwnerBase.h"

namespace gh
{
	template< typename T_DataType>
	class FunctionOwner : public FunctionOwnerBase
	{
	public:
		FunctionOwner( T_DataType* owner )
			: m_functionOwner( owner )
		{

		}
		T_DataType* m_functionOwner;
	};
}