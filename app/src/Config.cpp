
#include "Config.h"
#include <iostream>

std::shared_ptr<Config> Config::s_config = nullptr;


	Config::~Config() {
		if (m_fs.isOpened())
		{
			m_fs.release();
		}
	}

	bool Config::SetConfigFile(const std::string& filename) {
		if (s_config == nullptr) {
			s_config = std::shared_ptr<Config>(new Config);
		}
		s_config->m_fs = cv::FileStorage(filename.c_str(), cv::FileStorage::READ);
		if (s_config->m_fs.isOpened() ==false)
		{
			std::cout<<"Parameter file" << filename << "does not exist.";
			s_config->m_fs.release();
			return false;

		}
		return true;
	}