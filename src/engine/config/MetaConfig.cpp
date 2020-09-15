//
// Created by Vali on 9/15/2020.
//

#include "MetaConfig.hpp"

namespace vd::config {
    MetaConfig::MetaConfig()
        : enableFunc([]() {})
        , disableFunc([]() {})
    {
    }

    MetaConfig::MetaConfig(ConfigWorkerFunction enableFunc, ConfigWorkerFunction disableFunc)
        : enableFunc(std::move(enableFunc))
        , disableFunc(std::move(disableFunc))
    {
    }

    MetaConfig::~MetaConfig() = default;

    void MetaConfig::enable() {
        this->enableFunc();
    }
    void MetaConfig::disable() {
        this->disableFunc();
    }

    void MetaConfig::setEnableFunction(MetaConfig::ConfigWorkerFunction enableFunc) {
        this->enableFunc = std::move(enableFunc);
    }
    void MetaConfig::setDisableFunction(MetaConfig::ConfigWorkerFunction disableFunc) {
        this->disableFunc = std::move(disableFunc);
    }
}
