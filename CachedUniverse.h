///
/// @brief The class declaration for a Universe.
/// @author James Rhodes
///

#ifndef __MIR_UNIVERSE_H
#define __MIR_UNIVERSE_H

#include <list>
#include "CachedActor.h"
#include "CachedShip.h"
#include "CachedCelestialBody.h"
#include "Network/Controller.h"
#include "Network/IdentifiableObject.h"
#include "Network/UpdatableObject.h"
#include "RenderableObject.h"

///
/// @brief Represents a cached universe on the client side.
///
class CachedUniverse : public Network::IdentifiableObject, public Network::UpdatableObject, public RenderableObject
{
private:
    std::list<CachedActor*> m_CachedActors;
    std::list<CachedShip*> m_CachedShips;
    std::list<CachedCelestialBody*> m_CachedCelestialBodies;
    Network::Controller* m_Controller;
    bool m_SphereGenerated;
    int m_SphereID;

public:
    CachedUniverse(Network::Controller& controller, std::string identifier)
        : Network::IdentifiableObject(controller, identifier), m_SphereGenerated(false) {}
    virtual void ReceiveMessage(Network::Message& message);
    virtual void Update();
    virtual void Render();
};

#endif
