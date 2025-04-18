#include "TickFix.hpp"
#include <timeapi.h>

#include "Game\Bethesda\BGSSaveLoadGame.hpp"
#include "Game\Bethesda\bhkWorld.hpp"
#include "Game\Bethesda\TimeGlobal.hpp"
#include "Game\Bethesda\Interface.hpp"
#include "Game\Bethesda\TESMain.hpp"

#pragma comment(lib, "winmm.lib")

namespace TickFix {
	namespace Setting {
		bool	 bFPSFix				= true;
		bool	 bAlternateGTCFix		= false;
		bool	 bRemoveGTCLimits		= false;

		double	 dMaxTimeLowerBoundaryS	= 0.0;
		bool	 bSpiderHandsFix		= false;
		bool	 bfMaxTime				= true;
		uint16_t usMaxFPS				= UINT16_MAX;
		uint16_t usMinFPS				= UINT16_MAX;
	}

	constexpr float fTimerOffsetMult	= 0.9875f; // Formerly 0.9825
	float	fMaxTimeDefault				= 0.f;
	double	dMaxTimeLowerBoundaryMS		= 0.0;
	double	dDesiredMaxMS				= 1.0;
	double	dDesiredMinMS				= 1000.0;
	double	dDesiredMaxS				= 1.0;
	double	dDesiredMinS				= 1000.0;

	namespace QPC {
		LARGE_INTEGER	liFrequency;
		double			dLastCount = 0;

		double GetTime() {
			LARGE_INTEGER liCounter;
			QueryPerformanceCounter(&liCounter);
			return double(liCounter.QuadPart) / double(liFrequency.QuadPart);
		}

		double GetTimeMS() {
			return GetTime() * 1000.0;
		}

		DWORD ReturnCounter() {
			return GetTimeMS();
		}

		double GetTimeDelta() {
			double dCurrentCount = GetTimeMS();
			double dToReturn = dCurrentCount - dLastCount;
			dLastCount = dCurrentCount;
			return dToReturn;
		}

		void Initialize() {
			QueryPerformanceFrequency(&liFrequency);
			dLastCount = GetTimeMS();
		}
	}

	namespace tGt {
		double  dLastCount = 0;
		int32_t iTickCountBias = 0;

		DWORD ReturnCounter() {
			return DWORD(long(timeGetTime()) + iTickCountBias);
		}

		double GetTimeDelta() {
			double dCurrentCount = double(timeGetTime());
			double dToReturn = dCurrentCount - dLastCount;
			dLastCount = dCurrentCount;
			return dToReturn;

		}

		void Initialize() {
			dLastCount		= timeGetTime();
			iTickCountBias	= long(dLastCount) - long(GetTickCount());
		}
	}

	DWORD GetTickCountHook() {
		if (Setting::bAlternateGTCFix) [[unlikely]]
			return tGt::ReturnCounter();
		else [[likely]]
			return QPC::ReturnCounter();
	}

	double GetTimeDelta() {
		if (Setting::bAlternateGTCFix) [[unlikely]]
			return tGt::GetTimeDelta();
		else [[likely]]
			return QPC::GetTimeDelta();
	}

	void FPSStartCounter() {
		if (Setting::bAlternateGTCFix) [[unlikely]]
			tGt::Initialize();
		else [[likely]]
			QPC::Initialize();
	}

	void ClampGameCounters(float& fClamp) {
		float& fMaxTime = *bhkWorld::fMaxTime;
		if (Setting::bSpiderHandsFix && fClamp > FLT_EPSILON) {
			fClamp = 1000.f / ((1000.f / fClamp) * fTimerOffsetMult);
			if (Setting::bfMaxTime) [[likely]] {
				fMaxTime = 1000.f / ((1000.f / fMaxTime) * fTimerOffsetMult);
				if (fMaxTime < FLT_EPSILON)
					fMaxTime = FLT_EPSILON;
			}
		}

		if (Setting::bfMaxTime) [[likely]] {
			if (TESMain::IsInPauseFade() || fClamp < FLT_EPSILON) [[unlikely]]
				fMaxTime = fMaxTimeDefault;
			else if (fMaxTime > Setting::dMaxTimeLowerBoundaryS)
				fMaxTime = Setting::dMaxTimeLowerBoundaryS;
		}
	}

