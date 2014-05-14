#ifndef Artemis_Timer_h__
#define Artemis_Timer_h__

namespace artemis
{

class Timer
{
private:
   float delay;
	bool repeat;
	float acc;
	bool done;
	bool stopped;
	
public:
   Timer(float delay): Timer(delay, false) {} // C++11

   Timer(float delay, bool repeat): delay(delay), repeat(repeat), acc(0), done(false), stopped(false) {}

	void update(float delta)
   {
		if (!done && !stopped) {
			acc += delta;

			if (acc >= delay) {
				acc -= delay;

				if (repeat) {
					reset();
				} else {
					done = true;
				}

				execute();
			}
		}
	}

	void reset()
   {
		stopped = false;
		done = false;
		acc = 0;
	}

	bool isDone()
   {
		return done;
	}

	bool isRunning()
   {
		return !done && acc < delay && !stopped;
	}

	void stop()
   {
		stopped = true;
	}

	void setDelay(int delay)
   {
		this.delay = delay;
	}

	virtual void execute() = 0;

	float getPercentageRemaining()
   {
		if (done)
			return 100;
		else if (stopped)
			return 0;
		else
			return 1 - (delay - acc) / delay;
	}

	float getDelay()
   {
		return delay;
	}

};
}
#endif // Artemis_Timer_h__
