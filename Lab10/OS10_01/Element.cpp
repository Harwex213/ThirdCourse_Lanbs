#include "Element.h"
#include <iostream>

namespace HT
{
	Element::Element()
	{
		initDefault();
	}

	Element::Element(const void* key, int keyLength)
	{
		initDefault();
		setKey(key, keyLength);
	}

	Element::Element(const void* key, int keyLength, const void* payload, int payloadLength)
	{
		initDefault();
		setKey(key, keyLength);
		setPayload(payload, payloadLength);
	}

	Element::Element(Element* oldElement, const void* payload, int payloadLength)
	{
		initDefault();
		setKey(oldElement->key, oldElement->keyLength);
		setPayload(payload, payloadLength);
	}

	void Element::setKey(const void* key, int keyLength)
	{
		this->key = key;
		this->keyLength = keyLength;
	}

	char* Element::getKey() const
	{
		return (char*)(key == nullptr ? "" : key);
	}

	void Element::setPayload(const void* payload, int payloadLength)
	{
		this->payload = payload;
		this->payloadLength = payloadLength;
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
	}
}