#include "hw1.h"

void Strstr(char *, char *, FILE *);

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

	FILE *fp_res = fopen("result_naive.txt", "w");
	Strstr(string, pattern, fp_res);
	fclose(fp_res);

	return 0;
}

void Strstr(char *string, char *pattern, FILE *fp_res) {

	int	i = 0, j, count = 0;
	int *result = (int *)malloc(sizeof(int) * (strlen(string)));

	while (string[i]) {
		j = 0;
		if (string[i] == pattern[j]) {
			while (pattern[j] && (string[i + j] == pattern[j])) j++;
			if (!pattern[j]) {
				result[count] = i;
				count++;
			}
		}
		i++;
	}

	fprintf(fp_res, "%d\n", count);
	for (i = 0; i < count; i++)
		fprintf(fp_res, "%d ", result[i]);
	free(result);
}
