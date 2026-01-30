
#include "SFML/Graphics.hpp"
#include <cstdint>
#include <iostream>
#include<SFML/Network.hpp>
uint64_t board = 0ULL;

//Netwok packets
enum class Pakcettype : uint8_t
{
    Move = 01,
    chat = 02,
    join = 3,
    leave = 4
    
};

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

//Strcct  pawn
struct piece {
    sf::Sprite pie;
    int square= -1;
    bool alive;
    int val;
    piece(sf::Texture& tex, int sq, sf::Vector2f pos,bool a,int v) :pie(tex), square(sq),alive(a),val(v) {
        pie.setPosition(pos);
        pie.setScale({ 0.825f,0.825f });
    }
};

// PTS
int pixelToSquare(sf::Vector2f pos, int tile) {
    int file = pos.x / tile;
    int rank = 7 - (pos.y / tile);
    return rank * 8 + file;
}
//STP
sf::Vector2f squareToPixel(int square, int tileSize) {
    int file = square % 8;          // column: 0–7
    int rank = 7 - (square / 8);    // row: flip board for white
    return sf::Vector2f(
        file * tileSize,
        rank * tileSize
    );
}

//Grid snapp
int grip = 100;
sf::Vector2f snapp(sf::Vector2f pos) {
    return{ std::round(pos.x / grip) * grip,
            std::round(pos.y / grip) * grip };
}

