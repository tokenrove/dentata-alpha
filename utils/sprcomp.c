/*
 * Sprite description file compiler/validator
 *
 * Wishlist:
 *   - Optionally generate colliderect for the user
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFLEN 81

int main(int argc, char **argv)
{
	FILE *infp, *outfp, *testfp;
	char buffer[BUFLEN], *p;
	int lineno, mode, i, nframes, nanims;
	int curanim;

	outfp = NULL; nframes = -1;
	lineno = 0;
	mode = 0;
	nanims = -1;
	curanim = 0;
	i = 0;

	if(argc != 2) {
		printf("Usage:\nsprcomp <sprite description file>\n");
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
				       "while defining animations. (%d)\n",
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
		} else if(strcmp(p, "nanims") == 0) {
			if(mode != 0) {
				printf("You can't issue the ``%s'' command "
				       "while defining animations. (%d)\n",
				       p, lineno);
				exit(EXIT_FAILURE);
			}

			p = strtok(NULL, " \t\n");
			if(p == NULL) {
				printf("Looks like you forgot to tell me "
				       "how many animations there are. (%d)\n",
				       lineno);
				exit(EXIT_FAILURE);
			}

			if(nanims == -1)
				nanims = atoi(p);
			else {
				printf("Oops, nanims was defined twice. "
				       "Cowardly exiting now. (%d)\n", lineno);
				exit(EXIT_FAILURE);
			}

			if(nanims < 0) {
				printf("%d is not a sane value for nanims. (%d)\n",
				       nanims, lineno);
				exit(EXIT_FAILURE);
			}
			i = atoi(p);
			fputc(i&0xFF, outfp);
			fputc((i>>8)&0xFF, outfp);

		} else if(strcmp(p, "collide") == 0) {
			if(mode != 0) {
				printf("You can't issue the ``%s'' command "
				       "while defining animations. (%d)\n",
				       p, lineno);
				exit(EXIT_FAILURE);
			}

			p = strtok(NULL, " \t\n");
			if(p == NULL) {
				printf("Looks like you forgot to tell me "
				       "the collide mode. (%d)\n",
				       lineno);
				exit(EXIT_FAILURE);
			}

			if(strcmp(p, "none") == 0)
				fputc(0, outfp);
			else if(strcmp(p, "rectangle") == 0)
				fputc(1, outfp);
			else if(strcmp(p, "pixel") == 0)
				fputc(2, outfp);
			else if(strcmp(p, "threshold") == 0)
				fputc(3, outfp);
			else {
				printf("``%s'' is not a sane value "
				       "for collidemode. (%d)\n", p,
				       lineno);
				exit(EXIT_FAILURE);
			}
		} else if(strcmp(p, "origin") == 0) {
			if(mode != 0) {
				printf("You can't issue the ``%s'' command "
				       "while defining animations. (%d)\n",
				       p, lineno);
				exit(EXIT_FAILURE);
			}

			p = strtok(NULL, " \t\n");
			if(p == NULL) {
				printf("Looks like you forgot to tell me "
				       "the coordinates. (%d)\n",
				       lineno);
				exit(EXIT_FAILURE);
			}
			i = atoi(p);
			fputc(i&0xFF, outfp);
			fputc((i>>8)&0xFF, outfp);

			p = strtok(NULL, " \t\n");
			if(p == NULL) {
				printf("Looks like you forgot to tell me "
				       "the y coordinate. (%d)\n",
				       lineno);
				exit(EXIT_FAILURE);
			}
			i = atoi(p);
			fputc(i&0xFF, outfp);
			fputc((i>>8)&0xFF, outfp);
		} else if(strcmp(p, "colliderect") == 0) {
			if(mode != 0) {
				printf("You can't issue the ``%s'' command "
				       "while defining animations. (%d)\n",
				       p, lineno);
				exit(EXIT_FAILURE);
			}

			p = strtok(NULL, " \t\n");
			if(p == NULL) {
				printf("Looks like you forgot to tell me "
				       "the coordinates. (%d)\n",
				       lineno);
				exit(EXIT_FAILURE);
			}
			i = atoi(p);
			fputc(i&0xFF, outfp);
			fputc((i>>8)&0xFF, outfp);

			p = strtok(NULL, " \t\n");
			if(p == NULL) {
				printf("Looks like you forgot to tell me "
				       "the coordinates. (%d)\n",
				       lineno);
				exit(EXIT_FAILURE);
			}
			i = atoi(p);
			fputc(i&0xFF, outfp);
			fputc((i>>8)&0xFF, outfp);

			p = strtok(NULL, " \t\n");
			if(p == NULL) {
				printf("Looks like you forgot to tell me "
				       "the coordinates. (%d)\n",
				       lineno);
				exit(EXIT_FAILURE);
			}
			i = atoi(p);
			fputc(i&0xFF, outfp);
			fputc((i>>8)&0xFF, outfp);

			p = strtok(NULL, " \t\n");
			if(p == NULL) {
				printf("Looks like you forgot to tell me "
				       "the coordinates. (%d)\n",
				       lineno);
				exit(EXIT_FAILURE);
			}
			i = atoi(p);
			fputc(i&0xFF, outfp);
			fputc((i>>8)&0xFF, outfp);
		} else if(strcmp(p, "anim") == 0) {
			if(nanims == -1) {
				printf("It is important to know how many "
				       "animations there are. (You forgot "
				       "an ``nanims'' line)\n");
				exit(EXIT_FAILURE);
			}
			if(i < nframes) {
				printf("You haven't finished specifying the "
				       "frames for the previous animation. "
				       "(%d)\n", lineno);
				exit(EXIT_FAILURE);
			}

			mode = 1;
			p = strtok(NULL, " \t\n");
			if(p == NULL) {
				printf("I need to know the framelag for "
				       "this animation. (%d)\n",
				       lineno);
				exit(EXIT_FAILURE);
			}
			i = atoi(p);
			fputc(i&0xFF, outfp);
			fputc((i>>8)&0xFF, outfp);

			p = strtok(NULL, " \t\n");
			if(p == NULL) {
				printf("I need to know how many frames there "
				       "are in this animation. (%d)\n",
				       lineno);
				exit(EXIT_FAILURE);
			}
			nframes = atoi(p);

			if(nframes < 0) {
				printf("%d is not a sane value for nframes. "
				       "(%d)\n", nframes, lineno);
				exit(EXIT_FAILURE);
			}
			fputc(nframes&0xFF, outfp);
			fputc((nframes>>8)&0xFF, outfp);

			i = 0;
		} else if(strcmp(p, "frame") == 0) {
			if(mode != 1) {
				printf("Now is not the time to be defining "
				       "a frame. (%d)\n", lineno);
				exit(EXIT_FAILURE);
			}

			p = strtok(NULL, " \t\n");
			if(p == NULL) {
				printf("I need to know the format of this "
				       "frame. (%d)\n", lineno);
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
				       "frame. (%d)\n", lineno);
				exit(EXIT_FAILURE);
			}
			testfp = fopen(p, "r");
			if(testfp == NULL) {
				printf("The frame ``%s'' doesn't exist (for "
				       "me, anyway). (%d)\n", p, lineno);
				exit(EXIT_FAILURE);
			}
			fclose(testfp);
			fwrite(p, 1, strlen(p), outfp);
			fputc('\0', outfp);

			i++;
			if(i >= nframes) {
				curanim++;
			}

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
	fclose(outfp);
	if(curanim < nanims) {
		printf("Insufficient animations defined.\n");
		exit(EXIT_FAILURE);
	}
	if(i < nframes) {
		printf("Insufficient frames defined.\n");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}

/* EOF sprcomp.c */
