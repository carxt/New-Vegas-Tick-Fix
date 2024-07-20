#include <criticalsections/settings.hpp>
using namespace tickfix::criticalsections::settings;


void InitSettings() 
{
	tickfix::criticalsections::settings::iTweakRefCountCS.Read();
	tickfix::criticalsections::settings::iTweakPrecacheCS.Read();
	tickfix::criticalsections::settings::bTweakMiscCS.Read();
	tickfix::criticalsections::settings::bReplaceDeadlockCSForUserMutex.Read();

}