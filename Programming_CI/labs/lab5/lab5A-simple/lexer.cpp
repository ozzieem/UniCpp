/* lexer.c */

#include "global.h"
#include "treenode.h"

char lexbuf[BSIZE];
int  lineno = 1;
int  tokenval = NONE;

int lexan ()  /*  lexical analyzer  */
{
  int t;
  while(1) {
    t = getchar();
    if (t == ' ' || t == '\t')
      ;  /*  strip out white space  */
    else if (t == '\n')
      lineno = lineno + 1;
    else if (isdigit(t)) {  /*  t is a digit  */
      ungetc(t, stdin);
      scanf("%d", &tokenval);

        yylval.p = mkleaf(NUM, tokenval); // Added this to get $$, $1.. to work in parser
      
      return NUM;
    }
    else if (isalpha(t)) {  /*  t is a letter */
      int p, b = 0;
      while (isalnum(t)) {  /* t is alphanumeric  */
        lexbuf [b] = t;
        t = getchar ();
        b = b + 1;
        if (b >= BSIZE)
          error("compiler error");
      }
      lexbuf[b] = EOS;
      if (t != EOF)
        ungetc(t, stdin);
      p = lookup (lexbuf);
      if (p == 0)
        p = insert (lexbuf, ID);
      tokenval = p;

      yylval.p = mkleaf(ID, tokenval); // Added this to get $$, $1.. to work in parser
      
      return symtable[p].token;
    }
    else if (t == EOF)
      return DONE;
    else {
      tokenval = NONE;
      return t;
    }
  }
}