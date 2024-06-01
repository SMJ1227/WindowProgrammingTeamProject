#include <windows.h>
#include <vector>
#include <time.h>
#include <math.h>
#include <string>
#include "resource.h"

const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 600;
const int GRID = 40;
const int MAP_WIDTH = 13;
const int MAP_HEIGHT = 33;
const int BOARD_WIDTH = MAP_WIDTH * GRID;
const int BOARD_HEIGHT = MAP_HEIGHT * GRID;
const int PLAYER_SIZE = 20;
const double M_PI = 3.141592;
const int GRAVITY = 1; // �߷� ���

int map_num = 0;
int map0[MAP_HEIGHT][MAP_WIDTH] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 3, 0},
    {0, 1, 1, 1, 2, 1, 1, 1, 1, 3, 1, 0},
    {0, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 0},
    {0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 4, 2, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 3, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 0},
    {0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 2, 1, 1, 1, 1, 0, 1, 1, 3, 0},
    {0, 0, 0, 2, 1, 1, 1, 1, 1, 3, 0, 0},
    {0, 4, 0, 0, 2, 1, 1, 1, 3, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

using namespace std;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"JumpKing";

// ���� ����
struct Player {
    int x, y;
    int dx, dy;
    int jumpSpeed;
    bool isCharging;
    bool isSlooping;
    bool slip; // �̲������� ���� ��� true
    string face;// face: left, right  

} g_player;


struct Item {
    int x, y;
    int type;
};
vector<Item> g_items;

struct Enemy {
    int x, y;
};
vector<Enemy> g_enemies;

struct Bullet {
    int x, y;
    int dx, dy;
};
vector<Bullet> g_bullets;

void ProcessKeyboardDown(WPARAM wParam);
void ProcessKeyboardUp(WPARAM wParam);
void DrawMap(HDC hdc);
void InitPlayer();
void MovePlayer();
void DrawPlayer(HDC hDC);
void drawPlayerSprite(HDC hDC, HBITMAP playerBitmaps, HBITMAP playerBitmapsMask);
void ApplyGravity();
bool IsColliding(int x, int y);
bool IsSlopeGoRightColliding(int x, int y);
bool IsSlopeGoLeftColliding(int x, int y);
void GenerateItem(int x, int y, int num);
void DrawItems(HDC hdc);
void GenerateEnemy(int x, int y);
void DrawEnemies(HDC hDC);

void ShootBullet();
void MoveBullets();
void DrawBullets(HDC hDC);

void CheckCollisions();
void CheckEnemyPlayerCollisions();
void CheckItemPlayerCollisions();
void CheckPlayerBulletCollisions();

// WinMain �Լ�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {
    srand((unsigned int)time(NULL));
    HWND hWnd;
    MSG Message;
    WNDCLASSEX WndClass;
    g_hInst = hInstance;

    WndClass.cbSize = sizeof(WndClass);
    WndClass.style = CS_HREDRAW | CS_VREDRAW;
    WndClass.lpfnWndProc = (WNDPROC)WndProc;
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hInstance = hInstance;
    WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    WndClass.hCursor = LoadCursor(NULL, IDC_HAND);
    WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    WndClass.lpszMenuName = MAKEINTRESOURCE(NULL);
    WndClass.lpszClassName = lpszClass;
    WndClass.hIconSm = LoadIcon(NULL, IDI_QUESTION);
    RegisterClassEx(&WndClass);

    hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 0, 0, WINDOW_WIDTH + 16, WINDOW_HEIGHT + 59, NULL, (HMENU)NULL, hInstance, NULL);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&Message, 0, 0, 0)) {
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }
    return Message.wParam;
}

