
#ifndef SIMPLE_LOCK_H
#define SIMPLE_LOCK_H

class SimpleLock
{
	public:
		SimpleLock()
		{

		}
		void lock()
		{
			// while(mLock);
			// mLock = true;
		}
		void unlock()
		{
			// mLock = false;
		}
	private:
		volatile bool mLock;
};

#endif /* SIMPLE_LOCK_H */