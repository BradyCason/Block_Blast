#include "block_blast.h"
#include <iostream>
#include <windows.h>
#include <thread>
#include <chrono>

COLORREF GetPixelColor(int x, int y) {
    // Get a handle to the screen device context
    HDC hdcScreen = GetDC(NULL);

    if (hdcScreen) {
        // Get the color of the pixel at (x, y)
        COLORREF color = GetPixel(hdcScreen, x, y);

        // Release the device context
        ReleaseDC(NULL, hdcScreen);

        return color;
    } else {
        std::cerr << "Failed to get device context." << std::endl;

        return RGB(0,0,0);
    }
}

void clickAndDrag(int startX, int startY, int endX, int endY) {
    // Move to the start position
    SetCursorPos(startX, startY);
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Short delay for smoothness

    // Press the left mouse button down
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Delay to hold the click

    // Move the cursor to the end position
    SetCursorPos(endX, endY);
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Delay for the dragging motion

    // Release the left mouse button
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

void BlockBlastGame::input_board() {
    for (int i = 0; i < BOARD_WIDTH; i++){
        for (int j = 0; j < BOARD_WIDTH; j++){
            COLORREF color = GetPixelColor(START_SQUARE_X + (j * SQUARE_WIDTH), START_SQUARE_Y + (i * SQUARE_WIDTH));
            if ((int)GetRValue(color) == EMPTY_R && (int)GetGValue(color) == EMPTY_G && (int)GetBValue(color) == EMPTY_B){
                board[i][j] = 0;
            }
            else{
                board[i][j] = 1;
            }
        }
    }
}

void BlockBlastGame::print_board() const {
    std::cout << "-------------------\n";
    for (int i = 0; i < BOARD_WIDTH; i++){
        std::cout << "| ";
        for (int j = 0; j < BOARD_WIDTH; j++){
            std::cout << board[i][j] << " ";
        }
        std::cout << "|" << std::endl;
    }
    std::cout << "-------------------\n";
}

void BlockBlastGame::print_piece(int piece[MAX_PIECE_WIDTH][MAX_PIECE_WIDTH]) {
    std::cout << "-------------\n";
    for (int i = 0; i < MAX_PIECE_WIDTH; i++){
        std::cout << "| ";
        for (int j = 0; j < MAX_PIECE_WIDTH; j++){
            std::cout << piece[i][j] << " ";
        }
        std::cout << "|" << std::endl;
    }
    std::cout << "-------------\n";
}

void BlockBlastGame::check_for_clears(){
    int board_copy[BOARD_WIDTH][BOARD_WIDTH] = {};
    for (int i = 0; i < BOARD_WIDTH; i++){
        for (int j = 0; j < BOARD_WIDTH; j++){
            board_copy[i][j] = board[i][j];
        }
    }

    for (int i = 0; i < BOARD_WIDTH; i++){
        if (board[i][0] == 1 && board[i][1] == 1 && board[i][2] == 1 && board[i][3] == 1 && board[i][4] == 1 && board[i][5] == 1 && board[i][6] == 1 && board[i][7] == 1){
            board_copy[i][0] = 0;
            board_copy[i][1] = 0;
            board_copy[i][2] = 0;
            board_copy[i][3] = 0;
            board_copy[i][4] = 0;
            board_copy[i][5] = 0;
            board_copy[i][6] = 0;
            board_copy[i][7] = 0;
        }
        if (board[0][i] == 1 && board[1][i] == 1 && board[2][i] == 1 && board[3][i] == 1 && board[4][i] == 1 && board[5][i] == 1 && board[6][i] == 1 && board[7][i] == 1){
            board_copy[0][i] = 0;
            board_copy[1][i] = 0;
            board_copy[2][i] = 0;
            board_copy[3][i] = 0;
            board_copy[4][i] = 0;
            board_copy[5][i] = 0;
            board_copy[6][i] = 0;
            board_copy[7][i] = 0;
        }
    }

    for (int i = 0; i < BOARD_WIDTH; i++){
        for (int j = 0; j < BOARD_WIDTH; j++){
            board[i][j] = board_copy[i][j];
        }
    }
}

void BlockBlastGame::place_piece(int piece[MAX_PIECE_WIDTH][MAX_PIECE_WIDTH], int x_offset, int y_offset, int center_x, int center_y){
    for (int i = 0; i < MAX_PIECE_WIDTH && i < BOARD_WIDTH; i++){
        for (int j = 0; j < MAX_PIECE_WIDTH && i < BOARD_WIDTH; j++){
            if (i + y_offset >= 0 && j + x_offset >= 0){
                board[i + y_offset][j + x_offset] |= piece[i][j];
            }
        }
    }

    int x_even_offset = 0;
    int y_pos_offset = 0;
    int height = (piece[0][1] || piece[0][2] || piece[0][3]) + 
            (piece[1][1] || piece[1][2] || piece[1][3]) + 
            (piece[2][1] || piece[2][2] || piece[2][3]) + 
            (piece[3][1] || piece[3][2] || piece[3][3]) + 
            (piece[4][1] || piece[4][2] || piece[4][3]);
    if (height == 1) y_pos_offset = 266;
    if (height == 2) y_pos_offset = 304;
    if (height == 3) y_pos_offset = 295;
    if (height == 4) y_pos_offset = 350;
    if (height == 5) y_pos_offset = 340;
    if (((piece[1][0] || piece[2][0] || piece[3][0]) + 
            (piece[1][1] || piece[2][1] || piece[3][1]) + 
            (piece[1][2] || piece[2][2] || piece[3][2]) + 
            (piece[1][3] || piece[2][3] || piece[3][3]) + 
            (piece[1][4] || piece[2][4] || piece[3][4])) % 2 == 0){
        x_even_offset = SQUARE_WIDTH / 2;
    }
    clickAndDrag(center_x, center_y, START_SQUARE_X + (x_offset + 2) * SQUARE_WIDTH + x_even_offset, START_SQUARE_Y + (y_offset) * SQUARE_WIDTH + y_pos_offset);

    check_for_clears();
}

void BlockBlastGame::input_piece(int piece[MAX_PIECE_WIDTH][MAX_PIECE_WIDTH], int center_x, int center_y){
    for (int i = 0; i < MAX_PIECE_WIDTH; i++){
        for (int j = 0; j < MAX_PIECE_WIDTH; j++){
            COLORREF color = GetPixelColor(center_x + ((j-2) * PIECE_WIDTH), center_y + ((i-2) * PIECE_WIDTH));
            int r = GetRValue(color);
            int g = GetGValue(color);
            int b = GetBValue(color);

            if (r >= BACKGROUND_R_MIN && r <= BACKGROUND_R_MAX && g >= BACKGROUND_G_MIN && g <= BACKGROUND_G_MAX && b >= BACKGROUND_B_MIN && b <= BACKGROUND_B_MAX){
                piece[i][j] = 0;
            }
            else{
                piece[i][j] = 1;
            }
        }
    }
}

bool BlockBlastGame::piece_fits(int piece[MAX_PIECE_WIDTH][MAX_PIECE_WIDTH], int x_offset, int y_offset) const{
    for (int i = 0; i < MAX_PIECE_WIDTH; i++){
        for (int j = 0; j < MAX_PIECE_WIDTH; j++){
            if (i + y_offset < 0 || j + x_offset < 0 || i + y_offset >= BOARD_WIDTH || j + x_offset >= BOARD_WIDTH){
                if (piece[i][j] == 1) return false;
            }
            else{
                if (piece[i][j] == 1 && board[i + y_offset][j + x_offset] == 1) return false;
            }
        }
    }
    return true;
}

int BlockBlastGame::evaluate_board(int b[BOARD_WIDTH][BOARD_WIDTH]){
    
}

void BlockBlastGame::place_best_piece(int& piece_used, int piece1[MAX_PIECE_WIDTH][MAX_PIECE_WIDTH], int piece2[MAX_PIECE_WIDTH][MAX_PIECE_WIDTH], int piece3[MAX_PIECE_WIDTH][MAX_PIECE_WIDTH]){
    bool piece_1_empty = false;
    bool piece_2_empty = false;
    bool piece_3_empty = false;
    if (piece1[2][2] == 0 && piece1[1][2] == 0 && piece1[3][2] == 0 && piece1[2][1] == 0 && piece1[2][3] == 0){
        piece_1_empty = true;
    }
    if (piece2[2][2] == 0 && piece2[1][2] == 0 && piece2[3][2] == 0 && piece2[2][1] == 0 && piece2[2][3] == 0){
        piece_2_empty = true;
    }
    if (piece3[2][2] == 0 && piece3[1][2] == 0 && piece3[3][2] == 0 && piece3[2][1] == 0 && piece3[2][3] == 0){
        piece_3_empty = true;
    }
    
    if (!piece_1_empty){
        for (int i = -2; i < BOARD_WIDTH - 2; i++){
            for (int j = -2; j < BOARD_WIDTH - 2; j++){
                if (piece_fits(piece1, i, j)){
                    place_piece(piece1, i, j, PIECE_X_1, PIECE_Y);
                    piece_used = 1;
                    return;
                }
            }
        }
    }
    if (!piece_2_empty){
        for (int i = -2; i < BOARD_WIDTH - 2; i++){
            for (int j = -2; j < BOARD_WIDTH - 2; j++){
                if (piece_fits(piece2, i, j)){
                    place_piece(piece2, i, j, PIECE_X_2, PIECE_Y);
                    piece_used = 2;
                    return;
                }
            }
        }
    }
    if (!piece_3_empty){
        for (int i = -2; i < BOARD_WIDTH - 2; i++){
            for (int j = -2; j < BOARD_WIDTH - 2; j++){
                if (piece_fits(piece3, i, j)){
                    place_piece(piece3, i, j, PIECE_X_3, PIECE_Y);
                    piece_used = 3;
                    return;
                }
            }
        }
    }
}

bool BlockBlastGame::play_3_pieces(){
    int piece1[MAX_PIECE_WIDTH][MAX_PIECE_WIDTH] = {};
    int piece2[MAX_PIECE_WIDTH][MAX_PIECE_WIDTH] = {};
    int piece3[MAX_PIECE_WIDTH][MAX_PIECE_WIDTH] = {};
    input_piece(piece1, PIECE_X_1, PIECE_Y);
    input_piece(piece2, PIECE_X_2, PIECE_Y);
    input_piece(piece3, PIECE_X_3, PIECE_Y);

    int empty[MAX_PIECE_WIDTH][MAX_PIECE_WIDTH] = {};
    int first_piece = 0;
    int second_piece = 0;
    place_best_piece(first_piece, piece1, piece2, piece3);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    if (first_piece == 1) place_best_piece(second_piece, empty, piece2, piece3);
    else if (first_piece == 2) place_best_piece(second_piece, piece1, empty, piece3);
    else if (first_piece == 3) place_best_piece(second_piece, piece1, piece2, empty);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    if (first_piece == 1 && second_piece == 2) place_best_piece(second_piece, empty, empty, piece3);
    else if (first_piece == 2 && second_piece == 3) place_best_piece(second_piece, piece1, empty, empty);
    else if (first_piece == 1 && second_piece == 3) place_best_piece(second_piece, empty, piece2, empty);
    return true;
}