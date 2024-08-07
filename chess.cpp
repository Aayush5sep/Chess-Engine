#include <iostream>
#include <string>
#include <vector>
#include <limits.h>

using namespace std;

// Enumerating Pieces
// o -> Offboard Square
// e -> Empty Square
// P, B, N, R, Q, K -> Pawn, Bishop, Knight, Rook, Queen, King
// Uppercase -> White Piece, Lowercase -> Black Piece
enum pieces {o, e, P, B, N, R, Q, K, p, b, n, r, q, k};    // 0, 1, 2, 3, 4.......

// Ascii Encoding
string ascii_pieces = "o.PBNRQKpbnrqk";

// Unicode Encoding
string unicode_pieces[14] = {"o", ".", "♙", "♗", "♘", "♖", "♕", "♔", "♟︎", "♝", "♞", "♜", "♛", "♚"};

int char_pieces[] = {
    ['P'] = P,
    ['N'] = N,
    ['B'] = B,
    ['R'] = R,
    ['Q'] = Q,
    ['K'] = K,
    ['p'] = p,
    ['n'] = n,
    ['b'] = b,
    ['r'] = r,
    ['q'] = q,
    ['k'] = k,
};

// Chess Board
int chess_board[8][16] = {
    {r, n, b, q, k, b, n, r,    o, o, o, o, o, o, o, o},
    {p, p, p, p, p, p, p, p,    o, o, o, o, o, o, o, o},
    {e, e, e, e, e, e, e, e,    o, o, o, o, o, o, o, o},
    {e, e, e, e, e, e, e, e,    o, o, o, o, o, o, o, o},
    {e, e, e, e, e, e, e, e,    o, o, o, o, o, o, o, o},
    {e, e, e, e, e, e, e, e,    o, o, o, o, o, o, o, o},
    {P, P, P, P, P, P, P, P,    o, o, o, o, o, o, o, o},
    {R, N, B, Q, K, B, N, R,    o, o, o, o, o, o, o, o}
};

// Enumerating chess squares/positions
enum positions {
    a8 = 0, b8, c8, d8, e8, f8, g8, h8,
    a7 = 16, b7, c7, d7, e7, f7, g7, h7,
    a6 = 32, b6, c6, d6, e6, f6, g6, h6,
    a5 = 48, b5, c5, d5, e5, f5, g5, h5,
    a4 = 64, b4, c4, d4, e4, f4, g4, h4,
    a3 = 80, b3, c3, d3, e3, f3, g3, h3,
    a2 = 96, b2, c2, d2, e2, f2, g2, h2,
    a1 = 112, b1, c1, d1, e1, f1, g1, h1, no_sq
};

// Convert board index to chess position
string index_to_position[8][8] = {
    {"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8"},
    {"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7"},
    {"a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6"},
    {"a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5"},
    {"a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4"},
    {"a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3"},
    {"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2"},
    {"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"}
};

// Enumeration for castling
// castle = 0 0 0 0
//          q k Q K
enum castling {Kc = 1, Qc = 2, kc = 4, qc = 8};

// Castling rights
// Logic - all castling rights = 15
// If white king moves, then castling rights -> 1100 = 12
// If white king side rook moves, then castling rights -> 1110 = 14
// If white queen side rook moves, then castling rights -> 1101 = 13
// If black king moves, then castling rights -> 0011 = 3
// If black king side rook moves, then castling rights -> 1011 = 11
// If black queen side rook moves, then castling rights -> 0111 = 7
// So if source or destination square is among the king or rook starting square, then AND operation does the work
int castling_rights[8][16] = {
    {7, 15, 15, 15, 3, 15, 15, 11,      0, 0, 0, 0, 0, 0, 0, 0},
    {15, 15, 15, 15, 15, 15, 15, 15,    0, 0, 0, 0, 0, 0, 0, 0},
    {15, 15, 15, 15, 15, 15, 15, 15,    0, 0, 0, 0, 0, 0, 0, 0},
    {15, 15, 15, 15, 15, 15, 15, 15,    0, 0, 0, 0, 0, 0, 0, 0},
    {15, 15, 15, 15, 15, 15, 15, 15,    0, 0, 0, 0, 0, 0, 0, 0},
    {15, 15, 15, 15, 15, 15, 15, 15,    0, 0, 0, 0, 0, 0, 0, 0},
    {15, 15, 15, 15, 15, 15, 15, 15,    0, 0, 0, 0, 0, 0, 0, 0},
    {13, 15, 15, 15, 12, 15, 15, 14,    0, 0, 0, 0, 0, 0, 0, 0}
};

// King Squares White, Black
int king_track[2] = {e1, e8};

// 0 -> white, 1 -> black
int side_to_move = 0;

// Castling Rights
int castle = 0;

// En-passant square
int enpassant = no_sq;

