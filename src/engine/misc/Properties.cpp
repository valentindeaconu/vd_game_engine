//
// Created by Vali on 11/10/2020.
//

#include "Properties.hpp"

namespace vd::misc {
    template<> PropertiesPtr Properties::Create<Properties::PropertiesInputType::eString>(const std::string& content) {
        PropertiesPtr ptr = std::make_shared<Properties>();
        ptr->Parse(content);

        return ptr;
    }

    template<> PropertiesPtr Properties::Create<Properties::PropertiesInputType::eFile>(const std::string& input) {
        std::string content = ReadFile(input);

        return Properties::Create<eString>(content);
    }

    Properties::Properties()
        : m_Map()
    {
    }

    Properties::~Properties() = default;

    bool Properties::Empty() const {
        return m_Map.empty();
    }

    template<> bool Properties::Get<bool>(const std::string& key) const {
        auto it = m_Map.find(key);
        if (it != m_Map.end()) {
            auto& value = it->second;

            if (value.size() != 1) {
                throw std::bad_cast();
            }

            return std::stoi(value[0]) > 0;
        }

        throw std::invalid_argument("unknown key for this properties object");
    }

    template<> int Properties::Get<int>(const std::string& key) const {
        auto it = m_Map.find(key);
        if (it != m_Map.end()) {
            auto& value = it->second;

            if (value.size() != 1) {
                throw std::bad_cast();
            }

            return std::stoi(value[0]);
        }

        throw std::invalid_argument("unknown key for this properties object");
    }

    template<> float Properties::Get<float>(const std::string& key) const {
        auto it = m_Map.find(key);
        if (it != m_Map.end()) {
            auto& value = it->second;

            if (value.size() != 1) {
                throw std::bad_cast();
            }

            return std::stof(value[0]);
        }

        throw std::invalid_argument("unknown key for this properties object");
    }

    template<> glm::vec2 Properties::Get<glm::vec2>(const std::string& key) const {
        auto it = m_Map.find(key);
        if (it != m_Map.end()) {
            auto& value = it->second;

            if (value.size() != 2) {
                throw std::bad_cast();
            }

            return glm::vec2(std::stof(value[0]), std::stof(value[1]));
        }

        throw std::invalid_argument("unknown key for this properties object");
    }

    template<> glm::vec3 Properties::Get<glm::vec3>(const std::string& key) const {
        auto it = m_Map.find(key);
        if (it != m_Map.end()) {
            auto& value = it->second;

            if (value.size() != 3) {
                throw std::bad_cast();
            }

            return glm::vec3(std::stof(value[0]), std::stof(value[1]), std::stof(value[2]));
        }

        throw std::invalid_argument("unknown key for this properties object");
    }

    template<> glm::vec4 Properties::Get<glm::vec4>(const std::string& key) const {
        auto it = m_Map.find(key);
        if (it != m_Map.end()) {
            auto& value = it->second;

            if (value.size() != 4) {
                throw std::bad_cast();
            }

            return glm::vec4(std::stof(value[0]), std::stof(value[1]), std::stof(value[2]), std::stof(value[3]));
        }

        throw std::invalid_argument("unknown key for this properties object");
    }

    template<> std::string Properties::Get<std::string>(const std::string& key) const {
        auto it = m_Map.find(key);
        if (it != m_Map.end()) {
            auto& value = it->second;

            if (value.size() != 1) {
                throw std::bad_cast();
            }

            return value[0];
        }

        throw std::invalid_argument("unknown key for this properties object");
    }

    const std::vector<std::string>& Properties::Get(const std::string& key) const {
        auto it = m_Map.find(key);
        if (it != m_Map.end()) {
            return it->second;
        }

        throw std::invalid_argument("unknown key for this properties object");
    }

    bool Properties::Set(const std::string& key, const std::vector<std::string>& values) {
        if (m_Map.find(key) != m_Map.end()) {
            return false;
        }

        m_Map[key] = values;
        return true;
    }

    bool Properties::Set(const std::string& key, const std::string& value) {
        return this->Set(key, std::vector<std::string>({ value }));
    }

    void Properties::Change(const std::string& key, const std::vector<std::string>& values) {
        auto it = m_Map.find(key);
        if (it != m_Map.end()) {
            it->second = values;
        } else {
            this->Set(key, values);
        }
    }

    void Properties::Change(const std::string& key, const std::string& value) {
        this->Change(key, std::vector<std::string>({ value }));
    }

    std::string Properties::ReadFile(const std::string& filePath) {
        std::ifstream in(filePath);

        if (!in.is_open()) {
            throw std::invalid_argument("no properties file found at location " + filePath);
        }

        std::string content;

        in.seekg(0, std::ios::end);
        content.reserve(in.tellg());
        in.seekg(0, std::ios::beg);

        content.assign(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());

        in.close();

        return content;
    }

    void Properties::Parse(const std::string& content) {
        std::vector<std::string> lines;

        boost::split(lines, content, [](char c) { return c == '\n'; });

        for (size_t i = 0; i < lines.size(); ++i) {
            std::string& line = lines[i];
            boost::trim(line);

            // Ignore empty lines and comment lines
            if (line.empty() || line.starts_with("#")) {
                continue;
            }

            std::vector<std::string> key_and_value;
            boost::split(key_and_value, line, [](char c) { return c == '='; });

            if (key_and_value.size() != 2) {
                vd::Logger::warn("could not find a valid key-value pair on line " + std::to_string(i));
                continue;
            }

            std::string& key = key_and_value[0];
            std::string& value = key_and_value[1];

            boost::trim(key);
            boost::trim(value);

            std::vector<std::string> values;
            boost::split(values, value, [](char c) { return c == ' '; });

            this->m_Map[key] = values;
        }
    }
}