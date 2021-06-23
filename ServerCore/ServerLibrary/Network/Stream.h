#pragma once
#include "stdafx.h"

namespace leeder
{
using PACKET_SIZE = uint32_t;

class Stream
{
public:
	Stream();


	const char* GetBuffer() { return mBuffer; }
	PACKET_SIZE	GetLength() { return mHead; }




protected:
	void	ReallocBuffer(uint32_t size);

	char*		mBuffer;
	PACKET_SIZE	mHead;
	uint32_t	mCapacity;


};



class OutputStream : public Stream
{
public:
	OutputStream();



	template< typename T >
	void operator << (const T& t)
	{
		this->write(t);
	}

	template<>
	void operator << (const std::vector< int >& intVector)
	{
		size_t size = intVector.size();
		this->write(size);
		this->write(intVector.data(), size * sizeof(int));
	}

	template<>
	void operator << (const std::string& string)
	{
		size_t size = string.size();

		this->write(size);
		this->write(string.data(), sizeof(char) * size);
	}

	template<>
	void operator << (const std::wstring& wstr)
	{
		size_t size = wstr.size();

		this->write(size);
		this->write(wstr.data(), sizeof(char) * size);
	}

	template< typename T >
	void operator << (const std::vector< T >& tVector)
	{
		size_t size = tVector.size();
		this->write(size);
		for (const T& element : tVector)
		{
			this->write(element);
		}
	}




	//Class 객체를 받을 때는 해당 클래스를 특수화 할 것
	template<>
	void operator << (const std::unordered_map<uint32_t, ObjectInfo>& map)
	{
		size_t size = map.size();

		this->write(size);

		for (auto& element : map)
		{
			this->write(element.first);
			this->write(element.second);

		}
	}

	template<>
	void operator << (const std::unordered_map<uint32_t, GameObject>& map)
	{
		size_t size = map.size();

		this->write(size);

		for (auto element : map)
		{
			this->write(element.first);
			this->write(element.second);

		}
	}


protected:
	void		write(const void* data, size_t size);

	template<class T>
	void write(T value)
	{
		static_assert(std::is_arithmetic< T >::value ||
			std::is_enum< T >::value,
			"Not Generic Data");

			write(&value, sizeof(value));
	}

	template<>
	void write(Input input)
	{
		write(input.GetType());
		write(input.GetTimeStamp());
		
	}


	template<>
	void write(ObjectInfo info)
	{
		this->write(info.mType);
		this->write(&info.mState, sizeof(info.mState));
		this->write(info.mScale);
		this->write(info.mMoveState);

		switch (info.mState)
		{
		case eObjectState::CREATE:
		{
			this->write(info.mPos);
		}
					break;
		case eObjectState::ACTION:
		{
			this->write(info.mPos);

		}
		break;
		default:
			break;
		}
	}


	template<>
	void write(GameObject obj)
	{
		this->write(obj.GetType());
		this->write(obj.GetPosition());
		this->write(obj.GetScale());
	}


	template<>
	void write(Position position)
	{
		write(position.mX);
		write(position.mY);
	}

};



class InputStream : public Stream
{
public:
	InputStream(char* buffer, uint32_t size);


	template< typename T >
	void operator >> (T& t)
	{
		this->read(t);
	}

	template<>
	void operator >> (std::string& str)
	{
		size_t size;
		this->read(size);

		str.resize(size);

		for (auto& c : str)
		{
			this->read(c);
		}

	}

	template<>
	void operator >> (std::wstring& wstr)
	{
		size_t size;
		this->read(size);

		wstr.resize(size);

		for (auto& c : wstr)
		{
			this->read(c);
		}
	}


	template< typename T >
	void operator >> (std::vector< T >& tVector)
	{
		size_t size;
		this->read(size);
		tVector.resize(size);
		for (auto& element : tVector)
		{
			this->read(element);
		}
	}


	//Class 객체를 받을 때는 해당 클래스를 특수화 할 것
	template<>
	void operator >> (std::unordered_map<uint32_t, ObjectInfo>& map)
	{
		size_t size;
		this->read(size);

		for(int i=0 ; i< size; ++i)
		{
			uint32_t networkID;
			this->read(networkID);

			ObjectInfo info;
			this->read(info);

			map[networkID] = info;
		}
	}

	template<>
	void operator >> (std::unordered_map<uint32_t, GameObject>& map)
	{
		size_t size;
		this->read(size);

		for (int i = 0; i < size; ++i)
		{
			uint32_t networkID;
			this->read(networkID);

			GameObject obj;
			this->read(obj);

			map[networkID] = obj;
		}
	}

protected:
	void		read(void* data, uint32_t size);


	template< typename T > 
	void read(T& data)
	{
		static_assert(std::is_arithmetic< T >::value ||
			std::is_enum< T >::value,
			"Generic Read only supports primitive data types");

		read(&data, sizeof(data));
	}

	template<>
	void read(Input& input)
	{
		eInputType type;

		read(type);

		float timeStamp;
		read(timeStamp);

		input.SetType(type);
		input.SetTimeStamp(timeStamp);
	}



	template<>
	void read(ObjectInfo& info)
	{
		this->read(info.mType);
		this->read(&info.mState, sizeof(info.mState));
		this->read(info.mScale);
		this->read(info.mMoveState);

		switch (info.mState)
		{
		case eObjectState::CREATE:

			this->read(info.mPos);
			break;

		case eObjectState::ACTION:
			this->read(info.mPos);
			break;

		case eObjectState::DESTROY:
			break;


		default:
			break;

		}
	}

	template<>
	void read(GameObject& obj)
	{
		this->read(obj.GetType());
		this->read(obj.GetPosition());
		this->read(obj.GetScale());

	}


	template<>
	void read(Position& position)
	{
		read(position.mX);
		read(position.mY);
	}


};




}
