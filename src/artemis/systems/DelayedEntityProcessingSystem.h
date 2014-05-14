#ifndef Artemis_DelayedEntityProcessingSystem_h__
#define Artemis_DelayedEntityProcessingSystem_h__

#include "artemis/EntitySystem.h"

namespace artemis
{

/**
 * The purpose of this class is to allow systems to execute at varying intervals.
 * An example system would be an ExpirationSystem, that deletes entities after a certain 
 * lifetime. Instead of running a system that decrements a timeLeft value for each entity, 
 * you can simply use this system to execute in a future at a time of the shortest 
 * lived entity, and then reset the system to run at a time in a future at a time of 
 * the shortest lived entity, etc.
 *
 * Another example system would be an AnimationSystem. You know when you have to 
 * animate a certain entity, e.g. in 300 milliseconds. So you can set the system to 
 * run in 300 ms. to perform the animation. This will save CPU cycles in some scenarios.
 * There are three methods you must implement, processDelta(..) means you have to 
 * decrement the delta for the specified entity, getRemainingDelay(..) must return the 
 * delay that remains for the specified entity, and processExpired(..) will be called when 
 * an specified entity expires.
 * 
 * @author Arni Arent
 * @port   Vladimir Ivanov (ArCorvus)
 *
 */
class DelayedEntityProcessingSystem : public EntitySystem
{
private:
   float delay;
	bool running;
	float acc;

public:
   DelayedEntityProcessingSystem(Aspect *aspect, EntitySystemType tp): EntitySystem(aspect, tp), delay(0.f), running(false), acc(0.f) {}

protected:
   void processEntities(Bag<Entity *> *entities) override /*final*/;
	
   void inserted(Entity *e) override;
	
	/**
	 * Return the delay until this entity should be processed.
	 * 
	 * @param e entity
	 * @return delay
	 */
	virtual float getRemainingDelay(Entity *e) = 0;
	
	bool checkProcessing() override /*final*/;
	
	/**
	 * Process a entity this system is interested in. Substract the accumulatedDelta
	 * from the entities defined delay.
	 * 
	 * @param e the entity to process.
	 * @param accumulatedDelta the delta time since this system was last executed.
	 */
	virtual void processDelta(Entity *e, float accumulatedDelta) = 0;
	virtual void processExpired(Entity *e) = 0;
	
	/**
	 * Start processing of entities after a certain amount of delta time.
	 * 
	 * Cancels current delayed run and starts a new one.
	 * 
	 * @param delta time delay until processing starts.
	 */
public:
   void restart(float pDelay)
   {
		delay = pDelay;
		acc = 0;
		running = true;
	}
	
	/**
	 * Restarts the system only if the delay offered is shorter than the
	 * time that the system is currently scheduled to execute at.
	 * 
	 * If the system is already stopped (not running) then the offered
	 * delay will be used to restart the system with no matter its value.
	 * 
	 * If the system is already counting down, and the offered delay is 
	 * larger than the time remaining, the system will ignore it. If the
	 * offered delay is shorter than the time remaining, the system will
	 * restart itself to run at the offered delay.
	 * 
	 * @param delay
	 */
	void offerDelay(float delay)
   {
		if (!running || (delay < getRemainingTimeUntilProcessing())) {
			restart(delay);
		}
	}
	
	/**
	 * Get the initial delay that the system was ordered to process entities after.
	 * 
	 * @return the originally set delay.
	 */
	float getInitialTimeDelay() const { return delay; }
	
	/**
	 * Get the time until the system is scheduled to run at.
	 * Returns zero (0) if the system is not running.
	 * Use isRunning() before checking this value.
	 * 
	 * @return time when system will run at.
	 */
	float getRemainingTimeUntilProcessing() const { return running? (delay - acc) : 0; }
	
	/**
	 * Check if the system is counting down towards processing.
	 * 
	 * @return true if it's counting down, false if it's not running.
	 */
	bool isRunning() const { return running; }
	
	/**
	 * Stops the system from running, aborts current countdown.
	 * Call offerDelay or restart to run it again.
	 */
	void stop()
   {
		running = false;
		acc = 0;
	}
};
}
#endif // Artemis_DelayedEntityProcessingSystem_h__
