//
// Created by Vali on 11/10/2020.
//

#include "Properties.hpp"

namespace vd::property {
    Properties::Properties()
        : m_Map()
    {
    }

    Properties::~Properties() = default;

    Properties& Properties::operator=(const Properties& other) = default;

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

}