//���� �Լ�

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    PAINTSTRUCT ps;
    HDC hDC = BeginPaint(hWnd, &ps);
    HDC mDC;
    HBITMAP hBitmap;
    RECT rt;
    static HBITMAP playerBitmaps, playerBitmapsMask; // �÷��̾� ��Ʈ�� �̹���, ��Ʈ�� ����ũ �̹��� �ε�

    static int shootInterval = 0;

    switch (message) {
    case WM_CREATE:
        InitPlayer();
        SetTimer(hWnd, 1, 1000 / 60, NULL);
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {}
        break;
    case WM_CHAR:
        switch (wParam)
        {
        case 'Q':
        case 'q':
            PostQuitMessage(0);
            break;
        }
        break;
    case WM_TIMER:
        switch (wParam) {
        case 1:
            ApplyGravity();
            MovePlayer();
            MoveBullets();
            shootInterval++;
            if (shootInterval > 120) {
                ShootBullet();
                shootInterval = 0;
            }
            CheckCollisions();
            break;
        }
        InvalidateRect(hWnd, NULL, FALSE);
        break;
    case WM_PAINT:
    {
        GetClientRect(hWnd, &rt);
        mDC = CreateCompatibleDC(hDC);
        hBitmap = CreateCompatibleBitmap(hDC, BOARD_WIDTH, BOARD_HEIGHT);
        SelectObject(mDC, (HBITMAP)hBitmap);

        //--- ��� �׸��⸦ �޸� DC���Ѵ�.
        DrawMap(mDC);
        DrawEnemies(mDC);
        DrawBullets(mDC);
        drawPlayerSprite(mDC, playerBitmaps, playerBitmapsMask);
        //DrawPlayer(mDC);
        // �޸� DC���� ȭ�� DC�� �׸��� ����
        // #1 �� ��ü�� �׸���
        // BitBlt(hDC, 0, 0, BOARD_WIDTH, BOARD_HEIGHT, mDC, 0, 0, SRCCOPY);

        // #2 �÷��̾� �ֺ��� ������ ������ ��ü�� Ȯ��
        int stretchWidth = rt.right;
        int stretchHeight = rt.bottom;
        int sourceWidth = WINDOW_WIDTH;
        int sourceHeight = WINDOW_HEIGHT;
        int sourceX = g_player.x - WINDOW_WIDTH / 2;
        if (sourceX <= 0) { sourceX = 0; }
        if (g_player.x + WINDOW_WIDTH / 2 >= WINDOW_WIDTH) { sourceX = WINDOW_WIDTH - sourceWidth; }
        int sourceY = g_player.y - WINDOW_HEIGHT / 2;
        if (sourceY - sourceHeight >= 0) { sourceY = sourceHeight + GRID; }
        if (sourceY <= 0) { sourceY = 0; }


        StretchBlt(hDC, 0, 0, stretchWidth, stretchHeight, mDC, sourceX, sourceY, sourceWidth, sourceHeight, SRCCOPY);

        DeleteDC(mDC);
        DeleteObject(hBitmap);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_KEYDOWN:
        ProcessKeyboardDown(wParam);
        InvalidateRect(hWnd, NULL, FALSE);
        break;
    case WM_KEYUP:
        ProcessKeyboardUp(wParam);
        InvalidateRect(hWnd, NULL, FALSE);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Ű�Է�
void ProcessKeyboardDown(WPARAM wParam) {
    switch (wParam) {
    case VK_LEFT:
        if (g_player.isCharging)break;
        if (g_player.isSlooping)break;
        g_player.dx = -3;
        g_player.face = "left";
        break;
    case VK_RIGHT:
        if (g_player.isCharging)break;
        if (g_player.isSlooping)break;
        g_player.dx = 3;
        g_player.face = "right";
        break;
    case VK_SPACE:
        if (g_player.dy == 0 && g_player.jumpSpeed > -20) { // �ٴڿ� ��� ���� ���� ���� ����
            g_player.isCharging = true;
            g_player.dx = 0;
            g_player.jumpSpeed -= 5;
        }
        break;
    }
}

void ProcessKeyboardUp(WPARAM wParam) {
    switch (wParam) {
    case VK_LEFT:
    case VK_RIGHT:
        if (g_player.isSlooping)break;
        g_player.dx = 0;
        break;
    case VK_SPACE:
        g_player.dy = g_player.jumpSpeed;
        g_player.jumpSpeed = 0;
        g_player.isCharging = false;
        break;
    }
}

// ��
void DrawMap(HDC hdc) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            // ���� ���� ���� 1�̸� ���(�÷��̾� ����), 0�̸� ���������� �׸��ϴ�.
            if (map0[y][x] == 0) {
                // ������
                HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
                SelectObject(hdc, hBrush);
                Rectangle(hdc, x * GRID, y * GRID, (x + 1) * GRID, (y + 1) * GRID);
                DeleteObject(hBrush);
            }
            else if (map0[y][x] == 1) {
                // ���
                HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
                SelectObject(hdc, hBrush);
                Rectangle(hdc, x * GRID, y * GRID, (x + 1) * GRID, (y + 1) * GRID);
                DeleteObject(hBrush);
            }
            else if (map0[y][x] == 2) {  // ������ �Ʒ��� �귯������ ����
                // ������
                POINT point[3];
                point[0].x = x * GRID;
                point[0].y = y * GRID;
                point[1].x = x * GRID;
                point[1].y = (y + 1) * GRID;
                point[2].x = (x + 1) * GRID;
                point[2].y = (y + 1) * GRID;
                HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0));
                SelectObject(hdc, hBrush);
                Polygon(hdc, point, 3);
                DeleteObject(hBrush);
            }
            else if (map0[y][x] == 3) {  // ���� �Ʒ��� �귯������ ����
                // ������
                POINT point[3];
                point[0].x = (x + 1) * GRID;
                point[0].y = y * GRID;
                point[1].x = x * GRID;
                point[1].y = (y + 1) * GRID;
                point[2].x = (x + 1) * GRID;
                point[2].y = (y + 1) * GRID;
                HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0));
                SelectObject(hdc, hBrush);
                Polygon(hdc, point, 3);
                DeleteObject(hBrush);
            }
            else if (map0[y][x] == 4) {  // ��
                GenerateEnemy(x, y);
            }
        }
    }
}

