#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>

char mapa[13][36] = {    // Mapa do jogo
  "1011111110101111111110101111111101",
  "1000010000100000100000100000100001",
  "1010111011111101110111111101110101",
  "1010000000100000000000100000000101",
  "1011011101110111011101110111101101", //x é horizontal
  "0000010000000100000100000000100000", // y é vertical. Y AUMENTA=vai para BAIXO
  "1011110111110111111101111101111101",
  "1000000000100000000000100000000001", //y=c5  x= l14
  "1011111110101111111110101111111101",
  "0000010000000000100000000000100000", 
  "1110111011111101110111111101110111",
  "0000000000100000000000100000000000", 
  "1011111110101111111110101111111101"

  
};
bool moedas[13][36];

    //PACMAN   

int posx = 16; // posicao do PacMan
int posy = 7;

bool movendo = false; // para ele nao comecar se movendo
// direcao de movimento
bool cima = false;
bool baixo = false;
bool esq = false;
bool dir = false;

// está olhando para direita no inicio
bool olhadireita = true;
bool olhacima = false;
bool olhaesquerda = false;
bool olhabaixo = false;

int totalMoedas=0;
int moedasColetadas=0;

//FANTASMAS => 0-vermelho, 1-rosa, 2-azul, 3-laranja
int fposx[4] = {14, 15, 17, 18};
int fposy[4] = {5, 5, 5, 5};

    //direção fantasma => 00:03-vermelho. cada linha um fantasma
    //0:3=dir, esq, cima, baixo
bool dirFan[4][4] = {
    {false, false, true, false}, //vermelho
    {false, false, true, false},
    {false, true, false, false},
    {true, false, false, false}
};

int frameFan = 0; //todos terão o mesmo frame

/*
f -> fantasma 0 1 2 3
d -> direcao 0 1 - 2 3 - 4 5 - 6 7
fra -> frame 0 1

if direcao[f][0] dire = 0
else if direcao[f][1] dire = 1
...

spriteAtualFan[f] = &texFantasmas[f][2*dire + frame]
*/

