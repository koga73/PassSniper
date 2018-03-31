#ifndef ___GENERATOR___
#define ___GENERATOR___

#include "filebuffer.h"
#include "options.h"

using namespace std;

class Generator {
	public:
		static void generate(Options*& options, FileBuffer*& fb);
};

#endif