// �÷��̾�
void InitPlayer() {
    g_player.x = (MAP_WIDTH - 7) * GRID;
    g_player.y = (MAP_HEIGHT - 4) * GRID;
    g_player.dx = 0;
    g_player.dy = 0;
    g_player.jumpSpeed = 0;
    g_player.isCharging = false;
    g_player.face = "left";
}

void MovePlayer() {
    int newX = g_player.x + g_player.dx;
    int newY = g_player.y + g_player.dy;

    bool isVerticalCollision = IsColliding(g_player.x, newY);
    bool isHorizontalCollision = IsColliding(newX, g_player.y);

    // ���� �浹 ó��
    if (!isVerticalCollision) {
        g_player.y = newY;
    }
    else {
        // �ٴ� �浹 �� y�� ��ġ ����
        if (g_player.dy > 0) {
            while (!IsColliding(g_player.x, g_player.y + 1)) {
                g_player.y += 1;
            }
        }
        g_player.dy = 0; // �浹 �� y�� �ӵ� �ʱ�ȭ
    }

    // ���� �浹 ó��
    if (!isHorizontalCollision) {
        g_player.x = newX;
    }
    else {
        g_player.dx = 0; // �浹 �� x�� �ӵ� �ʱ�ȭ
    }
    bool isSlopeGoRighCollision = IsSlopeGoRightColliding(g_player.x, g_player.y);
    if (isSlopeGoRighCollision) {
        g_player.isSlooping = true;

        g_player.dy = 1; // ���� ������ �̲����� �ӵ�
        g_player.dx = 2; // ������ �Ʒ��� �̲�����
        newX = g_player.x + g_player.dx;
        newY = g_player.y + g_player.dy;
        g_player.x = newX;
        g_player.y = newY;
    }
    else {
        g_player.isSlooping = false;
    }
    bool isSlopeGoLeftCollision = IsSlopeGoLeftColliding(g_player.x, g_player.y);
    if (isSlopeGoLeftCollision) {
        g_player.isSlooping = true;

        g_player.dy = 1; // ���� ������ �̲����� �ӵ�
        g_player.dx = -2; // ������ �Ʒ��� �̲�����
        newX = g_player.x + g_player.dx;
        newY = g_player.y + g_player.dy;
        g_player.x = newX;
        g_player.y = newY;
    }
    else {
        g_player.isSlooping = false;
    }
}

void DrawPlayer(HDC hdc) {
    HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
    SelectObject(hdc, hBrush);
    Rectangle(hdc, g_player.x - PLAYER_SIZE / 2, g_player.y - PLAYER_SIZE / 2, g_player.x + PLAYER_SIZE / 2, g_player.y + PLAYER_SIZE / 2);
    DeleteObject(hBrush);
}

