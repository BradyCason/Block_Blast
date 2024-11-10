#include "block_blast.h"

#include <iostream>
#include <windows.h>
#include <thread>
#include <chrono>

COLORREF GetPixelColorAtCursor() {
    // Get cursor position
    POINT cursorPos;
    if (!GetCursorPos(&cursorPos)) {
        std::cerr << "Failed to get cursor position." << std::endl;
        return RGB(0, 0, 0); // Return black if unable to get position
    }

    std::cout << cursorPos.x << ", " << cursorPos.y << std::endl;

    // Get the device context for the entire screen
    HDC hdc = GetDC(NULL);
    if (hdc == NULL) {
        std::cerr << "Failed to get device context." << std::endl;
        return RGB(0, 0, 0);
    }

    // Retrieve the color of the pixel at the cursor position
    ShowCursor(FALSE);
    COLORREF color = GetPixel(hdc, cursorPos.x, cursorPos.y);
    if (color == CLR_INVALID) {
        std::cerr << "Failed to get pixel color at (" << cursorPos.x << ", " << cursorPos.y << ")." << std::endl;
    }

    // Release the device context
    ReleaseDC(NULL, hdc);

    return color;
}

bool CaptureScreenRegion(int x, int y, int width, int height, const char* filename) {
    // Get the device context of the screen
    HDC hdcScreen = GetDC(NULL);
    if (!hdcScreen) {
        std::cerr << "Failed to get device context." << std::endl;
        return false;
    }

    // Create a compatible device context and a compatible bitmap
    HDC hdcMemory = CreateCompatibleDC(hdcScreen);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, width, height);
    SelectObject(hdcMemory, hBitmap);

    // Copy the screen area into the memory device context
    if (!BitBlt(hdcMemory, 0, 0, width, height, hdcScreen, x, y, SRCCOPY)) {
        std::cerr << "Failed to capture screen region." << std::endl;
        DeleteObject(hBitmap);
        DeleteDC(hdcMemory);
        ReleaseDC(NULL, hdcScreen);
        return false;
    }

    // Save the captured bitmap to a file
    BITMAPFILEHEADER fileHeader = { 0 };
    BITMAPINFOHEADER infoHeader = { 0 };

    fileHeader.bfType = 0x4D42;  // "BM"
    fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    fileHeader.bfSize = fileHeader.bfOffBits + width * height * 4;

    infoHeader.biSize = sizeof(BITMAPINFOHEADER);
    infoHeader.biWidth = width;
    infoHeader.biHeight = -height;  // Negative height for top-down bitmap
    infoHeader.biPlanes = 1;
    infoHeader.biBitCount = 32;
    infoHeader.biCompression = BI_RGB;

    FILE* file = fopen(filename, "wb");
    if (file) {
        fwrite(&fileHeader, sizeof(fileHeader), 1, file);
        fwrite(&infoHeader, sizeof(infoHeader), 1, file);

        // Write bitmap data
        BYTE* pPixels = new BYTE[width * height * 4];
        GetDIBits(hdcMemory, hBitmap, 0, height, pPixels, (BITMAPINFO*)&infoHeader, DIB_RGB_COLORS);
        fwrite(pPixels, width * height * 4, 1, file);

        fclose(file);
        delete[] pPixels;
    } else {
        std::cerr << "Failed to open file for writing." << std::endl;
    }

    // Cleanup
    DeleteObject(hBitmap);
    DeleteDC(hdcMemory);
    ReleaseDC(NULL, hdcScreen);

    return true;
}

int main(){
    BlockBlastGame game = BlockBlastGame();
    game.input_board();
    game.print_board();
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
    //     if (CaptureScreenRegion(698 + 78 * 7 - 50, 331 + 78 * 0 + - 50, 100, 100, "screenshot.bmp")) {
    //         std::cout << "Screenshot saved as screenshot.bmp" << std::endl;
    //     } else {
    //         std::cerr << "Failed to capture screenshot." << std::endl;
    //     }
    // } else {
    //     std::cerr << "Failed to get cursor position." << std::endl;
    // }

    return 0;
}