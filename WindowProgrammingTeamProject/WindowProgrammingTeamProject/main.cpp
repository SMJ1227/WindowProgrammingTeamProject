#include <windows.h>
#include <vector>
#include <time.h>
#include <math.h>
#include <string>
#include <atlImage.h>
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
const int GRAVITY = 1; // 중력 상수

int map_num = 0;
int map0[MAP_HEIGHT][MAP_WIDTH] = {
    {0, 0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 3, 0},
    {0, 1, 1, 0, 2, 1, 1, 1, 1, 3, 0, 0},
    {0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0},
    {0, 1, 1, 0, 0, 0, 2, 1, 1, 1, 1, 0},
    {0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0},
    {0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0},
    {0, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 3, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0},
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
int map1[MAP_HEIGHT][MAP_WIDTH] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 0},
    {4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 3, 0, 0, 1, 1, 1, 1, 1, 0},
    {4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 3, 0, 0},
    {0, 0, 0, 1, 1, 1, 1, 1, 3, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 3, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};
int tile_num = 0;
int tile0[MAP_HEIGHT][MAP_WIDTH] = {
    {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3},
    {4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6},
    {4, 0, 0, 0, 0, 14, 15, 16, 0, 0, 0, 6},
    {4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6},
    {4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6},
    {4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6},
    {4, 0, 0, 1, 0, 0, 0, 0, 0, 0, 10, 6},
    {4, 0, 0, 4, 12, 0, 0, 0, 0, 10, 11, 6},
    {4, 0, 0, 4, 13, 3, 0, 0, 0, 7, 8, 6},
    {4, 0, 0, 4,  5, 13, 12, 0, 0, 0, 0, 6},
    {4, 0, 0, 4,  5,  5, 13, 3, 0, 0, 0, 6},
    {4, 0, 0, 7,  8,  8,  8, 9, 0, 0, 0, 6},
    {4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6},
    {4, 14, 15, 15, 16, 0, 15, 0, 0, 0, 10, 6},
    {4, 0, 0, 0, 0, 0, 0, 0, 15, 0, 11, 6},
    {4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6},
    {4, 0, 0, 15, 0, 15, 0, 0, 0, 0, 0, 6},
    {4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6},
    {4, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6},
    {4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6},
    {4, 0, 0, 15, 0, 0, 0, 0, 15, 0, 0, 6},
    {4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6},
    {4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6},
    {4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6},
    {4, 0, 0, 0, 0, 15, 0, 0, 0, 0, 0, 6},
    {4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6},
    {4, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6},
    {4, 13, 12, 0, 0, 0, 0, 15, 0, 0, 10, 6},
    {4, 5, 13, 12, 0, 0, 0, 0, 0, 10, 11, 6},
    {4, 0, 5, 13, 12, 0, 0, 0, 10, 11, 5, 6},
    {17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17},
};
int tile1[MAP_HEIGHT][MAP_WIDTH] = {
    {1, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 2},
    {7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8},
    {7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8},
    {7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8},
    {7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8},
    {7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8},
    {7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8},
    {7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8},
    {7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8},
    {7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8},
    {7, 9, 9, 11, 0, 0, 0, 0, 0, 0, 0, 8},
    {7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8},
    {7, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 8},
    {7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8},
    {7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8},
    {7, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 8},
    {7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8},
    {7, 0, 0, 0, 0, 9, 9, 0, 0, 12, 0, 8},
    {7, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 8},
    {7, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 8},
    {7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 8},
    {7, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 8},
    {7, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 8},
    {7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8},
    {7, 0, 0, 10, 9, 9, 0, 0, 0, 0, 0, 8},
    {7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 8},
    {7, 0, 0, 0, 0, 0, 0, 0, 0, 10, 9, 8},
    {7, 9, 9, 0, 0, 0, 0, 0, 10, 9, 9, 8},
    {7, 0, 0, 0, 0, 0, 0, 10, 9, 9, 9, 8},
    {7, 0, 0, 0, 0, 0, 0, 9, 9, 9, 9, 8},
    {3, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 4}
};

