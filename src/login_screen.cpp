#include "raylib.h"
#include "screens.h"
#include <string>
#include <fstream>
// đọc file
#include <iostream>
using namespace std;

string username = "";
string password = "";
bool loggedIn = false;
bool enteringUsername = true;  // lệnh bool dùng để đăng nhập vào trường username và password

extern GameScreen currentScreen;  
// Biến toàn cục cho trạng thái toàn màn hình, khai báo rồi có thể sử dụng nó ở các file khác mà không cần phải khai báo lại

// Hàm kiểm tra thông tin đăng nhập từ file text tui tạo
bool CheckLogin(const string& inputUsername, const string& inputPassword) {
    ifstream file("informationplayer.txt");
    // hàm này dùng để nhập thông tin vào 
    if (!file.is_open()) {
        // file.is_open(): hàm này cho biết file mở lên được
        // !file.is_open() : hàm này cho biết file không thể mở lên được
        cout << "Could not open file!" << endl;
        return false;
    }

    string fileUsername, filePassword;
    while (file >> fileUsername >> filePassword) {  
        // đọc lần lượt từng username và password để check thông tin đầu vào khi đăng nhập
        if (inputUsername == fileUsername && inputPassword == filePassword) {
            file.close();
            return true;  // đăng nhập thành công nếu hai thông tin khớp với file đã lưu
        }
    }

    file.close();
    return false;  
    // Không tìm thấy thông tin đăng nhập phù hợp trong file đã lưu
}

// Hàm khởi tạo màn hình login
void InitLoginScreen() {  // tên function được đặt theo file đã quy ước rồi, file pdf
    username = "";
    password = "";
    loggedIn = false;
    enteringUsername = true;     
    // mặc định ta sẽ vào username trước chứ không phải password trước
}

// Hàm cập nhật logic màn hình login
void UpdateLoginScreen() {
    if (IsKeyPressed(KEY_TAB)) {
        enteringUsername = !enteringUsername;  // Chuyển giữa username và password khi bấm TAB
    }

    int key = GetKeyPressed();
    // Get key pressed (keycode), call it multiple times for keys queued, returns 0 when the queue is empty
    
    // Nhập thông tin từ bàn phím
    if (key >= 32 && key <= 126) {  // Ký tự có thể in ra được
        if (enteringUsername) {
            username += (char)key;
            /*  
            key: Là giá trị số nguyên (int) trả về từ GetKeyPressed()
            (char)key: Chuyển đổi (cast) giá trị số nguyên thành ký tự
            user_name += : Thêm ký tự vào cuối chuỗi user_name*/
        } else {
            password += (char)key;
        }
    }

    // Xử lý khi đăng nhập có phím backspace
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
