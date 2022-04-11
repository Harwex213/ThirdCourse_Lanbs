#pragma once

#define WIN32_LEAN_AND_MEAN             // Исключите редко используемые компоненты из заголовков Windows
// Файлы заголовков Windows
#include <windows.h>
#include <objbase.h>
#include <assert.h>
#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <chrono>
#include <thread>
#include <mutex>
#include <random>
#include "sddl.h"
#include <Lm.h>
#pragma comment (lib,"netapi32.lib")
