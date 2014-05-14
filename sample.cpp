#include <artemis/World.h>
#include <artemis/Entity.h>
#include <artemis/Component.h>
#include <artemis/systems/EntityProcessingSystem.h>
#include <artemis/Aspect.h>
#include <iostream>
#include <iomanip>

//using namespace artemis;

enum ComponentTypes {
   ctPosition = 0,
   ctRender,
   ctMove,
};

enum EntitySystemTypes {
   estRender = 0,
   estMove,
};

class PositionComponent : public artemis::Component
{
public:
   float pos;

   PositionComponent(float posX, float posY): Component(ctPosition), pos(posX) {}
};

class RenderComponent : public artemis::Component
{
public:
   void *mEntity;
   void *mNode;

   RenderComponent(): Component(ctRender) {};
};

class MoveComponent : public artemis::Component
{
public:
   float speed;
   bool bGo;
   MoveComponent(float s): Component(ctMove), speed(s), bGo(false) {}
};

class EntityRenderSystem : public artemis::EntityProcessingSystem
{
private:
   artemis::ComponentMapper<RenderComponent, ctRender> renderMapper;
   artemis::ComponentMapper<PositionComponent, ctPosition> posMapper;
public:
   EntityRenderSystem(): EntityProcessingSystem(artemis::Aspect::getAspectForAll(ctRender, ctPosition), estRender)
   {
      registerMapper(&renderMapper);
      registerMapper(&posMapper);
   }
   virtual void process(artemis::Entity *e) override
   {
      std::cout << e->getId() << ':' << posMapper.get(e)->pos << " ";
   };
};

class EntityMoveSystem : public artemis::EntityProcessingSystem
{
private:
   artemis::ComponentMapper<PositionComponent, ctPosition> posMapper;
   artemis::ComponentMapper<MoveComponent, ctMove> moveMapper;
public:
   EntityMoveSystem(): EntityProcessingSystem(artemis::Aspect::getAspectForAll(ctPosition, ctMove), estMove)
   {
      registerMapper(&posMapper);
      registerMapper(&moveMapper);
   }
   virtual void process(artemis::Entity *e) override
   {
      posMapper.get(e)->pos += getWorld()->delta * moveMapper.get(e)->speed;
   };
};

int main()
{
   artemis::World *mWorld = new artemis::World();
   mWorld->setSystem(new EntityRenderSystem());
   mWorld->setSystem(new EntityMoveSystem());

   mWorld->initialize();

   artemis::Entity *ent1 = mWorld->createEntity();
   ent1->addComponent(new PositionComponent(0,0));
   ent1->addComponent(new RenderComponent());
   ent1->addComponent(new MoveComponent(40));
   ent1->addToWorld();
   artemis::Entity *ent2 = mWorld->createEntity();
   ent2->addComponent(new PositionComponent(0,0));
   ent2->addComponent(new MoveComponent(1));
   ent2->addToWorld();

   int i = 0;
   while (++i < 30) {
      std::cout << std::setprecision(2) << std::fixed << std::endl << '(' << i << '|' << i*0.25f << ")\t";
      mWorld->setDelta(0.25f);
      mWorld->process();
      if (i == 5) {
         ent1->removeComponent(ctMove);
         ent1->changedInWorld();
      }
      else if (i == 10) {
         ent1->addComponent(new MoveComponent(100));
         ent1->addComponent(new RenderComponent());
         ent1->changedInWorld();
         ent2->addComponent(new RenderComponent());
         ent2->changedInWorld();
      }
      else if (i == 15) {
         ent2->deleteFromWorld();
      }
      else if (i == 20) {
         artemis::Entity *ent3 = mWorld->createEntity();
         ent3->addComponent(new PositionComponent(0,0));
         ent3->addComponent(new MoveComponent(10));
         ent3->addComponent(new RenderComponent());
         ent3->addToWorld();
      }
   }

   delete mWorld;
}
