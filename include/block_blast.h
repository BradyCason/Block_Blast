#ifndef _BLOCK_BLAST
#define _BLOCK_BLAST

#define BOARD_WIDTH 8
#define SQUARE_WIDTH 76
#define START_SQUARE_X 698
#define START_SQUARE_Y 331

#define MAX_PIECE_WIDTH 5
#define PIECE_WIDTH 39
#define PIECE_Y 1038
#define PIECE_X_1 733
#define PIECE_X_2 967
#define PIECE_X_3 1192

#define EMPTY_R 32
#define EMPTY_G 43
#define EMPTY_B 82
#define BACKGROUND_R_MIN 48
#define BACKGROUND_R_MAX 57
#define BACKGROUND_G_MIN 76
#define BACKGROUND_G_MAX 86
#define BACKGROUND_B_MIN 142
#define BACKGROUND_B_MAX 150

class BlockBlastGame {
    int board[BOARD_WIDTH][BOARD_WIDTH] = {};
    void check_for_clears();
    int evaluate_board(int b[BOARD_WIDTH][BOARD_WIDTH]);
    void place_piece(int piece[MAX_PIECE_WIDTH][MAX_PIECE_WIDTH], int x_offset, int y_offset, int center_x, int center_y);
    void input_piece(int piece[MAX_PIECE_WIDTH][MAX_PIECE_WIDTH], int center_x, int center_y);
    bool piece_fits(int piece[MAX_PIECE_WIDTH][MAX_PIECE_WIDTH], int x_offset, int y_offset) const;
    void place_best_piece(int& piece_used, int piece1[MAX_PIECE_WIDTH][MAX_PIECE_WIDTH], int piece2[MAX_PIECE_WIDTH][MAX_PIECE_WIDTH], int piece3[MAX_PIECE_WIDTH][MAX_PIECE_WIDTH]);

    public:
        void input_board();
        void print_board() const;
        void print_piece(int piece[MAX_PIECE_WIDTH][MAX_PIECE_WIDTH]);
        bool play_3_pieces();
};

#endif