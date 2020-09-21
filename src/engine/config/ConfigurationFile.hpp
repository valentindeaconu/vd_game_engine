#ifndef __CONFIGURATION_FILE_HPP_
#define __CONFIGURATION_FILE_HPP_

#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>

#include <engine/logger/Logger.hpp>

namespace vd::config
{
    class ConfigurationFile
    {
    public:
        explicit ConfigurationFile(const std::string& filePath);
        ~ConfigurationFile();

        void parse();

        [[nodiscard]] const std::string& getFilePath() const;
    private:
        const std::string filePath;

        virtual void onTokenReceived(const std::string& command, const std::vector<std::string>& tokens) = 0;
        virtual void onParseFinish() = 0;
    };
}

#endif // !__CONFIGURATION_FILE_HPP_
