//
// Created by Vali on 10/14/2020.
//

#ifndef VD_GAME_ENGINE_TREE_HPP
#define VD_GAME_ENGINE_TREE_HPP

#include <memory>
#include <array>

namespace vd::datastruct {
    template<int Dim>
    class Tree {
    public:
        typedef std::shared_ptr< Tree<Dim> >    ptr_type_t;
        typedef std::array<ptr_type_t, Dim>     arr_type_t;

        Tree();
        Tree(const Tree* parent, int level, int nodeIndex);
        ~Tree();

        virtual void Populate();
        virtual void Clear();

        [[nodiscard]] int GetLevel() const;
        [[nodiscard]] int GetNodeIndex() const;
        [[nodiscard]] bool IsLeaf() const;

        [[nodiscard]] const Tree* GetParent() const;

        ptr_type_t& GetChild(size_t index);
        const ptr_type_t& GetChild(size_t index) const;

        const arr_type_t& GetChildren() const;
    protected:
        const int m_kLevel;
        const int m_kNodeIndex;

        bool m_Leaf;

        const Tree* m_kParent;
        arr_type_t m_Children;
    };

    typedef Tree<4> Quadtree;
};


#endif //VD_GAME_ENGINE_TREE_HPP
