#include "raylib.h"
#include "screens.h"

// Biến toàn cục cho trạng thái màn hình hiện tại
GameScreen currentScreen = LOGIN_SCREEN;

int main(void) {
    InitWindow(800, 600, "Login System Example");
    // Khởi tạo màn hình đầu tiên
    InitLoginScreen();

    while (!WindowShouldClose()) {
        // Cập nhật màn hình hiện tại
        switch (currentScreen) {
            case LOGIN_SCREEN:
                UpdateLoginScreen();
                break;
            case SIGNUP_SCREEN:
                UpdateSignupScreen();
                break;
            case MAIN_GAME_SCREEN:
                // Giả sử chúng ta vẽ màn hình game chính (chưa có)
                ClearBackground(RAYWHITE);
                DrawText("MAIN GAME SCREEN", 100, 100, 40, BLACK);
                break;
            case LOGOUT_SCREEN:
                // Xử lý logic logout (quay lại màn hình đăng nhập)
                currentScreen = LOGIN_SCREEN;
                break;
        }
        

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Vẽ màn hình hiện tại
        switch (currentScreen) {
            case LOGIN_SCREEN:
                DrawLoginScreen();
                break;
            case SIGNUP_SCREEN:
                DrawSignupScreen();
                break;
            case MAIN_GAME_SCREEN:
                // Chỉ vẽ text "Main Game Screen" tạm thời
                DrawText("MAIN GAME SCREEN", 100, 100, 40, BLACK);
                break;
            default:
                break;
        }

        EndDrawing();
    }

    // Giải phóng tài nguyên khi đóng chương trình
    CloseWindow();

    return 0;
}
