flex lexer.l && gcc lex.yy.c -o lexer
yacc -y -d syntax.y && flex tok.l && gcc y.tab.c lex.yy.c -o syntax
yacc -y -d icg.y    && flex tok.l && gcc y.tab.c lex.yy.c -o icg
gcc opt.c -o opt
gcc gen.c -o codegen

echo "===== 1. LEXICAL ANALYSIS =====";   ./lexer  < $1
echo "===== 2. SYNTAX ANALYSIS =====";    ./syntax < $1
echo "===== 3. INTERMEDIATE CODE =====";  ./icg < $1 | tee tac.txt
echo "===== 4. OPTIMIZED CODE =====";     ./opt < tac.txt | tee opt.txt
echo "===== 5. TARGET CODE =====";        ./codegen < opt.txt