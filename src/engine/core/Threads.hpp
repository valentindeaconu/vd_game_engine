//
// Created by vali on 4/16/21.
//

#ifndef VDGE_THREADS_HPP
#define VDGE_THREADS_HPP

#include <engine/injector/ObjectOfType.hpp>
#include <engine/core/ThreadPool.hpp>
#include <engine/defines/Types.hpp>

inline void ON_RENDER(vd::Consumer fn) {
    vd::ObjectOfType<vd::core::ThreadPool>::Find()->CreateJobFor(std::move(fn), "Render", true);
}

inline void ON_UPDATE(vd::Consumer fn) {
    vd::ObjectOfType<vd::core::ThreadPool>::Find()->CreateJobFor(std::move(fn), "Update", true);
}

#endif //VDGE_THREADS_HPP
