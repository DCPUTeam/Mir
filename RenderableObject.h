///
/// @brief The class declaration for updatable entities.
/// @author James Rhodes
///

#ifndef __MIR_RENDERABLE_OBJECT_H
#define __MIR_RENDERABLE_OBJECT_H

///
/// @brief Represents the updatable attributes of an entity.
///
class RenderableObject
{
public:
    virtual void Render() = 0;
};

#endif