	class BSTimerSafe : public BSTimer {
	public:
		void TimeGlobalHook() {
			double dDelta = GetTimeDelta();

			if (Setting::bfMaxTime) [[likely]] {
				float fMaxTime = Setting::dMaxTimeLowerBoundaryS;
				if (dDelta > FLT_EPSILON) [[likely]] {
					if (dDelta < dDesiredMinMS)
						fMaxTime = dDelta > dDesiredMaxMS ? (dDelta / 1000.0) : dDesiredMaxS;
					else
						fMaxTime = dDesiredMinS;
				}

				*bhkWorld::fMaxTime = fMaxTime;
			}

			fClamp = 0.f;
			BGSSaveLoadGame* pSaveLoad = BGSSaveLoadGame::GetSingleton();
			bool bSaveLoading = pSaveLoad && pSaveLoad->IsLoading();
			if (!bSaveLoading && !Interface::IsLoadingNewGame() && dDelta > 0.f) [[likely]] {
				if (dDelta < dDesiredMinMS)
					fClamp = dDelta > dDesiredMaxMS ? dDelta : dDesiredMaxMS;
				else
					fClamp = dDesiredMinMS;
			}

			ClampGameCounters(fClamp);
			Update(GetTickCountHook());
		}

		void TimeGlobalHook_NoSafeGuards() {
			double dDelta = GetTimeDelta();

			if (Setting::bfMaxTime) [[likely]] {
				float fMaxTime = Setting::dMaxTimeLowerBoundaryS;
				if (dDelta > 0.9 && dDelta < dMaxTimeLowerBoundaryMS)
					fMaxTime = dDelta > dDesiredMaxMS ? (dDelta / 1000.0) : dDesiredMaxS;

				*bhkWorld::fMaxTime = fMaxTime;
			}

			fClamp = 0.f;
			if (dDelta > 0.0) [[likely]] {
				if (dDelta < dDesiredMinMS)
					fClamp = dDelta > dDesiredMaxMS ? dDelta : dDesiredMaxMS;
				else
					fClamp = dDesiredMinMS;
			}

			ClampGameCounters(fClamp);
			Update(GetTickCountHook());
		}

	};

	void InitTimerHook() {
		if (Setting::bRemoveGTCLimits) [[unlikely]]
			ReplaceCallEx(0x86F296, &BSTimerSafe::TimeGlobalHook_NoSafeGuards);
		else [[likely]]
			ReplaceCallEx(0x86F296, &BSTimerSafe::TimeGlobalHook);
	}

	void ReadINI(const char* iniPath) {
		Setting::bFPSFix				= GetPrivateProfileInt("GTC", "bFPSFix", 0, iniPath);
		Setting::bAlternateGTCFix		= GetPrivateProfileInt("GTC", "bAlternateGTCFix", 0, iniPath);
		Setting::bRemoveGTCLimits		= GetPrivateProfileInt("GTC", "bRemoveGTCLimits", 0, iniPath);

		char cFloatBuffer[64];
		GetPrivateProfileString("FPSFix", "fMaxTimeLowerBoundary", "0.016", cFloatBuffer, sizeof(cFloatBuffer) - 1, iniPath);
		Setting::dMaxTimeLowerBoundaryS = atof(cFloatBuffer);
		Setting::bSpiderHandsFix		= GetPrivateProfileInt("FPSFix", "bSpiderHandsFix", 0, iniPath);
		Setting::usMaxFPS				= GetPrivateProfileInt("FPSFix", "iMaxFPSTolerance", 59, iniPath);
		Setting::usMinFPS				= GetPrivateProfileInt("FPSFix", "iMinFPSTolerance", 20, iniPath);
		Setting::bfMaxTime				= GetPrivateProfileInt("FPSFix", "bfMaxTime", 1, iniPath);
	}

	void InitHooks() {
		FPSStartCounter();
		if (Setting::bAlternateGTCFix) [[unlikely]]
			timeBeginPeriod(1);

		SafeWrite32(0xFDF060, uint32_t(GetTickCountHook));

		if (Setting::bFPSFix) [[likely]] {
			dDesiredMaxMS	= 1000.0 / double(Setting::usMaxFPS);
			dDesiredMinMS	= 1000.0 / double(Setting::usMinFPS);
			dDesiredMaxS	= dDesiredMaxMS / 1000.0;
			dDesiredMinS	= dDesiredMinMS / 1000.0;
			dMaxTimeLowerBoundaryMS = Setting::dMaxTimeLowerBoundaryS * 1000.0;
			fMaxTimeDefault = *bhkWorld::fMaxTime;
			InitTimerHook();
		}
	}
}