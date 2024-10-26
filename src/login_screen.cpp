#include "raylib.h"
#include "screens.h"
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm> // Thêm thư viện để sử dụng transform
// check coi submit chưa
using namespace std;

string username = "";
string password = "";
bool loggedIn = false;
bool enteringUsername = true;  // Biến bool dùng để đăng nhập vào trường username và password
bool loginSuccess = false;      // Biến theo dõi trạng thái đăng nhập thành công
bool loginFailed = false;       // Biến theo dõi trạng thái đăng nhập thất bại

extern GameScreen currentScreen;  

// Hàm kiểm tra thông tin đăng nhập từ file text
bool CheckLogin(const string& inputUsername, const string& inputPassword) {
    ifstream file("infoplayers.txt"); // Đảm bảo tên file là chính xác
    if (!file.is_open()) {
        cout << "Could not open file!" << endl;
        return false;
    }

    string fileUsername, filePassword;
    while (file >> fileUsername >> filePassword) {
        // Chuyển đổi thành chữ thường
        string lowerFileUsername = fileUsername;
        string lowerFilePassword = filePassword;
        string lowerInputUsername = inputUsername;
        string lowerInputPassword = inputPassword;

        transform(lowerFileUsername.begin(), lowerFileUsername.end(), lowerFileUsername.begin(), ::tolower);
        transform(lowerFilePassword.begin(), lowerFilePassword.end(), lowerFilePassword.begin(), ::tolower);
        transform(lowerInputUsername.begin(), lowerInputUsername.end(), lowerInputUsername.begin(), ::tolower);
        transform(lowerInputPassword.begin(), lowerInputPassword.end(), lowerInputPassword.begin(), ::tolower);

        // Thông báo debug
        cout << "Comparing: " << lowerInputUsername << " with " << lowerFileUsername << endl;
        cout << "Comparing: " << lowerInputPassword << " with " << lowerFilePassword << endl;

        if (lowerInputUsername == lowerFileUsername && lowerInputPassword == lowerFilePassword) {
            file.close();
            return true;  // Đăng nhập thành công
        }
    }

    file.close();
    return false;  
}

// Hàm khởi tạo màn hình login
void InitLoginScreen() {
    username = "";
    password = "";
    loggedIn = false;
    enteringUsername = true;     
    loginSuccess = false;        // Đặt lại trạng thái đăng nhập thành công
    loginFailed = false;         // Đặt lại trạng thái đăng nhập thất bại
}

// Hàm cập nhật logic màn hình login
void UpdateLoginScreen() {
    if (IsKeyPressed(KEY_TAB)) {
        enteringUsername = !enteringUsername;  // Chuyển giữa username và password khi bấm TAB
    }

    int key = GetKeyPressed();

    // Nhập thông tin từ bàn phím
    if (key >= 32 && key <= 126) {  
        if (enteringUsername) {
            username += (char)key;
        } else {
            password += (char)key;
        }
    }

    // Xử lý khi có phím backspace
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
            loginSuccess = true;  // Đăng nhập thành công
            currentScreen = MAIN_GAME_SCREEN;  // Chuyển đến màn hình chính
        } else {
            loginFailed = true;  // Đăng nhập thất bại
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

    // Xử lý nút Login
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        // Giả sử nút Login ở vị trí (100, 360) kích thước 100x50
        if (CheckCollisionPointRec(GetMousePosition(), {100, 360, 100, 50})) {
            if (CheckLogin(username, password)) {
                loggedIn = true;
                loginSuccess = true;  // Đăng nhập thành công
                currentScreen = MAIN_GAME_SCREEN;  // Chuyển đến màn hình chính
            } else {
                loginFailed = true;  // Đăng nhập thất bại
                cout << "Login failed! Incorrect username or password." << endl;
            }
        }
    }
}

// Hàm vẽ màn hình login
void DrawLoginScreen() {
    ClearBackground(BLACK);  // Đặt nền màn hình thành màu đen

    // Tính toán vị trí để căn giữa chữ "LOGIN SCREEN"
    const char* title = "LOGIN SCREEN";
    int textWidth = MeasureText(title, 40); // Đo chiều rộng của văn bản
    int x = (GetScreenWidth() - textWidth) / 2; // Tính toán tọa độ x để căn giữa

    DrawText(title, x, 100, 40, WHITE);  // Văn bản tiêu đề màu trắng

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

    // Vẽ nút Login
    DrawRectangle(100, 360, 100, 50, LIGHTGRAY);
    DrawText("Login", 120, 375, 20, BLACK);

    // Hướng dẫn người chơi chuyển đổi giữa username và password
    DrawText("Press TAB to switch between Username and Password", 100, 420, 20, DARKGRAY);

    // Vẽ nút "Back" và xử lý nút này
    Rectangle backButton = { 10, 10, 100, 40 };
    DrawRectangleRec(backButton, LIGHTGRAY);
    DrawText("Back", backButton.x + 10, backButton.y + 10, 20, BLACK);

    // Xử lý sự kiện khi nhấn nút "Back"
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), backButton)) {
        currentScreen = SIGNUP_SCREEN;  // Quay lại màn hình đăng ký
    }

    // Thông báo khi đăng nhập thành công
    if (loginSuccess) {
        DrawText("Login successful!", 100, 460, 20, GREEN);  // Thông báo màu xanh
    }

    // Thông báo khi đăng nhập không thành công
    if (loginFailed) {
        DrawText("Login failed! Incorrect username or password.", 100, 460, 20, RED);  // Thông báo màu đỏ
    }
}

// Hàm giải phóng tài nguyên màn hình login (nếu có)
void UnloadLoginScreen() {
    // Không cần làm gì đặc biệt ở đây
}