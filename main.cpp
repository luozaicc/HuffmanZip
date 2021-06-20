#include "zip.h"
#include "rezip.h"


int main()
{
	const char* ori_name = "123.doc";
	const char* zip_name = "zip123.txt";
	zip(ori_name);
	rezip(zip_name);
	return 0;
}