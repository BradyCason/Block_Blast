#include "block_blast.h"

#include <iostream>
#include <windows.h>
#include <thread>
#include <chrono>

int main(){
    BlockBlastGame game = BlockBlastGame();
    game.input_board();
    while(!game.dead()){
        if (!game.play_3_pieces()){
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            game.input_board();
        }
    }

    return 0;
}