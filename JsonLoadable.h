
#ifndef JSON_LOADABLE_H
#define JSON_LOADABLE_H

#include <ArduinoJson.h>
#include <exception>

class InvalidJsonException : public std::exception 
{
	public:
		InvalidJsonException(const char * w)
		: mWhat(w)
		{
		}

		InvalidJsonException()
		: mWhat("Invalid JSON configuration")
		{
		}

		const char * what() const throw()
		{
			return mWhat;
		}
	
	private:
		const char * mWhat;
	
};

class JsonLoadable
{
	public:
		// Method for loading JSON data
		virtual void loadFromJson(JsonObject const &json) throw(InvalidJsonException) = 0;
};

#endif /* JSON_LOADABLE_H */