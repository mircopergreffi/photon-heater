
#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <exception>

// Source: https://en.wikipedia.org/wiki/Circular_buffer
// TODO: size(), isFull()

typedef size_t buffer_t;

struct OutOfBoundsException : public exception
{
	const char * what() const throw()
	{
		return "Out Of Bounds Exception";
	}
}
struct EmptyBufferException : public exception
{
	const char * what() const throw()
	{
		return "Empty Buffer Exception";
	}
}

template<typename T, buffer_t size>
class CircularBuffer
{
	public:
		// Initialize variables
		CircularBuffer()
		{
			mStart = 0;
			mEnd = 0;
		}
		// Push a new elment at the tail of the buffer
		void push(const T & item)
		{
			if (isFull())
				mStart = (mStart + 1) % size)
			mBuffer[mEnd++] = item;
			mEnd %= size;
		}
		// Pop the element at the head of the buffer
		T pop()
		{
			if (isEmpty())
				throw EmptyBufferException();
			T item = mBuffer[mStart++];
			mStart %= size;
			return item;
		}
		// Returns the element a the give index
		T get(buffer_t index) const
		{
			if (isEmpty() || index >= size())
				throw OutOfBoundsException();
			index += mStart;
			index %= size;
			return mBuffer[index];
		}
		// Returns the size of the buffer
		buffer_t size() const
		{
			if (mEnd >= mStart)
				return mEnd - mStart;
			else
				return mEnd - mStart + size;
		}
		// Returns whether the buffer is empty
		bool isEmpty() const
		{
			return mStart == mEnd;
		}
		// Returns whether the buffer is full
		bool isFull() const
		{
			return mStart == ((mEnd + 1) % size);
		}
	private:
		volatile buffer_t mStart, mEnd;
		volatile T mBuffer[size];
}

#endif /* CIRCULAR_BUFFER_H */