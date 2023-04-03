#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test(char *cmdline, char *buf) {
	int i_b = 0;
	while (buf[i_b]) {
		if (buf[i_b] == '\'' || buf[i_b] == '\"') {
			char quote = buf[i_b];	// quote = ' or "
			int opening_q = i_b + 1;	// opening_q = index of the opening quote
			while (buf[opening_q] && buf[opening_q] != quote)
				opening_q++;
			if (buf[opening_q] == quote) {
				int closing_q = opening_q;	// closing_q = index of the closing quote
				while (buf[closing_q]) {
					buf[closing_q - 1] = buf[closing_q];
					closing_q++;
				}
				buf[closing_q] = ' ';
			}
		}
		i_b++;
	}
}

int main() {
	char *cmdline = "ls -l | grep \"hello world\" | wc -l &";
	char buf[8192];
	test(cmdline, buf);
	printf("%s\n", buf);
	return 0;
}