#include "block_blast.h"
#include <iostream>
#include <windows.h>
#include <thread>
#include <chrono>
#include <limits>
#include <cmath>

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
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

bool BlockBlastGame::check_death(){
    COLORREF color = GetPixelColor(DEATH_X, DEATH_Y);
    if ((int)GetRValue(color) == DEATH_R && (int)GetGValue(color) == DEATH_G && (int)GetBValue(color) == DEATH_B){
        has_died = true;
        return false;
    }
    return true;
}

bool BlockBlastGame::dead() const{
    return has_died;
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

void BlockBlastGame::check_for_clears(int b[BOARD_WIDTH][BOARD_WIDTH]){
    int b_copy[BOARD_WIDTH][BOARD_WIDTH] = {};
    for (int i = 0; i < BOARD_WIDTH; i++){
        for (int j = 0; j < BOARD_WIDTH; j++){
            b_copy[i][j] = b[i][j];
        }
    }

    for (int i = 0; i < BOARD_WIDTH; i++){
        if (b[i][0] == 1 && b[i][1] == 1 && b[i][2] == 1 && b[i][3] == 1 && b[i][4] == 1 && b[i][5] == 1 && b[i][6] == 1 && b[i][7] == 1){
            b_copy[i][0] = 0;
            b_copy[i][1] = 0;
            b_copy[i][2] = 0;
            b_copy[i][3] = 0;
            b_copy[i][4] = 0;
            b_copy[i][5] = 0;
            b_copy[i][6] = 0;
            b_copy[i][7] = 0;
        }
        if (b[0][i] == 1 && b[1][i] == 1 && b[2][i] == 1 && b[3][i] == 1 && b[4][i] == 1 && b[5][i] == 1 && b[6][i] == 1 && b[7][i] == 1){
            b_copy[0][i] = 0;
            b_copy[1][i] = 0;
            b_copy[2][i] = 0;
            b_copy[3][i] = 0;
            b_copy[4][i] = 0;
            b_copy[5][i] = 0;
            b_copy[6][i] = 0;
            b_copy[7][i] = 0;
        }
    }

    for (int i = 0; i < BOARD_WIDTH; i++){
        for (int j = 0; j < BOARD_WIDTH; j++){
            b[i][j] = b_copy[i][j];
        }
    }
}

void BlockBlastGame::copy_board(int ret[BOARD_WIDTH][BOARD_WIDTH], int orig[BOARD_WIDTH][BOARD_WIDTH]){
    for (int i = 0; i < BOARD_WIDTH; i++){
        for (int j = 0; j < BOARD_WIDTH; j++){
            ret[i][j] = orig[i][j];
        }
    }
}

void BlockBlastGame::cursor_place_piece(int piece[MAX_PIECE_WIDTH][MAX_PIECE_WIDTH], int x_offset, int y_offset, int center_x, int center_y) const{
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
}

void BlockBlastGame::place_piece(int piece[MAX_PIECE_WIDTH][MAX_PIECE_WIDTH], int b[BOARD_WIDTH][BOARD_WIDTH], int x_offset, int y_offset){
    for (int i = 0; i < MAX_PIECE_WIDTH && i < BOARD_WIDTH; i++){
        for (int j = 0; j < MAX_PIECE_WIDTH && i < BOARD_WIDTH; j++){
            if (i + y_offset >= 0 && j + x_offset >= 0){
                b[i + y_offset][j + x_offset] |= piece[i][j];
            }
        }
    }

    check_for_clears(b);
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

bool BlockBlastGame::piece_fits(int piece[MAX_PIECE_WIDTH][MAX_PIECE_WIDTH], int b[BOARD_WIDTH][BOARD_WIDTH], int x_offset, int y_offset) const{
    for (int i = 0; i < MAX_PIECE_WIDTH; i++){
        for (int j = 0; j < MAX_PIECE_WIDTH; j++){
            if (i + y_offset < 0 || j + x_offset < 0 || i + y_offset >= BOARD_WIDTH || j + x_offset >= BOARD_WIDTH){
                if (piece[i][j] == 1) return false;
            }
            else{
                if (piece[i][j] == 1 && b[i + y_offset][j + x_offset] == 1) return false;
            }
        }
    }
    return true;
}

float BlockBlastGame::evaluate_board(int b[BOARD_WIDTH][BOARD_WIDTH]) const{
    float score = 0;
    for (int i = 0; i < BOARD_WIDTH; i++){
        for (int j = 0; j < BOARD_WIDTH; j++){
            // Subtract for alternating 1 and 0
            // if (i < BOARD_WIDTH - 1 && b[i][j] != b[i+1][j]) score -= 1;
            // if (j < BOARD_WIDTH - 1 && b[i][j] != b[i][j+1]) score -= 1;

            
            if (b[i][j] == 0){
                // Add for every 0 (Clearing lines == Better Score)
                // score += 0.1;

                // Subtract more for each aditional each of hole
                int num_edges = 0;
                if (i > 0) num_edges += b[i-1][j];
                else num_edges += 1;
                if (i < BOARD_WIDTH - 1) num_edges += b[i+1][j];
                else num_edges += 1;
                if (j > 0) num_edges += b[i][j-1];
                else num_edges += 1;
                if (j < BOARD_WIDTH - 1) num_edges += b[i][j+1];
                else num_edges += 1;

                score -= std::pow(num_edges, 2);

                // if (num_edges == 3) score -= 50;
                // if (num_edges == 4) score -= 100;
            }

            // subtract for points
            if (b[i][j] == 1){
                int num_edges = 0;
                if (i > 0) num_edges += !b[i-1][j];
                if (i < BOARD_WIDTH - 1) num_edges += !b[i+1][j];
                if (j > 0) num_edges += !b[i][j-1];
                if (j < BOARD_WIDTH - 1) num_edges += !b[i][j+1];
                score -= 0.5 * std::pow(num_edges, 2);
            }

            // Reward for blocks of the same type
            if (i < BOARD_WIDTH - 1 && b[i][j] == b[i+1][j]) score += 1;
            if (j < BOARD_WIDTH - 1 && b[i][j] == b[i][j + 1]) score += 1;
        }
    }
    return score;
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
    float best_score = -std::numeric_limits<float>::infinity();
    int best_piece;
    int best_x;
    int best_y;
    
    if (!piece_1_empty){
        for (int i = -2; i < BOARD_WIDTH - 2; i++){
            for (int j = -2; j < BOARD_WIDTH - 2; j++){
                if (piece_fits(piece1, board, i, j)){
                    int b[BOARD_WIDTH][BOARD_WIDTH] = {};
                    copy_board(b, board);
                    place_piece(piece1, b, i, j);
                    int score = evaluate_board(b);
                    if (score > best_score){
                        best_score = score;
                        best_x = i;
                        best_y = j;
                        best_piece = 1;
                    }
                }
            }
        }
    }
    if (!piece_2_empty){
        for (int i = -2; i < BOARD_WIDTH - 2; i++){
            for (int j = -2; j < BOARD_WIDTH - 2; j++){
                if (piece_fits(piece2, board, i, j)){
                    int b[BOARD_WIDTH][BOARD_WIDTH] = {};
                    copy_board(b, board);
                    place_piece(piece2, b, i, j);
                    int score = evaluate_board(b);
                    if (score > best_score){
                        best_score = score;
                        best_x = i;
                        best_y = j;
                        best_piece = 2;
                    }
                }
            }
        }
    }
    if (!piece_3_empty){
        for (int i = -2; i < BOARD_WIDTH - 2; i++){
            for (int j = -2; j < BOARD_WIDTH - 2; j++){
                if (piece_fits(piece3, board, i, j)){
                    int b[BOARD_WIDTH][BOARD_WIDTH] = {};
                    copy_board(b, board);
                    place_piece(piece3, b, i, j);
                    int score = evaluate_board(b);
                    if (score > best_score){
                        best_score = score;
                        best_x = i;
                        best_y = j;
                        best_piece = 3;
                    }
                }
            }
        }
    }
    piece_used = best_piece;

    if (best_piece == 1){
        place_piece(piece1, board, best_x, best_y);
        cursor_place_piece(piece1, best_x, best_y, PIECE_X_1, PIECE_Y);
    }
    if (best_piece == 2){
        place_piece(piece2, board, best_x, best_y);
        cursor_place_piece(piece2, best_x, best_y, PIECE_X_2, PIECE_Y);
    }
    if (best_piece == 3){
        place_piece(piece3, board, best_x, best_y);
        cursor_place_piece(piece3, best_x, best_y, PIECE_X_3, PIECE_Y);
    }
}

bool BlockBlastGame::play_3_pieces(){
    int pieces[3][MAX_PIECE_WIDTH][MAX_PIECE_WIDTH] = {};
    int piece_xs[3] = {PIECE_X_1, PIECE_X_2, PIECE_X_3};
    input_piece(pieces[0], PIECE_X_1, PIECE_Y);
    input_piece(pieces[1], PIECE_X_2, PIECE_Y);
    input_piece(pieces[2], PIECE_X_3, PIECE_Y);

    bool piece_empty[3] = {false, false, false};
    for (int i = 0; i < 3; i++){
        if (pieces[i][2][2] == 0 && pieces[i][1][2] == 0 && pieces[i][3][2] == 0 && pieces[i][2][1] == 0 && pieces[i][2][3] == 0){
            piece_empty[i] = true;
        }
    }

    int best_piece1 = 0;
    int best_piece2 = 0;
    int best_piece3 = 0;
    int best_x1;
    int best_y1;
    int best_x2;
    int best_y2;
    int best_x3;
    int best_y3;
    float best_score = -std::numeric_limits<float>::infinity();

    for (int first = 0; first < 3; first++){
        if (!piece_empty[first]){
            for (int x1 = -2; x1 < BOARD_WIDTH - 2; x1++){
                for (int y1 = -2; y1 < BOARD_WIDTH - 2; y1++){
                    if (piece_fits(pieces[first], board, x1, y1)){
                        int board_1[BOARD_WIDTH][BOARD_WIDTH];
                        copy_board(board_1, board);
                        place_piece(pieces[first], board_1, x1, y1);

                        for (int second = 0; second < 3; second++){
                            if (second != first && !piece_empty[second]){
                                for (int x2 = -2; x2 < BOARD_WIDTH - 2; x2++){
                                    for (int y2 = -2; y2 < BOARD_WIDTH - 2; y2++){
                                        if (piece_fits(pieces[second], board_1, x2, y2)){
                                            int board_2[BOARD_WIDTH][BOARD_WIDTH];
                                            copy_board(board_2, board_1);
                                            place_piece(pieces[second], board_2, x2, y2);

                                            for (int third = 0; third < 3; third++){
                                                if (third != first && third != second && !piece_empty[third]){
                                                    for (int x3 = -2; x3 < BOARD_WIDTH - 2; x3++){
                                                        for (int y3 = -2; y3 < BOARD_WIDTH - 2; y3++){
                                                            if (piece_fits(pieces[third], board_2, x3, y3)){
                                                                int board_3[BOARD_WIDTH][BOARD_WIDTH];
                                                                copy_board(board_3, board_2);
                                                                place_piece(pieces[third], board_3, x3, y3);

                                                                float score = evaluate_board(board_3);
                                                                if (score > best_score){
                                                                    best_score = score;
                                                                    best_piece1 = first;
                                                                    best_piece2 = second;
                                                                    best_piece3 = third;
                                                                    best_x1 = x1;
                                                                    best_y1 = y1;
                                                                    best_x2 = x2;
                                                                    best_y2 = y2;
                                                                    best_x3 = x3;
                                                                    best_y3 = y3;
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    if (best_piece1 == 0 && best_piece2 == 0 && best_piece3 == 0){
        input_board();
        int empty[MAX_PIECE_WIDTH][MAX_PIECE_WIDTH] = {};
        int first_piece = 0;
        int second_piece = 0;
        place_best_piece(first_piece, pieces[0], pieces[1], pieces[2]);
        if (!check_death()) return false;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (first_piece == 1) place_best_piece(second_piece, empty, pieces[1], pieces[2]);
        else if (first_piece == 2) place_best_piece(second_piece, pieces[0], empty, pieces[2]);
        else if (first_piece == 3) place_best_piece(second_piece, pieces[0], pieces[1], empty);
        if (!check_death()) return false;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (first_piece == 1 && second_piece == 2) place_best_piece(second_piece, empty, empty, pieces[2]);
        else if (first_piece == 2 && second_piece == 3) place_best_piece(second_piece, pieces[0], empty, empty);
        else if (first_piece == 1 && second_piece == 3) place_best_piece(second_piece, empty, pieces[1], empty);
        check_death();
        return false;
    }
    else{
        place_piece(pieces[best_piece1], board, best_x1, best_y1);
        cursor_place_piece(pieces[best_piece1], best_x1, best_y1, piece_xs[best_piece1], PIECE_Y);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        place_piece(pieces[best_piece2], board, best_x2, best_y2);
        cursor_place_piece(pieces[best_piece2], best_x2, best_y2, piece_xs[best_piece2], PIECE_Y);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        place_piece(pieces[best_piece3], board, best_x3, best_y3);
        cursor_place_piece(pieces[best_piece3], best_x3, best_y3, piece_xs[best_piece3], PIECE_Y);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return true;
}