using namespace std;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"JumpKing";

// 전역 변수
struct Player {
    int x, y;
    int dx, dy;
    int jumpSpeed;
    bool isCharging;
    bool isJumping;
    bool isSliding;
    bool slip; // 미끄러지는 동안 계속 true
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

void ProcessKeyboard();
void DrawSnowBg(HDC hDC);
void DrawDesertBg(HDC hDC);
void DrawSnowTile(HDC hDC);
void DrawDesertTile(HDC hDC);
void InitMap(int dst[MAP_HEIGHT][MAP_WIDTH], int src[MAP_HEIGHT][MAP_WIDTH]);
void InitPlayer();
void MovePlayer(int map[MAP_HEIGHT][MAP_WIDTH]);
void DrawSprite(HDC hDC, const int& x, const int& y, const int& width, const int& height);
void ApplyGravity();
bool IsColliding(int map[MAP_HEIGHT][MAP_WIDTH], int x, int y);
bool IsSlopeGoRightColliding(int map[MAP_HEIGHT][MAP_WIDTH], int x, int y);
bool IsSlopeGoLeftColliding(int map[MAP_HEIGHT][MAP_WIDTH], int x, int y);
bool IsNextColliding(int map[MAP_HEIGHT][MAP_WIDTH], int x, int y);
void GenerateItem(int x, int y, int num);
void DrawItems(HDC hDC);
void InitEnemy(int map[MAP_HEIGHT][MAP_WIDTH]);
void GenerateEnemy(int x, int y);
void DrawEnemies(HDC hDC);
void DeleteAllEnemies();
void ShootBullet();
void MoveBullets();
void DrawBullets(HDC hDC);
void DeleteAllBullets();
void CheckCollisions();
void CheckEnemyPlayerCollisions();
void CheckItemPlayerCollisions();
void CheckPlayerBulletCollisions();

// WinMain 함수
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

    while (1) {
        if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE)) {
            if (Message.message == WM_QUIT)
                break;
        }
        // #1 마우스 관련된 메세지를 무시하는 첫번째 방법
        //if (Message.message == WM_MOUSEMOVE || Message.message == WM_LBUTTONDOWN || Message.message == WM_RBUTTONDOWN) {
        //    // 마우스 메시지 무시
        //    continue;
        //}
        TranslateMessage(&Message);
        DispatchMessage(&Message);
        ProcessKeyboard();
    }
    return Message.wParam;
}

//--- CImage 관련 변수 선언
CImage Snowtile;
CImage Snowbg;
CImage Desertbg;
CImage Deserttile;
CImage cannon;
HBITMAP spriteSheet;
HBITMAP spriteSheetMask;

static int map[MAP_HEIGHT][MAP_WIDTH];
static int shootInterval = 0;
static int spriteX = 0;
static int spriteY = 0;
static int spriteWidth = 30;
static int spriteHeight = 0;

