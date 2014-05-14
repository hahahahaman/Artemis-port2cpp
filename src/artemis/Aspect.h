#ifndef Atemis_Aspect_h__
#define Atemis_Aspect_h__

#include <bitset>
#include "artemis/ComponentType.h"

namespace artemis
{

/**
 * An Aspects is used by systems as a matcher against entities, to check if a system is
 * interested in an entity. Aspects define what sort of component types an entity must
 * possess, or not possess.
 *
 * This creates an aspect where an entity must possess A and B and C:
 * Aspect.getAspectForAll(A.class, B.class, C.class)
 *
 * This creates an aspect where an entity must possess A and B and C, but must not possess U or V.
 * Aspect.getAspectForAll(A.class, B.class, C.class).exclude(U.class, V.class)
 *
 * This creates an aspect where an entity must possess A and B and C, but must not possess U or V, but must possess one of X or Y or Z.
 * Aspect.getAspectForAll(A.class, B.class, C.class).exclude(U.class, V.class).one(X.class, Y.class, Z.class)
 *
 * You can create and compose aspects in many ways:
 * Aspect.getEmpty().one(X.class, Y.class, Z.class).all(A.class, B.class, C.class).exclude(U.class, V.class)
 * is the same as:
 * Aspect.getAspectForAll(A.class, B.class, C.class).exclude(U.class, V.class).one(X.class, Y.class, Z.class)
 *
 * @author Arni Arent
 * @port   Vladimir Ivanov (ArCorvus)
 *
 */
class Aspect
{
    friend class EntitySystem;
private:
    std::bitset<64> allSet;
    std::bitset<64> exclusionSet;
    std::bitset<64> oneSet;

    Aspect() {}

protected:
    std::bitset<64> & getAllSet()
    {
        return allSet;
    }
    std::bitset<64> & getExclusionSet()
    {
        return exclusionSet;
    }
    std::bitset<64> & getOneSet()
    {
        return oneSet;
    }

public:

    /**
     * Returns an aspect where an entity must possess all of the specified component types.
     * @param type a required component type
     * @param types a required component type
     * @return an aspect that can be matched against entities
     */
    Aspect * all()
    {
        return this;
    }

    template<typename... Args>
    Aspect * all(ComponentType c1, Args... args)
    {
        allSet.set(c1);

        all(args...);

        return this;
    }

    /**
     * Excludes all of the specified component types from the aspect. A system will not be
     * interested in an entity that possesses one of the specified exclusion component types.
     *
     * @param type component type to exclude
     * @param types component type to exclude
     * @return an aspect that can be matched against entities
     */
    Aspect * exclude(size_t n, ...);

    /**
     * Returns an aspect where an entity must possess one of the specified component types.
     * @param type one of the types the entity must possess
     * @param types one of the types the entity must possess
     * @return an aspect that can be matched against entities
     */
    Aspect * one(size_t n, ...);

    /**
     * Creates an aspect where an entity must possess all of the specified component types.
     *
     * @param type the type the entity must possess
     * @param types the type the entity must possess
     * @return an aspect that can be matched against entities
     *
     * @deprecated
     * @see getAspectForAll
     */
    //static Aspect * getAspectFor(size_t n, ...);

    /**
     * Creates an aspect where an entity must possess all of the specified component types.
     *
     * @param type a required component type
     * @param types a required component type
     * @return an aspect that can be matched against entities
     */

    template<typename... Args>
    static Aspect * getAspectForAll(ComponentType c1, Args... args)
    {
        Aspect *aspect = new Aspect();
        aspect->all(c1, args...);
        return aspect;
    }

    /**
     * Creates an aspect where an entity must possess one of the specified component types.
     *
     * @param type one of the types the entity must possess
     * @param types one of the types the entity must possess
     * @return an aspect that can be matched against entities
     */
    static Aspect * getAspectForOne(size_t n, ...);

    /**
     * Creates and returns an empty aspect. This can be used if you want a system that processes no entities, but
     * still gets invoked. Typical usages is when you need to create special purpose systems for debug rendering,
     * like rendering FPS, how many entities are active in the world, etc.
     *
     * You can also use the all, one and exclude methods on this aspect, so if you wanted to create a system that
     * processes only entities possessing just one of the components A or B or C, then you can do:
     * Aspect.getEmpty().one(A,B,C);
     *
     * @return an empty Aspect that will reject all entities.
     */
    static Aspect * getEmpty()
    {
        return new Aspect();
    }
};
}
#endif // Atemis_Aspect_h__
