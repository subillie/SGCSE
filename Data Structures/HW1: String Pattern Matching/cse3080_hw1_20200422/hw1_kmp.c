#include "hw1.h"

void getFailure(char *);
void matchWithPattern(char *, char *, FILE *);

int main() {

	FILE *fp_str = fopen("string.txt", "r");
	FILE *fp_pat = fopen("pattern.txt", "r");
	if (!fp_str || !fp_pat) {
		printf("The string file does not exist.\n");
		exit(1);
	}
	fgets(string, MAX_STRING_SIZE, fp_str);
	fgets(pattern, MAX_PATTERN_SIZE, fp_pat);
	fclose(fp_str);
	fclose(fp_pat);

	FILE *fp_res = fopen("result_kmp.txt", "w");
	getFailure(pattern);
	matchWithPattern(string, pattern, fp_res);
	fclose(fp_res);

	return 0;
}

/* Initialize (int) failure[MAX_PATTERN_SIZE] */
void getFailure(char *pattern) {

	int value, i;
	int patternlen = strlen(pattern);

	failure[0] = -1;
	for(i = 1; i < patternlen; i++) {
		value = failure[i - 1];

		// If there was matching prefix and suffix,
		// find the longest prefix of pattern[0..i] that is also a suffix of pattern[0..i]
		while ((pattern[i] != pattern[value + 1]) && (value >= 0))
			value = failure[value];

		// If there is matching prefix and suffix, failure[i] = 0
		if (pattern[i] == pattern[value + 1])
			failure[i] = value + 1;

		// If there is no matching prefix and suffix, failure[i] = -1
		else failure[i] = -1;
	}
}

/* Match string with pattern, based on failure */
void matchWithPattern(char *string, char *pattern, FILE *fp_res) {

	int iterator = 0, index_p = 0, count = 0;
	int stringlen = strlen(string);
	int patternlen = strlen(pattern);
	int *result = (int *)malloc(sizeof(int) * stringlen);

	while (iterator < stringlen) {
		while (index_p >= 0) {
			// Break if there is a matching prefix and suffix
			if (string[iterator] == pattern[index_p]) break;
			// Break if index_p = -1
			index_p = failure[index_p];
		}
		iterator++;
		index_p++;
		// If index_p == patternlen, there is a full match
		if (index_p == patternlen)
			result[count++] = iterator - index_p;
	}

	fprintf(fp_res, "%d\n", count);
	for (int i = 0; i < count; i++)
		fprintf(fp_res, "%d ", result[i]);
	free(result);
}
