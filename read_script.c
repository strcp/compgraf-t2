#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "structs.h"
char *trim(char *str)
{
	size_t len = 0;
	char *frontp = str - 1;
	char *endp = NULL;

	if (str == NULL)
		return NULL;

	if (str[0] == '\0')
		return str;

	len = strlen(str);
	endp = str + len;

	while (isspace(*(++frontp)));
	while (isspace(*(--endp)) && endp != frontp);

	if (str + len - 1 != endp)
		*(endp + 1) = '\0';
	else if (frontp != str && endp == frontp)
		*str = '\0';

	endp = str;
	if (frontp != str) {
		while (*frontp) 
			*endp++ = *frontp++;
		*endp = '\0';
	}

	return str;
}

int main(int argc, char **argv) {
	FILE *f;
	char buf[80], *script;
	size_t rb;
	int i;

	if (argc != 2) {
		printf("usage:%s <script file>\n", argv[0]);
		return 1;
	}

	i = 0; 
	script = malloc(1);
	f = fopen(argv[1], "r");
	do {
		memset(buf, 0, 80);
		rb = fread(buf, 80, 1, f);
		if (!rb && ferror(f)) {
			perror("error reading file\n");
			break;
		} else if (rb < 80)
			script = realloc(script, strlen(buf) + strlen(script));
		else
			script = realloc(script, strlen(script) + 80);

		strncpy(&script[i], buf, (rb < 80) ? strlen(buf) : 80);

		i += (rb < 80) ? strlen(buf) : 80;
		script[i] = '\0';
	} while (!feof(f));
	fclose(f);

	printf("%s\n", script);

	char *tok;
	scene_t s;
	actor_t *a;
	struct transf_t *t;

	i = 0;
	tok = strtok(script, "#actors ");
	s.nactors = atoi(tok);
	printf("===== %d atores\n", s.nactors);
	for (i = 0, tok = strtok(NULL, "\n"); 
			(tok != NULL); 
			tok = strtok(NULL, "\n"), i++) {
		
		tok = trim(tok);
		if (strlen(tok) == 0)
			continue;
		printf("tok[%d] %s\n", i, tok);
		tok = strtok(NULL, "#id_actor ");
		tok = trim(tok);
		if (strlen(tok) == 0)
			continue;
		puts(tok);
	}

	if (script != NULL)
		free(script);
}