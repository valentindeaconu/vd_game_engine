//
// Created by Vali on 9/15/2020.
//

#ifndef VD_GAME_ENGINE_METACONFIG_HPP
#define VD_GAME_ENGINE_METACONFIG_HPP

#include <memory>
#include <functional>

namespace vd::config {
    class MetaConfig {
    public:
        typedef std::function<void()>   ConfigWorkerFunction;

        MetaConfig();
        MetaConfig(ConfigWorkerFunction enableFunc, ConfigWorkerFunction disableFunc);
        ~MetaConfig();

        void enable();
        void disable();

        void setEnableFunction(ConfigWorkerFunction enableFunc);
        void setDisableFunction(ConfigWorkerFunction disableFunc);
    private:
        ConfigWorkerFunction enableFunc;
        ConfigWorkerFunction disableFunc;
    };
    typedef std::shared_ptr<MetaConfig> MetaConfigPtr;
}


#endif //VD_GAME_ENGINE_METACONFIG_HPP
