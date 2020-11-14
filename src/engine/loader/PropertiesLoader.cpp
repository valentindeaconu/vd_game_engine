//
// Created by Vali on 11/14/2020.
//

#include "PropertiesLoader.hpp"

namespace vd::loader {

    misc::PropertiesPtr PropertiesLoader::Load(const std::string& path) {
        std::vector<std::string> lines;
        FileLoader::Load(path, lines);

        misc::PropertiesPtr propertiesPtr = std::make_shared<misc::Properties>();

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
                throw SyntaxError("could not find a valid key-value pair", i);
            }

            std::string& key = key_and_value[0];
            std::string& value = key_and_value[1];

            boost::trim(key);
            boost::trim(value);

            std::vector<std::string> values;
            boost::split(values, value, [](char c) { return c == ' '; });

            propertiesPtr->Set(key, values);
        }

        return propertiesPtr;
    }

}