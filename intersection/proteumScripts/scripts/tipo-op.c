#include	<stdio.h>

#define		STR_NMUTA	"MUTANT #"
#define		STR_TIPO	"\tOperator:"

main(argc, argv)
int	argc;
char	*argv[];
{
int	i,j,k, ntcase, l;
char	buf[300], nome[30];
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
		if (strncmp(buf, STR_TIPO, strlen(STR_TIPO)) != 0)
		     continue;
		printf("%s\n",buf);
		//sscanf(&buf[strlen(STR_TIPO)], "%s", nome);
		l++;
		printf("%d\t%s\n", ntcase, nome);
    } while (1);
    fprintf(stderr, "\n%d alives mutants\n----------------------\n", l);
}
