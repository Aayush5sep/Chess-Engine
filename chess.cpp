#include <iostream>
#include <string>

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

// Generate Moves
void generate_moves(int side){
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){

            // White Pawn Moves
            if(!side && chess_board[i][j] == P){
                // Single Move
                if(valid_move(i-1,j) && chess_board[i-1][j] == e){
                    // Promotion
                    if(i == 1){
                        cout << index_to_position[i][j] << " -> " << index_to_position[i-1][j] << " Q" << endl;
                        cout << index_to_position[i][j] << " -> " << index_to_position[i-1][j] << " R" << endl;
                        cout << index_to_position[i][j] << " -> " << index_to_position[i-1][j] << " B" << endl;
                        cout << index_to_position[i][j] << " -> " << index_to_position[i-1][j] << " N" << endl;
                    }
                    // Normal Move
                    else{
                        cout << index_to_position[i][j] << " -> " << index_to_position[i-1][j] << endl;
                    }
                }

                // Double Move
                if(i == 6 && chess_board[i-1][j] == e && chess_board[i-2][j] == e){
                    cout << index_to_position[i][j] << " -> " << index_to_position[i-2][j] << endl;
                }

                // Capture Move
                // Normal Captures
                if(i>1 && valid_move(i-1,j-1) && chess_board[i-1][j-1] >= p && chess_board[i-1][j-1] <= k){
                    cout << index_to_position[i][j] << " takes " << index_to_position[i-1][j-1] << endl;
                }
                if(i>1 && valid_move(i-1,j+1) && chess_board[i-1][j+1] >= p && chess_board[i-1][j+1] <= k){
                    cout << index_to_position[i][j] << " takes " << index_to_position[i-1][j+1] << endl;
                }
                // Capture En-passant
                if(valid_move(i-1,j-1) && chess_board[i][j-1] == p && enpassant == i*16+j-1){
                    cout << index_to_position[i][j] << " takes " << index_to_position[i-1][j-1] << " En-passant" << endl;
                }
                if(valid_move(i-1,j+1) && chess_board[i][j+1] == p && enpassant == i*16+j+1){
                    cout << index_to_position[i][j] << " takes " << index_to_position[i-1][j+1] << " En-passant" << endl;
                }
                // Capture Promotion
                if(i==1 && valid_move(i-1,j-1) && chess_board[i-1][j-1] >= p && chess_board[i-1][j-1] <= k){
                    cout << index_to_position[i][j] << " takes " << index_to_position[i-1][j-1] << " Q" << endl;
                    cout << index_to_position[i][j] << " takes " << index_to_position[i-1][j-1] << " R" << endl;
                    cout << index_to_position[i][j] << " takes " << index_to_position[i-1][j-1] << " B" << endl;
                    cout << index_to_position[i][j] << " takes " << index_to_position[i-1][j-1] << " N" << endl;
                }
                if(i==1 && valid_move(i-1,j+1) && chess_board[i-1][j+1] >= p && chess_board[i-1][j+1] <= k){
                    cout << index_to_position[i][j] << " takes " << index_to_position[i-1][j+1] << " Q" << endl;
                    cout << index_to_position[i][j] << " takes " << index_to_position[i-1][j+1] << " R" << endl;
                    cout << index_to_position[i][j] << " takes " << index_to_position[i-1][j+1] << " B" << endl;
                    cout << index_to_position[i][j] << " takes " << index_to_position[i-1][j+1] << " N" << endl;
                }
            }

            // Black Pawn Moves
            else if(side && chess_board[i][j] == p){
                // Single Move
                if(valid_move(i+1,j) && chess_board[i+1][j] == e){
                    // Promotion
                    if(i == 6){
                        cout << index_to_position[i][j] << " -> " << index_to_position[i+1][j] << " q" << endl;
                        cout << index_to_position[i][j] << " -> " << index_to_position[i+1][j] << " r" << endl;
                        cout << index_to_position[i][j] << " -> " << index_to_position[i+1][j] << " b" << endl;
                        cout << index_to_position[i][j] << " -> " << index_to_position[i+1][j] << " n" << endl;
                    }
                    else{
                        cout << index_to_position[i][j] << " -> " << index_to_position[i+1][j] << endl;
                    }
                }

                // Double Move
                if(i == 1 && chess_board[i+1][j] == e && chess_board[i+2][j] == e){
                    cout << index_to_position[i][j] << " -> " << index_to_position[i+2][j] << endl;
                }

                // Capture Move
                // Normal Captures
                if(i<6 && valid_move(i+1,j-1) && chess_board[i+1][j-1] >= P && chess_board[i+1][j-1] <= K){
                    cout << index_to_position[i][j] << " takes " << index_to_position[i+1][j-1] << endl;
                }
                if(i<6 && valid_move(i+1,j+1) && chess_board[i+1][j+1] >= P && chess_board[i+1][j+1] <= K){
                    cout << index_to_position[i][j] << " takes " << index_to_position[i+1][j+1] << endl;
                }
                // Capture En-passant
                if(valid_move(i+1,j-1) && chess_board[i][j-1] == p && enpassant == i*16+j-1){
                    cout << index_to_position[i][j] << " takes " << index_to_position[i+1][j-1] << " En-passant" << endl;
                }
                if(valid_move(i+1,j+1) && chess_board[i][j+1] == p && enpassant == i*16+j+1){
                    cout << index_to_position[i][j] << " takes " << index_to_position[i+1][j+1] << " En-passant" << endl;
                }
                // Capture Promotion
                if(i==6 && valid_move(i+1,j-1) && chess_board[i+1][j-1] >= p && chess_board[i+1][j-1] <= k){
                    cout << index_to_position[i][j] << " takes " << index_to_position[i+1][j-1] << " q" << endl;
                    cout << index_to_position[i][j] << " takes " << index_to_position[i+1][j-1] << " r" << endl;
                    cout << index_to_position[i][j] << " takes " << index_to_position[i+1][j-1] << " b" << endl;
                    cout << index_to_position[i][j] << " takes " << index_to_position[i+1][j-1] << " n" << endl;
                }
                if(i==6 && valid_move(i+1,j+1) && chess_board[i+1][j+1] >= p && chess_board[i+1][j+1] <= k){
                    cout << index_to_position[i][j] << " takes " << index_to_position[i+1][j+1] << " q" << endl;
                    cout << index_to_position[i][j] << " takes " << index_to_position[i+1][j+1] << " r" << endl;
                    cout << index_to_position[i][j] << " takes " << index_to_position[i+1][j+1] << " b" << endl;
                    cout << index_to_position[i][j] << " takes " << index_to_position[i+1][j+1] << " n" << endl;
                }
            }

            // White Side King Castling
            else if(!side && chess_board[i][j] == K){
                // Check if castling option is available
                // Check for empty squares between king and rook
                // Check squares for king movement are not attacked
                
                // King Side Castling
                if((castle & Kc) && chess_board[7][5] == e && chess_board[7][6] == e && !is_square_attacked(7,4,!side) && !is_square_attacked(7,5,!side) && !is_square_attacked(7,6,!side)){
                    cout << index_to_position[i][j] << " -> " << index_to_position[7][6] << endl;
                }
                // Queen Side Castling
                if((castle & Qc) && chess_board[7][1] == e && chess_board[7][2] == e && chess_board[7][3] == e && !is_square_attacked(7,4,!side) && !is_square_attacked(7,3,!side) && !is_square_attacked(7,2,!side)){
                    cout << index_to_position[i][j] << " -> " << index_to_position[7][2] << endl;
                }
            }

            // Black Side King Castling
            else if(side && chess_board[i][j] == k){
                // King Side Castling
                if((castle & kc) && chess_board[0][5] == e && chess_board[0][6] == e && !is_square_attacked(0,4,side) && !is_square_attacked(0,5,side) && !is_square_attacked(0,6,side)){
                    cout << index_to_position[i][j] << " -> " << index_to_position[0][6] << endl;
                }
                // Queen Side Castling
                if((castle & qc) && chess_board[0][1] == e && chess_board[0][2] == e && chess_board[0][3] == e && !is_square_attacked(0,4,side) && !is_square_attacked(0,3,side) && !is_square_attacked(0,2,side)){
                    cout << index_to_position[i][j] << " -> " << index_to_position[0][2] << endl;
                }
            }

        }
    }
}

string test_position = "8/8/8/8/8/8/8/8 w KQkq - 0 1";

int main() {
    // Your code here
    cout << "Initializing Chess Board..... \n\n";
    // chess_board[e2/16][e2%16] = e;
    // chess_board[e4/16][e4%16] = P;
    
    clear_board();
    parse_fen_string_to_board(random_position);
    print_chess_board();
    // print_attacked_squares(side_to_move);
    generate_moves(side_to_move);

    // TODO: Remove this later
    // for(int i=0;i<8;i++){
    //     for(int j=0;j<8;j++){
    //         cout<<char('a'+j)<<8-i<<", ";
    //     }
    //     cout<<endl;
    // }

    return 0;
}
