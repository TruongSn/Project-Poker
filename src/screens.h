#ifndef SCREENS_H
#define SCREENS_H

// Định nghĩa các trạng thái màn hình
enum GameScreen {
    LOGIN_SCREEN,
    SIGNUP_SCREEN,
    MAIN_GAME_SCREEN,
    LOGOUT_SCREEN
};

// Khai báo các hàm cho các màn hình
void InitLoginScreen();
void UpdateLoginScreen();
void DrawLoginScreen();
void UnloadLoginScreen();

void InitSignupScreen();
void UpdateSignupScreen();
void DrawSignupScreen();
void UnloadSignupScreen();

void InitMainGameScreen();
void UpdateMainGameScreen();
void DrawMainGameScreen();
void UnloadMainGameScreen();

#endif // SCREENS_H