// 타이머 콜백
void CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    ApplyGravity();
    MovePlayer(map);
    MoveBullets();
    shootInterval++;
    if (shootInterval > 120) {
        ShootBullet();
        shootInterval = 0;
    }
    if (IsNextColliding(map, g_player.x, g_player.y) == true) {
        InitPlayer();
        InitMap(map, map1);
        DeleteAllEnemies();
        DeleteAllBullets();
        InitEnemy(map);
        map_num++;
    }
    CheckCollisions();
    if (g_player.dx < 0) g_player.face = "left";
    else if (g_player.dx > 0) g_player.face = "right";
    if (g_player.dy == 0 && g_player.jumpSpeed == 0 && g_player.dx != 0) {
        if ((spriteX += spriteWidth) > 230) {
            spriteX = 0;
        }
        spriteY = 24;
        spriteHeight = 24;

    }
    else if (g_player.dy == 0 && g_player.jumpSpeed < 0) {
        spriteX = 0;
        spriteY = 116;
        spriteHeight = 22;
        if (g_player.jumpSpeed == -20) {
            spriteX = 30;
        }
    }
    else if (g_player.dy < 0) {
        if ((spriteX += spriteWidth) > 119) {
            spriteX = 0;
        }
        spriteY = 48;
        spriteHeight = 29;
    }
    else if (g_player.dy > 0 && g_player.isSliding == false) {
        if ((spriteX += spriteWidth) > 59) {
            spriteX = 0;
        }
        spriteY = 77;
        spriteHeight = 39;
    }
    else if (g_player.dy > 0 && g_player.isSliding == true) {
        if ((spriteX += spriteWidth) > 29) {
            spriteX = 0;
        }
        spriteY = 138;
        spriteHeight = 25;
    }
    else {
        if ((spriteX += spriteWidth) > 230) {
            spriteX = 0;
        }
        spriteY = 0;
        spriteHeight = 24;
    }
    InvalidateRect(hWnd, NULL, FALSE);
}

//메인 함수

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    PAINTSTRUCT ps;
    HDC hDC;
    HDC mDC;
    HBITMAP hBitmap;
    RECT rt;

    static int playerFrameIndex = 0;

    switch (message) {
    case WM_CREATE:
        InitPlayer();
        InitMap(map, map0);
        InitEnemy(map0);
        Snowtile.Load(L"snowtile.png");
        Snowbg.Load(L"SnowBg.png");
        cannon.Load(L"Cannon.png");
        Desertbg.Load(L"desertbg_sand4.png");
        Deserttile.Load(L"deserttiles.png");
        spriteSheet = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(PLAYER_SPRITE));
        spriteSheetMask = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(PLAYER_SPRITE_MASK));

        SetTimer(hWnd, 1, 1000 / 60, (TIMERPROC)TimerProc);
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
    case WM_PAINT:
    {
        hDC = BeginPaint(hWnd, &ps);
        GetClientRect(hWnd, &rt);
        mDC = CreateCompatibleDC(hDC);
        hBitmap = CreateCompatibleBitmap(hDC, BOARD_WIDTH, BOARD_HEIGHT);
        SelectObject(mDC, (HBITMAP)hBitmap);

        //--- 모든 그리기를 메모리 DC에한다.  ---> 바꾼 부분: CImage 변수는 전역변수로 선언하여 함수의 인자로 보내지 않도록 한다.
        if (map_num == 0) {
            DrawSnowBg(mDC);
            DrawSnowTile(mDC);
        }
        else if (map_num == 1) {
            DrawDesertBg(mDC);
            DrawDesertTile(mDC);
        }
        DrawEnemies(mDC);
        DrawBullets(mDC);
        DrawSprite(mDC, spriteX, spriteY, spriteWidth, spriteHeight);


        // 메모리 DC에서 화면 DC로 그림을 복사
        // #1 맵 전체를 그리기
        // BitBlt(hDC, 0, 0, BOARD_WIDTH, BOARD_HEIGHT, mDC, 0, 0, SRCCOPY);
        // #2 플레이어 주변의 영역을 윈도우 전체로 확대
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
    case WM_DESTROY:
        Snowtile.Destroy();
        Snowbg.Destroy();
        cannon.Destroy();
        Desertbg.Destroy();
        Deserttile.Destroy();
        DeleteObject(spriteSheet);
        DeleteObject(spriteSheetMask);
        KillTimer(hWnd, 1);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 키입력
void ProcessKeyboard() {
    // 왼쪽 키 처리
    if (GetAsyncKeyState(VK_LEFT) & 0x8000) { // 키가 눌린 상태
        if (!g_player.isCharging && !g_player.isSliding) {
            g_player.dx = -3;
            g_player.face = "left";
        }
    }
    else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) { // 오른쪽 키 처리
        if (!g_player.isCharging && !g_player.isSliding) {
            g_player.dx = 3;
            g_player.face = "right";
        }
    }
    else {
        g_player.dx = 0; // 왼쪽, 오른쪽 키가 모두 눌리지 않은 상태
    }

    // 스페이스 키 처리
    if (GetAsyncKeyState(VK_SPACE) & 0x8000) { // 스페이스 키가 눌린 상태
        if (!g_player.isJumping && g_player.dy == 0 && g_player.jumpSpeed > -20) {
            g_player.isCharging = true;
            g_player.dx = 0;
            g_player.jumpSpeed -= 5;
        }
    }
    else { // 스페이스 키가 눌리지 않은 상태
        if (g_player.isCharging) {
            g_player.dy = g_player.jumpSpeed;
            g_player.jumpSpeed = 0;
            g_player.isCharging = false;
            g_player.isJumping = true;
        }
    }
}

