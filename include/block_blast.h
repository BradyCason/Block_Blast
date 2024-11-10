#ifndef _BLOCK_BLAST
#define _BLOCK_BLAST

#define BOARD_WIDTH 8
#define SQUARE_WIDTH 76
#define START_SQUARE_X 698
#define START_SQUARE_Y 331

#define EMPTY_R 32
#define EMPTY_G 43
#define EMPTY_B 82

class BlockBlastGame {
    int board[BOARD_WIDTH][BOARD_WIDTH] = {};
    void set_full(int x, int y);
    void set_empty(int x, int y);
    void place_piece(int piece[BOARD_WIDTH][BOARD_WIDTH]);
    void next_moves();
    bool available_moves() const;

    public:
        void input_board();
        void print_board() const;
        bool play_3_pieces();
};

#endif