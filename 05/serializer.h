#pragma once
#include <string>       
#include <iostream>    
#include <sstream>
#include <type_traits> 

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
			if (std::is_same <uint64_t, std::remove_reference_t<T>>::value)
				return setUint64 (std::forward<T> (val));
			else if (std::is_same <bool, std::remove_reference_t<T>>::value)
				return setBool (std::forward<T> (val));
			else
				return Error::CorruptedArchive;	
		}
		
		template <typename T, typename...ArgsT>
		Error process (T&& val, ArgsT&&...args)
		{
			Error r = process (std::forward <T> (val));
			if ( r == Error::NoError)
				return process(std::forward<ArgsT>(args)...);
			else 
				return Error::CorruptedArchive;
		}
		
		template <class T>
		Error setBool (T&& value)
		{
			if (value == true)
				out_ << "true"<<Separator;
			else if (value == false)
				out_ << "false"<<Separator;
			else
				return Error::CorruptedArchive;
			return Error::NoError;
		}
		
		template <class T>
		Error setUint64 (T&& value)
		{
			out_ <<value<<Separator;
			return Error::NoError;
		}
};

class Deserializer 
{
	std::istream& in_;
	public:
		explicit Deserializer (std::istream& in)
			: in_(in)
		{
		}
		
		template <typename DT>
		Error load (DT& object)
		{
			return object.serialize(*this);
		}
		
		template <typename... ArgsT>
		Error operator()(ArgsT&... args)
		{
			return processDes(args...);
		}
		
	private:
		template <class T>
		Error processDes(T&& val)
		{	
			if (std::is_same <uint64_t, std::remove_reference_t<T>>::value)
				return loadUint64 (std::forward<T> (val));
			else if (std::is_same <bool, std::remove_reference_t<T>>::value)
				return loadBool (std::forward<T> (val));
			else
				return Error::CorruptedArchive;	
		}
		
		template <typename T, typename...ArgsT>
		Error processDes (T&& val, ArgsT&&...args)
		{
			Error r = processDes (std::forward <T> (val));
			if ( r == Error::NoError)
				return processDes(std::forward<ArgsT>(args)...);
			else 
				return Error::CorruptedArchive;
		}
		
		template <class T>
		Error loadBool (T&& value)
		{
			std::string oneWord;
			oneWord = getWord();
			
			if (oneWord == "true")
				value = true;
			else if (oneWord == "false")
				value = false;
			else
				return Error::CorruptedArchive;
			return Error::NoError;
		}
		
		template <class T>
		Error loadUint64 (T&& value)
		{
			std::stringstream ss;
			std::string oneWord;
			int64_t a1;
			
			oneWord = getWord();
			ss<<oneWord;
			ss>>a1;
			
			if ( a1 < 0)
				return Error::CorruptedArchive;
			value = a1;
			return Error::NoError;
		}
		
		std::string getWord()
		{
			std::string oneWord;
			in_>>oneWord;
			return oneWord;
		}
};
