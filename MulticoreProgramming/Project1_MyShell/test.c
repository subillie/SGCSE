#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// void test(char *cmdline, char *buf) {
// 	/* Ignore matching quotes */
// 	i_b = 0;
// 	int inside_quote = 0;
// 	char quote_char = '\0';

// 	for (int i = 0; i < strlen(cmdline); i++) {
// 		if (cmdline[i] == '"' || cmdline[i] == '\'') {
// 			if (inside_quote && cmdline[i] == quote_char) {
// 				// End of quote found, do not include the quotes in the buf
// 				inside_quote = 0;
// 				quote_char = '\0';
// 			} else if (!inside_quote) {	// Start of quote found
// 				inside_quote = 1;
// 				quote_char = cmdline[i];
// 			} else
// 				buf[i_b++] = cmdline[i];	// Nested quotes found
// 		} else {
// 			if (inside_quote == 0)
// 				buf[i_b++] = cmdline[i];	// Add non-quote characters to the buf
// 			else
// 				buf[i_b++] = cmdline[i];	// Add quote characters to the buf
// 		}
// 	}
// }

void test(char *cmdline, char *buf) {
	int i_b = 0;
	while (buf[i_b]) {
		if (buf[i_b] == '\'' || buf[i_b] == '\"') {
			char quote = buf[i_b];			// quote - ' or "
			int opening_q = i_b + 1;		// opening_q - index of the opening quote
			while (buf[opening_q] && buf[opening_q] != quote)
				opening_q++;
			if (buf[opening_q] == quote) {
				int closing_q = opening_q;	// closing_q - index of the closing quote
				while (buf[closing_q]) {
					buf[closing_q - 1] = buf[closing_q];
					closing_q++;
				}
				buf[closing_q] = ' ';
			}
		}
		i_b++;
	}
	buf[i_b] = 0;
}

int main() {
	char *cmdline = "ls -l | grep \"hello world\" | wc -l &";
	char *buf = (char *)malloc(strlen(cmdline) + 1);
	// char buf[8192];
	test(cmdline, buf);
	printf("%s\n", buf);
	return 0;
}