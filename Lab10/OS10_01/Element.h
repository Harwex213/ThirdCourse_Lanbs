#pragma once

namespace HT
{
	struct Element
	{
		Element();
		Element(const void* key, int keyLength);
		Element(const void* key, int keyLength, const void* payload, int payloadLength);
		Element(Element* oldElement, const void* payload, int payloadLength);

		int keyLength;
		int payloadLength;
		const void* key;
		const void* payload;

		void initDefault();
		void setKey(const void* key, int keyLength);
		char* getKey() const;
		void setPayload(const void* payload, int payloadLength);
		char* getPayload() const;
	};
}