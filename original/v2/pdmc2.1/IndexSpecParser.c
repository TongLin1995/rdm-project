#include <stdio.h>
#include <ctype.h>
#include "lex.yy.c"
#include "y.tab.c"


comment()
{
   char  c, c1;
   int   Num = 0;
loop:
   while ((c = input()) != '*' && c != 0 && c != '\n');
   if (c == '\n') {
      Num++;
      goto loop;
   }
   if (c == 0) 
     goto exit;
   if ((c1 = input()) != '/') {
      unput(c1);
      goto loop;
   }
   else
      LineNumber += Num;
exit:;
}


int check_type()
{
   char Result[500];
   int P1 = 0; 
   int P2 = 0; 
   int Status = 0;
   int First = 1;
   strcpy(Result, "");
Start:
   if (yytext[P1] == '\0') {
      if (Result[0] == '\0') yyerror("Substition variable error");
      if (Status == 1) yyerror("Substitution variable error");
      if (Result[strlen(Result)-1] != ' ') 
         if (First == 1) strcat(Result, " Id 1 ");
         else strcat(Result, " Id 1 SubstitutionList 2 ");
      strcpy(yytext, Result);
      return(IDENTIFIER);
   }  
   if (yytext[P1] == '$') {
      if (Status == 2) yyerror("Substitution variable missing");  
      P1++;
      if (Status == 0) {
         Status = 2;
         if (First == 1) {
            if (Result[0] != '\0') {
               First = 0;
               strcat(Result, " Id 1 ");
            }
         }
         else 
            strcat(Result, " Id 1 SubstitutionList 2 ");
      }
      else if (Status == -1) {
         Status = 1;
      }
      else if (Status == 1) {
         Status = -1;
         if (First == 1) { 
            if (Result[0] != '\0') {
               First = 0;
               strcat(Result, " Substitution 1 ");
            }
			}
         else
            strcat(Result, " Substitution 1 SubstitutionList 2 ");
      }
      goto Start;
   }
   if (Status == 2) Status = 1;
	if (Status == -1) Status = 0;
   P2 = strlen(Result);
   Result[P2] = yytext[P1];
   Result[P2+1] = '\0';
   P1++;
   goto Start;
}


main()
{
   int yyparse(), Result;
   strcpy( LineBuffer, "" );
   printf( "(" );
   Result = yyparse(); 
   if ( Result )
      printf( "**error**" );
   printf( ")" );
   return( Result );
}


yyerror(s)
char *s;
{
   int i;
   char c;
   i = 0;
	while ((yytext[i] != '\0') && (yytext[i] != ' ')) i++;
   if (yytext[i] == ' ') yytext[i] = '\0'; 
   fprintf( stderr, "\n%s", LineBuffer );
   c = input();
   while ( c != '\n'  &&  c != 0 ) {
      putc(c, stderr);
      c = input();
   }
   putc('\n', stderr);
   for (i=1; i <= strlen( LineBuffer ) - strlen( yytext ); i++)
      if (LineBuffer[i-1] == '\t') putc('\t', stderr);
      else putc(' ', stderr);
   for (i=1; i <= strlen( yytext ); i++)
      putc('^', stderr);
   putc('\n', stderr);
   fprintf ( stderr, "** error: %s found in line %d of Index Specification file.\n\n", s, LineNumber );
}


yywrap()
{
   return(1);
}
