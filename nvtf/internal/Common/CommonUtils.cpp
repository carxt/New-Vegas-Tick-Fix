


#include "internal/Common/CommonUtils.hpp"
#include <vector>
#include "nvse/PluginAPI.h"
#include <algorithm>
#include <mutex>

namespace CommonUtils
{

	class UtilsManager : public UtilsInterface
	{
	public:
		std::vector<bool(__cdecl*)()> m_Callbacks;
		std::recursive_mutex m_Lock;
		virtual ~UtilsManager() {}
		virtual bool RegisterMainLoopFunction(bool (__cdecl *pToRegister)()) override 
		{
			std::scoped_lock lLock(m_Lock);
			auto it = std::find(m_Callbacks.begin(), m_Callbacks.end(), pToRegister);
			if (it == m_Callbacks.end())
				m_Callbacks.push_back(pToRegister);
			return true;
		};
		virtual bool UnregisterMainLoopFunction(bool (__cdecl *pToRegister)()) override 
		{
			std::scoped_lock lLock(m_Lock);
			m_Callbacks.erase(std::remove(m_Callbacks.begin(), m_Callbacks.end(), pToRegister), m_Callbacks.end());
			return true;
		};


	};
	UtilsInterface* GetUtilsManager()
	{
		static UtilsManager g_Utils;
		return &g_Utils;

	}
	void NVSEMessageHandler(void* recievedMsg)
	{
		auto msg = static_cast<NVSEMessagingInterface::Message*>(recievedMsg);
		switch (msg->type)
		{
		case NVSEMessagingInterface::kMessage_MainGameLoop:
			[]() -> void
			{
				auto lUtilsMan = static_cast<UtilsManager*>(GetUtilsManager());
				std::scoped_lock lLock(lUtilsMan->m_Lock);
				auto& lCallbacks = lUtilsMan->m_Callbacks;
				for (auto callback : lCallbacks)
				{
					callback();
				}
			}();
			break;
		default:
			break;
		}
	
	}

}