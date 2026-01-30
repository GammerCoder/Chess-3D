// Chess-Server.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<SFML/Network.hpp>
#include<cstdint>

//Netwok packets
enum class Pakcettype : uint8_t
{
    Move = 01,
    chat = 02,
    join = 3,
    leave = 4

};

//Board print packet
enum Piece : uint8_t {
    Empty,
    WP, WR, WN, WB, WQ, WK,
    BP, BR, BN, BB, BQ, BK
};

Piece board[64];
void initBoard()
{
    //Empty
    for (int i = 0; i < 64; i++)
        board[i] = Empty;


    // pawns
    for (int i = 8; i < 16; i++) board[i] = WP;
    for (int i = 48; i < 56; i++) board[i] = BP;

    // rooks
    board[0] = WR; board[7] = WR;
    board[56] = BR; board[63] = BR;

    // knight
    board[1] = WN; board[6] = WN;
    board[57] = BN; board[62] = BN;

    //Bishop
    board[2] = WB; board[5] = WB;
    board[58] = BB; board[61] = BB;

    //Queen
    board[3] = WQ;
    board[59] = BQ;

    //King
    board[4] = WK;
    board[60] = BK;

}


int main()
{
    std::cout << "Hello World!\n";
    sf::TcpListener listener;

    if (listener.listen(54000) != sf::Socket::Status::Done) {
        std::cout << " Failed to listen on port\n";
        return 1;
    }

    std::cout << " Server listening on port 54000...\n";

    sf::TcpSocket client;
    if (listener.accept(client) != sf::Socket::Status::Done) {
        std::cout << " Failed to accept client\n";
        return 1;
    }

    std::cout << " Client connected!\n";

    sf::Packet packet;
    if (client.receive(packet) == sf::Socket::Status::Done) {
        std::string message;
        packet >> message;
        std::cout << " Received from client: " << message << "\n";
    }
    if (client.receive(packet) == sf::Socket::Status::Done) {
        uint8_t raw;
        packet >> raw;
        Pakcettype type = static_cast<Pakcettype>(raw);
        switch (type)
        {
        case Pakcettype::Move:

            break;
        case Pakcettype::chat:

            break;
        case Pakcettype::join:

            break;
        case Pakcettype::leave:

            break;
        default:
            break;
        }
    }

    std::cin.get(); 
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
