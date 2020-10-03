#pragma once

#include <wil/result_macros.h>

#define BOOL_RETURN_E_IF_FAILED(res)				\
{													\
	bool _result = res;								\
	if(!_result)									\
	{												\
		return GetLastError();						\
	}												\
}													

#define Win32Err_RETURN_IF_FAILED(res)				\
{													\
	DWORD result = res;								\
	if(result != ERROR_SUCCESS)						\
	{												\
		return result;								\
	}												\
}													