// Piece Moves
int knight_moves[8][2] = {{2,1},{2,-1},{-2,1},{-2,-1},{1,2},{-1,2},{1,-2},{-1,-2}};
int bishop_offsets[4][2] = {{1,1},{1,-1},{-1,1},{-1,-1}};
int rook_offsets[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
int king_moves[8][2] = {{1,0},{-1,0},{0,1},{0,-1},{1,1},{1,-1},{-1,1},{-1,-1}};

// Piece Valuation
int piece_value[14] = {0, 0, 100, 350, 325, 500, 900, 100000, -100, -350, -325, -500, -900, -100000}; // usual piece value times 100

// Positional Valuation {o, e, P, B, N, R, Q, K, p, b, n, r, q, k}
// Opening positional values picked from Chess Programming Wiki
int positional_value[14][64] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {
        0,  0,  0,  0,  0,  0,  0,  0,
        50, 50, 50, 50, 50, 50, 50, 50,
        10, 10, 20, 30, 30, 20, 10, 10,
        5,  5, 10, 25, 25, 10,  5,  5,
        0,  0,  0, 20, 20,  0,  0,  0,
        5, -5,-10,  0,  0,-10, -5,  5,
        5, 10, 10,-20,-20, 10, 10,  5,
        0,  0,  0,  0,  0,  0,  0,  0
    },
    {
        -20, -10, -10, -10, -10, -10, -10, -20,
        -10,   0,   0,   0,   0,   0,   0, -10,
        -10,   0,   5,  10,  10,   5,   0, -10,
        -10,   5,   5,  10,  10,   5,   5, -10,
        -10,   0,  10,  10,  10,  10,   0, -10,
        -10,  10,  10,  10,  10,  10,  10, -10,
        -10,   5,   0,   0,   0,   0,   5, -10,
        -20, -10, -10, -10, -10, -10, -10, -20
    },
    {
        -50, -40, -30, -30, -30, -30, -40, -50,
        -40, -20,   0,   0,   0,   0, -20, -40,
        -30,   0,  10,  15,  15,  10,   0, -30,
        -30,   5,  15,  20,  20,  15,   5, -30,
        -30,   0,  15,  20,  20,  15,   0, -30,
        -30,   5,  10,  15,  15,  10,   5, -30,
        -40, -20,   0,   5,   5,   0, -20, -40,
        -50, -40, -30, -30, -30, -30, -40, -50
    },
    {
        0,  0,  0,  0,  0,  0,  0,  0,
        5, 10, 10, 10, 10, 10, 10,  5,
       -5,  0,  0,  0,  0,  0,  0, -5,
       -5,  0,  0,  0,  0,  0,  0, -5,
       -5,  0,  0,  0,  0,  0,  0, -5,
       -5,  0,  0,  0,  0,  0,  0, -5,
       -5,  0,  0,  0,  0,  0,  0, -5,
        0,  0,  0,  5,  5,  0,  0,  0
    },
    {
        -20, -10, -10,  -5,  -5, -10, -10, -20,
        -10,   0,   0,   0,   0,   0,   0, -10,
        -10,   0,   5,   5,   5,   5,   0, -10,
        -5,   0,   5,   5,   5,   5,   0,  -5,
        0,   0,   5,   5,   5,   5,   0,  -5,
        -10,   5,   5,   5,   5,   5,   0, -10,
        -10,   0,   5,   0,   0,   0,   0, -10,
        -20, -10, -10,  -5,  -5, -10, -10, -20
    },
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {
        0,  0,  0,  0,  0,  0,  0,  0,
       -5,-10,-10, 20, 20,-10,-10, -5,
       -5,  5, 10,  0,  0, 10,  5, -5,
        0,  0,  0,-20,-20,  0,  0,  0,
       -5, -5,-10,-25,-25,-10, -5, -5,
      -10,-10,-20,-30,-30,-20,-10,-10,
      -50,-50,-50,-50,-50,-50,-50,-50,
        0,  0,  0,  0,  0,  0,  0,  0
    },
    {
        20,  10,  10,  10,  10,  10,  10,  20,
        10,  -5,   0,   0,   0,   0,  -5,  10,
        10, -10, -10, -10, -10, -10, -10,  10,
        10,   0, -10, -10, -10, -10,   0,  10,
        10,  -5,  -5, -10, -10,  -5,  -5,  10,
        10,   0,  -5, -10, -10,  -5,   0,  10,
        10,   0,   0,   0,   0,   0,   0,  10,
        20,  10,  10,  10,  10,  10,  10,  20
    },
    {
        50,  40,  30,  30,  30,  30,  40,  50,
        40,  20,   0,  -5,  -5,   0,  20,  40,
        30,  -5, -10, -15, -15, -10,  -5,  30,
        30,   0, -15, -20, -20, -15,   0,  30,
        30,  -5, -15, -20, -20, -15,  -5,  30,
        30,   0, -10, -15, -15, -10,   0,  30,
        40,  20,   0,   0,   0,   0,  20,  40,
        50,  40,  30,  30,  30,  30,  40,  50
    },
    {
        0,  0,  0, -5, -5,  0,  0,  0,
        5,  0,  0,  0,  0,  0,  0,  5,
        5,  0,  0,  0,  0,  0,  0,  5,
        5,  0,  0,  0,  0,  0,  0,  5,
        5,  0,  0,  0,  0,  0,  0,  5,
        5,  0,  0,  0,  0,  0,  0,  5,
       -5,-10,-10,-10,-10,-10,-10, -5,
        0,  0,  0,  0,  0,  0,  0,  0
    },
    {
        20,  10,  10,   5,   5,  10,  10,  20,
        10,   0,  -5,   0,   0,   0,   0,  10,
        10,  -5,  -5,  -5,  -5,  -5,   0,  10,
        0,   0,  -5,  -5,  -5,  -5,   0,   5,
        5,   0,  -5,  -5,  -5,  -5,   0,   5,
        5,   0,  -5,  -5,  -5,  -5,   0,  10,
        10,   0,   0,   0,   0,   0,   0,  10,
        20,  10,  10,   5,   5,  10,  10,  20
    },
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

// Check if square is on board
bool valid_move(int i, int j){
    return i >=0 && i < 8 && j >=0 && j < 8;
}

// FEN String parsing
string starting_position = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
string random_position = "r2qk2r/pp1b1pp1/2nppn1p/3p4/3P4/2NBPN2/PPPQ1PPP/R3K2R w KQkq h2 0 9";

void parse_fen_string_to_board(string &fen_position){
    int curr = 0, fen_length = fen_position.length();
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            // Check for piece
            if((fen_position[curr] >= 'a' && fen_position[curr] <= 'z') || (fen_position[curr] >= 'A' && fen_position[curr] <= 'Z')){
                chess_board[i][j] = char_pieces[fen_position[curr]];
                curr++;
            }
            // Check for empty spaces via numbers
            else if(fen_position[curr] >= '0' && fen_position[curr] <= '9'){
                j += (fen_position[curr] - '0')-1;
                curr++;
            }
            // Skip any /
            if(fen_position[curr] == '/'){
                curr++;
            }
            // Change configuration when space
            else if(fen_position[curr] == ' '){
                curr++;
            }
        }
    }

    // Check side to move
    (fen_position[curr] == 'w') ? side_to_move = 0 : side_to_move = 1;
    curr += 2;

    // Check for castling rights
    while(fen_position[curr] != ' '){
        switch (fen_position[curr]){
            case 'K': castle += Kc; break;
            case 'Q': castle += Qc; break;
            case 'k': castle += kc; break;
            case 'q': castle += qc; break;
        }
        curr++;
    }
    curr++;

    // Check for en-passant square
    if(fen_position[curr] != '-'){
        int file = fen_position[curr] - 'a';
        int rank = fen_position[curr+1] - '0';

        enpassant = file + (8 - rank) * 16;
    }

}

