#pragma once
#include "HtHandle.h"

#define DLL_API EXTERN_C  __declspec(dllexport)

using namespace Core;


namespace HT
{
	DLL_API Core::HTHANDLE* Create   //  создать HT             
	(
		int	  capacity,					   // емкость хранилища
		int   secSnapshotInterval,		   // переодичность сохранения в сек.
		int   maxKeyLength,                // максимальный размер ключа
		int   maxPayloadLength,            // максимальный размер данных
		const char fileName[CHAR_MAX_LENGTH]           // имя файла
	); 	// != NULL успешное завершение  

	DLL_API Core::HTHANDLE* Load     //  загрузить HT             
	(
		const char fileName[CHAR_MAX_LENGTH]		   // имя файла 
	); 	// != NULL успешное завершение  

	DLL_API Core::HTHANDLE* Open     //  открыть HT             
	(
		const char fileName[CHAR_MAX_LENGTH]		   // имя файла 
	); 	// != NULL успешное завершение  

	DLL_API BOOL Snap         // выполнить Snapshot
	(
		Core::HTHANDLE* hthandle           // управление HT (File, FileMapping)
	);

	DLL_API BOOL Close        // Snap и закрыть HT  и  очистить HTHANDLE
	(
		Core::HTHANDLE* hthandle           // управление HT (File, FileMapping)
	);	//  == TRUE успешное завершение   

	DLL_API BOOL Insert      // добавить элемент в хранилище
	(
		Core::HTHANDLE* hthandle,            // управление HT
		const Core::Element* element              // элемент
	);	//  == TRUE успешное завершение 

	DLL_API BOOL Update     //  именить элемент в хранилище
	(
		Core::HTHANDLE* hthandle,            // управление HT
		const Core::Element* oldelement,          // старый элемент (ключ, размер ключа)
		const void* newpayload,          // новые данные  
		int             newpayloadlength     // размер новых данных
	); 	//  != NULL успешное завершение 

	DLL_API BOOL Delete      // удалить элемент в хранилище
	(
		Core::HTHANDLE* hthandle,            // управление HT (ключ)
		const Core::Element* element              // элемент 
	);	//  == TRUE успешное завершение 

	DLL_API Core::Element* Get     //  читать элемент в хранилище
	(
		Core::HTHANDLE* hthandle,            // управление HT
		const Core::Element* element              // элемент 
	); 	//  != NULL успешное завершение 

	DLL_API char* GetHTLastError  // получить сообщение о последней ошибке
	(
		Core::HTHANDLE* ht                         // управление HT
	);

	DLL_API void Print                               // распечатать элемент 
	(
		const Core::Element* element              // элемент 
	);

	DLL_API void PrintAllElements
	(
		Core::HTHANDLE* ht
	);

	DLL_API Core::Element* CreateElementWithKey(const void* key, int keyLength);

	DLL_API Core::Element* CreateElementWithKeyPayload(const void* key, int keyLength, const void* payload, int payloadLength);

	DLL_API Core::StorageConfig* GetStorageConfig(Core::HTHANDLE* htHandle);
}