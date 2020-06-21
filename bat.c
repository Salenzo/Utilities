#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {
	if (argc < 2 || argc > 3) {
		puts("[bat] argc should be 2 or 3");
		puts("[bat] usage: bat.exe [title] <command>");
		return 1;
	}
	argv++;
	{
		char t[128] = "title ";
		size_t i = 6, j = 0;
		for (i = 6; i < 126; i++) {
			if (strchr("^&<>|", argv[0][j])) {
				t[i] = '^';
				i++;
			}
			t[i] = argv[0][j];
			j++;
		}
		t[i] = 0;
		system(t);
	}
	if (argc >= 3) argv++;
	{
		int a = system(*argv);
		system("pause");
		return a;
	}
}