// Encode Moves to Integers
// source_square -> 7 bits
// target_square -> 7 bits
// promoted_piece -> 4 bits
// capture_flag -> 1 bit
// enpassant_flag -> 1 bit
// castling_flag -> 1 bit
// doublepawnmove_flag -> 1 bit
int encode_move_to_integer(int source_square, int target_square, int promoted_piece, int capture_flag, int enpassant_flag, int castling_flag, int doublepawnmove_flag){
    return (source_square | (target_square << 7) | (promoted_piece << 14) | (capture_flag << 18) | (enpassant_flag << 19) | (castling_flag << 20) | (doublepawnmove_flag << 21));
}

// Decoding Integer to Move & Flags
int source_square(int move){
    return move & 0x7F;
}

int target_square(int move){
    return (move >> 7) & 0x7F;
}

int promoted_piece(int move){
    return (move >> 14) & 0xF;
}

int capture_flag(int move){
    return (move >> 18) & 1;
}

int enpassant_flag(int move){
    return (move >> 19) & 1;
}

int castling_flag(int move){
    return (move >> 20) & 1;
}

int doublepawnmove_flag(int move){
    return (move >> 21) & 1;
}

void print_decoded_move(int move){
    int source = source_square(move), target = target_square(move), promoted = promoted_piece(move), capture = capture_flag(move), enpassant_capture = enpassant_flag(move), castling = castling_flag(move), doublepawnmove = doublepawnmove_flag(move);

    if(capture) cout << index_to_position[source_square(move)/16][source_square(move)%16] << " takes " << index_to_position[target_square(move)/16][target_square(move)%16]<<" ";
    else cout << index_to_position[source_square(move)/16][source_square(move)%16] << " -> " << index_to_position[target_square(move)/16][target_square(move)%16]<<" ";

    if(promoted) cout << ascii_pieces[promoted_piece(move)];
    if(castling) cout << "Castling";
    if(enpassant_capture) cout << "Enpassant";
    if(doublepawnmove) cout << "Double Pawn Move";
}


// Print Chess Board
void print_chess_board(){
    for(int i=0;i<8;i++){
        for(int j=0;j<16;j++){

            // Printing ranks 1,2,....8
            if(j == 0) cout << 8-i << "  ";

            // 0x88 is 10001000 in binary used when using 1D array of 128 ==> Since for j<8 it will return 0 in bitwise AND
            // if(j<8) cout<<ascii_pieces[chess_board[i][j]]<<" ";
            if(j<8) cout << unicode_pieces[chess_board[i][j]] << " ";

        }
        cout<<endl;
    }
    // Printing file a,b....h
    cout << "\n   a b c d e f g h \n" << endl;

    cout << "Side to move: " << side_to_move << endl;

    cout << "Castling rights: " << castle << endl;

    cout << "Enpassant Square: " << index_to_position[enpassant/16][enpassant%16] << endl;
}

// Clearing Board
void clear_board(){
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            chess_board[i][j] = e;
        }
    }
    side_to_move = -1;
    castle = 0;
}

// Moves History
class Moves{
    private:
        vector<int> moves;
        int count;
    public:
        Moves(){
            count = 0;
        }
        int get_count(){
            return count;
        }
        void add_move(int move){
            moves.push_back(move);
            count++;
        }
        void undo_move(){
            moves.pop_back();
            count--;
        }
        vector<int> get_all_moves(){
            return moves;
        }
        void print_all_moves(){
            cout<<"\n\nPrinting Moves History\n\n";
            for(int i=0;i<moves.size();i++){
                int move = moves[i];
                print_decoded_move(move);
                cout<<"\n";
            }
            cout << endl;
        }
}moves_history;

// Check if square is attacked
bool is_square_attacked(int i, int j, int side){
    // Pawn Attacks
    if(!side){
        if(valid_move(i+1,j-1) && chess_board[i+1][j-1] == P) return 1;
        if(valid_move(i+1,j+1) && chess_board[i+1][j+1] == P) return 1;
    }
    else{
        if(valid_move(i-1,j-1) && chess_board[i-1][j-1] == p) return 1;
        if(valid_move(i-1,j+1) && chess_board[i-1][j+1] == p) return 1;
    }

    // Knight Attacks
    for(int ind=0;ind<8;ind++){
        int x = knight_moves[ind][0];
        int y = knight_moves[ind][1];

        if(!side && valid_move(i+x,j+y) && chess_board[i+x][j+y] == N) return 1;
        if(side && valid_move(i+x,j+y) && chess_board[i+x][j+y] == n) return 1;
    }

    // King Attacks
    for(int ind=0;ind<8;ind++){
        int x = king_moves[ind][0];
        int y = king_moves[ind][1];

        if(!side && valid_move(i+x,j+y) && chess_board[i+x][j+y] == K) return 1;
        if(side && valid_move(i+x,j+y) && chess_board[i+x][j+y] == k) return 1;
    }

    // Bishop & Queen Attacks
    for(int ind=0;ind<4;ind++){
        int tarx = i + bishop_offsets[ind][0], tary = j + bishop_offsets[ind][1];
        while(valid_move(tarx,tary)){
            // Check for bishop or queen
            if(!side && (chess_board[tarx][tary] == B || chess_board[tarx][tary] == Q)) return 1;
            if(side && (chess_board[tarx][tary] == b || chess_board[tarx][tary] == q)) return 1;

            // Break if any piece comes in between
            if(chess_board[tarx][tary] != e) break;

            tarx = tarx + bishop_offsets[ind][0];
            tary = tary + bishop_offsets[ind][1];
        }
    }

    // Rook & Queen Attacks
    for(int ind=0;ind<4;ind++){
        int tarx = i + rook_offsets[ind][0], tary = j + rook_offsets[ind][1];
        while(valid_move(tarx,tary)){
            // Check for rook or queen
            if(!side && (chess_board[tarx][tary] == R || chess_board[tarx][tary] == Q)) return 1;
            if(side && (chess_board[tarx][tary] == r || chess_board[tarx][tary] == q)) return 1;

            // Break if any piece comes in between
            if(chess_board[tarx][tary] != e) break;

            tarx = tarx + rook_offsets[ind][0];
            tary = tary + rook_offsets[ind][1];
        }
    }

    return 0;
}

