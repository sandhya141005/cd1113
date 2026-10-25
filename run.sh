flex lexer.l && gcc lex.yy.c -o lexer
yacc -y -d syntax.y && flex tok.l && gcc y.tab.c lex.yy.c -o syntax
yacc -y -d icg.y && flex tok.l && gcc y.tab.c lex.yy.c -o icg
gcc opt.c -o opt
gcc gen.c -o codegen




echo "-----LEXICAL ANALYSIS-----"; ./lexer <$1
echo "-----SYNTAX ANALYSIS------"; ./syntax <$1
echo "-----ICG ANALYSISSSS------"; ./icg <$1 | tee tac.txt
echo "----OPTIMIZATIONSSSS------"; ./opt <tac.txt | tee opt.txt 
echo "------TARGET CODE---------"; ./codegen <opt.txt
