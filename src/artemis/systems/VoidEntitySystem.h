#ifndef Artemis_VoidEntitySystem_h__
#define Artemis_VoidEntitySystem_h__

namespace artemis
{

/**
 * This system has an empty aspect so it processes no entities, but it still gets invoked.
 * You can use this system if you need to execute some game logic and not have to concern
 * yourself about aspects or entities.
 * 
 * @author Arni Arent
 * @port   Vladimir Ivanov (ArCorvus)
 *
 */
class VoidEntitySystem : public EntitySystem
{
public:
   VoidEntitySystem(EntitySystemType tp): EntitySystem(Aspect::getEmpty(), tp) {}

protected:
   void processEntities(Bag<Entity *> *entities) override
   {
      processSystem();
   }

   virtual void processSystem() = 0;

   bool checkProcessing() override { return true; }

};
}
#endif // Artemis_VoidEntitySystem_h__
