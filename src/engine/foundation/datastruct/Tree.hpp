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
        typedef std::shared_ptr< Tree<Dim> >  ptr_type_t;

        Tree();
        Tree(const Tree* parent, int level, int nodeIndex);
        ~Tree();

        void Populate();
        void Clear();

        [[nodiscard]] int GetLevel() const;
        [[nodiscard]] int GetNodeIndex() const;
        [[nodsicard]] bool IsLeaf() const;

        ptr_type_t& GetChild(size_t index);
        const ptr_type_t& GetChild(size_t index) const;
    private:

        const Tree* m_kParent;

        const int m_kLevel;
        const int m_kNodeIndex;

        bool m_Leaf;


        std::array<ptr_type_t, Dim>  m_Children;
    };

    typedef Tree<4> Quadtree;
};


#endif //VD_GAME_ENGINE_TREE_HPP
