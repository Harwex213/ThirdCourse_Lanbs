#include "pch.h"
#include "Element.h"

namespace Core
{
	Element::Element()
	{
		this->initDefault();
	}

	Element::Element(const void* key, int keyLength)
	{
		this->initDefault();
		this->setKeyPointer(key, keyLength);
	}

	Element::Element(const void* key, int keyLength, const void* payload, int payloadLength)
	{
		this->initDefault();
		this->setKeyPointer(key, keyLength);		
		this->setPayloadPointer(payload, payloadLength);
	}

	Element::Element(const Element* oldElement, const void* payload, int payloadLength)
	{
		this->initDefault();		
		this->setKeyPointer(oldElement->key, oldElement->keyLength);
		this->setPayloadPointer(payload, payloadLength);
	}

	void Element::initDefault()
	{
		this->keyLength = 0;
		this->payloadLength = 0;
		this->key = nullptr;
		this->payload = nullptr;
		this->isDeleted = false;
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

	int Element::getKeyLength() const
	{
		return keyLength;
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

	int Element::getPayloadLength() const
	{
		return payloadLength;
	}

	bool Element::getIsDeleted() const
	{
		return isDeleted;
	}

	void Element::setIsDeleted(bool value)
	{
		isDeleted = value;
	}

	void Element::setKeyPointer(const void* key, int keyLength)
	{
		this->key = key;
		this->keyLength = keyLength;
	}

	void Element::setPayloadPointer(const void* payload, int payloadLength)
	{
		this->payload = payload;
		this->payloadLength = payloadLength;
	}
}