//Move possible
struct move {
    int to;
    int from;
    bool capt;
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
    board = 0ull;
    WhiteP = 0ull;
    BlackP = 0ull;

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

//RevUpdate
void revupdate() {
     WhiteKing &= WhiteP;
     WhiteKnight &= WhiteP;
     WhiteBishop &= WhiteP;
     WhiteRook &= WhiteP;
     WhiteQueen &= WhiteP;
     WhitePawn &= WhiteP;

    BlackKing  &= BlackP;
    BlackQueen  &= BlackP;
    BlackKnight  &= BlackP;
    BlackBishop &= BlackP;
    BlackRook  &= BlackP;
    BlackPawn  &= BlackP;
}

//Capture
void Capture(uint64_t &enmy, int to) {
    enmy &= ~(1ull << to);
    revupdate();
    update();
}

//warp
bool samefilewarp(int from, int to) {
    int fromfile = from % 8;
    int tofile = to % 8;
    return abs(fromfile - tofile) > 1;
}


//King move
int kingm[] = { 9,8,7,1,-1,-9,-8,-7 };
void kinglegal( int from, std::vector<move>& moves,uint64_t own,uint64_t enmy) {
    for (auto i : kingm) {
        bool c = false;
        int to = from + i;
        if (to < 0 || to>63)continue;
        if (samefilewarp(from, to))continue;
        if (own & (1ull << to))continue;
        //Make cpature 
        if (enmy & (1ull << to))c=true;
        moves.push_back({ to,from,c });
        
    }
}

//Knight move
int Knightm[] = { 17,15,10,6,-17,-15,-10,-6 };
void knightlegal( int from, std::vector<move>& moves, uint64_t own,uint64_t enmy) {
    for (auto i : Knightm) {
        int to = from + i;
        bool c = false;
        if (to < 0 || to>63)continue;
        int fromfile = from % 8;
        int tofile = to % 8;    
        if(abs(fromfile - tofile) > 2){
            
            continue;
        }
        if ((own & (1ull << to)))continue;
        if (enmy & (1ull << to))c=true;
        moves.push_back({ to,from,c });
    }
}

//Rook Move
int rookm[] = { 1,8,-8,-1 };
void RookLegal( int from, std::vector<move>& moves, uint64_t own,uint64_t enmy) {
    int ofrom = from;
    for (int dir : rookm) {
        int next =  from + dir;
        bool c = false;
        bool brk = false;
        while (true) {
            if (next < 0 || next>63)break;
            if (own & (1ull << next))break;
            if (samefilewarp(from, next))break;
            if (enmy & (1ull << next)) {
                c = true;
                brk = true;
            }
            moves.push_back({ next,from,c });
            if (brk)break;

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
void BishopLegal( int from, std::vector<move>& moves, uint64_t own, uint64_t enmy) {
    int ofrom = from;
    for (int dir : bism) {
        int next = from + dir;
        bool c = false;
        bool brk = false;
        while (true) {
            if (next < 0 || next>63)break;
            if (own & (1ull << next))break;
            if (samefilewarp(from, next))break;
            if (enmy & (1ull << next)) {
                c = true;
                brk = true;
            }
            moves.push_back({ next,from,c });
            if (brk)break;
            next = dir + next;
            from = from + dir;
        }
        from = ofrom;
    }
}

//Queen move
void QueenLegal( int from, std::vector<move>& moves, uint64_t own, uint64_t enmy) {
    int ofrom = from;
    for (int dir : kingm) {
        int next = from + dir;
        bool c = false;
        bool brk = false;
        while (true) {
            if (next < 0 || next>63)break;
            if (own & (1ull << next))break;
            if (samefilewarp(from, next))break;
            if (enmy & (1ull << next)) {
                c = true;
                brk = true;
            }
            moves.push_back({ next,from,c });
            if (brk)break;
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
        bool c = false;
        if (to > 63)continue;
        if (from < 8 || from > 15 && dis == 16)continue;
        if (own & (1ull << to))continue;
        if (!(enmy & 1ull << to) && (dis == 7 || dis == 9))continue;
        if ((enmy & 1ull << to) && (dis == 7 || dis == 9))c=true;
        moves.push_back({ to,from,c });
    }
}
void pawnlegalB(int from, std::vector<move>&moves, uint64_t own, uint64_t enmy) {
    for (auto dis : pawnb) {
        int to = from + dis;
        bool c = false;
        if (to < 0)continue;
        if (from < 47 && dis == -16)continue;
        if (own & (1ull << to))continue;
        if (!(enmy & 1ull << to) && (dis == -7 || dis == -9))continue;
        if ((enmy & 1ull << to) && (dis == -7 || dis == -9))c=true;
        moves.push_back({ to,from,c });
    }
}

//Poston calculator
int posval(int a,int b) {
    return ((a-1) * 8) + (b-1);
}

//Mouse pos
sf::Vector2f getMousePos(sf::RenderWindow& window) {
    return window.mapPixelToCoords(sf::Mouse::getPosition(window));
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

//Checkker
uint64_t* Checker(int from, std::vector<move>& m) {
    if (WhitePawn &(1ull<< from)) {
        pawnlegal(from, m, WhiteP, BlackP);
        return &WhitePawn;
    }
    if (BlackPawn & (1ull << from)) {
        pawnlegalB(from, m, BlackP, WhiteP);
        return &BlackPawn;
    }
    if (BlackKing & (1ull << from)) {
        kinglegal(from, m, BlackP, WhiteP);
        return &BlackKing;
    }
    if (WhiteKing & (1ull << from)) {
        kinglegal(from, m, WhiteP, BlackP);
        return &WhiteKing;
    }
    if (WhiteKnight & (1ull << from)) {
        knightlegal(from, m, WhiteP, BlackP);
        return &WhiteKnight;
    }
    if (BlackKnight & (1ull << from)) {
        knightlegal(from, m, BlackP, WhiteP);
        return &BlackKnight;
    }
    if (BlackRook & (1ull << from)) {
        RookLegal(from, m, BlackP, WhiteP);
        return &BlackRook;
    }
    if (WhiteRook & (1ull << from)) {
        RookLegal(from, m, WhiteP, BlackP);
        return &WhiteRook;
    }
    if (WhiteBishop & (1ull << from)) {
        BishopLegal(from, m, WhiteP, BlackP);
        return &WhiteBishop;
    }
    if (BlackBishop & (1ull << from)) {
        BishopLegal(from, m, BlackP, WhiteP);
        return &BlackBishop;
    }
    if (WhiteQueen & (1ull << from)) {
        QueenLegal(from, m, WhiteP, BlackP);
        return &WhiteQueen;
    }
    if (BlackQueen & (1ull << from)) {
        QueenLegal(from, m, BlackP, WhiteP);
        return &BlackQueen;
    }
}


//Bar pos update
void bar2(sf::RectangleShape& a,int val) {
    val = val * 55;
    sf::Vector2f b = a.getSize();
    a.setSize({ 20,b.y + val });

}

int main() {

    sf::TcpSocket socket;
    sf::IpAddress serevr({127,0,0,1});

    if (socket.connect(serevr, 54000) != sf::Socket::Status::Done) {
        std::cout << " Failed to connect to server\n";
        return 1;
    }

    std::cout << " Connected to server\n";

    sf::Packet packet;
    packet << std::string("Hello from client!");

    if (socket.send(packet) != sf::Socket::Status::Done) {
        std::cout << "Failed to send packet\n";
        return 1;
    }

    std::cout << " Message sent to server\n";

    sf::RenderWindow window(sf::VideoMode({ 1200, 800 }), "Chess");
    bool select = 0;
    bool isdragging= false;
    piece* slt = nullptr;
    bool done = false;
    sf::Vector2f offset;
    if (select == false) {
        basicboard(board);
    }
    printBoard(board);
    std::vector<move>moves;

    int turn= 1;
    bool redraw = true;

    sf::Texture wk("../pieces-png/white-king.png");
    sf::Texture bk("../pieces-png/black-king.png");
    sf::Texture wp("../pieces-png/white-pawn.png");
    sf::Texture bp("../pieces-png/black-pawn.png");
    sf::Texture wr("../pieces-png/white-rook.png");
    sf::Texture br("../pieces-png/black-rook.png");
    sf::Texture wb("../pieces-png/white-bishop.png");
    sf::Texture bb("../pieces-png/black-bishop.png");
    sf::Texture wkn("../pieces-png/white-knight.png");
    sf::Texture bkn("../pieces-png/black-knight.png");
    sf::Texture wq("../pieces-png/white-queen.png");
    sf::Texture bq("../pieces-png/black-queen.png");

    // Board width
    sf::Vector2u winsiz = window.getSize();
    float boardw = winsiz.x * 0.7;
    float boardz = std::min(boardw, (float)winsiz.y);
    float tilesize = boardz / 8.0f;
    float evlbar_posx = boardw*0.95 ;


    // Draw
    std::vector<piece> wpas;
    sf::RectangleShape evlbar({20.f,boardz-5});
    evlbar.setPosition({ evlbar_posx,5 });
    evlbar.setFillColor(sf::Color::White);
    evlbar.setOutlineColor(sf::Color(152,162,162,255));
    evlbar.setOutlineThickness(4.f);

    sf::RectangleShape evlbar2({ 20.f,boardz/2 });
    evlbar2.setPosition({ evlbar_posx,5 });
    evlbar2.setFillColor(sf::Color::Blue);
    evlbar2.setOutlineColor(sf::Color(152, 162, 162, 255));
    evlbar2.setOutlineThickness(4.f);

    //Pawns
    float x = 0, y = 0;
    for (int i = 7; i >= 0; i--) {
        x = 0;
        for (int j = 0; j < 8; j++) {
            int sq = i * 8 + j;
            if ((WhitePawn >> sq) & 1) {
                wpas.emplace_back(wp,sq,sf::Vector2f(x,y),true,1);
            }
            x = x + tilesize;
        }
        y = y + tilesize;
    }
    y = 0;
    for (int i = 7; i >= 0; i--) {
        x = 0;
        for (int j = 0; j < 8; j++) {
            int sq = i * 8 + j;
            if ((BlackPawn >> sq) & 1) {
                wpas.emplace_back(bp, sq, sf::Vector2f(x, y),true,-1);
            }
            x = x + tilesize;
        }
        y = y + tilesize;
    }
    //Rooks
    y = 0;
    for (int i = 7; i >= 0; i--) {
        x = 0;
        for (int j = 0; j < 8; j++) {
            int sq = i * 8 + j;
            if ((WhiteRook >> sq) & 1) {
                wpas.emplace_back(wr, sq, sf::Vector2f(x, y),true,5);
            }
            x = x + tilesize;
        }
        y = y + tilesize;
    }
    y = 0;
    for (int i = 7; i >= 0; i--) {
        x = 0;
        for (int j = 0; j < 8; j++) {
            int sq = i * 8 + j;
            if ((BlackRook >> sq) & 1) {
                wpas.emplace_back(br, sq, sf::Vector2f(x, y),true,-5);
            }
            x = x + tilesize;
        }
        y = y + tilesize;
    }
    //Bishop
    y = 0;
    for (int i = 7; i >= 0; i--) {
        x = 0;
        for (int j = 0; j < 8; j++) {
            int sq = i * 8 + j;
            if ((WhiteBishop >> sq) & 1) {
                wpas.emplace_back(wb, sq, sf::Vector2f(x, y),true,3);
            }
            x = x + tilesize;
        }
        y = y + tilesize;
    }
    y = 0;
    for (int i = 7; i >= 0; i--) {
        x = 0;
        for (int j = 0; j < 8; j++) {
            int sq = i * 8 + j;
            if ((BlackBishop >> sq) & 1) {
                wpas.emplace_back(bb, sq, sf::Vector2f(x, y),true,-3);
            }
            x = x + tilesize;
        }
        y = y + tilesize;
    }
    //Knight
    y = 0;
    for (int i = 7; i >= 0; i--) {
        x = 0;
        for (int j = 0; j < 8; j++) {
            int sq = i * 8 + j;
            if ((WhiteKnight >> sq) & 1) {
                wpas.emplace_back(wkn, sq, sf::Vector2f(x, y),true,3);
            }
            x = x + tilesize;
        }
        y = y + tilesize;
    }
    y = 0;
    for (int i = 7; i >= 0; i--) {
        x = 0;
        for (int j = 0; j < 8; j++) {
            int sq = i * 8 + j;
            if ((BlackKnight >> sq) & 1) {
                wpas.emplace_back(bkn, sq, sf::Vector2f(x, y),true,-3);
            }
            x = x + tilesize;
        }
        y = y + tilesize;
    }
    //Queen
    y = 0;
    for (int i = 7; i >= 0; i--) {
        x = 0;
        for (int j = 0; j < 8; j++) {
            int sq = i * 8 + j;
            if ((WhiteQueen >> sq) & 1) {
                wpas.emplace_back(wq, sq, sf::Vector2f(x, y),true,10);
            }
            x = x + tilesize;
        }
        y = y + tilesize;
    }
    y = 0;
    for (int i = 7; i >= 0; i--) {
        x = 0;
        for (int j = 0; j < 8; j++) {
            int sq = i * 8 + j;
            if ((BlackQueen >> sq) & 1) {
                wpas.emplace_back(bq, sq, sf::Vector2f(x, y),true,-10);
            }
            x = x + tilesize;
        }
        y = y + tilesize;
    }
    //King
    y = 0;
    for (int i = 7; i >= 0; i--) {
        x = 0;
        for (int j = 0; j < 8; j++) {
            int sq = i * 8 + j;
            if ((WhiteKing >> sq) & 1) {
                wpas.emplace_back(wk, sq, sf::Vector2f(x, y),true,15);
            }
            x = x + tilesize;
        }
        y = y + tilesize;
    }
    y = 0;
    for (int i = 7; i >= 0; i--) {
        x = 0;
        for (int j = 0; j < 8; j++) {
            int sq = i * 8 + j;
            if ((BlackKing >> sq) & 1) {
                wpas.emplace_back(bk, sq, sf::Vector2f(x, y),true,-15);
            }
            x = x + tilesize;
        }
        y = y + tilesize;
    }
    
    //Tile sprites
    std::vector<sf::RectangleShape>tle;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            sf::RectangleShape Tile({ tilesize,tilesize });
            Tile.setPosition({ i * tilesize, j * tilesize });
            if ((i + j) % 2 == 0) {
                Tile.setFillColor(sf::Color(238, 238, 210));
            }
            else {
                Tile.setFillColor(sf::Color(118, 150, 56));
            }
            tle.push_back(Tile);
        }
    }

    while (window.isOpen())
    {
            while (const std::optional event = window.pollEvent())
            {
                if (event->is<sf::Event::Closed>())
                    window.close();

                // Mouse catch
                if (event->is<sf::Event::MouseButtonPressed>()) {
                    const auto& e = event->getIf<sf::Event::MouseButtonPressed>();
                    if (isdragging)break;
                    if (e->button == sf::Mouse::Button::Left) {
                        sf::Vector2f m =getMousePos(window);
                        for (auto &spr : wpas) {
                            if (spr.pie.getGlobalBounds().contains(m)) {
                                isdragging = true;
                                slt = &spr;
                                offset = spr.pie.getPosition() - m;
                                int from = slt->square;
                                if (!((turn % 2 == 0 && BlackP & (1ull << from)) || (turn % 2 != 0 && WhiteP & (1ull << from)))) {
                                    slt = nullptr;
                                    isdragging = false;
                                    std::cout << "Invlaid chance" << std::endl;
                                }

                                Checker(from, moves);
                                for (auto mo : moves) {
                                    std::cout << mo.to<<"  ";
                                }
                                std::cout << std::endl;
                            }
                        }
                    }
                }
                //Mouse drag
                if (isdragging && slt) {
                    sf::Vector2f m = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    if (slt) {
                        slt->pie.setPosition(snapp(m + offset));
                        redraw = true;
                    }
                }

                //Mouse release
                if (event->is<sf::Event::MouseButtonReleased>()) {
                    const auto& e = event->getIf<sf::Event::MouseButtonReleased>();
                    if (e->button == sf::Mouse::Button::Left && isdragging) {
                        if (slt) {
                            int from = slt->square;
                            int to = pixelToSquare(slt->pie.getPosition(), tilesize);
                            uint64_t* temp = Checker(from, moves);
                            for (auto mov : moves) {
                                if (mov.to == to) {
                                    if (mov.capt) {
                                        Capture(WhiteP, to);
                                        Capture(BlackP, to);
                                        for (auto& i : wpas) {
                                            if (i.square == to) {
                                                i.alive = false;
                                                i.square = -1;
                                                i.pie.setPosition({ 900,0 });
                                                bar2(evlbar2, i.val);
                                            }
                                        }
                                    }
                                    std::cout << mov.capt << std::endl << turn << std::endl;

                                    *temp &= ~(1ull << from);
                                    *temp |= (1ull << to);
                                    slt->square = to;
                                    done = true;
                                    turn++;
                                    break;
                                }
                            }
                            if (!done) {
                                slt->pie.setPosition(squareToPixel(from, tilesize));
                            }
                            done = false;
                            moves.clear();
                            update();
                            printBoard(board);
                            isdragging = false;
                            slt = nullptr;
                            redraw = true;
                        }
                    }
                }
            }

            if (!redraw) {
                continue;
            }

            for (sf::RectangleShape jj : tle) {
                window.draw(jj);
            }
            for (auto &i : wpas) {
                if (i.alive) {
                    window.draw(i.pie);
                }
            }            
            window.draw(evlbar);
            window.draw(evlbar2);
        window.display();
        redraw = false;
    }

	return 0;
}