// Print all Attacked Squares
void print_attacked_squares(int side){
    cout<<"\n\n";
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){

            // Printing ranks 1,2,....8
            if(j == 0) cout << 8-i << "  ";

            if(is_square_attacked(i,j,side)) cout << "x ";
            else cout << ". ";

        }
        cout<<endl;
    }
    // Printing file a,b....h
    cout << "\n   a b c d e f g h \n" << endl;
}

int evaluate_position(){
    int score = 0;
    // cout<<"\n\n";
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            score += piece_value[chess_board[i][j]];
            score += positional_value[chess_board[i][j]][i*8+j];
            // cout << positional_value[chess_board[i][j]][i*8+j] <<"\t\t";
        }
        // cout<<endl;
    }
    return score;
}

bool set_move(int move){
    // Being handled in find_best_move now
    // Make Board Copy
    // int chess_board_copy[8][16];
    // copy(&chess_board[0][0], &chess_board[0][0] + 8*16, &chess_board_copy[0][0]);
    // int castle_copy = castle;
    // int enpassant_copy = enpassant;
    // int king_track_copy[2];
    // copy(&king_track[0], &king_track[0] + 2, &king_track_copy[0]);


    // Decode Integer into moves
    int source = source_square(move), target = target_square(move), promoted = promoted_piece(move), capture = capture_flag(move), enpassant_capture = enpassant_flag(move), castling = castling_flag(move), doublepawnmove = doublepawnmove_flag(move);

    // Update Moved Piece on Chess Board
    chess_board[target/16][target%16] = chess_board[source/16][source%16];
    chess_board[source/16][source%16] = e;

    if(promoted > 0){
        chess_board[target/16][target%16] = promoted;
    }

    // Handle enpassant captures
    if(enpassant_capture){
        if(side_to_move == 0){
            chess_board[target/16 + 1][target%16] = e;
        }
        else{
            chess_board[target/16 - 1][target%16] = e;
        }
    }
    // Reset enpassant_square
    enpassant = no_sq;

    // Add enpassant square in case of double move
    if(doublepawnmove){
        if(side_to_move == 0){
            enpassant = target + 16;
        }
        else{
            enpassant = target - 16;
        }
    }

    // Castling Move
    if(castling){
        if(side_to_move == 0){
            if(target == 7*16+6){   // g1
                chess_board[7][5] = R;
                chess_board[7][7] = e;
            }
            else if(target == 7*16+2){  // c1
                chess_board[7][3] = R;
                chess_board[7][0] = e;
            }
        }
        else{
            if(target == 0*16+6){   // g8
                chess_board[0][5] = r;
                chess_board[0][7] = e;
            }
            else if(target == 0*16+2){  // c8
                chess_board[0][3] = r;
                chess_board[0][0] = e;
            }
        }
    }

    // Update Castling Rights
    castle = castle & castling_rights[source/16][source%16] & castling_rights[target/16][target%16];

    // Update king square
    if(chess_board[target/16][target%16] == K || chess_board[target/16][target%16] == k) king_track[side_to_move] = target;

    // Check Legal Move
    // Check if after move for a side, the same side king is not in check then
    if(is_square_attacked(king_track[side_to_move]/16,king_track[side_to_move]%16,!side_to_move)){
        // Reset values
        // Being handled in find_best_move now
        // copy(&chess_board_copy[0][0], &chess_board_copy[0][0] + 8*16, &chess_board[0][0]);
        // castle = castle_copy;
        // enpassant = enpassant_copy;
        // copy(&king_track_copy[0], &king_track_copy[0] + 2, &king_track[0]);
        return false;
    }

    // Update Side to Move
    // Being handled in find_best_move now
    // side_to_move = !side_to_move;
    return true;
}

