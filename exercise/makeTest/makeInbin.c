#include "../mylib.h"

int main(int argc, char** argv)
{
	FILE *fo = fopen("in.bin", "wb");
	char buffer[1024];
	memset(buffer, 0, sizeof(buffer));
	strcpy(buffer, "Hello World!!!");
	int i = 0;
	for(; i < 10; i++)
		fwrite(buffer, sizeof(buffer), 1, fo);
	i = 0;
	strcpy(buffer, "Goodbye World!!!");
	for(; i < 15; i++)
		fwrite(buffer, sizeof(buffer), 1, fo);
	i = 0;
	strcpy(buffer, "Hi all");
	for(; i < 20; i++)
		fwrite(buffer, sizeof(buffer), 1, fo);
	fclose(fo);
	return 0;
}
