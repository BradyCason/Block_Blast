#include "block_blast.h"

#include <iostream>
#include <windows.h>
#include <thread>
#include <chrono>

int main(){
    BlockBlastGame game = BlockBlastGame();
    game.input_board();
    while(!game.dead()){
        game.play_3_pieces();
    }

    return 0;
}