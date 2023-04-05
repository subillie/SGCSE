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

	int i, j, lenp = strlen(pattern);

	failure[0] = -1;
	for(j = 1; j < lenp; j++) {
		i = failure[j - 1];
		while ((pattern[j] != pattern[i + 1]) && (i >= 0)) i = failure[i];
		if (pattern[j] == pattern[i + 1]) failure[j] = i + 1;
		else failure[j] = -1;
	}
}

/* Match string with pattern, based on failure */
void matchWithPattern(char *string, char *pattern, FILE *fp_res) {

	int i_str = 0, i_pat = 0, count = 0;
	int lens = strlen(string);
	int lenp = strlen(pattern);
	int *result = (int *)malloc(sizeof(int) * lens);

	while (i_str < lens) {
		while (i_pat >= 0) {
			if (string[i_str] == pattern[i_pat])
				break;
			i_pat = failure[i_pat];
		}
		i_str++;
		i_pat++;
		if (i_pat == lenp) {
			result[count] = i_str - i_pat;
			count++;
		}
	}

	fprintf(fp_res, "%d\n", count);
	for (int i = 0; i < count; i++)
		fprintf(fp_res, "%d ", result[i]);
	free(result);
}
