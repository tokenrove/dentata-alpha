/*
 * Tilemap Description Compiler
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <flash.h>

#define BUFLEN 81

int main(int argc, char **argv)
{
	FILE *infp, *outfp, *testfp;
	char buffer[BUFLEN], *p;
	int lineno, mode, i, curtile, ntiles;
	flash_image_t *baseim;

	lineno = 0;
	mode = 0;
	i = 0;
	curtile = 0;
	ntiles = -1;
	outfp = NULL;

	if(argc != 2) {
		printf("Usage:\ntmcomp <tilemap description file>\n");
		exit(EXIT_FAILURE);
	}
	infp = fopen(argv[1], "r");
	if(infp == NULL) {
		printf("Failed to open %s.\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	while(fgets(buffer, BUFLEN-1, infp) != NULL) {
		lineno++;
		if(buffer[strlen(buffer)-1] != '\n') {
			printf("Sorry, but no line should be longer than %d"
			       " characters, even comments. "
			       "(%d)\n",
			       BUFLEN-1, lineno);
		}

		p = strtok(buffer, " \t\n");
		if(p == NULL || *p == '#') continue;

		if(strcmp(p, "name") == 0) {
			if(mode != 0) {
				printf("You can't issue the ``%s'' command "
				       "while defining tiles. (%d)\n",
				       p, lineno);
				exit(EXIT_FAILURE);
			}

			p = strtok(NULL, " \t\n");
			if(p == NULL) {
				printf("Looks like you forgot to tell me "
				       "what to name the output file. (%d)\n",
				       lineno);
				exit(EXIT_FAILURE);
			}

			if(outfp == NULL) {
				outfp = fopen(p, "wb");
				if(outfp == NULL) {
					printf("Failed to open ``%s'' for "
					       "writing. (%d)\n", p, lineno);
					exit(EXIT_FAILURE);
				}
			} else {
				printf("Oops, name was defined twice. "
				       "Cowardly exiting now. (%d)\n", lineno);
				exit(EXIT_FAILURE);
			}
		} else if(strcmp(p, "ntiles") == 0) {
			if(mode != 0) {
				printf("You can't issue the ``%s'' command "
				       "while defining tiles. (%d)\n",
				       p, lineno);
				exit(EXIT_FAILURE);
			}

			p = strtok(NULL, " \t\n");
			if(p == NULL) {
				printf("Looks like you forgot to tell me "
				       "how many tiles there are. (%d)\n",
				       lineno);
				exit(EXIT_FAILURE);
			}

			if(ntiles == -1)
				ntiles = atoi(p);
			else {
				printf("Oops, ntiles was defined twice. "
				       "Cowardly exiting now. (%d)\n", lineno);
				exit(EXIT_FAILURE);
			}

			if(ntiles < 0 || ntiles > 255) {
				printf("%d is not a sane value for ntiles. (%d)\n",
				       ntiles, lineno);
				exit(EXIT_FAILURE);
			}
			fputc(ntiles, outfp);
		} else if(strcmp(p, "walkthresh") == 0) {
			if(mode != 0) {
				printf("You can't issue the ``%s'' command "
				       "while defining tiles. (%d)\n",
				       p, lineno);
				exit(EXIT_FAILURE);
			}

			p = strtok(NULL, " \t\n");
			if(p == NULL) {
				printf("Looks like you forgot to tell me "
				       "the walkable threshold. (%d)\n",
				       lineno);
				exit(EXIT_FAILURE);
			}

			i = atoi(p);
			fputc(i, outfp);
		} else if(strcmp(p, "baseimage") == 0) {
			if(mode != 0) {
				printf("You can't issue the ``%s'' command "
				       "while defining tiles. (%d)\n",
				       p, lineno);
				exit(EXIT_FAILURE);
			}

			p = strtok(NULL, " \t\n");
			if(p == NULL) {
				printf("Looks like you forgot to tell me "
				       "the base image name. (%d)\n",
				       lineno);
				exit(EXIT_FAILURE);
			}

			baseim = flash_loadpcx(p);
			if(baseim == NULL) {
				printf("Couldn't use ``%s'' as the base "
				       "image. (%d)\n", p, lineno);
				exit(EXIT_FAILURE);
			}
			i = baseim->width;
			fputc(i&0xFF, outfp);
			fputc((i>>8)&0xFF, outfp);

			i = baseim->height;
			fputc(i&0xFF, outfp);
			fputc((i>>8)&0xFF, outfp);
		} else if(strcmp(p, "tile") == 0) {
			mode = 1;

			p = strtok(NULL, " \t\n");
			if(p == NULL) {
				printf("Looks like you forgot to tell me "
				       "the format. (%d)\n",
				       lineno);
				exit(EXIT_FAILURE);
			}
			if(strcmp(p, "pcx") == 0) {
				fputc(1, outfp);
			} else if(strcmp(p, "pnm") == 0) {
				fputc(0, outfp);
			} else {
				printf("Never heard of the ``%s'' format. "
				       "(%d)\n", p, lineno);
				exit(EXIT_FAILURE);
			}

			p = strtok(NULL, " \t\n");
			if(p == NULL) {
				printf("I need to know the filename of this "
				       "tile. (%d)\n", lineno);
				exit(EXIT_FAILURE);
			}
			testfp = fopen(p, "r");
			if(testfp == NULL) {
				printf("The tile ``%s'' doesn't exist (for "
				       "me, anyway). (%d)\n", p, lineno);
				exit(EXIT_FAILURE);
			}
			fclose(testfp);
			fwrite(p, 1, strlen(p), outfp);
			fputc('\0', outfp);
			curtile++;

		} else {
			printf("Unrecognized command ``%s'', fleeing. (%d)\n",
			       p, lineno);
			exit(EXIT_FAILURE);
		}
	}
	fclose(infp);

	if(outfp == NULL) {
		printf("Erg, please name the file something. "
		       "(Add a ``names'' line)\n");
		exit(EXIT_FAILURE);
	}
	if(curtile < ntiles) {
		printf("Insufficient animations defined.\n");
		exit(EXIT_FAILURE);
	}

	for(i = 0; i < baseim->width*baseim->height; i++)
		fputc(baseim->data[i], outfp);
	fclose(outfp);

	exit(EXIT_SUCCESS);
}

/* EOF tmcomp.c */
