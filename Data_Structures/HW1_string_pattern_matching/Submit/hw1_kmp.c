#include "hw1.h"

void getFailure(char *);
void matchWithPattern(char *, char *, FILE *);

int main() {

	FILE *fp_str = fopen("string.txt", "r");
	FILE *fp_pat = fopen("pattern.txt", "r");
	if (!fp_str || !fp_pat) {
		printf("The string file does not exist.\n");
		return 0;
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

void getFailure(char *pattern) {

	int idx_pat = 1, idx_fail = -1;
	int size_pat = strlen(pattern);

	failure[0] = -1;
	while (idx_pat <= size_pat) {
		if (idx_fail == -1 || pattern[idx_pat - 1] == pattern[idx_fail]) {
			failure[idx_pat] = idx_fail + 1;
			idx_pat++;
			idx_fail++;
		}
		else
			idx_fail = failure[idx_fail];
	}
}

void matchWithPattern(char *string, char *pattern, FILE *fp_res) {

	int idx_str = 0, idx_pat = 0, count = 0;
	int size_str = strlen(string);
	int size_pat = strlen(pattern);
	int *result = (int *)malloc(sizeof(int) * size_str);

	while (idx_str < size_str) {
		while (idx_pat >= 0) {
			if (string[idx_str] == pattern[idx_pat])
				break;
			idx_pat = failure[idx_pat];
		}
		idx_str++;
		idx_pat++;
		if (idx_pat == size_pat) {
			result[count] = idx_str - idx_pat;
			count++;
		}
	}

	fprintf(fp_res, "%d\n", count);
	for (int i = 0; i < count; i++)
		fprintf(fp_res, "%d ", result[i]);
	free(result);
}