int main() {

    srand(time(0));

    // cria a janela
    sf::RenderWindow window(sf::VideoMode({1700, 710}), "Minha janela");

    // cria um círculo de raio 50
    sf::CircleShape circ(50.f);
    // define a posição absoluta do círculo
    circ.setPosition({10.f, 50.f});
    // define a cor do círculo (verde)
    circ.setFillColor(sf::Color(100, 250, 50));

    // cria um quadrado de tamanho 50
    sf::RectangleShape quad({45.f, 45.f});
    quad.setFillColor(sf::Color(80, 70, 20));

    sf::CircleShape moeda(7.f); // moedas
        moeda.setFillColor(sf::Color(255, 255, 100));
        moeda.setOrigin({7.f, 7.f});

    // FONTE
    sf::Font font;
        if (!font.openFromFile("arial.ttf")) {
        std::cout << "Erro lendo arial.ttf\n";
        return 0;
    }

    sf::Text textPlacar(font);
    char s[50] = "Pontos: 0";
    textPlacar.setString(s);
    textPlacar.setCharacterSize(40);
    textPlacar.setFillColor(sf::Color::Yellow);
    textPlacar.setStyle(sf::Text::Bold);
    textPlacar.setPosition({20.f, 655.f});
    
    sf::Texture texturafundo;
    if (!texturafundo.loadFromFile("fundo.png")) {
        std::cout << "Erro lendo fundo.png\n";
        return 0;
    }
    sf::Sprite spritefundo(texturafundo);
    spritefundo.setScale({1700.f / texturafundo.getSize().x, 650.f / texturafundo.getSize().y});

//matriz fantasmas. 0-vermelho, 1-rosa, 2-azul, 3-laranja
//[fantasma][direção] => 0:3=dir, esq, cima, baixo

sf::Texture texFantasmas[4][8];
for (int f = 0; f < 4; f++) {
    int linhaY = f * 40; // na sheet, cada fantasma tem 42 de altura e 38 de largura, então aqui
    //talvez eu coloque a opção da perninha deles mecherem tbm, mas ainda to descobrindo (por isso o x não ta mudando de 38 em 38)
    //o fantasma azul ta pegando uma parte da linha de baixo

    if (!texFantasmas[f][0].loadFromFile("fantasmas-sheet1.png", false, sf::IntRect({0, linhaY}, {38, 42}))) {
        std::cout << "Erro lendo fantasma " << f << " direita frame 1\n";
        return 0;
    }
    if (!texFantasmas[f][1].loadFromFile("fantasmas-sheet1.png", false, sf::IntRect({38, linhaY}, {38, 42}))) {
        std::cout << "Erro lendo fantasma " << f << " direita frame 2\n";
        return 0;
    }
    if (!texFantasmas[f][2].loadFromFile("fantasmas-sheet1.png", false, sf::IntRect({76, linhaY}, {38, 42}))) {
        std::cout << "Erro lendo fantasma " << f << " esquerda f1\n";
        return 0;
    }
    if (!texFantasmas[f][3].loadFromFile("fantasmas-sheet1.png", false, sf::IntRect({114, linhaY}, {38, 42}))) {
        std::cout << "Erro lendo fantasma " << f << " esquerda f2\n";
        return 0;
    }
    if (!texFantasmas[f][4].loadFromFile("fantasmas-sheet1.png", false, sf::IntRect({152, linhaY}, {38, 42}))) {
        std::cout << "Erro lendo fantasma " << f << " cima f1\n";
        return 0;
    }
    if (!texFantasmas[f][5].loadFromFile("fantasmas-sheet1.png", false, sf::IntRect({190, linhaY}, {38, 42}))) {
        std::cout << "Erro lendo fantasma " << f << " cima f2\n";
        return 0;
    }
    if (!texFantasmas[f][6].loadFromFile("fantasmas-sheet1.png", false, sf::IntRect({228, linhaY}, {38, 42}))) {
        std::cout << "Erro lendo fantasma " << f << " baixo frame 1\n";
        return 0;
    }
    if (!texFantasmas[f][7].loadFromFile("fantasmas-sheet1.png", false, sf::IntRect({267, linhaY}, {38, 42}))) {
        std::cout << "Erro lendo fantasma " << f << " baixo f2\n";
        return 0;
    }
}

sf::Sprite spriteFantasma[4] = {
    sf::Sprite(texFantasmas[0][0]),
    sf::Sprite(texFantasmas[1][0]),
    sf::Sprite(texFantasmas[2][0]),
    sf::Sprite(texFantasmas[3][0])
};
    
    // sprites do Sonic
sf::Texture texturedir0, texturedir1, texturedir2;
if (!texturedir0.loadFromFile("sonicdireita0.png")) { std::cout << "Erro lendo sonicdireita0.png\n"; return 0; }
if (!texturedir1.loadFromFile("sonicdireita1.png")) { std::cout << "Erro lendo sonicdireita1.png\n"; return 0; }
if (!texturedir2.loadFromFile("sonicdireita2.png")) { std::cout << "Erro lendo sonicdireita2.png\n"; return 0; }
sf::Sprite spritedir0(texturedir0), spritedir1(texturedir1), spritedir2(texturedir2);

// texturas esquerda
sf::Texture texturaesq0, texturaesq1, texturaesq2;
if (!texturaesq0.loadFromFile("sonicesquerda0.png")) { std::cout << "Erro lendo sonicesquerda0.png\n"; return 0; }
if (!texturaesq1.loadFromFile("sonicesquerda1.png")) { std::cout << "Erro lendo sonicesquerda1.png\n"; return 0; }
if (!texturaesq2.loadFromFile("sonicesquerda2.png")) { std::cout << "Erro lendo sonicesquerda2.png\n"; return 0; }
sf::Sprite spriteesq0(texturaesq0), spriteesq1(texturaesq1), spriteesq2(texturaesq2);

// texturas cima
sf::Texture texturacima0, texturacima1, texturacima2;
if (!texturacima0.loadFromFile("soniccima0.png")) { std::cout << "Erro lendo soniccima0.png\n"; return 0; }
if (!texturacima1.loadFromFile("soniccima1.png")) { std::cout << "Erro lendo soniccima1.png\n"; return 0; }
if (!texturacima2.loadFromFile("soniccima2.png")) { std::cout << "Erro lendo soniccima2.png\n"; return 0; }
sf::Sprite spritecima0(texturacima0), spritecima1(texturacima1), spritecima2(texturacima2);

// texturas baixo
sf::Texture texturabaixo0, texturabaixo1, texturabaixo2;
if (!texturabaixo0.loadFromFile("sonicbaixo0.png")) { std::cout << "Erro lendo sonicbaixo0.png\n"; return 0; }
if (!texturabaixo1.loadFromFile("sonicbaixo1.png")) { std::cout << "Erro lendo sonicbaixo1.png\n"; return 0; }
if (!texturabaixo2.loadFromFile("sonicbaixo2.png")) { std::cout << "Erro lendo sonicbaixo2.png\n"; return 0; }
sf::Sprite spritebaixo0(texturabaixo0), spritebaixo1(texturabaixo1), spritebaixo2(texturabaixo2);

// índice do frame atual
int frame = 0;

    // cria um relogio para medir o tempo
    sf::Clock clock;
    sf::Clock clockFantasmas;

    // executa o programa enquanto a janela está aberta

    // coloca a posição das moedas no mapa
    for(int i=0;i<13;i++)
    for(int j=0;j<37;j++)
        if(mapa[i][j] == '0'){
            moedas[i][j]=true;
            totalMoedas++;
        }
    //tira moedas da casa dos fantasmas
    for(int j=14; j<19; j++){
        moedas[5][j]=false;
        totalMoedas--;
    }

    while (window.isOpen()) {

        // verifica todos os eventos que foram acionados na janela desde a última iteração do loop
        while (const std::optional event = window.pollEvent()) {
            // evento "fechar" acionado: fecha a janela
            if (event->is<sf::Event::Closed>())
                window.close();
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                  if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                      window.close();
                  else if (keyPressed->scancode == sf::Keyboard::Scancode::Left) {
                      esq = true;   // left key: PacMan passa a mover para esquerda
                      dir = cima = baixo = false;
                      movendo = true; 
                  }
                  else if (keyPressed->scancode == sf::Keyboard::Scancode::Right) {
                    dir = true;   // right key: PacMan passa a mover para direita
                    esq = cima = baixo = false;
                    movendo = true;

                  }
                  else if (keyPressed->scancode == sf::Keyboard::Scancode::Up) {
                    cima = true;  // up key: PacMan passa a mover para cima
                    esq = dir = baixo = false;
                    movendo = true;

                  }
                  else if (keyPressed->scancode == sf::Keyboard::Scancode::Down) {
                    baixo = true; // down key:PacMan passa a mover para baixo
                    esq = dir = cima = false;
                    movendo = true;

                }
            }
        }

        // Muda a posição do PacMan a cada 0.2 segundos
if (clock.getElapsedTime() > sf::seconds(0.2)) {
    clock.restart();
    
    if (movendo) {

        if (frame == 2) frame = 0;
        else frame++;
        // tenta virar e andar na nova direção
        
        if (cima && mapa[posy-1][posx] != '1') {
            posy--;
            olhacima = true; olhadireita = olhaesquerda = olhabaixo = false;
        } else if (baixo && mapa[posy+1][posx] != '1') {
            posy++;
            olhabaixo = true; olhadireita = olhaesquerda = olhacima = false;
        } else if (esq && mapa[posy][posx-1] != '1') {
            posx--;
            olhaesquerda = true; olhadireita = olhacima = olhabaixo = false;
        } else if (dir && mapa[posy][posx+1] != '1') {
            posx++;
            olhadireita = true; olhaesquerda = olhacima = olhabaixo = false;
        } else {
            // nova direção bloqueada, continua na direção atual. caso a direção desejada tenha parede
            if (olhacima && mapa[posy-1][posx] != '1') posy--;
            if (olhabaixo && mapa[posy+1][posx] != '1') posy++;
            if (olhaesquerda && mapa[posy][posx-1] != '1') posx--;
            if (olhadireita && mapa[posy][posx+1] != '1') posx++;
        }

        if (posy < 0) posy = 12;
        if (posy > 12) posy = 0;
        if (posx < 0) posx = 34;
        if (posx > 34) posx = 0;

        if(moedas[posy][posx]){ //se tiver moedas na pos, conta +1
            moedas[posy][posx] = false;
            moedasColetadas++;
        }
    }

    // FRAME DOS FANTASMAS

    if(frameFan==0) frameFan =1;
    else frameFan = 0;

}

    // MOVIMENTAÇÃO DOS FANTASMAS
    if (clockFantasmas.getElapsedTime() > sf::seconds(0.35)) {
        clockFantasmas.restart();

        for(int f = 0; f < 4; f++) {
            // verifica quais dierções em volta do fantasma são validas (!=1)
            int valida = 0;
            if(mapa[fposy[f]][fposx[f]+1] != '1') valida++;
            if(mapa[fposy[f]][fposx[f]-1] != '1') valida++;
            if(mapa[fposy[f]-1][fposx[f]] != '1') valida++;
            if(mapa[fposy[f]+1][fposx[f]] != '1') valida++;

            // calcula próxima posição possível (px, py) 
            int px = fposx[f], py = fposy[f];
            if(dirFan[f][0]) px++;
            else if(dirFan[f][1]) px--;
            else if(dirFan[f][2]) py--;
            else if(dirFan[f][3]) py++;

            // se tem 3 ou mais posições validas (encruzilhada) ou se for parede na prox pos posivel é sorteado de novo
            if(mapa[py][px] == '1' || valida > 2) { 
                int opcoes[4], n = 0; 
                
                if(mapa[fposy[f]][fposx[f]+1] != '1'){
                    opcoes[n] = 0; 
                    n++;
                }
                if(mapa[fposy[f]][fposx[f]-1] != '1') {
                    opcoes[n] = 1;
                    n++;
                }
                if(mapa[fposy[f]-1][fposx[f]] != '1'){
                        opcoes[n] = 2;
                        n++;
                }
                if(mapa[fposy[f]+1][fposx[f]] != '1'){
                    opcoes[n] = 3;
                    n++;
                }
                // sorteio dentre as posições q o fantasma tem disponivel
                if(n > 0) {
                    dirFan[f][0] = dirFan[f][1] = dirFan[f][2] = dirFan[f][3] = false;
                    dirFan[f][opcoes[rand() % n]] = true;
                }

                // recalcula próxima posição com nova direção
                px = fposx[f]; py = fposy[f];
                if(dirFan[f][0]) px++;
                else if(dirFan[f][1]) px--;
                else if(dirFan[f][2]) py--;
                else if(dirFan[f][3]) py++;
            }

            // move se não tiver parede
            if(mapa[py][px] != '1' ) {
                fposx[f] = px;
                fposy[f] = py;
            }
        }
    }
    /****************************/

        // limpa a janela com a cor preta
        window.clear(sf::Color::Black);
        window.draw(spritefundo);
        // desenhar tudo aqui...

        // desenha paredes
        for(int i=0;i<13;i++)
            for(int j=0;j<36;j++)
                if (mapa[i][j]=='1') {
                    quad.setPosition({j*50.f, i*50.f});
                    window.draw(quad);
                }
                // desenha moedas
        for(int i=0;i<13;i++)
            for(int j=0;j<36;j++)
                if (moedas[i][j]) {
                    moeda.setPosition({j*50.f + 25.f, i*50.f + 25.f});
                    window.draw(moeda);
                }        


        // desenha o Sonic
// avança o frame a cada movimento
        sf::Sprite* spriteAtual = nullptr;

        if (olhadireita) {
            if (frame == 0) spriteAtual = &spritedir0;
            else if (frame == 1) spriteAtual = &spritedir1;
            else spriteAtual = &spritedir2;
        }
        if (olhacima) {
            if (frame == 0) spriteAtual = &spritecima0;
            else if (frame == 1) spriteAtual = &spritecima1;
            else spriteAtual = &spritecima2;
        }
        if (olhaesquerda) {
            if (frame == 0) spriteAtual = &spriteesq0;
            else if (frame == 1) spriteAtual = &spriteesq1;
            else spriteAtual = &spriteesq2;
        }
        if (olhabaixo) {
            if (frame == 0) spriteAtual = &spritebaixo0;
            else if (frame == 1) spriteAtual = &spritebaixo1;
            else spriteAtual = &spritebaixo2;
        }

        if (spriteAtual) {
            spriteAtual->setPosition({posx*50.f, posy*50.f});
            window.draw(*spriteAtual);
        }
        


        //DESENHA FANTASMA

        for(int f=0; f<4; f++){
            if(dirFan[f][0]) spriteFantasma[f].setTexture(texFantasmas[f][0+frameFan]); //se frame=0 continua direita 1, se frame=1, vira a direita 2 (indice 1)
            else if(dirFan[f][2]) spriteFantasma[f].setTexture(texFantasmas[f][2+frameFan]);
            else if(dirFan[f][4]) spriteFantasma[f].setTexture(texFantasmas[f][4+frameFan]);
            else if(dirFan[f][6]) spriteFantasma[f].setTexture(texFantasmas[f][6+frameFan]);

            spriteFantasma[f].setPosition({fposx[f]*50.f, fposy[f]*50.f});
                window.draw(spriteFantasma[f]);
        }

        char s[50];
        snprintf(s, 50, "Pontos: %d", moedasColetadas);
        textPlacar.setString(s);
        window.draw(textPlacar);

        window.display();
    }
    return 0;
    
}
