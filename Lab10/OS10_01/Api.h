#pragma once
#include <Windows.h>
#include "HtHandle.h"
#include "Element.h"

namespace HT
{
	/* API HT - программный интерфейс для доступа к НТ - хранилищу
				НТ-хранилище предназначено для хранения данных в ОП в формате ключ/значение
				Персистестеность (сохранность) данных обеспечивается с помощью snapshot-менханизма
				Create - создать  и открыть HT-хранилище для использования
				Open   - открыть HT-хранилище для использования
				Insert - создать элемент данных
				Delete - удалить элемент данных
				Get    - читать  элемент данных
				Update - изменить элемент данных
				Snap   - выпонить snapshot
				Close  - выполнить Snap и закрыть HT-хранилище для использования
				GetLastError - получить сообщение о последней ошибке
	*/

	HTHANDLE* Create   //  создать HT             
	(
		int	  capacity,					   // емкость хранилища
		int   secSnapshotInterval,		   // переодичность сохранения в сек.
		int   maxKeyLength,                // максимальный размер ключа
		int   maxPayloadLength,            // максимальный размер данных
		const char fileName[CHAR_MAX_LENGTH]           // имя файла 
	); 	// != NULL успешное завершение  

	HTHANDLE* Open     //  открыть HT             
	(
		const char fileName[CHAR_MAX_LENGTH]		   // имя файла 
	); 	// != NULL успешное завершение  

	BOOL Snap         // выполнить Snapshot
	(
		HTHANDLE* hthandle           // управление HT (File, FileMapping)
	);

	BOOL Close        // Snap и закрыть HT  и  очистить HTHANDLE
	(
		HTHANDLE* hthandle           // управление HT (File, FileMapping)
	);	//  == TRUE успешное завершение   

	BOOL Insert      // добавить элемент в хранилище
	(
		HTHANDLE* hthandle,            // управление HT
		const Element* element              // элемент
	);	//  == TRUE успешное завершение 

	BOOL Update     //  именить элемент в хранилище
	(
		HTHANDLE* hthandle,            // управление HT
		const Element* oldelement,          // старый элемент (ключ, размер ключа)
		const void* newpayload,          // новые данные  
		int             newpayloadlength     // размер новых данных
	); 	//  != NULL успешное завершение 

	BOOL Delete      // удалить элемент в хранилище
	(
		HTHANDLE* hthandle,            // управление HT (ключ)
		const Element* element              // элемент 
	);	//  == TRUE успешное завершение 

	Element* Get     //  читать элемент в хранилище
	(
		HTHANDLE* hthandle,            // управление HT
		const Element* element              // элемент 
	); 	//  != NULL успешное завершение 

	char* GetLastError  // получить сообщение о последней ошибке
	(
		HTHANDLE* ht                         // управление HT
	);

	void Print                               // распечатать элемент 
	(
		const Element* element              // элемент 
	);

	void PrintAllElements
	(
		HTHANDLE* ht
	);
};