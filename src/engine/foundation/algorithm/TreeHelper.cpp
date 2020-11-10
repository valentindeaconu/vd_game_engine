//
// Created by Vali on 10/14/2020.
//

#include "TreeHelper.hpp"

namespace vd::algorithm {
    template<int Dim>
    void TreeHelper::CollectLeafNodes(const typename datastruct::Tree<Dim>::ptr_type_t& root,
                                      std::vector<typename datastruct::Tree<Dim>::ptr_type_t>& output) {
        if (root->IsLeaf()) {
            output.emplace_back(root);
        } else {
            for (int i = 0; i < Dim; ++i) {
                CollectLeaves<Dim>(root->GetChild(i), output);
            }
        }
    }

    template<int Dim>
    void TreeHelper::CollectNodesOnLevel(const typename datastruct::Tree<Dim>::ptr_type_t& root,
                                         int level,
                                         std::vector<typename datastruct::Tree<Dim>::ptr_type_t> &output) {
        if (root->GetLevel() == level) {
            output.emplace_back(root);
        } else {
            for (int i = 0; i < Dim; ++i) {
                CollectLeaves<Dim>(root->GetChild(i), output);
            }
        }
    }
}