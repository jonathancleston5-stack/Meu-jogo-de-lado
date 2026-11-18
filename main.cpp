#include "raylib.h"
#include <vector>

int main() {
    InitWindow(800, 450, "Shadow Protocol: Side-Scroll Cyberpunk");
    SetTargetFPS(60);

    // Posição e física do Herói (Ninja Preto)
    Vector2 posHeroi = { 100.0f, 260.0f };
    float velocidadeY = 0.0f;
    bool noChao = false;
    bool atacando = false;
    int contadorAtaque = 0;
    
    // Variáveis do Deslizar (Dash / Rasteira)
    bool deslizando = false;
    int contadorDeslize = 0;

    float larguraHeroi = 36.0f;
    float alturaHeroi = 64.0f;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // Movimentação lateral (com velocidade reduzida se estiver a deslizar)
        float velocidadeMovimento = deslizando ? 500.0f : 280.0f;
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) posHeroi.x += velocidadeMovimento * dt;
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) posHeroi.x -= velocidadeMovimento * dt;

        // Gravidade (se estiver a deslizar no chão, anula a gravidade extra)
        if (!noChao) {
            velocidadeY += 1100.0f * dt;
        } else {
            velocidadeY = 0.0f;
        }
        posHeroi.y += velocidadeY * dt;

        // Colisão com o chão de aço (Y = 330)
        float chaoY = 330.0f;
        if (posHeroi.y + alturaHeroi >= chaoY) {
            posHeroi.y = chaoY - alturaHeroi;
            noChao = true;
        } else {
            noChao = false;
        }

        // Pulo (Espaço ou W)
        if ((IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_W)) && noChao && !deslizando) {
            velocidadeY = -500.0f;
            noChao = false;
        }

        // Deslizar / Rasteira (Tecla S ou Baixo)
        if ((IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) && noChao && !deslizando) {
            deslizando = true;
            contadorDeslize = 25; // Duração do deslize
        }
        if (deslizando) {
            contadorDeslize--;
            if (contadorDeslize <= 0) deslizando = false;
        }

        // Ataque com Katana (Tecla J ou K)
        if ((IsKeyPressed(KEY_J) || IsKeyPressed(KEY_K)) && !atacando) {
            atacando = true;
            contadorAtaque = 15;
        }
        if (atacando) {
            contadorAtaque--;
            if (contadorAtaque <= 0) atacando = false;
        }

        // --- RENDERIZAÇÃO ---
        BeginDrawing();
        ClearBackground(Color{ 8, 10, 15, 255 }); // Fundo noite escura futurista

        // CAMADA 1: FUNDO (Cidades Futuras e Naves)
        DrawRectangle(50, 120, 90, 210, Color{ 18, 22, 35, 255 });
        DrawRectangle(180, 80, 120, 250, Color{ 15, 18, 28, 255 });
        DrawRectangle(450, 100, 110, 230, Color{ 20, 25, 40, 255 });
        DrawRectangle(650, 70, 100, 260, Color{ 15, 18, 28, 255 });

        // Naves futuristas no fundo
        DrawPixel(250, 110, RED);
        DrawPixel(251, 110, RED);
        DrawRectangle(320, 95, 24, 4, Color{ 100, 200, 255, 200 });

        // CAMADA 2: O CHÃO DE AÇO E FIOS SOLTOS
        DrawRectangle(0, (int)chaoY, 800, 120, Color{ 45, 50, 60, 255 });
        for (int x = 0; x < 800; x += 80) {
            DrawLine(x, (int)chaoY, x, (int)chaoY + 120, Color{ 30, 35, 42, 255 });
        }
        DrawRectangle(0, (int)chaoY, 800, 5, Color{ 90, 105, 120, 255 });

        // Fios soltos eletrizados
        DrawLineBezier(Vector2{ 150.0f, chaoY }, Vector2{ 165.0f, chaoY + 45.0f }, 2.0f, YELLOW);
        DrawLineBezier(Vector2{ 420.0f, chaoY }, Vector2{ 400.0f, chaoY + 55.0f }, 2.0f, YELLOW);
        DrawLineBezier(Vector2{ 600.0f, chaoY }, Vector2{ 620.0f, chaoY + 40.0f }, 2.0f, YELLOW);

        // CAMADA 3: PERSONAGEM (Ninja de Preto com Katana / Altura ajustada se deslizar)
        float alturaAtualHeroi = deslizando ? 35.0f : alturaHeroi;
        float yAtualHeroi = deslizando ? posHeroi.y + 29.0f : posHeroi.y;

        // Corpo do Ninja
        DrawRectangleRec(Rectangle{ posHeroi.x, yAtualHeroi, larguraHeroi, alturaAtualHeroi }, Color{ 20, 22, 26, 255 });
        DrawRectangle((int)posHeroi.x + 6, (int)yAtualHeroi + 8, 24, 6, DARKGRAY);

        // Katana / Ataque / Efeito de deslize
        if (atacando) {
            DrawRectangle((int)posHeroi.x + 30, (int)yAtualHeroi + 15, 35, 6, LIGHTGRAY);
            DrawCircle((int)posHeroi.x + 50, (int)yAtualHeroi + 18, 8, SKYBLUE);
        } else if (deslizando) {
            DrawRectangle((int)posHeroi.x - 10, (int)yAtualHeroi + 20, 15, 4, YELLOW); // Faísca da rasteira
        } else {
            DrawRectangle((int)posHeroi.x + 25, (int)yAtualHeroi + 15, 6, 25, LIGHTGRAY);
        }

        // --- HUD / INTERFACE (BOTÕES VIRTUAIS NA TELA E FPS) ---
        // Contador de FPS clichê no canto superior direito
        DrawFPS(710, 15);

        // Botões Virtuais na Tela (Simulação visual de comandos touch para mobile)
        // Botão Esquerdo / Direito (Movimento)
        DrawRectangle(30, 370, 50, 45, Color{ 40, 45, 60, 180 });
        DrawText("<", 48, 382, 20, WHITE);
        DrawRectangle(90, 370, 50, 45, Color{ 40, 45, 60, 180 });
        DrawText(">", 108, 382, 20, WHITE);

        // Botão Deslizar (S)
        DrawRectangle(560, 370, 60, 45, Color{ 60, 50, 30, 180 });
        DrawText("DASH", 572, 385, 12, YELLOW);

        // Botão Pular (W / Espaço)
        DrawRectangle(630, 370, 60, 45, Color{ 30, 60, 40, 180 });
        DrawText("PULO", 643, 385, 12, GREEN);

        // Botão Atacar (J / K)
        DrawRectangle(700, 370, 70, 45, Color{ 70, 30, 30, 180 });
        DrawText("ATAKE", 715, 385, 12, RED);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