// ��Ʈ�� �޸�DC�� ����
void drawPlayerSprite(HDC hDC, HBITMAP playerBitmaps, HBITMAP playerBitmapsMask) {
    if (g_player.dy == 0 && g_player.jumpSpeed == 0 && g_player.dx != 0) {
        playerBitmaps = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(MOVE1));
        playerBitmapsMask = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(MOVE1_MASK));
    }
    else if (g_player.dy < 0) {
        playerBitmaps = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(JUMP));
        playerBitmapsMask = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(JUMP_MASK));
    }
    else if (IsSlopeGoRightColliding(g_player.x, g_player.y) || IsSlopeGoLeftColliding(g_player.x, g_player.y)) {
        playerBitmaps = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(SLIP2));
        playerBitmapsMask = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(SLIP2_MASK));
    }
    else if (g_player.dy > 0) {
        playerBitmaps = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(FALL));
        playerBitmapsMask = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(FALL_MASK));
    }
    else if (g_player.dy == 0 && g_player.jumpSpeed < 0) {
        if (g_player.jumpSpeed == -20) {
            playerBitmaps = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(CHARGE1));
            playerBitmapsMask = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(CHARGE1_MASK));
        }
        else {
            playerBitmaps = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(CHARGE2));
            playerBitmapsMask = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(CHARGE2_MASK));
        }
    }
    else {
        playerBitmaps = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDLE));
        playerBitmapsMask = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDLE_MASK));
    }
    HDC hmemDC = CreateCompatibleDC(hDC);
    HBITMAP oldBitmap;
    BITMAP bmp;
    GetObject(playerBitmaps, sizeof(BITMAP), &bmp);

    if (g_player.face == "left") {
        oldBitmap = (HBITMAP)SelectObject(hmemDC, playerBitmapsMask);
        StretchBlt(hDC, g_player.x - PLAYER_SIZE / 2, g_player.y - PLAYER_SIZE / 2, PLAYER_SIZE, PLAYER_SIZE, hmemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCAND);
        oldBitmap = (HBITMAP)SelectObject(hmemDC, playerBitmaps);
        StretchBlt(hDC, g_player.x - PLAYER_SIZE / 2, g_player.y - PLAYER_SIZE / 2, PLAYER_SIZE, PLAYER_SIZE, hmemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCPAINT);
        SelectObject(hmemDC, oldBitmap);
    }
    else if (g_player.face == "right") {
        oldBitmap = (HBITMAP)SelectObject(hmemDC, playerBitmapsMask);
        StretchBlt(hDC, g_player.x + PLAYER_SIZE / 2, g_player.y - PLAYER_SIZE / 2, -PLAYER_SIZE, PLAYER_SIZE, hmemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCAND);
        oldBitmap = (HBITMAP)SelectObject(hmemDC, playerBitmaps);
        StretchBlt(hDC, g_player.x + PLAYER_SIZE / 2, g_player.y - PLAYER_SIZE / 2, -PLAYER_SIZE, PLAYER_SIZE, hmemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCPAINT);
        SelectObject(hmemDC, oldBitmap);
    }

    DeleteDC(hmemDC);
    DeleteObject(playerBitmaps);
    DeleteObject(playerBitmapsMask);
}

void ApplyGravity() {
    if (g_player.dy < 20) {
        g_player.dy += GRAVITY; // �߷� ����
    }
}

bool IsColliding(int x, int y) {
    int leftX = (x - PLAYER_SIZE / 2) / GRID;
    int rightX = (x + PLAYER_SIZE / 2 - 1) / GRID;
    int topY = (y - PLAYER_SIZE / 2) / GRID;
    int bottomY = (y + PLAYER_SIZE / 2 - 1) / GRID;

    // �� ��踦 ����� �ʵ��� üũ
    if (leftX < 0 || rightX >= MAP_WIDTH || topY < 0 || bottomY >= MAP_HEIGHT) {
        return true;
    }

    // �浹 ����
    if (map0[topY][leftX] == 0 || map0[topY][rightX] == 0 ||
        map0[bottomY][leftX] == 0 || map0[bottomY][rightX] == 0) {
        return true;
    }

    return false;
}

bool IsSlopeGoRightColliding(int x, int y) {
    int leftX = (x - PLAYER_SIZE / 2) / GRID;
    int rightX = (x + PLAYER_SIZE / 2 - 1) / GRID;
    int topY = (y - PLAYER_SIZE / 2) / GRID;
    int bottomY = (y + PLAYER_SIZE / 2 - 1) / GRID;

    // �浹 ����
    if (map0[bottomY][leftX] == 2 || map0[bottomY][rightX] == 2) {
        g_player.slip = true;
        return true;
    }

    return false;
}

