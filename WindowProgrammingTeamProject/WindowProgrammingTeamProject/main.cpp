#include <windows.h>
#include <vector>
#include <time.h>
#include <math.h>

const int BOARD_WIDTH = 800;
const int BOARD_HEIGHT = 600;
const int GRID = 40;
const int MAP_WIDTH = BOARD_WIDTH / GRID;
const int MAP_HEIGHT = BOARD_HEIGHT / GRID;
const int PLAYER_SIZE = 20;
const double M_PI = 3.141592;
const int GRAVITY = 1; // 중력 상수

int map[MAP_HEIGHT][MAP_WIDTH];

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
} g_player;

struct Item {
    int x, y;
    int type;
};
vector<Item> g_items;

struct Enemy {
    int x, y;
    int dx, dy;
};
vector<Enemy> g_enemies;

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

    hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 0, 0, BOARD_WIDTH + 16, BOARD_HEIGHT + 59, NULL, (HMENU)NULL, hInstance, NULL);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&Message, 0, 0, 0)) {
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }
    return Message.wParam;
}

// 함수

// 키입력
void ProcessKeyboardDown(WPARAM wParam) {
    switch (wParam) {
    case VK_LEFT:
        g_player.dx = -1;
        break;
    case VK_RIGHT:
        g_player.dx = 1;
        break;
    case VK_SPACE:
        if (g_player.dy == 0 && g_player.jumpSpeed > -25) { // 바닥에 닿아 있을 때만 점프 가능
            g_player.jumpSpeed -= 1;
        }        
        break;
    }
}

void ProcessKeyboardUp(WPARAM wParam) {
    switch (wParam) {
    case VK_LEFT:
    case VK_RIGHT:
        g_player.dx = 0;
        break;
    case VK_SPACE:
        g_player.dy = g_player.jumpSpeed;
        g_player.jumpSpeed = 0;
        break;
    }
}

// 맵
void InitMap() {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            map[y][x] = 0;
        }
    }
    int centerX = MAP_WIDTH / 2 - 1;
    int centerY = MAP_HEIGHT / 2;
    for (int y = centerY - 4; y <= centerY + 4; ++y) {
        for (int x = centerX - 4; x <= centerX + 4; ++x) {
            if (abs(x - centerX) + abs(y - centerY) <= 4) {
                if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT) {
                    map[y][x] = 1;
                }
            }
        }
    }
    for (int y = 0; y < MAP_HEIGHT / 2; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            map[y][x] = 1;
        }
    }
}

void DrawMap(HDC hdc) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            // 현재 맵의 값이 1이면 흰색(플레이어 영역), 0이면 검은색으로 그립니다.
            if (map[y][x] == 0) {
                // 검은색
                HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
                SelectObject(hdc, hBrush);
                Rectangle(hdc, x * GRID, y * GRID, (x + 1) * GRID, (y + 1) * GRID);
                DeleteObject(hBrush);
            }
            else if (map[y][x] == 1) {
                // 흰색
                HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
                SelectObject(hdc, hBrush);
                Rectangle(hdc, x * GRID, y * GRID, (x + 1) * GRID, (y + 1) * GRID);
                DeleteObject(hBrush);
            }
        }
    }
}

// 플레이어
void InitPlayer() {
    g_player.x = BOARD_WIDTH / 2 - 20;
    g_player.y = BOARD_HEIGHT / 2;
    g_player.dx = 0;
    g_player.dy = 0;
    g_player.jumpSpeed = 0;
}

void ApplyGravity() {
    g_player.dy += GRAVITY; // 중력 적용
}

bool IsColliding(int x, int y) {
    int leftX = (x - PLAYER_SIZE / 2) / GRID;
    int rightX = (x + PLAYER_SIZE / 2 - 1) / GRID;
    int topY = (y - PLAYER_SIZE / 2) / GRID;
    int bottomY = (y + PLAYER_SIZE / 2 - 1) / GRID;

    // 맵 경계를 벗어나지 않도록 체크
    if (leftX < 0 || rightX >= MAP_WIDTH || topY < 0 || bottomY >= MAP_HEIGHT) {
        return true;
    }

    // 충돌 감지
    if (map[topY][leftX] == 0 || map[topY][rightX] == 0 ||
        map[bottomY][leftX] == 0 || map[bottomY][rightX] == 0) {
        return true;
    }

    return false;
}

void MovePlayer() {
    int newX = g_player.x + g_player.dx * 3;
    int newY = g_player.y + g_player.dy;

    bool isVerticalCollision = IsColliding(g_player.x, newY);
    bool isHorizontalCollision = IsColliding(newX, g_player.y);

    // 수직 충돌 처리
    if (!isVerticalCollision) {
        g_player.y = newY;
    }
    else {
        // 바닥 충돌 시 y축 위치 보정
        if (g_player.dy > 0) {
            while (!IsColliding(g_player.x, g_player.y + 1)) {
                g_player.y += 1;
            }
        }
        g_player.dy = 0; // 충돌 후 y축 속도 초기화
    }

    // 수평 충돌 처리
    if (!isHorizontalCollision) {
        g_player.x = newX;
    }
    else {
        g_player.dx = 0; // 충돌 후 x축 속도 초기화
    }
}

void DrawPlayer(HDC hdc) {
    HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
    SelectObject(hdc, hBrush);
    Rectangle(hdc, g_player.x - PLAYER_SIZE / 2, g_player.y - PLAYER_SIZE / 2, g_player.x + PLAYER_SIZE / 2, g_player.y + PLAYER_SIZE / 2);
    DeleteObject(hBrush);
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

// 충돌 확인 함수
void CheckItemPlayerCollisions() {
    for (auto it = g_items.begin(); it != g_items.end(); ) {
        if (it->x >= g_player.x - PLAYER_SIZE / 2 && it->x <= g_player.x + PLAYER_SIZE / 2 &&
            it->y >= g_player.y - PLAYER_SIZE / 2 && it->y <= g_player.y + PLAYER_SIZE / 2) {
            it = g_items.erase(it); // 아이템과 충돌 시 제거
        }
        else {
            ++it;
        }
    }
}

void CheckCollisions() {
    CheckItemPlayerCollisions();
}

//메인 함수

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    PAINTSTRUCT ps;
    HDC hDC = BeginPaint(hWnd, &ps);
    HDC mDC;
    HBITMAP hBitmap;
    RECT rt;

    COLORREF COLOR_WHITE = RGB(255, 255, 255);
    COLORREF COLOR_BLACK = RGB(0, 0, 0);

    switch (message) {
    case WM_CREATE:
        InitMap();
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
            break;
        }
        InvalidateRect(hWnd, NULL, FALSE);
        break;
    case WM_PAINT:
    {
        GetClientRect(hWnd, &rt);
        mDC = CreateCompatibleDC(hDC);
        hBitmap = CreateCompatibleBitmap(hDC, rt.right, rt.bottom);
        SelectObject(mDC, (HBITMAP)hBitmap);

        //--- 모든 그리기를 메모리 DC에한다.
        DrawMap(mDC);
        DrawPlayer(mDC);

        // 메모리 DC에서 화면 DC로 그림을 복사
        BitBlt(hDC, 0, 0, BOARD_WIDTH, BOARD_HEIGHT, mDC, 0, 0, SRCCOPY);
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
