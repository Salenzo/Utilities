#include <stdio.h>

int main(int argc, char** argv) {
	argc--; argv++;
	while (argc--) printf("%s%c", *argv++, argc ? ' ' : '\n');
	return 0;
}