// 맵
void DrawSnowTile(HDC hDC) {
    // 칸당 96x96
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            int tileType = tile0[y][x];
            switch (tileType) {
            case 1:
                Snowtile.Draw(hDC, x * GRID, y * GRID, GRID, GRID, 0, 0, 96, 96);
                break;
            case 2:
                Snowtile.Draw(hDC, x * GRID, y * GRID, GRID, GRID, 96, 0, 96, 96);
                break;
            case 3:
                Snowtile.Draw(hDC, x * GRID, y * GRID, GRID, GRID, 96 * 2, 0, 96, 96);
                break;
            case 4:
                Snowtile.Draw(hDC, x * GRID, y * GRID, GRID, GRID, 0, 96, 96, 96);
                break;
            case 5:
                Snowtile.Draw(hDC, x * GRID, y * GRID, GRID, GRID, 96, 96, 96, 96);
                break;
            case 6:
                Snowtile.Draw(hDC, x * GRID, y * GRID, GRID, GRID, 96 * 2, 96, 96, 96);
                break;
            case 7:
                Snowtile.Draw(hDC, x * GRID, y * GRID, GRID, GRID, 0, 96 * 2, 96, 96);
                break;
            case 8:
                Snowtile.Draw(hDC, x * GRID, y * GRID, GRID, GRID, 96, 96 * 2, 96, 96);
                break;
            case 9:
                Snowtile.Draw(hDC, x * GRID, y * GRID, GRID, GRID, 96 * 2, 96 * 2, 96, 96);
                break;
            case 10:
                Snowtile.Draw(hDC, x * GRID, y * GRID, GRID, GRID, 96 * 3, 0, 96, 96);
                break;
            case 11:
                Snowtile.Draw(hDC, x * GRID, y * GRID, GRID, GRID, 96 * 3, 96, 96, 96);
                break;
            case 12:
                Snowtile.Draw(hDC, x * GRID, y * GRID, GRID, GRID, 96 * 4, 0, 96, 96);
                break;
            case 13:
                Snowtile.Draw(hDC, x * GRID, y * GRID, GRID, GRID, 96 * 4, 96, 96, 96);
                break;
            case 14:
                Snowtile.Draw(hDC, x * GRID, y * GRID, GRID, GRID * 2, 96 * 5, 0, 96, 96);
                break;
            case 15:
                Snowtile.Draw(hDC, x * GRID, y * GRID, GRID, GRID * 2, 96 * 6, 0, 96, 96);
                break;
            case 16:
                Snowtile.Draw(hDC, x * GRID, y * GRID, GRID, GRID * 2, 96 * 7, 0, 96, 96);
                break;
            case 17:
                Snowtile.Draw(hDC, x * GRID, y * GRID, GRID, GRID, 96 * 8, 0, 96, 96);
                break;
            }
        }
    }
}
void DrawDesertTile(HDC hDC) {
    // 칸당 32x32
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            int tileType = tile1[y][x];
            switch (tileType) {
            case 1:
                Deserttile.Draw(hDC, x * GRID, y * GRID, GRID, GRID, 32 * 5, 0, 32, 32);
                break;
            case 2:
                Deserttile.Draw(hDC, x * GRID, y * GRID, GRID, GRID, 32 * 4, 0, 32, 32);
                break;
            case 3:
                Deserttile.Draw(hDC, x * GRID, y * GRID, GRID, GRID, 32 * 7, 0, 32, 32);
                break;
            case 4:
                Deserttile.Draw(hDC, x * GRID, y * GRID, GRID, GRID, 32 * 6, 0, 32, 32);
                break;
            case 5:
                Deserttile.Draw(hDC, x * GRID, y * GRID, GRID, GRID, 32 * 8, 0, 32, 32);
                break;
            case 6:
                Deserttile.Draw(hDC, x * GRID, y * GRID, GRID, GRID, 32 * 9, 0, 32, 32);
                break;
            case 7:
                Deserttile.Draw(hDC, x * GRID, y * GRID, GRID, GRID, 32 * 10, 0, 32, 32);
                break;
            case 8:
                Deserttile.Draw(hDC, x * GRID, y * GRID, GRID, GRID, 32 * 11, 0, 32, 32);
                break;
            case 9:
                Deserttile.Draw(hDC, x * GRID, y * GRID, GRID, GRID, 0, 0, 32, 32);
                break;
            case 10:
                Deserttile.Draw(hDC, x * GRID, y * GRID, GRID, GRID, 32 * 2, 0, 32, 32);
                break;
            case 11:
                Deserttile.Draw(hDC, x * GRID, y * GRID, GRID, GRID, 32 * 3, 0, 32, 32);
                break;
            case 12:
                Deserttile.Draw(hDC, x * GRID, y * GRID, GRID, GRID, 32 * 1, 0, 32, 32);
                break;
            }
        }
    }
}
void DrawSnowBg(HDC hDC) {
    Snowbg.StretchBlt(hDC, -GRID / 2, 0, BOARD_WIDTH, BOARD_HEIGHT, SRCCOPY);
}
void DrawDesertBg(HDC hDC) {
    Desertbg.StretchBlt(hDC, -GRID / 2, 0, BOARD_WIDTH, BOARD_HEIGHT, SRCCOPY);
}
void InitMap(int dst[MAP_HEIGHT][MAP_WIDTH], int src[MAP_HEIGHT][MAP_WIDTH]) {
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            dst[i][j] = src[i][j];
        }
    }
}
// 플레이어
void InitPlayer() {
    g_player.x = (MAP_WIDTH - 7) * GRID;
    g_player.y = (MAP_HEIGHT - 4) * GRID;
    g_player.dx = 0;
    g_player.dy = 0;
    g_player.jumpSpeed = 0;
    g_player.isCharging = false;
    g_player.isJumping = false;
    g_player.face = "left";
}

