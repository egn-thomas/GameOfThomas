#ifndef BLOCK_TYPE_UTILS_HPP
#define BLOCK_TYPE_UTILS_HPP

#include "../environnement/Node.hpp"
#include "BlockEnum.hpp"

inline BlockType getBlockTypeFromNode(const Node* n)
{
    int mask = 0;

    if (!n->left)   mask |= LEFT_EXIT;
    if (!n->top)    mask |= TOP_EXIT;
    if (!n->right)  mask |= RIGHT_EXIT;
    if (!n->bottom) mask |= BOTTOM_EXIT;

    return static_cast<BlockType>(mask);
}

#endif
