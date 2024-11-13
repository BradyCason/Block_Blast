#include "block_blast.h"

#include <iostream>
#include <windows.h>
#include <thread>
#include <chrono>

int main(){
    BlockBlastGame game = BlockBlastGame();
    while(true){
        game.input_board();
        while(!game.dead() && !(GetAsyncKeyState(VK_SPACE) & 0x8000)){
            if (!game.play_3_pieces()){
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                game.check_death();
                game.input_board();
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        game.restart();
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }

    return 0;
}