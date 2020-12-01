//
// Created by Vali on 10/14/2020.
//

#include "Tree.hpp"

namespace vd::datastruct {
    template class Tree<4>;

    template<int Dim>
    Tree<Dim>::Tree()
        : m_kpParent(nullptr)
        , m_Level(0)
        , m_kNodeIndex(-1)
        , m_Leaf(true)
    {
    }

    template<int Dim>
    Tree<Dim>::Tree(const Tree* parent, int level, int nodeIndex)
        : m_kpParent(parent)
        , m_Level(level)
        , m_kNodeIndex(nodeIndex)
        , m_Leaf(true)
    {
    }

    template<int Dim>
    Tree<Dim>::~Tree() {
        Clear();
    }

    template<int Dim>
    void Tree<Dim>::Populate() {
        if (!m_Leaf) {
            Clear();
        }

        m_Leaf = false;
        for (int i = 0; i < Dim; ++i) {
            m_Children[i] = std::make_shared<Tree<Dim>>(this, m_Level + 1, i);
        }
    }

    template<int Dim>
    void Tree<Dim>::Clear() {
        if (!m_Leaf) {
            for (int i = 0; i < Dim; ++i) {
                if (m_Children[i] != nullptr) {
                    m_Children[i] = nullptr;
                }
            }

            m_Leaf = true;
        }
    }

    template<int Dim>
    void Tree<Dim>::ResetLevel() {
        m_Level = 0;
    }


    template<int Dim>
    int Tree<Dim>::Level() const {
        return m_Level;
    }

    template<int Dim>
    int Tree<Dim>::NodeIndex() const {
        return m_kNodeIndex;
    }

    template<int Dim>
    bool Tree<Dim>::Leaf() const {
        return m_Leaf;
    }

    template<int Dim>
    const Tree<Dim>* Tree<Dim>::Parent() const {
        return m_kpParent;
    }

    template<int Dim>
    typename Tree<Dim>::ptr_type_t& Tree<Dim>::Child(size_t index) {
        return m_Children[index];
    }

    template<int Dim>
    const typename Tree<Dim>::ptr_type_t& Tree<Dim>::Child(size_t index) const {
        return m_Children[index];
    }

    template<int Dim>
    const typename Tree<Dim>::arr_type_t& Tree<Dim>::Children() const {
        return m_Children;
    }
}