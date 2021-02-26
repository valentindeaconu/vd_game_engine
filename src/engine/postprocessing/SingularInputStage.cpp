//
// Created by Vali on 2/24/2021.
//

#include "SingularInputStage.hpp"

namespace mod::postprocessing {

    SingularInputStage::SingularInputStage(std::string name,
                                           vd::postprocessing::FrameBufferGetter inputGetter,
                                           vd::Predicate precondition,
                                           vd::Consumer beforeExecution,
                                           vd::Consumer afterExecution)
        : vd::postprocessing::Stage(std::move(name))
        , m_Getter(std::move(inputGetter))
        , m_Precondition(std::move(precondition))
        , m_BeforeExecution(std::move(beforeExecution))
        , m_AfterExecution(std::move(afterExecution))
    {
    }
}