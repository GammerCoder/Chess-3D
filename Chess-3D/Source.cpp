
#include "SFML/Graphics.hpp"
#include <cstdint>
#include <iostream>
uint64_t board = 0ULL;

//White Pieces
uint64_t WhiteKing;
uint64_t WhiteQueen;
uint64_t WhiteRook;
uint64_t WhiteKnight;
uint64_t WhiteBishop;
uint64_t WhitePawn;


//Black pieces
uint64_t BlackKing;
uint64_t BlackQueen;
uint64_t BlackRook;
uint64_t BlackKnight;
uint64_t BlackBishop;
uint64_t BlackPawn;


//Poston calculator
int posval(int a,int b) {
    return ((a-1) * 8) + (b-1);
}

//initiation of basic board
void basicboard(uint64_t b) {
    WhiteKing = 1ull << posval(1, 5); board |= WhiteKing;
    WhiteQueen = 1ull << posval(1, 4); board |= WhiteQueen;
    WhiteBishop = 1ull << posval(1, 3) | 1ull << posval(1, 6); board |= WhiteBishop;
    WhiteKnight = 1ull << posval(1, 2) | 1ull << posval(1, 7); board |= WhiteKnight;
    WhiteRook = 1ull << posval(1, 1) | 1ull << posval(1, 8); board |= WhiteRook;
    //WhitePawn = 0x000000000000FF00ULL; board |= WhitePawn;
    int white[8];
    for (int i = 0; i <= 7; i++) {
        
    }
    

    BlackKing = 1ull << posval(8, 5); board |= BlackKing;
    BlackQueen = 1ull << posval(8, 4); board |= BlackQueen;
    BlackBishop = 1ull << posval(8, 3) | 1ull << posval(8, 6); board |= BlackBishop;
    BlackKnight = 1ull << posval(8, 2) | 1ull << posval(8, 7); board |= BlackKnight;
    BlackRook = 1ull << posval(8, 1) | 1ull << posval(8, 8); board |= BlackRook;
    BlackPawn = 0x000000000000FF00ULL; board |= BlackPawn;

}

//pawen class
class pawn {
    public:
        int pos;
        bool isWhite;
        pawn(int a,bool b) {
            pos = a;
            isWhite = b;
        }
        void draw() {
            sf::RectangleShape pawnrect({ 100.f,100.f });
            if(isWhite)
            pawnrect.setFillColor(sf::Color::White);
            else
                pawnrect.setFillColor(sf::Color::Black);
        }
};


//Board display console
void printBoard(uint64_t b) {
    for (int r = 7; r >= 0; r--) {
        for (int f = 0; f < 8; f++) {
            int sq = r * 8 + f;
            std::cout << ((b >> sq) & 1) << " ";
        }
        std::cout << "\n";
    }
}


int main() {

    sf::RenderWindow window(sf::VideoMode({ 1200, 800 }), "Chess");
    //uint64_t board = 0ULL;
    bool select=0;
    if (select == false) {
        basicboard(board);
    }
    printBoard(board);

    
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        window.clear();

            sf::Vector2u winsiz = window.getSize();
            float boardw = winsiz.x * 0.7;
            float boardz = std::min(boardw, (float)winsiz.y);
            float tilesize = boardz / 8.0f;
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    sf::RectangleShape Tile({ tilesize,tilesize });
                    Tile.setPosition({ i * tilesize, j * tilesize });
                    if ((i + j) % 2 == 0) {
                        Tile.setFillColor(sf::Color(238, 238, 210));
                    }
                    else {
                        Tile.setFillColor(sf::Color(118,150,56));
                    }
                    window.draw(Tile);
                }
            }


        
        window.display();
    }

	return 0;
}