# Block_Blast
Auto Block Blast Bot

# How To Use
- Go to https://blockblastonline.com/, expand it to full screen, and get to a point with 3 pieces on the bottom.
- Update the following variables in `include/block_blast.h` to match the position of pixels on your screen (May need to update the RGB values of pixels as well):
    - SQUARE_WIDTH
    - START_SQUARE_X
    - START_SQUARE_Y
    - PIECE_WIDTH
    - PIECE_Y
    - PIECE_X_1
    - PIECE_X_2
    - PIECE_X_3
    - DEATH_X
    - DEATH_Y
- Run the following Commands in the directory of the file:
```
make
./main
```