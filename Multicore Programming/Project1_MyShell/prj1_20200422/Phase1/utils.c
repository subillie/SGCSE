#include "myshell.h"

void *Malloc(size_t size) 
{
	void *p;

	if ((p  = malloc(size)) == NULL)
		unix_error("Malloc error");
	return p;
}

void whaaaaat(char *cmdline, t_history **history, FILE *fp_history) {

	if (explamation_flag == -1) {		// !! command
		for (int i = 0; argv[i]; i++) {
			char *token = strtok(argv[i], "!!"); // TODO !만 인식되는지 !!가 인식되는지
			if (token == NULL)
				printf("%s", cur->cmd);
			while (token != NULL) {
				char *prev_token = token;
				token = strtok(NULL, "!!");
				if (token == NULL)
					printf("%s", cur->cmd);
				printf("%s", prev_token);
			}
		}
		printf("\n");
		eval(cur->cmd, history, fp_history);
		explamation_flag = -2;
		return 1;
	}

	if (explamation_flag > -1) {		// !# command
		int start = 0;
		while ((argv[explamation_flag][start] == '!') && isdigit(argv[explamation_flag][start + 1]))
			start++;
		int num = atoi(argv[explamation_flag][start]부터 끝까지); //TODO
		int count = 1;
		t_history *cur = *history;
		while (cur->next && count < num) {
			count++;
			cur = cur->next;
		}
		if (count == num && cur) {
			for (int i = 0; argv[i]; i++) {
				argv[explamation_flag] -= 1; //TODO
				char *token = strtok(argv[i], argv[explamation_flag]);
				if (token == NULL)
					printf("%s", cur->cmd);
				while (token != NULL) {
					char *prev_token = token;
					token = strtok(NULL, (argv[explamation_flag]));
					if (token == NULL)
						printf("%s", cur->cmd);
					printf("%s", prev_token);
				}
			}
			printf("\n");
		}
		else
			fprintf(stderr, "CSE4100: %s: event not found\n", argv[0]);
		explamation_flag = -2;
		return 1;
	}
}
