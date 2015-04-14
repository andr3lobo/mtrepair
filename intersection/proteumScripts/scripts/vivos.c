#include	<stdio.h>

#define		STR_NMUTA	"MUTANT #"
#define		STR_VIVO	"\tStatus Alive"

main(argc, argv)
int	argc;
char	*argv[];
{
int	i,j,k, ntcase, l;
char	buf[300];
FILE	*fp;
char	*p;

   fp = stdin;
   l = 0;
   j = 0;
   do 
   {
		if (feof(fp))
			break;
		p = fgets(buf, 100, fp);
		if (strncmp(buf, STR_NMUTA, strlen(STR_NMUTA)) != 0)
		   continue;
		sscanf(&buf[strlen(STR_NMUTA)], "%d", &ntcase);

		p = fgets(buf, 100, fp);
		if (strncmp(buf, STR_VIVO, strlen(STR_VIVO)) != 0)
		     continue;
		l++;
		printf("%d\n", ntcase);
    } while (1);
    fprintf(stderr, "\n%d alives mutants\n----------------------\n", l);
}
