#include "pch.h"
#include "Element.h"

namespace HT
{
	Element::Element()
	{
		initDefault();
	}

	Element::Element(const void* key, int keyLength)
	{
		initDefault();
		setKeyPointer(key, keyLength);
	}

	Element::Element(const void* key, int keyLength, const void* payload, int payloadLength)
	{
		initDefault();
		setKeyPointer(key, keyLength);
		setPayloadPointer(payload, payloadLength);
	}

	Element::Element(Element* oldElement, const void* payload, int payloadLength)
	{
		initDefault();
		setKeyPointer(oldElement->key, oldElement->keyLength);
		setPayloadPointer(payload, payloadLength);
	}

	void Element::setKeyPointer(const void* key, int keyLength)
	{
		this->key = key;
		this->keyLength = keyLength;
	}

	bool Element::setKey(const void* key, int keyLength)
	{
		if (this->key == nullptr)
			return false;

		strcpy_s((char*)this->key, keyLength, (char*)key);
		this->keyLength = keyLength;
		
		return true;
	}

	char* Element::getKey() const
	{
		return (char*)(key == nullptr ? "" : key);
	}

	void Element::setPayloadPointer(const void* payload, int payloadLength)
	{
		this->payload = payload;
		this->payloadLength = payloadLength;
	}

	bool Element::setPayload(const void* payload, int payloadLength)
	{
		if (this->payload == nullptr)
			return false;

		strcpy_s((char*)this->payload, payloadLength, (char*)payload);
		this->payloadLength = payloadLength;

		return true;
	}

	char* Element::getPayload() const
	{
		return (char*)(payload == nullptr ? "" : payload);
	}

	void Element::initDefault()
	{
		keyLength = 0;
		payloadLength = 0;
		key = nullptr;
		payload = nullptr;
		isDeleted = false;
	}
}