//
// Created by Vali on 11/14/2020.
//

#include "VDGEGLSLImpl.hpp"

namespace vd::loader::impl {

    VDGEGLSLImpl::VDGEGLSLImpl() {
        misc::PropertiesPtr propsPtr = ObjectOfType<misc::Properties>::Find();

        m_IncludeDirectory = propsPtr->Get<std::string>("Shader.IncludeDirectory");

        for (int i = 0; ; ++i) {
            const std::string prefix = "Shader.Constants." + std::to_string(i);
            try {
                const auto constName = propsPtr->Get<std::string>(prefix + ".Name");
                const auto constValue = propsPtr->Get<std::string>(prefix + ".Value");

                m_ConstantMap[constName] = constValue;
            } catch (std::invalid_argument& e) {
                break;
            }
        }
    }

    VDGEGLSLImpl::~VDGEGLSLImpl() = default;

    void VDGEGLSLImpl::Load(const std::string& path, std::string& output) {
        output.clear();
        output.shrink_to_fit();

        std::vector<std::string> lines;
        FileLoader::Load(path, lines);

        for (size_t i = 0; i < lines.size(); ++i) {
            std::string& line = lines[i];

            if (line.empty()) {
                continue;
            }

            if (line.starts_with("#")) {
                if (line.starts_with("#version")) {
                    output += line + '\n';
                } else if (line.starts_with("#include")) {
                    size_t open = line.find_first_of('<') + 1;
                    size_t close = line.find_last_of('>');

                    bool usingIncludeDirectory = true;

                    if (open == std::string::npos && close == std::string::npos) {
                        open = line.find_first_of('"') + 1;
                        close = line.find_last_of('"');

                        if (open == std::string::npos || close == std::string::npos) {
                            throw SyntaxError("Preprocessor include is not written properly", i);
                        } else {
                            usingIncludeDirectory = false;
                        }
                    } else if (open == std::string::npos || close == std::string::npos) {
                        throw SyntaxError("Preprocessor include expects '<' location '>', or '\"' relative location '\"'", i);
                    }

                    size_t len = close - open;

                    /*
                    std::filesystem::path dirname(m_IncludeDirectory);
                    std::filesystem::path basename(line.substr(open, close));

                    std::filesystem::path location = dirname / basename;
                    */

                    std::string location = usingIncludeDirectory ? m_IncludeDirectory + '/' + line.substr(open, len) : line.substr(open, len);

                    std::string include_content;
                    Load(location, include_content);

                    output += include_content + '\n';
                }
            } else {
                for (const auto& pair : m_ConstantMap) {
                    size_t pos = line.find(pair.first);
                    if (pos != std::string::npos) {
                        line.replace(pos, pair.first.length(), pair.second);
                    }
                }

                output += line + "\n";
            }
        }
    }

}