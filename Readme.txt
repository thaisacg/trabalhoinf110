
8-text.cpp

Exemplo com leitura de fonte, definição de texto e suas propriedades (logo antes
do loop principal). Para mostrar o texto, window.draw (veja no final do loop)

No exemplo, foi usada fonte arial. Existem várias fontes disponíveis para
download na internet (arquivo .ttf)

Esse exemplo foi baseado no tutorial da SFML:
https://www.sfml-dev.org/tutorials/3.1/graphics/text/

8b-text.cpp

Acrescentado um texto cujo conteúdo é definido durante a execução do programa.
O string usado para definir o texto a ser escrito é criado com a função snprintf.

Esta função é como a printf, mas "escreve" no string em vez de escrever no terminal.
Tem dois parâmetros adicionais, antes do formato: string e máximo de caracteres.
- printf("Linha %d",posy) escreve "Linha 7" no terminal, se posy valer 7.
- sprintf(s,"Linha %d",posy) escreve "Linha 7" no string s.
- snprintf(s,20,"Linha %d",posy) escreve "Linha 7" no string s (no máximo 20 char).
