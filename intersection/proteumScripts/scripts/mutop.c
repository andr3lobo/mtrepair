#include	<stdio.h>

#define		STR_NMUTA	"MUTANT #"
#define		STR_OP		"\tOperator"

main(argc, argv)
int	argc;
char	*argv[];
{
int	i,j,k, ntcase, l;
char	buf[300], op[100];
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

		while (strncmp(buf, STR_OP, strlen(STR_OP)) != 0)
		     p = fgets(buf, 100, fp);
		
		p = strrchr(buf,'(');
		strncpy(op, &p[1], 6);
			
	
		l++;

		printf("%d:%s\n", ntcase,op);
    } while (1);
    fprintf(stderr, "\n%d mutants\n----------------------\n", l);
}
