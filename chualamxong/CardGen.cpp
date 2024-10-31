#include "raylib.h"
#include <vector>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

// Cấu trúc lá bài với trạng thái lật
struct Card {
    Texture2D frontTexture;
    Texture2D backTexture;
    string name;
    Rectangle position;   // Vị trí và kích thước của lá bài
    float flipProgress;   // Tiến trình lật (0.0 - 1.0)
    bool isFlipping;      // Trạng thái đang lật
    bool showBack;        // Trạng thái hiển thị mặt sau
};

// Hàm cập nhật trạng thái lật của lá bài
float speed = 0.05f;
void UpdateCardFlip(Card &card) {
    if (card.isFlipping) {
        card.flipProgress += speed;
        if (card.flipProgress >= 1.0f) {
            card.flipProgress = 1.0f;
            speed = -speed;
            card.isFlipping = false;
            card.showBack = !card.showBack; // Đổi trạng thái mặt sau/mặt trước
        }
        if (card.flipProgress <= 0.0f) {
            card.flipProgress = 0.0f;
            speed = -speed;
            card.isFlipping = false;
            card.showBack = !card.showBack;
        }
    }
}

// Vẽ lá bài
void DrawCard(Card &card) {
    // Chọn hình ảnh mặt trước hoặc mặt sau dựa vào tiến trình lật
    Texture2D texture = (card.flipProgress < 0.5f) ? card.backTexture : card.frontTexture;

    // Tính tỷ lệ lật
    float scaleX = (card.flipProgress < 0.5f) ? 1.0f - 2.0f * card.flipProgress : -1.0f + 2.0f * card.flipProgress;
    float cardWidth = card.position.width * scaleX;
    float cardHeight = card.position.height;

    // Tính vị trí để căn giữa hình ảnh
    float posX = card.position.x + (card.position.width - cardWidth) / 2;
    float posY = card.position.y;

    // Vẽ hình ảnh lá bài
    DrawTexturePro(
        texture,
        Rectangle{0, 0, (float)texture.width, (float)texture.height},
        Rectangle{posX, posY, cardWidth, cardHeight},
        Vector2{cardWidth / 2.0f, cardHeight / 2.0f},
        0.0f,
        WHITE
    );
}

// Lấy bài từ bộ ngẫu nhiên
Card* GetCardByNumber(int cardNumber, vector<Card>& deck) {
    if (cardNumber < 0 || cardNumber >= deck.size()) {
        return nullptr;
    }
    return &deck[cardNumber];
}


// Tạo bộ bài 52 lá
vector<Card> deck;
void createDeck(const int screenWidth, const int screenHeight){
    for (int i = 1; i <= 52; ++i) {
        Rectangle rec = {screenWidth / 2 , screenHeight / 2 , 500, 750};
        stringstream ss;
        ss << "cards/" << i << ".png";  // Dẫn ra file ảnh
        Texture2D frontTexture = LoadTexture(ss.str().c_str());
        Texture2D backTexture = LoadTexture("resources/back.png"); // File mặt sau

        Card card = { frontTexture, backTexture, "Lá bài " + to_string(i), rec, 0.0f, false, true };
        deck.push_back(card);
    }
}

int main() {
    const int screenWidth = 1800;
    const int screenHeight = 1300;
    InitWindow(screenWidth, screenHeight, "RANDOM CARD FLIPPING!");

    int cardNumber = -1;      // Số của lá bài được chọn
    Card* selectedCard = nullptr;

    createDeck(screenWidth, screenHeight);

    SetTargetFPS(60);
    float timer = 0.0f;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Rút bài ngẫu nhiên
        timer += GetFrameTime();
        if (timer > 2.5f) {
            timer = 0.0f;
            if (speed > 0) {
                cardNumber = rand() % 52;
                selectedCard = GetCardByNumber(cardNumber, deck);
            }
            deck[cardNumber].isFlipping = true;
        }

        // Cập nhật lật lá bài được chọn
        if (selectedCard != nullptr) {
            UpdateCardFlip(deck[cardNumber]);
        }

        // Vẽ bài
        if (selectedCard != nullptr) {
            DrawCard(deck[cardNumber]);
        }
        EndDrawing();
    }

    // Giải phóng bộ nhớ
    for (Card& card : deck) {
        UnloadTexture(card.frontTexture);
        UnloadTexture(card.backTexture);
    }

    CloseWindow();
    return 0;
}