// Generate Moves
void generate_moves(int side, Moves &possible_moves){
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){

            // White Pawn Moves
            if(!side && chess_board[i][j] == P){
                // Single Move
                if(valid_move(i-1,j) && chess_board[i-1][j] == e){
                    // Promotion
                    if(i == 1){
                        possible_moves.add_move(encode_move_to_integer(i*16+j,(i-1)*16+j,Q,0,0,0,0));
                        possible_moves.add_move(encode_move_to_integer(i*16+j,(i-1)*16+j,R,0,0,0,0));
                        possible_moves.add_move(encode_move_to_integer(i*16+j,(i-1)*16+j,B,0,0,0,0));
                        possible_moves.add_move(encode_move_to_integer(i*16+j,(i-1)*16+j,N,0,0,0,0));
                    }
                    // Normal Move
                    else{
                        possible_moves.add_move(encode_move_to_integer(i*16+j,(i-1)*16+j,0,0,0,0,0));
                    }
                }

                // Double Move
                if(i == 6 && chess_board[i-1][j] == e && chess_board[i-2][j] == e){
                    possible_moves.add_move(encode_move_to_integer(i*16+j,(i-2)*16+j,0,0,0,0,1));
                }

                // Capture Move
                // Normal Captures
                if(i>1 && valid_move(i-1,j-1) && chess_board[i-1][j-1] >= p && chess_board[i-1][j-1] <= k){
                    possible_moves.add_move(encode_move_to_integer(i*16+j,(i-1)*16+j-1,0,1,0,0,0));
                }
                if(i>1 && valid_move(i-1,j+1) && chess_board[i-1][j+1] >= p && chess_board[i-1][j+1] <= k){
                    possible_moves.add_move(encode_move_to_integer(i*16+j,(i-1)*16+j+1,0,1,0,0,0));
                }
                // Capture En-passant
                if(valid_move(i-1,j-1) && chess_board[i][j-1] == p && enpassant == (i-1)*16+j-1){
                    possible_moves.add_move(encode_move_to_integer(i*16+j,(i-1)*16+j-1,0,1,1,0,0));
                }
                if(valid_move(i-1,j+1) && chess_board[i][j+1] == p && enpassant == (i-1)*16+j+1){
                    possible_moves.add_move(encode_move_to_integer(i*16+j,(i-1)*16+j+1,0,1,1,0,0));
                }
                // Capture Promotion
                if(i==1 && valid_move(i-1,j-1) && chess_board[i-1][j-1] >= p && chess_board[i-1][j-1] <= k){
                    possible_moves.add_move(encode_move_to_integer(i*16+j,(i-1)*16+j-1,Q,1,0,0,0));
                    possible_moves.add_move(encode_move_to_integer(i*16+j,(i-1)*16+j-1,R,1,0,0,0));
                    possible_moves.add_move(encode_move_to_integer(i*16+j,(i-1)*16+j-1,B,1,0,0,0));
                    possible_moves.add_move(encode_move_to_integer(i*16+j,(i-1)*16+j-1,N,1,0,0,0));
                }
                if(i==1 && valid_move(i-1,j+1) && chess_board[i-1][j+1] >= p && chess_board[i-1][j+1] <= k){
                    possible_moves.add_move(encode_move_to_integer(i*16+j,(i-1)*16+j+1,Q,1,0,0,0));
                    possible_moves.add_move(encode_move_to_integer(i*16+j,(i-1)*16+j+1,R,1,0,0,0));
                    possible_moves.add_move(encode_move_to_integer(i*16+j,(i-1)*16+j+1,B,1,0,0,0));
                    possible_moves.add_move(encode_move_to_integer(i*16+j,(i-1)*16+j+1,N,1,0,0,0));
                }
            }

            // Black Pawn Moves
            else if(side && chess_board[i][j] == p){
                // Single Move
                if(valid_move(i+1,j) && chess_board[i+1][j] == e){
                    // Promotion
                    if(i == 6){
                        possible_moves.add_move(encode_move_to_integer(i*16+j,(i+1)*16+j,q,0,0,0,0));
                        possible_moves.add_move(encode_move_to_integer(i*16+j,(i+1)*16+j,r,0,0,0,0));
                        possible_moves.add_move(encode_move_to_integer(i*16+j,(i+1)*16+j,b,0,0,0,0));
                        possible_moves.add_move(encode_move_to_integer(i*16+j,(i+1)*16+j,n,0,0,0,0));
                    }
                    else{
                        possible_moves.add_move(encode_move_to_integer(i*16+j,(i+1)*16+j,0,0,0,0,0));
                    }
                }

                // Double Move
                if(i == 1 && chess_board[i+1][j] == e && chess_board[i+2][j] == e){
                    possible_moves.add_move(encode_move_to_integer(i*16+j,(i+2)*16+j,0,0,0,0,1));
                }

                // Capture Move
                // Normal Captures
                if(i<6 && valid_move(i+1,j-1) && chess_board[i+1][j-1] >= P && chess_board[i+1][j-1] <= K){
                    possible_moves.add_move(encode_move_to_integer(i*16+j,(i+1)*16+j-1,0,1,0,0,0));
                }
                if(i<6 && valid_move(i+1,j+1) && chess_board[i+1][j+1] >= P && chess_board[i+1][j+1] <= K){
                    possible_moves.add_move(encode_move_to_integer(i*16+j,(i+1)*16+j+1,0,1,0,0,0));
                }
                // Capture En-passant
                if(valid_move(i+1,j-1) && chess_board[i][j-1] == P && enpassant == (i+1)*16+j-1){
                    possible_moves.add_move(encode_move_to_integer(i*16+j,(i+1)*16+j-1,0,1,1,0,0));
                }
                if(valid_move(i+1,j+1) && chess_board[i][j+1] == P && enpassant == (i+1)*16+j+1){
                    possible_moves.add_move(encode_move_to_integer(i*16+j,(i+1)*16+j+1,0,1,1,0,0));
                }
                // Capture Promotion
                if(i==6 && valid_move(i+1,j-1) && chess_board[i+1][j-1] >= P && chess_board[i+1][j-1] <= K){
                    possible_moves.add_move(encode_move_to_integer(i*16+j,(i+1)*16+j-1,q,1,0,0,0));
                    possible_moves.add_move(encode_move_to_integer(i*16+j,(i+1)*16+j-1,r,1,0,0,0));
                    possible_moves.add_move(encode_move_to_integer(i*16+j,(i+1)*16+j-1,b,1,0,0,0));
                    possible_moves.add_move(encode_move_to_integer(i*16+j,(i+1)*16+j-1,n,1,0,0,0));
                }
                if(i==6 && valid_move(i+1,j+1) && chess_board[i+1][j+1] >= P && chess_board[i+1][j+1] <= K){
                    possible_moves.add_move(encode_move_to_integer(i*16+j,(i+1)*16+j+1,q,1,0,0,0));
                    possible_moves.add_move(encode_move_to_integer(i*16+j,(i+1)*16+j+1,r,1,0,0,0));
                    possible_moves.add_move(encode_move_to_integer(i*16+j,(i+1)*16+j+1,b,1,0,0,0));
                    possible_moves.add_move(encode_move_to_integer(i*16+j,(i+1)*16+j+1,n,1,0,0,0));
                }
            }

            // White Side King Castling
            else if(!side && chess_board[i][j] == K){
                // Check if castling option is available
                // Check for empty squares between king and rook
                // Check squares for king movement are not attacked
                
                // King Side Castling
                if((castle & Kc) && chess_board[7][5] == e && chess_board[7][6] == e && !is_square_attacked(7,4,!side) && !is_square_attacked(7,5,!side) && !is_square_attacked(7,6,!side)){
                    possible_moves.add_move(encode_move_to_integer(i*16+j,7*16+6,0,0,0,1,0));
                }
                // Queen Side Castling
                if((castle & Qc) && chess_board[7][1] == e && chess_board[7][2] == e && chess_board[7][3] == e && !is_square_attacked(7,4,!side) && !is_square_attacked(7,3,!side) && !is_square_attacked(7,2,!side)){
                    possible_moves.add_move(encode_move_to_integer(i*16+j,7*16+2,0,0,0,1,0));
                }
                // Non-Castling King Moves
                for(int ind=0;ind<8;ind++){
                    int tarx = i + king_moves[ind][0];
                    int tary = j + king_moves[ind][1];

                    // Normal Moves
                    if(valid_move(tarx,tary) && !is_square_attacked(tarx,tary,!side) && chess_board[tarx][tary] == e){
                        possible_moves.add_move(encode_move_to_integer(i*16+j,tarx*16+tary,0,0,0,0,0));
                    }
                    // Capture Moves
                    if(valid_move(tarx,tary) && !is_square_attacked(tarx,tary,!side) && chess_board[tarx][tary] >= p && chess_board[tarx][tary] <= k){
                        possible_moves.add_move(encode_move_to_integer(i*16+j,tarx*16+tary,0,1,0,0,0));
                    }
                }
            }

            // Black Side King Castling
            else if(side && chess_board[i][j] == k){
                // King Side Castling
                if((castle & kc) && chess_board[0][5] == e && chess_board[0][6] == e && !is_square_attacked(0,4,!side) && !is_square_attacked(0,5,!side) && !is_square_attacked(0,6,!side)){
                    possible_moves.add_move(encode_move_to_integer(i*16+j,6,0,0,0,1,0));
                }
                // Queen Side Castling
                if((castle & qc) && chess_board[0][1] == e && chess_board[0][2] == e && chess_board[0][3] == e && !is_square_attacked(0,4,!side) && !is_square_attacked(0,3,!side) && !is_square_attacked(0,2,!side)){
                    possible_moves.add_move(encode_move_to_integer(i*16+j,2,0,0,0,1,0));
                }
                // Non-Castling King Moves
                for(int ind=0;ind<8;ind++){
                    int tarx = i + king_moves[ind][0];
                    int tary = j + king_moves[ind][1];

                    // Normal Moves
                    if(valid_move(tarx,tary) && !is_square_attacked(tarx,tary,!side) && chess_board[tarx][tary] == e){
                        possible_moves.add_move(encode_move_to_integer(i*16+j,tarx*16+tary,0,0,0,0,0));
                    }
                    // Capture Moves
                    if(valid_move(tarx,tary) && !is_square_attacked(tarx,tary,!side) && chess_board[tarx][tary] >= P && chess_board[tarx][tary] <= K){
                        possible_moves.add_move(encode_move_to_integer(i*16+j,tarx*16+tary,0,1,0,0,0));
                    }
                }
            }

            // White Knight Moves
            else if(!side && chess_board[i][j] == N){
                for(int ind=0;ind<8;ind++){
                    int tarx = i + knight_moves[ind][0];
                    int tary = j + knight_moves[ind][1];

                    // Normal Moves
                    if(valid_move(tarx,tary) && chess_board[tarx][tary] == e){
                        possible_moves.add_move(encode_move_to_integer(i*16+j,tarx*16+tary,0,0,0,0,0));
                    }
                    // Capture Moves
                    if(valid_move(tarx,tary) && chess_board[tarx][tary] >= p && chess_board[tarx][tary] <= k){
                        possible_moves.add_move(encode_move_to_integer(i*16+j,tarx*16+tary,0,1,0,0,0));
                    }
                }
            }

            // Black Knight Moves
            else if(side && chess_board[i][j] == n){
                for(int ind=0;ind<8;ind++){
                    int tarx = i + knight_moves[ind][0];
                    int tary = j + knight_moves[ind][1];

                    // Normal Moves
                    if(valid_move(tarx,tary) && chess_board[tarx][tary] == e){
                        possible_moves.add_move(encode_move_to_integer(i*16+j,tarx*16+tary,0,0,0,0,0));
                    }
                    // Capture Moves
                    if(valid_move(tarx,tary) && chess_board[tarx][tary] >= P && chess_board[tarx][tary] <= K){
                        possible_moves.add_move(encode_move_to_integer(i*16+j,tarx*16+tary,0,1,0,0,0));
                    }
                }
            }

            // White Bishop & Queen Moves
            if(!side && (chess_board[i][j] == B || chess_board[i][j] == Q)){
                for(int ind=0;ind<4;ind++){
                    int tarx = i + bishop_offsets[ind][0];
                    int tary = j + bishop_offsets[ind][1];
                    while(valid_move(tarx,tary)){
                        // Normal Moves
                        if(chess_board[tarx][tary] == e){
                            possible_moves.add_move(encode_move_to_integer(i*16+j,tarx*16+tary,0,0,0,0,0));
                        }
                        // Capture Moves
                        if(chess_board[tarx][tary] >= p && chess_board[tarx][tary] <= k){
                            possible_moves.add_move(encode_move_to_integer(i*16+j,tarx*16+tary,0,1,0,0,0));
                            break;
                        }
                        // Break if any there is any chess piece in between
                        if(chess_board[tarx][tary] != e) break;

                        tarx = tarx + bishop_offsets[ind][0];
                        tary = tary + bishop_offsets[ind][1];
                    }
                }
            }

            // Black Bishop & Queen Moves
            else if(side && (chess_board[i][j] == b || chess_board[i][j] == q)){
                for(int ind=0;ind<4;ind++){
                    int tarx = i + bishop_offsets[ind][0];
                    int tary = j + bishop_offsets[ind][1];
                    while(valid_move(tarx,tary)){
                        // Normal Moves
                        if(chess_board[tarx][tary] == e){
                            possible_moves.add_move(encode_move_to_integer(i*16+j,tarx*16+tary,0,0,0,0,0));
                        }
                        // Capture Moves
                        if(chess_board[tarx][tary] >= P && chess_board[tarx][tary] <= K){
                            possible_moves.add_move(encode_move_to_integer(i*16+j,tarx*16+tary,0,1,0,0,0));
                            break;
                        }
                        // Break if any there is any chess piece in between
                        if(chess_board[tarx][tary] != e) break;

                        tarx = tarx + bishop_offsets[ind][0];
                        tary = tary + bishop_offsets[ind][1];
                    }
                }
            }

            // White Rook & Queen Moves
            if(!side && (chess_board[i][j] == R || chess_board[i][j] == Q)){
                for(int ind=0;ind<4;ind++){
                    int tarx = i + rook_offsets[ind][0];
                    int tary = j + rook_offsets[ind][1];
                    while(valid_move(tarx,tary)){
                        // Normal Moves
                        if(chess_board[tarx][tary] == e){
                            possible_moves.add_move(encode_move_to_integer(i*16+j,tarx*16+tary,0,0,0,0,0));
                        }
                        // Capture Moves
                        if(chess_board[tarx][tary] >= p && chess_board[tarx][tary] <= k){
                            possible_moves.add_move(encode_move_to_integer(i*16+j,tarx*16+tary,0,1,0,0,0));
                            break;
                        }
                        // Break if any there is any chess piece in between
                        if(chess_board[tarx][tary] != e) break;

                        tarx = tarx + rook_offsets[ind][0];
                        tary = tary + rook_offsets[ind][1];
                    }
                }
            }

            // Black Rook & Queen Moves
            else if(side && (chess_board[i][j] == r || chess_board[i][j] == q)){
                for(int ind=0;ind<4;ind++){
                    int tarx = i + rook_offsets[ind][0];
                    int tary = j + rook_offsets[ind][1];
                    while(valid_move(tarx,tary)){
                        // Normal Moves
                        if(chess_board[tarx][tary] == e){
                            possible_moves.add_move(encode_move_to_integer(i*16+j,tarx*16+tary,0,0,0,0,0));
                        }
                        // Capture Moves
                        if(chess_board[tarx][tary] >= P && chess_board[tarx][tary] <= K){
                            possible_moves.add_move(encode_move_to_integer(i*16+j,tarx*16+tary,0,1,0,0,0));
                            break;
                        }
                        // Break if any there is any chess piece in between
                        if(chess_board[tarx][tary] != e) break;

                        tarx = tarx + rook_offsets[ind][0];
                        tary = tary + rook_offsets[ind][1];
                    }
                }
            }

            // Update king square
            if(chess_board[i][j] == K) king_track[0] = i*16+j;
            if(chess_board[i][j] == k) king_track[1] = i*16+j;

        }
    }
}

