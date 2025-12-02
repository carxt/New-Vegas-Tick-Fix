#pragma once

namespace CommonUtils
{
	class UtilsInterface
	{
	public:
		virtual ~UtilsInterface() {}
		virtual bool RegisterMainLoopFunction(bool(__cdecl *pToRegister)()) = 0;
		virtual bool UnregisterMainLoopFunction(bool(__cdecl *pToUnregister)()) = 0;

	};
	UtilsInterface* GetUtilsManager();
	void NVSEMessageHandler(void* msg);
}
