
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
uint64_t WhiteP;


//Black pieces
uint64_t BlackKing;
uint64_t BlackQueen;
uint64_t BlackRook;
uint64_t BlackKnight;
uint64_t BlackBishop;
uint64_t BlackPawn;
uint64_t BlackP;


//Move possible
struct move {
    int to;
    int from;
};

//revPos
int revpos(int a) {
    return (a % 8);
}
int revpos2(int a) {
    return (a / 8)+1;
}


//Update 
void update() {
    WhiteP |= WhiteKing;
    WhiteP |= WhiteKnight;
    WhiteP |= WhiteBishop;
    WhiteP |= WhiteRook;
    WhiteP |= WhiteQueen;
    WhiteP |= WhitePawn;

    BlackP |= BlackKing;
    BlackP |= BlackQueen;
    BlackP |= BlackKnight;
    BlackP |= BlackBishop;
    BlackP |= BlackRook;
    BlackP |= BlackPawn;

    board |= WhiteKing;
    board  |= WhiteKnight;
    board |= WhiteBishop;
    board |= WhiteRook;
    board |= WhiteQueen;
    board |= WhitePawn;

    board |= BlackKing;
    board |= BlackQueen;
    board |= BlackKnight;
    board |= BlackBishop;
    board |= BlackRook;
    board |= BlackPawn;
}

//warp
bool samefilewarp(int from, int to) {
    int fromfile = from % 8;
    int tofile = to % 8;
    return abs(fromfile - tofile) > 1;
}


//King move
int kingm[] = { 9,8,7,1,-1,-9,-8,-7 };
void kinglegal(uint64_t k, int from, std::vector<move>& moves,uint64_t own) {
    uint64_t king = k;
    //int from = __builtin_ctzll(king);
    for (auto i : kingm) {
        int to = from + i;
        if (to < 0 || to>63)continue;
        if (samefilewarp(from, to))continue;
        if (!(own & (1ull << to)))continue;
        //Make cpature 
        moves.push_back({ to,from });
    }
}

//Knight move
int Knightm[] = { 17,15,10,6,-17,-15,-10,-6 };
void knightlegal(uint64_t k, int from, std::vector<move>& moves, uint64_t own) {
    uint64_t knight = k;
    //int from = __builtin_ctzll(king);
    for (auto i : Knightm) {
        int to = from + i;
        if (to < 0 || to>63)continue;
        if (samefilewarp(from, to))continue;
        if (!(own & (1ull << to)))continue;
        //Make cpature 
        moves.push_back({ to,from });
    }
}

//Rook Move
int rookm[] = { 1,8,-8,-1 };
void RookLegal(uint64_t k, int from, std::vector<move>& moves, uint64_t own,uint64_t enmy) {
    uint64_t rook = k;
    int ofrom = from;
    for (int dir : rookm) {
        int next =  from + dir;
        while (true) {
            if (next < 0 || next>63)break;
            if (own & (1ull << next))break;
            if (samefilewarp(from, next))break;
            moves.push_back({ next,from });
            if (enmy & (1ull << next))break;
            next = dir + next;
            if (dir == -1) {
                from--;
            }
            if (dir == 1) {
                from++;
            }
        }
        from = ofrom;
    }
}

//Bishop move
int bism[] = { 7,9,-7,-9 };
void BishopLegal(uint64_t k, int from, std::vector<move>& moves, uint64_t own, uint64_t enmy) {
    uint64_t rook = k;
    int ofrom = from;
    for (int dir : bism) {
        int next = from + dir;
        while (true) {
            if (next < 0 || next>63)break;
            if (own & (1ull << next))break;
            if (samefilewarp(from, next))break;
            moves.push_back({ next,from });
            if (enmy & (1ull << next))break;
            next = dir + next;
            from = from + dir;
        }
        from = ofrom;
    }
}

//Queen move
void QueenLegal(uint64_t k, int from, std::vector<move>& moves, uint64_t own, uint64_t enmy) {
    uint64_t rook = k;
    int ofrom = from;
    for (int dir : kingm) {
        int next = from + dir;
        while (true) {
            if (next < 0 || next>63)break;
            if (own & (1ull << next))break;
            if (samefilewarp(from, next))break;
            moves.push_back({ next,from });
            if (enmy & (1ull << next))break;
            next = dir + next;
            from = from + dir;
        }
        from = ofrom;
    }
}

//Pawn movemnt
int pawn[4] = { 8, 16, 7, 9 };
int pawnb[4] = { -8,-16,-7,-9 };
void pawnlegal(int from, std::vector<move>&moves,uint64_t own,uint64_t enmy) {
    for (auto dis : pawn) {
        int to = from + dis;
        if (to > 63)continue;
        if (from < 8 || from > 15 && dis == 16)continue;
        if (own & (1ull << to))continue;
        if (!(enmy & 1ull << to) && (dis == 7 || dis == 9))continue;
        moves.push_back({ to,from });
    }
}
void pawnlegalB(int from, std::vector<move>&moves, uint64_t own, uint64_t enmy) {
    for (auto dis : pawnb) {
        int to = from + dis;
        if (to < 0)continue;
        if (from < 47 && dis == 16)continue;
        if (own & (1ull << to))continue;
        if (!(enmy & 1ull << to) && (dis == -7 || dis == -9))continue;
        moves.push_back({ to,from });
    }
}




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
    WhitePawn = 0x000000000000FF00ULL; board |= WhitePawn;
 
    

    BlackKing = 1ull << posval(8, 5); board |= BlackKing;
    BlackQueen = 1ull << posval(8, 4); board |= BlackQueen;
    BlackBishop = 1ull << posval(8, 3) | 1ull << posval(8, 6); board |= BlackBishop;
    BlackKnight = 1ull << posval(8, 2) | 1ull << posval(8, 7); board |= BlackKnight;
    BlackRook = 1ull << posval(8, 1) | 1ull << posval(8, 8); board |= BlackRook;
    BlackPawn = 0x00FF000000000000ULL; board |= BlackPawn;

    update();
}

//pawen class
//class pawn {
//    public:
//        int pos;
//        bool isWhite;
//        pawn(int a,bool b) {
//            pos = a;
//            isWhite = b;
//        }
//        void draw() {
//            sf::RectangleShape pawnrect({ 100.f,100.f });
//            if(isWhite)
//            pawnrect.setFillColor(sf::Color::White);
//            else
//                pawnrect.setFillColor(sf::Color::Black);
//        }
//};


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
    std::vector<move>moves;
    
    pawnlegal(posval(2, 4),moves, WhiteP, BlackP);
    for (auto mo : moves) {
        //std::cout << revpos(mo.to)<<revpos2(mo.to) << "  ";
        std::cout << mo.to << "  ";
    }

    
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