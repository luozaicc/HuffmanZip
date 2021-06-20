#include "zip.h"
#include "rezip.h"


int main()
{
	const char* s = "123.doc";
	const char* str2 = "zip123.txt";
	zip(s);
	rezip(str2);
	return 0;
}