void MovePlayer(int map[MAP_HEIGHT][MAP_WIDTH]) {
    int newX = g_player.x + g_player.dx;
    int newY = g_player.y + g_player.dy;

    bool isVerticalCollision = IsColliding(map, g_player.x, newY);
    bool isHorizontalCollision = IsColliding(map, newX, g_player.y);
    bool isSlopeGoRightCollision = IsSlopeGoRightColliding(map, g_player.x, g_player.y);
    bool isSlopeGoLeftCollision = IsSlopeGoLeftColliding(map, g_player.x, g_player.y);

    // 수직 충돌 처리
    if (!isVerticalCollision) {
        g_player.y = newY;
        g_player.isJumping = true;
    }
    else {
        // 바닥 충돌 시 y축 위치 보정
        if (g_player.dy > 0) {
            while (!IsColliding(map, g_player.x, g_player.y + 1)) {
                g_player.y += 1;
            }
        }
        g_player.dy = 0; // 충돌 후 y축 속도 초기화
        g_player.isJumping = false;
        g_player.isSliding = false;
    }

    // 수평 충돌 처리
    if (!isHorizontalCollision) {
        g_player.x = newX;
    }
    else {
        g_player.dx = 0; // 충돌 후 x축 속도 초기화
    }

    if (isSlopeGoRightCollision) {
        g_player.isSliding = true;

        g_player.dy = 1; // 경사면 위에서 미끄러짐 속도
        g_player.dx = 2; // 오른쪽 아래로 미끄러짐
        newX = g_player.x + g_player.dx;
        newY = g_player.y + g_player.dy;
        g_player.x = newX;
        g_player.y = newY;
    }

    if (isSlopeGoLeftCollision) {
        g_player.isSliding = true;

        g_player.dy = 1; // 경사면 위에서 미끄러짐 속도
        g_player.dx = -2; // 오른쪽 아래로 미끄러짐
        newX = g_player.x + g_player.dx;
        newY = g_player.y + g_player.dy;
        g_player.x = newX;
        g_player.y = newY;
    }
}