bool IsSlopeGoLeftColliding(int x, int y) {
    int leftX = (x - PLAYER_SIZE / 2) / GRID;
    int rightX = (x + PLAYER_SIZE / 2 - 1) / GRID;
    int topY = (y - PLAYER_SIZE / 2) / GRID;
    int bottomY = (y + PLAYER_SIZE / 2 - 1) / GRID;

    // �浹 ����
    if (map0[bottomY][leftX] == 3 || map0[bottomY][rightX] == 3) {
        g_player.slip = true;
        return true;
    }

    return false;
}

// ������
void GenerateItem(int x, int y, int num) {
    Item newItem;
    newItem.x = x;
    newItem.y = y;
    g_items.push_back(newItem);
}

void DrawItems(HDC hdc) {
    for (const auto& item : g_items) {
        HBRUSH hBrush1 = CreateSolidBrush(RGB(255, 255, 255));
        SelectObject(hdc, hBrush1);
        Rectangle(hdc, item.x * GRID, item.y * GRID, (item.x + 1) * GRID, (item.y + 1) * GRID); // ������ �׸���
        DeleteObject(hBrush1);
    }
}

// ��
void GenerateEnemy(int x, int y) {
    Enemy newEnemy;
    newEnemy.x = x;
    newEnemy.y = y;
    g_enemies.push_back(newEnemy);
}

void DrawEnemies(HDC hdc) {
    for (const auto& enemy : g_enemies) {
        HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 0));
        SelectObject(hdc, hBrush);
        Rectangle(hdc, enemy.x * GRID, enemy.y * GRID, (enemy.x + 1) * GRID, (enemy.y + 1) * GRID);
        DeleteObject(hBrush);
    }
}

void ShootBullet() {
    for (const auto& enemy : g_enemies) {
        Bullet newBullet;
        newBullet.x = enemy.x * GRID; // ���� ��ġ���� �Ѿ��� �������� ����
        newBullet.y = enemy.y * GRID + GRID / 2;
        newBullet.dx = 2;
        newBullet.dy = 0;
        g_bullets.push_back(newBullet);
    }
}


void MoveBullets() {
    for (auto it = g_bullets.begin(); it != g_bullets.end();) {
        it->x += it->dx;
        it->y += it->dy;
        if (it->x < 0 || it->x > BOARD_WIDTH) {
            it = g_bullets.erase(it);
        }
        else {
            ++it;
        }
    }
}

void DrawBullets(HDC hdc) {
    HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
    SelectObject(hdc, hBrush);
    for (const auto& bullet : g_bullets) {
        if (bullet.x >= 0 && bullet.x <= BOARD_WIDTH && bullet.y >= 0 && bullet.y <= BOARD_HEIGHT) {
            Ellipse(hdc, bullet.x - 10, bullet.y - 10, bullet.x + 10, bullet.y + 10);
        }
    }
    DeleteObject(hBrush);
}

// �浹 Ȯ�� �Լ�
void CheckCollisions() {
    CheckItemPlayerCollisions();
    CheckPlayerBulletCollisions();
}

void CheckItemPlayerCollisions() {
    for (auto it = g_items.begin(); it != g_items.end(); ) {
        if (it->x >= g_player.x - PLAYER_SIZE / 2 && it->x <= g_player.x + PLAYER_SIZE / 2 &&
            it->y >= g_player.y - PLAYER_SIZE / 2 && it->y <= g_player.y + PLAYER_SIZE / 2) {
            it = g_items.erase(it); // �����۰� �浹 �� ����
        }
        else {
            ++it;
        }
    }
}

void CheckPlayerBulletCollisions() {
    for (auto it = g_bullets.begin(); it != g_bullets.end(); ) {
        if (it->x >= g_player.x - PLAYER_SIZE / 2 && it->x <= g_player.x + PLAYER_SIZE / 2 &&
            it->y >= g_player.y - PLAYER_SIZE / 2 && it->y <= g_player.y + PLAYER_SIZE / 2) {
            // �÷��̾�� �浹 �� ����
            it = g_bullets.erase(it);
            // �÷��̾ �ڷ� ��ħ

        }
        else {
            ++it;
        }
    }
}