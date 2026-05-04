
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h> // Trigonometri işlemleri (sin, cos) için eklendi

#define PI 3.14159265f

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// --- 1. VEKTÖR VE GEMİ YAPILARI ---

// 2 Boyutlu Vektör Yapısı (Konum, Hız ve İvme için kullanılacak)
typedef struct {
    float x;
    float y;
} Vector2D;

// Uzay Gemisi Yapısı
typedef struct {
    Vector2D pos;    // Geminin konumu (Position)
    Vector2D vel;    // Geminin hızı (Velocity)
    float angle;     // Geminin baktığı açı (Radyan cinsinden)
    float size;      // Geminin büyüklüğü (Yarıçapı)
} Spaceship;


// --- 2. ÇİZİM FONKSİYONLARI ---

// Gemiyi (üçgen şeklinde) ekrana çizen fonksiyon
void DrawSpaceship(SDL_Renderer* renderer, Spaceship ship) {
    // SDL2'de doğrudan üçgen çizme fonksiyonu yoktur, çizgileri birleştirerek yaparız.
    // Geminin 3 köşesini trigonometri (sin, cos) kullanarak hesaplıyoruz.

    // 1. Nokta: Geminin Burnu (Baktığı açıya doğru)
    float x1 = ship.pos.x + cos(ship.angle) * ship.size;
    float y1 = ship.pos.y + sin(ship.angle) * ship.size;

    // 2. Nokta: Sol arka köşe (+2.5 radyan açı ekleyerek buluyoruz)
    float x2 = ship.pos.x + cos(ship.angle + 2.5f) * ship.size;
    float y2 = ship.pos.y + sin(ship.angle + 2.5f) * ship.size;

    // 3. Nokta: Sağ arka köşe (-2.5 radyan açı çıkararak buluyoruz)
    float x3 = ship.pos.x + cos(ship.angle - 2.5f) * ship.size;
    float y3 = ship.pos.y + sin(ship.angle - 2.5f) * ship.size;

    // Çizim rengini Beyaz yap (R, G, B, Alpha)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // 3 noktayı çizgilerle birleştirerek üçgeni oluştur
    SDL_RenderDrawLine(renderer, (int)x1, (int)y1, (int)x2, (int)y2);
    SDL_RenderDrawLine(renderer, (int)x2, (int)y2, (int)x3, (int)y3);
    SDL_RenderDrawLine(renderer, (int)x3, (int)y3, (int)x1, (int)y1);
}

// --- ANA FONKSİYON ---

int main(int argc, char* args[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL baslatilamadi! Hata: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Asteroids - KOU Proje", 
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (window == NULL) {
        printf("Pencere olusturulamadi! Hata: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    // Gemimizi oluşturup başlangıç değerlerini atıyoruz
    Spaceship player;
    player.pos.x = SCREEN_WIDTH / 2.0f;  // Ekranın tam ortası X
    player.pos.y = SCREEN_HEIGHT / 2.0f; // Ekranın tam ortası Y
    player.vel.x = 0.0f;                 // Başlangıçta hızı 0
    player.vel.y = 0.0f;
    player.angle = -PI / 2.0f;           // Yukarıya bakması için -90 derece
    player.size = 20.0f;                 // Boyutu

    bool quit = false;
    SDL_Event e;

    // Oyun Döngüsü
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Ekranı temizle (Siyah)
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Gemiyi çiz
        DrawSpaceship(renderer, player);

        // Çizilenleri ekrana yansıt
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}