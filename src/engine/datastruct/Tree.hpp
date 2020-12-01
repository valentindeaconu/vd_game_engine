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

        void ResetLevel();

        [[nodiscard]] bool Leaf() const;

        [[nodiscard]] int Level() const;
        [[nodiscard]] int NodeIndex() const;

        [[nodiscard]] const Tree* Parent() const;

        ptr_type_t& Child(size_t index);
        [[nodiscard]] const ptr_type_t& Child(size_t index) const;

        const arr_type_t& Children() const;
    protected:
        bool        m_Leaf;
        int         m_Level;
        const int   m_kNodeIndex;

        const Tree* m_kpParent;
        arr_type_t  m_Children;
    };

    typedef Tree<4> Quadtree;
};


#endif //VD_GAME_ENGINE_TREE_HPP
