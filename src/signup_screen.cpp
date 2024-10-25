#include "raylib.h"
#include <cstring>
#include <string>
#include <stdio.h> 
// Thêm thư viện stdio.h để làm việc với file

using namespace std;

const int MAX_INPUT_LENGTH = 32;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

struct UserData {
    char username[MAX_INPUT_LENGTH + 1] = "";
    char password[MAX_INPUT_LENGTH + 1] = "";
};

struct GameState {
    UserData userData;
    bool enteringUsername;
    Texture2D backgroundTexture;
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
    // Load texture directly (no need for image conversion in this case)
    gameState.backgroundTexture = LoadTexture("Picture/phongxanhban.png");
    
    if (gameState.backgroundTexture.id == 0) {
        TraceLog(LOG_ERROR, "Failed to load background texture!");
    }
}

void ShowError(const char* message) {
    gameState.showError = true;
    gameState.errorMessage = message;
    gameState.errorTimer = 2.0f;
}

void SaveUserDataToFile(const UserData& userData) {
    // Mở file ở chế độ append (thêm vào cuối file)
    FILE *file = fopen("infoplayers.txt", "a");
    if (file != NULL) {
        // Ghi thông tin username và password vào file
        fprintf(file, "Username: %s\nPassword: %s\n\n", userData.username, userData.password);
        fclose(file);
    } else {
        ShowError("Could not save user data!");
    }
}

void UpdateSignupScreen() {
    // Update error timer
    if (gameState.showError) {
        gameState.errorTimer -= GetFrameTime();
        if (gameState.errorTimer <= 0) {
            gameState.showError = false;
        }
    }

    // Handle input switching
    if (IsKeyPressed(KEY_TAB)) {
        gameState.enteringUsername = !gameState.enteringUsername;
    }

    int key = GetKeyPressed();
    char* currentField = gameState.enteringUsername ? 
                        gameState.userData.username : 
                        gameState.userData.password;

    // Handle text input
    if (key >= 32 && key <= 126 && strlen(currentField) < MAX_INPUT_LENGTH) {
        currentField[strlen(currentField)] = (char)key;
        currentField[strlen(currentField) + 1] = '\0';
    }

    // Handle backspace
    if (IsKeyPressed(KEY_BACKSPACE) && strlen(currentField) > 0) {
        currentField[strlen(currentField) - 1] = '\0';
    }

    // Handle Enter key or Sign Up button click
    bool trySignUp = IsKeyPressed(KEY_ENTER) || 
                    (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && 
                     CheckCollisionPointRec(GetMousePosition(), 
                     Rectangle{SCREEN_WIDTH/2.0f - 50, 300, 100, 40}));

    if (trySignUp) {
        if (strlen(gameState.userData.username) < 5) {
            ShowError("Username must be at least 5 characters!");
        }
        else if (strlen(gameState.userData.password) < 5) {
            ShowError("Password must be at least 5 characters!");
        }
        else {
            ShowError("Sign-up successful!");

            // Lưu thông tin người chơi vào file sau khi đăng ký thành công
            SaveUserDataToFile(gameState.userData);
        }
    }
}

void DrawSignupScreen() {
    ClearBackground(RAYWHITE);
    
    // Draw background if texture was loaded successfully
    if (gameState.backgroundTexture.id != 0) {
        DrawTexture(gameState.backgroundTexture, 0, 0, WHITE);
    }

    // Draw title
    DrawText("SIGN UP SCREEN", 
             SCREEN_WIDTH/2 - MeasureText("SIGN UP SCREEN", 40)/2, 
             100, 40, BLACK);

    // Draw input fields
    Color usernameColor = gameState.enteringUsername ? BLUE : DARKGRAY;
    Color passwordColor = !gameState.enteringUsername ? BLUE : DARKGRAY;

    DrawText(TextFormat("Username: %s", gameState.userData.username), 
             100, 200, 20, usernameColor);

    string hiddenPassword(strlen(gameState.userData.password), '*');
    DrawText(TextFormat("Password: %s", hiddenPassword.c_str()), 
             100, 250, 20, passwordColor);

    // Draw Sign Up button
    Rectangle btnBounds = {SCREEN_WIDTH/2.0f - 50, 300, 100, 40};
    DrawRectangleRec(btnBounds, BLUE);
    DrawText("Sign Up", 
             btnBounds.x + (btnBounds.width - MeasureText("Sign Up", 20))/2, 
             btnBounds.y + 10, 20, WHITE);

    // Draw error message if active
    if (gameState.showError) {
        DrawText(gameState.errorMessage, 
                SCREEN_WIDTH/2 - MeasureText(gameState.errorMessage, 20)/2, 
                350, 20, RED);
    }

    // Draw input field instructions
    DrawText(gameState.enteringUsername ? "Enter Username" : "Enter Password", 
             SCREEN_WIDTH/2 - 100, 
             400, 20, DARKGRAY);
    DrawText("Press TAB to switch fields", 
             SCREEN_WIDTH/2 - 100, 
             430, 20, DARKGRAY);

             
}

void UnloadSignupScreen() {
    UnloadTexture(gameState.backgroundTexture);
}

int main2() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sign Up Screen");
    InitSignupScreen();
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        UpdateSignupScreen();
        BeginDrawing();
        DrawSignupScreen();
        EndDrawing();
    }

    UnloadSignupScreen();
    CloseWindow();
    return 0;
}
