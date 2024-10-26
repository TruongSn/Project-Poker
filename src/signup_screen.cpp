#include "raylib.h"
#include <cstring>
#include <string>
#include <stdio.h>

using namespace std;

const int MAX_INPUT_LENGTH = 32;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

enum GameScreen {
    SIGNIN_SCREEN,
    SIGNUP_SCREEN,
    // Add other screens if necessary
};

extern GameScreen currentScreen;

struct UserData {
    char username[MAX_INPUT_LENGTH + 1] = "";
    char password[MAX_INPUT_LENGTH + 1] = "";
};

struct GameState {
    UserData userData;
    bool enteringUsername;
    bool showError;
    const char* errorMessage;
    float errorTimer;
};

GameState gameState = {
    .enteringUsername = true,
    .showError = false,
    .errorMessage = "",
    .errorTimer = 0
};

void InitSignupScreen() {
    // Xóa tải hình ảnh nền
}

void ShowError(const char* message) {
    gameState.showError = true;
    gameState.errorMessage = message;
    gameState.errorTimer = 2.0f;
}

void ClearUserDataFields() {
    memset(gameState.userData.username, 0, sizeof(gameState.userData.username));
    memset(gameState.userData.password, 0, sizeof(gameState.userData.password));
}

void SaveUserDataToFile(const UserData& userData) {
    FILE *file = fopen("infoplayers.txt", "a");
    if (file != NULL) {
        fprintf(file, "Username: %s\nPassword: %s\n\n", userData.username, userData.password);
        fclose(file);
    } else {
        ShowError("Could not save user data!");
    }
}

void UpdateSignupScreen() {
    if (gameState.showError) {
        gameState.errorTimer -= GetFrameTime();
        if (gameState.errorTimer <= 0) {
            gameState.showError = false;
        }
    }

    if (IsKeyPressed(KEY_TAB)) {
        gameState.enteringUsername = !gameState.enteringUsername;
    }

    int key = GetKeyPressed();
    char* currentField = gameState.enteringUsername ? gameState.userData.username : gameState.userData.password;

    if (key >= 32 && key <= 126 && strlen(currentField) < MAX_INPUT_LENGTH) {
        currentField[strlen(currentField)] = (char)key;
        currentField[strlen(currentField) + 1] = '\0';
    }

    if (IsKeyPressed(KEY_BACKSPACE) && strlen(currentField) > 0) {
        currentField[strlen(currentField) - 1] = '\0';
    }

    bool trySignUp = IsKeyPressed(KEY_ENTER) ||
                     (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
                      CheckCollisionPointRec(GetMousePosition(), Rectangle{SCREEN_WIDTH / 2.0f - 50, 300, 100, 40}));

    if (trySignUp) {
        if (strlen(gameState.userData.username) < 5) {
            ShowError("Username must be at least 5 characters!");
        } else if (strlen(gameState.userData.password) < 5) {
            ShowError("Password must be at least 5 characters!");
        } else {
            ShowError("Sign-up successful!");
            SaveUserDataToFile(gameState.userData);
            ClearUserDataFields();
        }
    }
}

void DrawSignupScreen() {
    ClearBackground(RAYWHITE);

    // Vẽ nền màu xanh lá cây
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){0, 128, 0, 255}); // Màu xanh lá cây

    DrawText("SIGN UP SCREEN", SCREEN_WIDTH / 2 - MeasureText("SIGN UP SCREEN", 40) / 2, 100, 40, WHITE);

    Color usernameColor = gameState.enteringUsername ? BLUE : DARKGRAY;
    Color passwordColor = !gameState.enteringUsername ? BLUE : DARKGRAY;

    DrawText(TextFormat("Username: %s", gameState.userData.username), 100, 200, 20, usernameColor);

    string hiddenPassword(strlen(gameState.userData.password), '*');
    DrawText(TextFormat("Password: %s", hiddenPassword.c_str()), 100, 250, 20, passwordColor);

    Rectangle btnBounds = {SCREEN_WIDTH / 2.0f - 50, 300, 100, 40};
    DrawRectangleRec(btnBounds, BLUE);
    DrawText("Sign Up", btnBounds.x + (btnBounds.width - MeasureText("Sign Up", 20)) / 2, btnBounds.y + 10, 20, WHITE);

    if (gameState.showError) {
        DrawText(gameState.errorMessage, SCREEN_WIDTH / 2 - MeasureText(gameState.errorMessage, 20) / 2, 350, 20, RED);
    }

    DrawText(gameState.enteringUsername ? "Enter Username" : "Enter Password", SCREEN_WIDTH / 2 - 100, 400, 20, DARKGRAY);
    DrawText("Press TAB to switch fields", SCREEN_WIDTH / 2 - 100, 430, 20, DARKGRAY);

    Rectangle backButton = {10, 10, 100, 40};
    DrawRectangleRec(backButton, LIGHTGRAY);
    DrawText("Back", backButton.x + 10, backButton.y + 10, 20, BLACK);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), backButton)) {
        currentScreen = SIGNIN_SCREEN;  // Quay lại màn hình đăng nhập
    }
}

void UnloadSignupScreen() {
    // Không cần giải phóng texture vì không sử dụng
}

void DrawSignInScreen() {
    // Implement your sign-in screen drawing logic here
    ClearBackground(RAYWHITE);
    DrawText("SIGN IN SCREEN", SCREEN_WIDTH / 2 - MeasureText("SIGN IN SCREEN", 40) / 2, 100, 40, BLACK);
}

int main2() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sign Up Screen");
    currentScreen = SIGNUP_SCREEN; // Initialize to sign-up screen
    InitSignupScreen();
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        if (currentScreen == SIGNUP_SCREEN) {
            UpdateSignupScreen();
            BeginDrawing();
            DrawSignupScreen();
            EndDrawing();
        } else if (currentScreen == SIGNIN_SCREEN) {
            DrawSignInScreen(); // You will need to implement this
        }
    }

    UnloadSignupScreen();
    CloseWindow();
    return 0;
}
