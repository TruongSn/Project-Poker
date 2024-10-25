#include "raylib.h"
#include "screens.h"
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

string username = "";
string password = "";
bool loggedIn = false;
bool enteringUsername = true;  // Đang nhập vào trường username hoặc password

extern GameScreen currentScreen;  // Biến toàn cục cho trạng thái màn hình

// Hàm kiểm tra thông tin đăng nhập từ file
bool CheckLogin(const string& inputUsername, const string& inputPassword) {
    ifstream file("informationplayer.txt");
    if (!file.is_open()) {
        cout << "Could not open file!" << endl;
        return false;
    }

    string fileUsername, filePassword;
    while (file >> fileUsername >> filePassword) {  // Đọc lần lượt từng username và password
        if (inputUsername == fileUsername && inputPassword == filePassword) {
            file.close();
            return true;  // Đăng nhập thành công nếu khớp
        }
    }

    file.close();
    return false;  // Không tìm thấy thông tin đăng nhập phù hợp
}

// Hàm khởi tạo màn hình login
void InitLoginScreen() {
    username = "";
    password = "";
    loggedIn = false;
    enteringUsername = true;  // Mặc định nhập vào username trước
}

// Hàm cập nhật logic màn hình login
void UpdateLoginScreen() {
    if (IsKeyPressed(KEY_TAB)) {
        enteringUsername = !enteringUsername;  // Chuyển giữa username và password khi bấm TAB
    }

    int key = GetKeyPressed();
    
    // Nhập thông tin từ bàn phím
    if (key >= 32 && key <= 126) {  // Ký tự có thể in ra
        if (enteringUsername) {
            username += (char)key;
        } else {
            password += (char)key;
        }
    }

    // Xử lý phím backspace
    if (IsKeyPressed(KEY_BACKSPACE)) {
        if (enteringUsername && !username.empty()) {
            username.pop_back();
        } else if (!enteringUsername && !password.empty()) {
            password.pop_back();
        }
    }

    // Xác thực thông tin đăng nhập khi bấm Enter
    if (IsKeyPressed(KEY_ENTER)) {
        if (CheckLogin(username, password)) {
            loggedIn = true;
            currentScreen = MAIN_GAME_SCREEN;  // Đăng nhập thành công
        } else {
            // Hiển thị lỗi nếu đăng nhập thất bại
            cout << "Login failed! Incorrect username or password." << endl;
        }
    }

    // Chuyển sang màn hình đăng ký khi bấm nút Sign Up
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        // Giả sử nút Sign Up ở vị trí (100, 300) kích thước 100x50
        if (CheckCollisionPointRec(GetMousePosition(), {100, 300, 100, 50})) {
            currentScreen = SIGNUP_SCREEN;
        }
    }
}

// Hàm vẽ màn hình login
void DrawLoginScreen() {
    ClearBackground(RAYWHITE);

    DrawText("LOGIN SCREEN", 100, 100, 40, BLACK);

    // Màu sắc cho trường đang nhập liệu
    Color usernameColor = enteringUsername ? BLUE : DARKGRAY;
    Color passwordColor = !enteringUsername ? BLUE : DARKGRAY;

    // Vẽ phần nhập username
    DrawText(TextFormat("Username: %s", username.c_str()), 100, 200, 20, usernameColor);

    // Vẽ phần nhập password (dấu '*')
    string hiddenPassword(password.length(), '*');  // Chuyển password thành dấu '*'
    DrawText(TextFormat("Password: %s", hiddenPassword.c_str()), 100, 250, 20, passwordColor);

    // Vẽ nút Sign Up
    DrawRectangle(100, 300, 100, 50, LIGHTGRAY);
    DrawText("Sign Up", 120, 315, 20, BLACK);

    // Hướng dẫn người chơi chuyển đổi giữa username và password
    DrawText("Press TAB to switch between Username and Password", 100, 350, 20, DARKGRAY);

    // Vẽ nút "Back" và xử lý nút này
    Rectangle backButton = { 10, 10, 100, 40 };
    DrawRectangleRec(backButton, LIGHTGRAY);
    DrawText("Back", backButton.x + 10, backButton.y + 10, 20, BLACK);

    // Xử lý sự kiện khi nhấn nút "Back"
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), backButton)) {
        currentScreen = SIGNUP_SCREEN;  // Quay lại màn hình đăng ký
    }
}

// Hàm giải phóng tài nguyên màn hình login (nếu có)
void UnloadLoginScreen() {
    // Không cần làm gì đặc biệt ở đây
}
