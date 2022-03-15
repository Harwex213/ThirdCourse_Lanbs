#pragma once
#include "pch.h"

#define INVALID_MUTEX_HANDLE_ERROR "Cannot create mutex for IPC with storage"

#define CREATE_FILE_ERROR "Cannot create storage file"
#define CLOSE_FILE_ERROR "Cannot close storage file"

#define CREATE_FILE_MAPING_ERROR "Cannot create file mapping of storage"
#define CLOSE_FILE_MAPPING_ERROR "Cannot close file mapping of storage"

#define MAP_FILE_VIEW_ERROR "Cannot map view of file storage"
#define UNMAP_FILE_VIEW_ERROR "Cannot unmap view of file storage"

#define FLUSH_VIEW_ERROR "Cannot flush view of file storage"

#define CLEAR_ALL_STORAGE_MEMORY_ERROR "Error was arise when tryed to clear all memory storage"

#define STORAGE_IS_FULL_ERROR "Cannot insert element. Storage is full"
#define ELEMENT_KEY_ALREADY_EXIST "Cannot insert element. Such key is already used"

#define ELEMENT_ARGUMENT_NULL_ERROR "Argument of Element null received"
#define ELEMENT_NOUT_FOUND_ERROR "Requested element not found in storage"