#pragma once
#include <map>
#include "ScriptFunction.h"
#include "MathematicExpression.h"
#include "FunctionOwner.h"
#include "GenericContainer.h"
#include <functional>
namespace gh
{
	//template
	//typedef void(T_DataType::*ObjectMemberFunctionType)( const VariableNamedProperties& eventArgs );
	

	class FunctionTable
	{
	public:
		
		template< typename T_FunctionOwner >
		FunctionTable( T_FunctionOwner* owner )
		{
			using FunctionOwnerType = T_FunctionOwner;
			m_functionOwner = new FunctionOwner< T_FunctionOwner >( owner );
		}
		/*template< typename T_ReturnType, typename T_MemberType, typename T_Param1Type >
		void addFunctionToTable( const std::string& functionName, T_ReturnType( T_MemberType::*func)( T_Param1Type ) )
		{
			ScriptFunction< func >* function = new ScriptFunction< func >( functionName, func );
		}*/

		
		//template <typename... Args>
		//auto g(Args&&... args) -> decltype(f(std::forward<Args>(args)...)) {
		//	return f(std::forward<Args>(args)...);
		//}
		
		
		//template< typename T_ReturnType, typename T_ClassType, typename... Args > using MemberFunctionPointer = T_ReturnType(T_ClassType::*)(Args...args);
		//template< typename T_MemberFuncPointer>
		template< typename T_ReturnType, typename T_ClassType, typename... Args >
		void addFunctionToTable(const std::string& functionName, T_ReturnType(T_ClassType::*func)(Args...args) )
		{
			
			ScriptFunction< T_ReturnType, T_ClassType, Args... >* function = new ScriptFunction< T_ReturnType, T_ClassType, Args... >(functionName, func);
			//setScriptFunctionData(func);
			m_functionMap[ functionName ] = function;
		}

		
		void setScriptFunctionData()
		{

		}

		//template< typename Container
		VariableBase* executeFunction(const std::string functionName, std::vector< MathematicExpression* > argList);
		VariableBase* executeFunction(const std::string functionName, const std::string& args);

		DataType getReturnType( const std::string& functionName );
		

	private:
		std::map< std::string, ScriptFunctionBase* > m_functionMap;
		FunctionOwnerBase* m_functionOwner;
	};
}