// Quiescence Search
int quiescence_search(int side, int depth, int alpha, int beta){
    int eval = evaluate_position();
    if(depth == 0) return eval;

    Moves possible_moves;
    generate_moves(side, possible_moves);

    // Keep an original board copy
    int chess_board_copy[8][16];
    copy(&chess_board[0][0], &chess_board[0][0] + 8*16, &chess_board_copy[0][0]);
    int castle_copy = castle;
    int enpassant_copy = enpassant;
    side_to_move = side;
    int king_track_copy[2];
    copy(&king_track[0], &king_track[0] + 2, &king_track_copy[0]);

    if(!side){ // White to move
        int best_score = eval;

        int count = possible_moves.get_count();
        vector<int> moves = possible_moves.get_all_moves();

        for(int i=0;i<count;i++){
            // Check legal move & also sets the move if legal
            if(!capture_flag(moves[i]) || !set_move(moves[i])){
                copy(&chess_board_copy[0][0], &chess_board_copy[0][0] + 8*16, &chess_board[0][0]);
                castle = castle_copy;
                enpassant = enpassant_copy;
                side_to_move = side;
                copy(&king_track_copy[0], &king_track_copy[0] + 2, &king_track[0]);
                continue;
            }

            // Find best move for the other side and thus the evaluation
            int score = quiescence_search(!side, depth-1, alpha, beta);
            // Currently found a better move, then set it as best move
            if(score > best_score){
                best_score = score;
            }

            // Reset board to original state for next possible move evaluation
            copy(&chess_board_copy[0][0], &chess_board_copy[0][0] + 8*16, &chess_board[0][0]);
            castle = castle_copy;
            enpassant = enpassant_copy;
            side_to_move = side;
            copy(&king_track_copy[0], &king_track_copy[0] + 2, &king_track[0]);
        }

        return best_score;
    }
    else{ // Black to move
        int best_score = eval;

        int count = possible_moves.get_count();
        vector<int> moves = possible_moves.get_all_moves();

        for(int i=0;i<count;i++){
            // Check legal move & also sets the move if legal
            if(!capture_flag(moves[i]) || !set_move(moves[i])){
                copy(&chess_board_copy[0][0], &chess_board_copy[0][0] + 8*16, &chess_board[0][0]);
                castle = castle_copy;
                enpassant = enpassant_copy;
                side_to_move = side;
                copy(&king_track_copy[0], &king_track_copy[0] + 2, &king_track[0]);
                continue;
            }

            // Find best move for the other side and thus the evaluation
            int score = quiescence_search(!side, depth-1, alpha, beta);
            // Currently found a better move, then set it as best move
            if(score < best_score){
                best_score = score;
            }

            // Reset board to original state for next possible move evaluation
            copy(&chess_board_copy[0][0], &chess_board_copy[0][0] + 8*16, &chess_board[0][0]);
            castle = castle_copy;
            enpassant = enpassant_copy;
            side_to_move = side;
            copy(&king_track_copy[0], &king_track_copy[0] + 2, &king_track[0]);
        }

        return best_score;
    }
}


