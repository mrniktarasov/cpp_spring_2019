#pragma once
#include <string>       
#include <iostream>    
#include <sstream>  

enum class Error
{
    NoError,
    CorruptedArchive
};

class Serializer
{
		static constexpr char Separator = ' ';
		std::ostream& out_;
		int counter = 0;
	public:
		explicit Serializer (std::ostream& out)
			: out_(out)
		{
		}	

		template <class T>
		Error save(T& object)
		{
			return object.serialize(*this);
		}

		template <typename... ArgsT>
		Error operator()(ArgsT... args)
		{
			return process(args...);
		}
    
	private:
		template <class T>
		Error process(T&& val)
		{
			counter++;
			if (typeid (T) == typeid (bool))
				{
					if (val == true)
						out_ << "true"<<Separator;
					else
						out_ << "false"<<Separator;
					return Error::NoError;
				}
			else
			{
				out_ << std::to_string(val)<<Separator;
				return Error::NoError;
			}
		}
		
		template <typename T, typename...ArgsT>
		Error process (T&& val, ArgsT&&...args)
		{
			process (std::forward <T> (val));
			return process(std::forward<ArgsT>(args)...);
		}
	
};

class Deserializer 
{
	static constexpr char Separator = ' ';
	std::istream& in_;
	public:
		explicit Deserializer (std::istream& in)
			: in_(in)
		{
		}
		
		template <typename DT>
		Error load (DT& object)
		{
			std::string input;
			int counter = 0;
			int64_t a1;
			std::string b1;
			int64_t c1;
			
			getline ( in_, input );
			std::stringstream ss (input);
			std::string inWord;
			
			while (ss>>inWord)
			{
				if (counter == 0)
				{
					std::stringstream oneWord;
					oneWord<<inWord;
					oneWord>>a1;
					if ( a1 < 0)
						return Error::CorruptedArchive;
					object.a = a1;
				}
				if (counter == 1)
				{
					if (inWord == "true")
						object.b = true;	
					else
						if (inWord == "false")
							object.b = false;
					else
						return Error::CorruptedArchive;
				}
				if (counter == 2)
				{
					std::stringstream oneWord;
					oneWord<<inWord;
					oneWord>>c1;
					if ( c1 < 0)
						return Error::CorruptedArchive;
					object.c = c1;
				}
				counter++;
			}
			if (counter < 1)
				return Error::CorruptedArchive;
			return Error::NoError;
		}
};
