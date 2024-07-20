#pragma once

#include <string>

namespace tickfix {
	namespace common {
		namespace settings {
			struct consts {
				static const std::string iniDir;
			};
			const std::string& GetFullIniPath();
			static int ReadInteger(int defaultValue, const std::string& section, const std::string& key);

			template <typename T>
			class SettingContainer 
			{
				private:
				protected:
				std::string m_section;
				std::string m_key;
				T m_defaultValue;
				T m_value;
				public:
				SettingContainer(T defaultValue, std::string&& section, std::string&& key) : m_defaultValue(defaultValue), m_section(section), m_key(key), m_value(NULL) {}
				virtual ~SettingContainer() {}
				T Get() { return m_value; }
				virtual void Read() { static_assert(false); };

			};

			template <> void SettingContainer<int>::Read();
			template <> void SettingContainer<float>::Read();
			template <> void SettingContainer<std::string>::Read();
			template <> void SettingContainer<bool>::Read();


		};
	};
};

