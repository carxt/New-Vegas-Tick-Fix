#include <common/settings.hpp>
#include <Windows.h>


namespace common = tickfix::common;


const std::string common::settings::consts::iniDir = "Data\\NVSE\\Plugins\\NVTF.ini";

const std::string& common::settings::GetFullIniPath()
{

	static std::string iniFullPath;
	if (iniFullPath.empty())
	{
		char iniDir[MAX_PATH];
		GetModuleFileNameA(GetModuleHandle(NULL), iniDir, MAX_PATH);
		iniFullPath = std::move(std::string(iniDir));
	}
	return iniFullPath;
}


int common::settings::ReadInteger(int defaultValue, const std::string& section, const std::string& key) {

	return GetPrivateProfileInt(section.c_str(), key.c_str(), defaultValue, settings::GetFullIniPath().c_str());

}

template <> void common::settings::SettingContainer<double>::Read()
{
	constexpr int bufSize = 64;
	char bufIn[bufSize], bufOut[bufSize];
	sprintf_s(bufIn, "%f", m_defaultValue);
	GetPrivateProfileStringA(m_section.c_str(), m_key.c_str(), bufIn, bufOut, bufSize, GetFullIniPath().c_str());
	m_value = atof(bufOut);

}

template <> void common::settings::SettingContainer<std::string>::Read()
{
	constexpr int bufSize = 64;
	char bufOut[bufSize];
	char bufIn[bufSize] = "";
	GetPrivateProfileStringA(m_section.c_str(), m_key.c_str(), bufIn, bufOut, bufSize, consts::iniDir.c_str());
	m_value = std::move(std::string(bufOut));
}

template <> void common::settings::SettingContainer<int>::Read()
{
	m_value = (int) settings::ReadInteger(m_defaultValue, m_section, m_key);
}

template <> void common::settings::SettingContainer<bool>::Read()
{
	m_value = (bool) settings::ReadInteger(m_defaultValue, m_section, m_key);

}



