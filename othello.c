#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

// 盤面の大きさ
#define BOARD_WIDTH 8
#define BOARD_HEIGHT 8

// ゲームモード c:vs CPU, p:vs PLAYER
char gamemode;

// 駒の場所
int cursorX, cursorY;

// プレイ中：1 終了：0
int bePlaying = 1;

// 駒の色
enum
{
    COLOR_NONE = 0,
    COLOR_BLACK = -1,
    COLOR_WHITE = 1,
    PIECES_MAX = BOARD_WIDTH * BOARD_HEIGHT
};

// 盤面
int cells[BOARD_HEIGHT][BOARD_WIDTH];

// ターン
int turn = -1;

// パスの回数
int passCount = 0;

/**
 * ひっくり返せるか判断し返す処理
 * @param int Y 確かめたいy座標
 * @param int X 確かめたいx座標
 * @return int totalReverseCount 何個返せたか
 **/
int judge(int Y, int X)
{
    // ひっくり返した回数
    int totalReverseCount = 0;

    // 全ての方向でひっくり返せるか確認
    for (int addX = -1; addX <= 1; addX++)
    {
        for (int addY = -1; addY <= 1; addY++)
        {
            int samePiece = 0;    // 直線上の同じピースの個数
            int reverseCount = 0; // ひっくり返した駒の数
            int y = Y;            // y座標
            int x = X;            // x座標

            // 駒がひっくり返せるかの判定
            while (1)
            {
                x = x + addX;
                y = y + addY;

                if (x < 0 || x >= BOARD_WIDTH || y < 0 || y >= BOARD_HEIGHT)
                {
                    break;
                }
                else if (cells[y][x] == 0)
                {
                    break;
                }
                else if (cells[y][x] == turn)
                {
                    samePiece += 1;
                    break;
                }
                else
                {
                    reverseCount = reverseCount + 1;
                }
            }

            // 1つ以上駒をひっくり返したかどうか
            if (reverseCount > 0)
            {
                if (samePiece > 0)
                {
                    totalReverseCount = totalReverseCount + reverseCount;
                }
            }
        }
    }
    return totalReverseCount;
}

/**
 * 駒を置いて相手の駒をひっくり返す
 * @param int Y y座標
 * @param int X x座標
 * @return int totalReverseCount 何個ひっくり返したか
 **/
int reverse(int Y, int X)
{
    // ひっくり返した回数
    int totalReverseCount = 0;

    // 全ての方向でひっくり返せるか確認
    for (int addX = -1; addX <= 1; addX++)
    {
        for (int addY = -1; addY <= 1; addY++)
        {
            // 配列のコピー
            int copyCells[BOARD_HEIGHT][BOARD_WIDTH];
            for (int i = 0; i < BOARD_HEIGHT; i++)
                for (int j = 0; j < BOARD_WIDTH; j++)
                    copyCells[i][j] = cells[i][j];

            int samePiece = 0;    // 直線上の同じピースの個数
            int reverseCount = 0; // ひっくり返した駒の数
            int y = Y;            // y座標
            int x = X;            // x座標

            // 駒がひっくり返せるかの判定
            while (1)
            {
                x = x + addX;
                y = y + addY;

                if (x < 0 || x >= BOARD_WIDTH || y < 0 || y >= BOARD_HEIGHT)
                {
                    break;
                }
                else if (cells[y][x] == 0)
                {
                    break;
                }
                else if (cells[y][x] == turn)
                {
                    samePiece += 1;
                    break;
                }
                else
                {
                    cells[y][x] = cells[y][x] * -1;
                    reverseCount = reverseCount + 1;
                }
            }

            // 1つ以上駒をひっくり返したかどうか
            if (reverseCount > 0)
            {
                // 同じコマに挟まれているか
                if (samePiece == 0)
                {
                    for (int i = 0; i < BOARD_HEIGHT; i++)
                    {

                        for (int j = 0; j < BOARD_WIDTH; j++)
                        {

                            cells[i][j] = copyCells[i][j];
                        }
                    }
                }
                else
                {
                    cells[Y][X] = turn;
                    totalReverseCount = totalReverseCount + reverseCount;
                }
            }
        }
    }
    return totalReverseCount;
}

// 自動で駒を置く
int computer()
{
    int maxReversePiecesCount = 0; // 一番多く駒を返せた個数
    int maxX = 0;                  // 一番多く駒を返した場所のx座標
    int maxY = 0;                  // 一番多く駒を返した場所のy座標

    for (int i = 0; i < BOARD_HEIGHT; i++)
    {
        for (int j = 0; j < BOARD_WIDTH; j++)
        {
            if (cells[i][j] == COLOR_NONE)
            {
                int result = judge(i, j);
                if (result > maxReversePiecesCount)
                {
                    maxReversePiecesCount = result;
                    maxY = i;
                    maxX = j;
                }
            }
        }
    }
    if (maxReversePiecesCount > 0)
    {
        int result = reverse(maxY, maxX);
        passCount = 0;
    }
    else
    {
        passCount = passCount + 1;
    }
    turn = turn * -1;
    return 0;
};