// Best final Move stored globally
int final_best_move = -1;
// Recursion Logic to find best move
int find_best_move(int side, int depth, int alpha, int beta){
    // if(depth == 0) return quiescence_search(side, 3, alpha, beta);
    if(depth == 0) return evaluate_position();

    Moves possible_moves;
    generate_moves(side, possible_moves);

    // Keep an original board copy
    int chess_board_copy[8][16];
    copy(&chess_board[0][0], &chess_board[0][0] + 8*16, &chess_board_copy[0][0]);
    int castle_copy = castle;
    int enpassant_copy = enpassant;
    side_to_move = side;
    int king_track_copy[2];
    copy(&king_track[0], &king_track[0] + 2, &king_track_copy[0]);

    if(!side){ // White to move
        int best_score = INT_MIN;
        int best_move = -1;
        int legal_moves = 0;

        int count = possible_moves.get_count();
        vector<int> moves = possible_moves.get_all_moves();

        for(int i=0;i<count;i++){
            // Check legal move & also sets the move if legal
            if(!set_move(moves[i])){
                copy(&chess_board_copy[0][0], &chess_board_copy[0][0] + 8*16, &chess_board[0][0]);
                castle = castle_copy;
                enpassant = enpassant_copy;
                side_to_move = side;
                copy(&king_track_copy[0], &king_track_copy[0] + 2, &king_track[0]);
                continue;
            }
            legal_moves++;

            // Find best move for the other side and thus the evaluation
            int score = find_best_move(!side, depth-1, alpha, beta);
            // Currently found a better move, then set it as best move
            if(score > best_score){
                best_score = score;
                best_move = moves[i];
            }

            // Alpha Beta Pruning
            alpha = max(alpha, best_score);
            if(alpha >= beta){
                // Reset board to original state for next possible move evaluation
                copy(&chess_board_copy[0][0], &chess_board_copy[0][0] + 8*16, &chess_board[0][0]);
                castle = castle_copy;
                enpassant = enpassant_copy;
                side_to_move = side;
                copy(&king_track_copy[0], &king_track_copy[0] + 2, &king_track[0]);
                break;
            }

            // Reset board to original state for next possible move evaluation
            copy(&chess_board_copy[0][0], &chess_board_copy[0][0] + 8*16, &chess_board[0][0]);
            castle = castle_copy;
            enpassant = enpassant_copy;
            side_to_move = side;
            copy(&king_track_copy[0], &king_track_copy[0] + 2, &king_track[0]);
        }

        if(legal_moves == 0){
            if(is_square_attacked(king_track[0]/16,king_track[0]%16,!side)){
                return -100000 - depth; // Doing it to find quickest mate, else something weird happens and it sometimes find mate in more moves
            }
            else{
                return 0;
            }
        }

        final_best_move = best_move;
        return best_score;
    }
    else{ // Black to move
        int best_score = INT_MAX;
        int best_move = -1;
        int legal_moves = 0;

        int count = possible_moves.get_count();
        vector<int> moves = possible_moves.get_all_moves();

        for(int i=0;i<count;i++){
            // Check legal move & also sets the move if legal
            if(!set_move(moves[i])){
                copy(&chess_board_copy[0][0], &chess_board_copy[0][0] + 8*16, &chess_board[0][0]);
                castle = castle_copy;
                enpassant = enpassant_copy;
                side_to_move = side;
                copy(&king_track_copy[0], &king_track_copy[0] + 2, &king_track[0]);
                continue;
            }
            legal_moves++;

            // Find best move for the other side and thus the evaluation
            int score = find_best_move(!side, depth-1, alpha, beta);
            // Currently found a better move, then set it as best move
            if(score < best_score){
                best_score = score;
                best_move = moves[i];
            }

            // Alpha Beta Pruning
            beta = min(beta, best_score);
            if(alpha >= beta){
                // Reset board to original state for next possible move evaluation
                copy(&chess_board_copy[0][0], &chess_board_copy[0][0] + 8*16, &chess_board[0][0]);
                castle = castle_copy;
                enpassant = enpassant_copy;
                side_to_move = side;
                copy(&king_track_copy[0], &king_track_copy[0] + 2, &king_track[0]);
                break;
            }

            // Reset board to original state for next possible move evaluation
            copy(&chess_board_copy[0][0], &chess_board_copy[0][0] + 8*16, &chess_board[0][0]);
            castle = castle_copy;
            enpassant = enpassant_copy;
            side_to_move = side;
            copy(&king_track_copy[0], &king_track_copy[0] + 2, &king_track[0]);
        }

        if(legal_moves == 0){
            if(is_square_attacked(king_track[1]/16,king_track[1]%16,!side)){
                return 100000 + depth;
            }
            else{
                return 0;
            }
        }

        final_best_move = best_move;
        return best_score;
    }
}

