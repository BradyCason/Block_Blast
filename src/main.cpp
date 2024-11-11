#include "block_blast.h"

#include <iostream>
#include <windows.h>
#include <thread>
#include <chrono>

int main(){
    BlockBlastGame game = BlockBlastGame();
    game.input_board();
    while(true){
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        game.play_3_pieces();
        // game.print_board();
    }
    // while (game.play_3_pieces()){
    //     game.print_board();
    // }
    // while (true){
    //     COLORREF color = GetPixelColorAtCursor();

    //     int red = GetRValue(color);
    //     int green = GetGValue(color);
    //     int blue = GetBValue(color);

    //     std::cout << "Pixel color at cursor: "
    //             << "R=" << red << " G=" << green << " B=" << blue << std::endl;
    // }

    // POINT cursorPos;
    // if (GetCursorPos(&cursorPos)) {
    //     // Capture a 100x100 region around the cursor
    //     if (CaptureScreenRegion(738 - 98, 1043 - 98, 196, 196, "screenshot1.bmp")) {
    //         std::cout << "Screenshot saved as screenshot1.bmp" << std::endl;
    //     } else {
    //         std::cerr << "Failed to capture screenshot." << std::endl;
    //     }
    //     if (CaptureScreenRegion(972 - 98, 1043 - 98, 196, 196, "screenshot2.bmp")) {
    //         std::cout << "Screenshot saved as screenshot2.bmp" << std::endl;
    //     } else {
    //         std::cerr << "Failed to capture screenshot." << std::endl;
    //     }
    //     if (CaptureScreenRegion(1197 - 98, 1043 - 98, 196, 196, "screenshot3.bmp")) {
    //         std::cout << "Screenshot saved as screenshot3.bmp" << std::endl;
    //     } else {
    //         std::cerr << "Failed to capture screenshot." << std::endl;
    //     }
    // } else {
    //     std::cerr << "Failed to get cursor position." << std::endl;
    // }

    return 0;
}