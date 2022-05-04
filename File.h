#pragma once
#include "QInt.h"
#include "Qfloat.h"

bool isConvert(string request); // is a convert request. Ex: decimal to binary
void execInt(fstream& fin, fstream& fout); // read, handle (QInt) and write file
void execFloat(fstream& fin, fstream& fout); // read, handle (QFloat) and write file