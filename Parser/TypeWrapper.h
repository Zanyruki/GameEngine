#pragma once

namespace gh
{
	template< typename T_TypeZero>
	class TypeWrapper
	{
	public:
		T_TypeZero getZeroIndex()
		{
			T_TypeZero default;
			return default;
		}
	};


	/*template< typename T_TypeZero, typename T_TypeOne>
	class TypeWrapper
	{
	public:
		T_TypeZero getZeroIndex()
		{
			T_TypeZero default;
			return default;
		}

		T_TypeOne getOneIndex()
		{
			T_TypeOne default;
			return default;
		}
	};*/
}