int main()
{
    // ゲームモード選択
    while (1)
    {
        printf("ゲームモードを選択してください\n\n");
        printf("VSCPU → PUSH c\n");
        printf("VSPLAYER → PUSH p\n");

        int ch = getch();

        if (ch == 'c')
        {
            gamemode = "c";
            break;
        }
        if (ch == 'p')
        {
            gamemode = "p";
            break;
        }
        system("cls");
    }

    // ゲーム開始
    cells[3][3] = cells[4][4] = -1;
    cells[4][3] = cells[3][4] = 1;
    while (bePlaying == 1)
    {
        system("cls");
        printf(" F:終了 P:パス その他のキー:駒を置く\n");
        printf(" カーソル移動 W:上 S:下 A:左 D:右\n\n");

        // コマ数を格納する変数
        int NUMBER_OF_BLACK_PIECES = 0;
        int NUMBER_OF_WHITE_PIECES = 0;
        int NUMBER_OF_NO_PIECE = 0;

        // 盤面の表示
        for (int y = 0; y < BOARD_WIDTH; y++)
        {
            for (int x = 0; x < BOARD_HEIGHT; x++)
            {
                // カーソルの表示
                if (x == cursorX && y == cursorY)
                {
                    printf(" ◎ ");

                    // 駒が無いマスの場合は1追加
                    if (cells[y][x] == COLOR_NONE)
                    {
                        NUMBER_OF_NO_PIECE = NUMBER_OF_NO_PIECE + 1;
                    }
                }
                else
                {
                    // 黒か白か何もないかを判断して表示
                    switch (cells[y][x])
                    {
                    case COLOR_NONE:
                        printf(" ・ ");
                        NUMBER_OF_NO_PIECE = NUMBER_OF_NO_PIECE + 1;
                        break;
                    case COLOR_BLACK:
                        printf(" ● ");
                        NUMBER_OF_BLACK_PIECES = NUMBER_OF_BLACK_PIECES + 1;
                        break;
                    case COLOR_WHITE:
                        printf(" ○ ");
                        NUMBER_OF_WHITE_PIECES = NUMBER_OF_WHITE_PIECES + 1;
                        break;

                    default:
                        break;
                    }
                }
            }
            printf("\n\n");
        }

        // ゲーム終了
        if (NUMBER_OF_NO_PIECE == 0 || passCount >= 4 || NUMBER_OF_BLACK_PIECES == 0 || NUMBER_OF_WHITE_PIECES == 0)
        {
            if (NUMBER_OF_WHITE_PIECES > NUMBER_OF_BLACK_PIECES)
            {
                printf("WHITE WIN");
            }
            else if (NUMBER_OF_BLACK_PIECES > NUMBER_OF_WHITE_PIECES)
            {
                printf("BLACK WIN");
            }
            else if (NUMBER_OF_WHITE_PIECES == NUMBER_OF_BLACK_PIECES)
            {
                printf("DRAW");
            }
            bePlaying = 0;
        }
        else
        {
            // 現在のターンの駒を表示
            switch (turn)
            {
            case -1:
                printf("BLACK TURN ● ");
                printf("\n");
                break;
            case 1:
                printf("WHITE TURN ○ ");
                printf("\n");
                break;
            default:
                break;
            }
        }

        // 移動処理と駒を置く処理
        switch (_getch())
        {
        case 'w':
            cursorY--;
            break;
        case 'a':
            cursorX--;
            break;
        case 's':
            cursorY++;
            break;
        case 'd':
            cursorX++;
            break;
        case 'f':
            bePlaying = 0;
            break;
        case 'p':
            turn = turn * -1;
            passCount = passCount + 1;
            if (gamemode == 'C' && turn == 1)
            {
                computer();
            }
            break;

        default:
            // 選んだマスに駒がおけるかどうか
            if (cells[cursorY][cursorX] == COLOR_NONE)
            {
                int result = reverse(cursorY, cursorX);
                // 1方向でも返せていればターンを変える
                if (result > 0)
                {
                    turn = turn * -1;
                    passCount = 0;
                    if (gamemode == 'C' && turn == 1)
                    {
                        computer();
                    }
                }
            }
            break;
        }

        // 盤面の制限
        if (cursorX < 0)
        {
            cursorX = 0;
        }
        else if (cursorX > BOARD_WIDTH - 1)
        {
            cursorX = BOARD_WIDTH - 1;
        }
        else if (cursorY < 0)
        {
            cursorY = 0;
        }
        else if (cursorY > BOARD_HEIGHT - 1)
        {
            cursorY = BOARD_HEIGHT - 1;
        }
    }
}
