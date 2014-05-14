#ifndef Artemis_IntervalEntitySystem_h__
#define Artemis_IntervalEntitySystem_h__

#include "artemis/EntitySystem.h"

namespace artemis
{

/**
 * A system that processes entities at a interval in milliseconds.
 * A typical usage would be a collision system or physics system.
 *
 * @author Arni Arent
 * @port   Vladimir Ivanov (ArCorvus)
 *
 */
class IntervalEntitySystem : public EntitySystem
{
private:
    float acc;
    float interval;

public:
    IntervalEntitySystem(Aspect *aspect, float pInterval, EntitySystemType tp): EntitySystem(aspect, tp), interval(pInterval), acc(0.f) {}

protected:
    bool checkProcessing() override;
};
}
#endif // Artemis_IntervalEntitySystem_h__
