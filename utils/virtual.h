#pragma once
#include "platform.h"
#include "common.h"

#define CALL_VIRTUAL(retType, idx, ...) \
	vmt::CallVirtual<retType>(idx, __VA_ARGS__)

namespace vmt
{
	template <typename T = void *>
	inline T GetVMethod(uint32 uIndex, void *pClass)
	{
		if (!pClass)
		{
			Message("Tried getting virtual function from a null class.\n");
			return T();
		}

		void **pVTable = *static_cast<void ***>(pClass);
		if (!pVTable)
		{
			Message("Tried getting virtual function from a null vtable.\n");
			return T();
		}

		return reinterpret_cast<T>(pVTable[uIndex]);
	}

	template <typename T, typename... Args>
	inline T CallVirtual(uint32 uIndex, void *pClass, Args... args)
	{
#ifdef _WIN32
		auto pFunc = GetVMethod<T(__thiscall *)(void *, Args...)>(uIndex, pClass);
#else
		auto pFunc = GetVMethod<T(__cdecl*)(void*, Args...)>(uIndex, pClass);
#endif
		if (!pFunc)
		{
			Message("Tried calling a null virtual function.\n");
			return T();
		}

		return pFunc(pClass, args...);
	}
}

#define CALL_VIRTUAL_DEBUG(debugInfo, retType, idx, ...) \
	vmt::CallVirtual<retType>(debugInfo, idx, __VA_ARGS__)
	
namespace vmt
{
    template <typename T = void *>
    inline T GetVMethod(const char *debugInfo, uint32 uIndex, void *pClass)
    {
        if (!pClass)
        {
            Message("Failed to get virtual function: null class pointer. in func ->: %s\n", debugInfo);
            return T();
        }

        void **pVTable = *static_cast<void ***>(pClass);
        if (!pVTable)
        {
            Message("Failed to get virtual function: null vtable. in func ->: %s\n", debugInfo);
            return T();
        }

        return reinterpret_cast<T>(pVTable[uIndex]);
    }

    template <typename T, typename... Args>
    inline T CallVirtual(const char *debugInfo, uint32 uIndex, void *pClass, Args... args)
    {
#ifdef _WIN32
        auto pFunc = GetVMethod<T(__thiscall *)(void *, Args...)>(debugInfo, uIndex, pClass);
#else
        auto pFunc = GetVMethod<T(__cdecl *)(void *, Args...)>(debugInfo, uIndex, pClass);
#endif
        if (!pFunc)
        {
            Message("Failed to call virtual function: null function pointer. in func ->: %s\n", debugInfo);
            return T();
        }

        return pFunc(pClass, args...);
    }
}
