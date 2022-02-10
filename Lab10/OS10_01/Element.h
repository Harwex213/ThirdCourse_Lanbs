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
		bool isDeleted;

		void initDefault();
		void setKeyPointer(const void* key, int keyLength);
		bool setKey(const void* key, int keyLength);
		char* getKey() const;
		void setPayloadPointer(const void* payload, int payloadLength);
		bool setPayload(const void* payload, int payloadLength);
		char* getPayload() const;
	};
}