// Performance Test by checking number of valid generated moves
int nodes=0;
// int captures=0;
// int castles=0;
// int promotions=0;
void performance_test(int side, int depth){
    if(depth == 0){
        nodes++;
        return;
    }

    Moves possible_moves;
    generate_moves(side, possible_moves);

    // if(depth == 1) cout<<possible_moves.get_count()<<endl;
    // if(depth == 1) possible_moves.print_all_moves();

    int chess_board_copy[8][16];
    copy(&chess_board[0][0], &chess_board[0][0] + 8*16, &chess_board_copy[0][0]);
    int castle_copy = castle;
    int enpassant_copy = enpassant;
    side_to_move = side;
    int king_track_copy[2];
    copy(&king_track[0], &king_track[0] + 2, &king_track_copy[0]);

    int count = possible_moves.get_count();
    vector<int> moves = possible_moves.get_all_moves();

    for(int i=0;i<count;i++){
        if(!set_move(moves[i])){
            copy(&chess_board_copy[0][0], &chess_board_copy[0][0] + 8*16, &chess_board[0][0]);
            castle = castle_copy;
            enpassant = enpassant_copy;
            side_to_move = side;
            copy(&king_track_copy[0], &king_track_copy[0] + 2, &king_track[0]);
            continue;
        }
        // if(depth == 2){
        //     cout<<"\n\n";
        //     print_decoded_move(moves[i]);
        //     cout<<endl;
        // }
        // if(depth == 1){
        //     print_decoded_move(moves[i]);
        //     cout<<endl;
        // }
        // if(capture_flag(moves[i])) captures++;
        // if(castling_flag(moves[i])) castles++;
        // if(promoted_piece(moves[i]) > 0) promotions++;

        // getchar();
        // print_chess_board();
        
        performance_test(!side, depth-1);
        copy(&chess_board_copy[0][0], &chess_board_copy[0][0] + 8*16, &chess_board[0][0]);
        castle = castle_copy;
        enpassant = enpassant_copy;
        side_to_move = side;
        copy(&king_track_copy[0], &king_track_copy[0] + 2, &king_track[0]);
    }
}

// TODO: Encoding & Decoding chess moves as integers

string test_position = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ";
// string mate_position = "3k4/5Q2/8/4Q3/2K5/8/8/8 w - - ";

int main() {
    // Your code here
    cout << "Initializing Chess Board..... \n\n";
    // chess_board[e2/16][e2%16] = e;
    // chess_board[e4/16][e4%16] = P;
    
    clear_board();
    // parse_fen_string_to_board(random_position);
    // parse_fen_string_to_board(starting_position);
    parse_fen_string_to_board(test_position);
    // parse_fen_string_to_board(mate_position);
    print_chess_board();
    // print_attacked_squares(side_to_move);
    // Moves possible_moves;
    // generate_moves(side_to_move, possible_moves);
    // possible_moves.print_all_moves();

    // cout << "\nScore: " << evaluate_position() << endl;

    int final_best_score = find_best_move(side_to_move, 5, INT_MIN, INT_MAX);
    cout << "\n\nBest Move: ";
    print_decoded_move(final_best_move);
    cout << "\n\nBest Score: " << final_best_score <<endl;

    // performance_test(0,5);
    // cout << "\n\nTotal Nodes: " << nodes << endl;
    // cout << "Total Captures: " << captures << endl;
    // cout << "Total Castles: " << castles << endl;
    // cout << "Total Promotions: " << promotions << endl;


    // moves_history.add_move(encode_move_to_integer(a2,a4,0,0,0,0,1));
    // moves_history.print_all_moves();

    // TODO: Remove this later
    // for(int i=0;i<8;i++){
    //     for(int j=0;j<8;j++){
    //         cout<<char('a'+j)<<8-i<<", ";
    //     }
    //     cout<<endl;
    // }

    return 0;
}
