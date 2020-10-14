//
// Created by Vali on 10/14/2020.
//

#ifndef VD_GAME_ENGINE_TREEHELPER_HPP
#define VD_GAME_ENGINE_TREEHELPER_HPP

#include <vector>

#include <engine/foundation/datastruct/Tree.hpp>

namespace vd::algorithm {
    class TreeHelper {
    public:
        template<int Dim>
        static void CollectLeafNodes(const typename datastruct::Tree<Dim>::ptr_type_t& root,
                                     std::vector<typename datastruct::Tree<Dim>::ptr_type_t>& output);

        template<int Dim>
        static void CollectNodesOnLevel(const typename datastruct::Tree<Dim>::ptr_type_t& root,
                                        int level,
                                        std::vector<typename datastruct::Tree<Dim>::ptr_type_t>& output);
    };

}


#endif //VD_GAME_ENGINE_TREEHELPER_HPP