void DrawSprite(HDC hDC, const int& x, const int& y, const int& width, const int& height) {
    HDC hmemDC = CreateCompatibleDC(hDC);
    HBITMAP oldBitmap = (HBITMAP)SelectObject(hmemDC, spriteSheetMask);
    if (g_player.face == "left") {
        StretchBlt(hDC, g_player.x - PLAYER_SIZE / 2, g_player.y - PLAYER_SIZE / 2, PLAYER_SIZE, PLAYER_SIZE, hmemDC, x, y, width, height, SRCAND);
        oldBitmap = (HBITMAP)SelectObject(hmemDC, spriteSheet);
        StretchBlt(hDC, g_player.x - PLAYER_SIZE / 2, g_player.y - PLAYER_SIZE / 2, PLAYER_SIZE, PLAYER_SIZE, hmemDC, x, y, width, height, SRCPAINT);
        SelectObject(hmemDC, oldBitmap);
    }
    else if (g_player.face == "right") {
        StretchBlt(hDC, g_player.x + PLAYER_SIZE / 2, g_player.y - PLAYER_SIZE / 2, -PLAYER_SIZE, PLAYER_SIZE, hmemDC, x, y, width, height, SRCAND);
        oldBitmap = (HBITMAP)SelectObject(hmemDC, spriteSheet);
        StretchBlt(hDC, g_player.x + PLAYER_SIZE / 2, g_player.y - PLAYER_SIZE / 2, -PLAYER_SIZE, PLAYER_SIZE, hmemDC, x, y, width, height, SRCPAINT);
        SelectObject(hmemDC, oldBitmap);
    }
    DeleteDC(hmemDC);
}

void ApplyGravity() {
    if (g_player.dy < 20) {
        g_player.dy += GRAVITY; // 중력 적용
    }
}

bool IsColliding(int map[MAP_HEIGHT][MAP_WIDTH], int x, int y) {
    int gridX = x / GRID;
    int gridY = y / GRID;

    if (gridX < 0 || gridX >= MAP_WIDTH || gridY < 0 || gridY >= MAP_HEIGHT) {
        return true;
    }

    if (map[gridY][gridX] == 0) {
        return true;
    }
    return false;
}

bool IsSlopeGoRightColliding(int map[MAP_HEIGHT][MAP_WIDTH], int x, int y) {
    int leftX = (x - PLAYER_SIZE / 2) / GRID;
    int rightX = (x + PLAYER_SIZE / 2 - 1) / GRID;
    int topY = (y - PLAYER_SIZE / 2) / GRID;
    int bottomY = (y + PLAYER_SIZE / 2 - 1) / GRID;

    // 충돌 감지
    if (map[bottomY][leftX] == 2 || map[bottomY][rightX] == 2) {
        return true;
    }
    return false;
}

