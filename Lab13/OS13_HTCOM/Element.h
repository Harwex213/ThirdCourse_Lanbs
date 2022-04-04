#pragma once

struct Element
{
public: // Constructors
	Element();
	Element(const void* key, int keyLength);
	Element(const void* key, int keyLength, const void* payload, int payloadLength);
	Element(const Element* oldElement, const void* payload, int payloadLength);

public: // Fields
	int keyLength;
	int payloadLength;
	const void* key;
	const void* payload;
	bool isDeleted;

public: // Getters & Setters
	void setKeyPointer(const void* key, int keyLength);
	bool setKey(const void* key, int keyLength);
	char* getKey() const;
	int getKeyLength() const;

	void setPayloadPointer(const void* payload, int payloadLength);
	bool setPayload(const void* payload, int payloadLength);
	char* getPayload() const;
	int getPayloadLength() const;

	bool getIsDeleted() const;
	void setIsDeleted(bool value);

private: // Private methods
	void initDefault();

public: // Public methods

};
