
#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <exception>

// Source: https://en.wikipedia.org/wiki/Circular_buffer
// TODO: size(), isFull()

class OutOfBoundsException : public std::exception
{
	public:
	const char * what() const throw()
	{
		return "Out Of Bounds Exception";
	}
};
class EmptyBufferException : public std::exception
{
	public:
	const char * what() const throw()
	{
		return "Empty Buffer Exception";
	}
};

template<typename T, size_t size>
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
				mStart = (mStart + 1) % size;
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
		const T& get(size_t index) const
		{
			if (isEmpty() || index >= length())
				throw OutOfBoundsException();
			index += mStart;
			index %= size;
			return mBuffer[index];
		}
		// const T& get(size_t index) const
		// {
		// 	return get(index);
		// }
		// Returns the size of the buffer
		size_t length() const
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
		size_t mStart, mEnd;
		T mBuffer[size];
};

#endif /* CIRCULAR_BUFFER_H */