bool IsSlopeGoLeftColliding(int map[MAP_HEIGHT][MAP_WIDTH], int x, int y) {
    int leftX = (x - PLAYER_SIZE / 2) / GRID;
    int rightX = (x + PLAYER_SIZE / 2 - 1) / GRID;
    int topY = (y - PLAYER_SIZE / 2) / GRID;
    int bottomY = (y + PLAYER_SIZE / 2 - 1) / GRID;

    // 충돌 감지
    if (map[bottomY][leftX] == 3 || map[bottomY][rightX] == 3) {
        return true;
    }
    return false;
}

bool IsNextColliding(int map[MAP_HEIGHT][MAP_WIDTH], int x, int y) {
    int leftX = (x - PLAYER_SIZE / 2) / GRID;
    int rightX = (x + PLAYER_SIZE / 2 - 1) / GRID;
    int topY = (y - PLAYER_SIZE / 2) / GRID;
    int bottomY = (y + PLAYER_SIZE / 2 - 1) / GRID;

    if (map[topY][leftX] == 6 || map[topY][rightX] == 6) {
        return true;
    }
    return false;
}

// 아이템
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
        Rectangle(hdc, item.x * GRID, item.y * GRID, (item.x + 1) * GRID, (item.y + 1) * GRID); // 아이템 그리기
        DeleteObject(hBrush1);
    }
}

// 적
void InitEnemy(int map[MAP_HEIGHT][MAP_WIDTH]) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (map[y][x] == 4) {  // 적
                GenerateEnemy(x, y);
            }
        }
    }
}

void GenerateEnemy(int x, int y) {
    Enemy newEnemy;
    newEnemy.x = x;
    newEnemy.y = y;
    g_enemies.push_back(newEnemy);
}

void DrawEnemies(HDC hDC) {
    for (const auto& enemy : g_enemies) {
        cannon.StretchBlt(hDC, enemy.x * GRID, enemy.y * GRID, GRID, GRID, SRCCOPY);
    }
}

void DeleteAllEnemies() {
    g_enemies.clear();
}

void ShootBullet() {
    for (const auto& enemy : g_enemies) {
        Bullet newBullet;
        newBullet.x = (enemy.x + 1) * GRID; // 적의 위치에서 총알이 나가도록 설정
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

void DeleteAllBullets() {
    g_bullets.clear();
}

// 충돌 확인 함수
void CheckCollisions() {
    //CheckItemPlayerCollisions();
    CheckPlayerBulletCollisions();
    CheckEnemyPlayerCollisions();
}

void CheckEnemyPlayerCollisions() {
    for (auto it = g_enemies.begin(); it != g_enemies.end(); ) {
        if (g_player.x >= it->x * GRID && g_player.x <= (it->x + 1) * GRID &&
            g_player.y >= it->y * GRID && g_player.y <= (it->y + 1) * GRID) {
            g_player.dx = 4;
            g_player.isCharging = false;
            g_player.jumpSpeed = 0;
            ++it; // 충돌 시 반복자를 증가시킵니다.
        }
        else {
            ++it; // 충돌이 발생하지 않았을 때도 반복자를 증가시킵니다.
        }
    }
}


void CheckItemPlayerCollisions() {
    for (auto it = g_items.begin(); it != g_items.end(); ) {
        if (it->x >= g_player.x - PLAYER_SIZE / 2 && it->x <= g_player.x + PLAYER_SIZE / 2 &&
            it->y >= g_player.y - PLAYER_SIZE / 2 && it->y <= g_player.y + PLAYER_SIZE / 2) {
            // 아이템과 충돌 시 제거
            it = g_items.erase(it);
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
            // 플레이어를 뒤로 밀침
            g_player.dx = it->dx * 2;
            g_player.isCharging = false;
            g_player.jumpSpeed = 0;
            // 플레이어와 충돌 시 제거
            it = g_bullets.erase(it);

        }
        else {
            ++it;
        }
    }
}
