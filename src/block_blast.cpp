#include "block_blast.h"
#include <iostream>
#include <windows.h>

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

void BlockBlastGame::set_empty(int x, int y){
    board[x][y] = 0;
}

void BlockBlastGame::set_full(int x, int y){
    board[x][y] = 1;
}

void BlockBlastGame::place_piece(int piece[BOARD_WIDTH][BOARD_WIDTH]){

}

bool BlockBlastGame::available_moves() const{
    return false;
}

bool BlockBlastGame::play_3_pieces(){
    return false;
}