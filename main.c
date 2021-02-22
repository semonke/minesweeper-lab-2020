#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

#define N 10                                                    //二维阵列尺寸
#define M 10                                                    //地雷数量


int grid[N][N];                                                 //代表比赛场地的二维数组
int visible[N][N];                                              //包含细胞可见性信息的二维数组
int counter = 0;                                                //指示符

void ShowGrid(HANDLE  hConsole);
void Clean(int y, int x);
void Lost(HANDLE  hConsole);
void Won(HANDLE  hConsole);

int main()
{
    int iny, x, y;
    char inx;

    HANDLE  hConsole = GetStdHandle(STD_OUTPUT_HANDLE);         //一个用于改变控制台中文本颜色的变量

    srand(time(NULL));                                          //随机产生的比赛场地

    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            grid[i][j] = 0;
            visible[i][j] = 0;
        }
    }

    for (int i = 0; i < M; ++i)
    {
        do
        {
            x = rand()%10;
            y = rand()%10;
        }
        while (grid[x][y] != 0);
        grid[x][y] = -1;
    }

    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            if (grid[i][j] != -1)
            {
                if (i-1 >= 0 && grid[i-1][j] == -1)
                    grid[i][j]++;
                if (i-1 >= 0 && j+1 <= 9 && grid[i-1][j+1] == -1)
                    grid[i][j]++;
                if (j+1 <= 9 && grid[i][j+1] == -1)
                    grid[i][j]++;
                if (i+1 <= 9 && j+1 <= 9 && grid[i+1][j+1] == -1)
                    grid[i][j]++;
                if (i+1 <= 9 && grid[i+1][j] == -1)
                    grid[i][j]++;
                if (i+1 <= 9 && j-1 >= 0 && grid[i+1][j-1] == -1)
                    grid[i][j]++;
                if (j-1 >=0 && grid[i][j-1] == -1)
                    grid[i][j]++;
                if (i-1 >=0 && j-1 >= 0 && grid[i-1][j-1] == -1)
                    grid[i][j]++;
            }

        }
    }

    ShowGrid(hConsole);

    while (1)                                                    //游戏主循环
    {
        system("cls");

        ShowGrid(hConsole);

        printf("%d / %d\n", counter, N*N-M);

        printf("Insert X: ");                                   //用户输入处理
        scanf("%c", &inx);

        if (inx < 65 || inx > 65+N)
            continue;

        printf("Insert Y: ");
        scanf("%d", &iny);

        if (iny < 1 || iny > N)
            continue;

        x = inx - 65;
        y = iny - 1;

        if (visible[y][x] == 1)
            continue;

        if (grid[y][x] != -1)
        {
            Clean(y, x);
        }
        else
        {
            Lost(hConsole);
            break;
        }

        if (counter == (N*N-M))
        {
            Won(hConsole);
            break;
        }
    }

    return 0;
}

void ShowGrid(HANDLE  hConsole)                                  //赛场展示
{
    printf("   ");
    for (int i = 0; i < N; ++i)
        printf("%c ", 'A'+i);
    printf("\n");
    for (int i = 0; i < N; ++i)
    {
        if (i < 9)
            printf(" %d:", i+1);
        else
            printf("10:");
        for (int j = 0; j < N; ++j)
        {
            if (visible[i][j] != 0)
            {
                switch (grid[i][j])
                {
                    case 0:
                        SetConsoleTextAttribute(hConsole, 0);
                        break;
                    case 1:
                        SetConsoleTextAttribute(hConsole, 3);
                        break;
                    case 2:
                        SetConsoleTextAttribute(hConsole, 2);
                        break;
                    case 3:
                        SetConsoleTextAttribute(hConsole, 12);
                        break;
                    case 4:
                        SetConsoleTextAttribute(hConsole, 9);
                        break;
                    case 5:
                        SetConsoleTextAttribute(hConsole, 14);
                        break;
                    case 6:
                        SetConsoleTextAttribute(hConsole, 10);
                        break;
                    case 7:
                        SetConsoleTextAttribute(hConsole, 1);
                        break;
                    case 8:
                        SetConsoleTextAttribute(hConsole, 13);
                        break;
                    case 9:
                        SetConsoleTextAttribute(hConsole, 8);
                        break;
                }
                printf("%d ", grid[i][j]);
                SetConsoleTextAttribute(hConsole, 7);
            }
            else
                printf("# ");
        }
        printf("\n");
    }
}

void Clean(int y, int x)                                         //递归函数打开单元格
{
    if (y >= 0 && y <= 9 && x >= 0 && x <= 9 && visible[y][x] == 0)
    {
        visible[y][x] = 1;
        counter++;
        if (grid[y][x] == 0)
        {
            Clean(y-1, x);
            Clean(y-1, x+1);
            Clean(y, x+1);
            Clean(y+1, x+1);
            Clean(y+1, x);
            Clean(y+1, x-1);
            Clean(y, x-1);
            Clean(y-1, x-1);
        }
        else
            return;
    }
    else
        return;
}

void Lost(HANDLE  hConsole)                                      //输函数
{
    SetConsoleTextAttribute(hConsole, 12);
    system("cls");
    printf("Y O U  L O S E\n");
    SetConsoleTextAttribute(hConsole, 7);
    printf("   ");
    for (int i = 0; i < N; ++i)
        printf("%c ", 'A'+i);
    printf("\n");
    for (int i = 0; i < N; ++i)
    {
        if (i < 9)
            printf(" %d:", i+1);
        else
            printf("10:");
        for (int j = 0; j < N; ++j)
        {
            if (grid[i][j] != -1)
            {
                printf("%d ", grid[i][j]);
            }
            else
            {
                SetConsoleTextAttribute(hConsole, 12);
                printf("* ");
                SetConsoleTextAttribute(hConsole, 7);
            }
        }
        printf("\n");
    }
}

void Won(HANDLE  hConsole)                                       //胜利函数
{
    SetConsoleTextAttribute(hConsole, 2);
    system("cls");
    printf("Y O U  W I N\n");
    SetConsoleTextAttribute(hConsole, 7);
    printf("   ");
    for (int i = 0; i < N; ++i)
        printf("%c ", 'A'+i);
    printf("\n");
    for (int i = 0; i < N; ++i)
    {
        if (i < 9)
            printf(" %d:", i+1);
        else
            printf("10:");
        for (int j = 0; j < N; ++j)
        {
            if (grid[i][j] != -1)
            {
                printf("%d ", grid[i][j]);
            }
            else
            {
                SetConsoleTextAttribute(hConsole, 2);
                printf("* ");
                SetConsoleTextAttribute(hConsole, 7);
            }
        }
        printf("\n");
    }
}
