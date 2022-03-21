#pragma once
#include <fstream>
extern std::fstream LogCOM;
#define LOG(x,y) LogCOM<<":"<<(x)<<(y)<<"\n"
