
#include "common.h"

class Config {
	public:
		Config() = default;
		~Config();

		// access parameter
		template<typename T>
		static T Get(const std::string& key) {
			return static_cast<T>(Config::s_config->m_fs[key]);
		}

		// set a config file
		static bool SetConfigFile(const std::string& filename);

	private:
		static std::shared_ptr<Config> s_config;
		cv::FileStorage m_fs;
	};