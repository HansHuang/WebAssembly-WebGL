#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#include <random>
#include <string>

#include "imgui.h"

using namespace ImGui;

namespace App {

static void ShowDemoBlotter(bool* p_open);

}
static void writeTable(int start, int count);
static char* randomStr(size_t size);
static void gerenateData(int size, int col);
static long long timeInMilliseconds();

static bool isVirtualization = true;
static bool loadData = true;
static bool realtimeData = false;
static long long dataUpdatedAt = timeInMilliseconds();

static int dataSize = 10000;
static int colCount = 10;
static char*** dataSet = NULL;
static int toFreeRow = 0;
static int toFreeCol = 0;

void App::ShowDemoBlotter(bool* p_open) {
    SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);

    if (!Begin("Demo Blotter", p_open)) {
        End();
        return;
    }

    PushItemWidth(-GetWindowWidth() * 0.25f);
    Text("Simulate real-time data blotter. Currently all data is generated from memory");
    NewLine();
    Checkbox("Table Virtualization", &isVirtualization);
    SameLine(0, 20);
    Checkbox("Load Data", &loadData);
    SameLine(0, 20);
    Checkbox("Realtime Data", &realtimeData);
    SliderInt("Data Size", &dataSize, 100, 100000, "%d rows");

    static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable | ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollX;

    if (BeginTable("table1", colCount, flags)) {
        TableSetupScrollFreeze(0, 1);
        char title[10];
        for (int i = 0; i < colCount; i++) {
            sprintf(title, "Col_%d", i);
            TableSetupColumn(title, ImGuiTableColumnFlags_WidthStretch);
        }

        TableHeadersRow();
        if (loadData) {
            if (realtimeData || !dataSet) {
                long long now = timeInMilliseconds();
                if (now - dataUpdatedAt >= 500) {  // update date 2x pre second
                    gerenateData(dataSize, colCount);
                }
            }

            if (isVirtualization) {
                ImGuiListClipper clipper;
                clipper.Begin(dataSize);
                while (clipper.Step()) {
                    writeTable(clipper.DisplayStart, clipper.DisplayEnd);
                }
            } else {
                writeTable(0, dataSize);
            }
        }
        EndTable();
    }

    PopItemWidth();
    End();
}

void writeTable(int start, int count) {
    for (int row = start; row < count; row++) {
        TableNextRow();
        for (int column = 0; column < colCount; column++) {
            TableSetColumnIndex(column);
            // Text("%s %d %d %d", "Cell", rand() % 10000, column, row);
            Text(dataSet[row][column]);
        }
    }
}

void gerenateData(int size, int col) {
    if (dataSet) {
        for (int i = 0; i < toFreeRow; i++) {
            for (int j = 0; j < toFreeCol; j++) {
                free(dataSet[i][j]);
            }
        }
        free(dataSet[0]);
        free(dataSet);
    }

    char*** data;
    // allocate array pointer
    data = (char***)malloc(sizeof(char**) * size);
    // allocate all memory
    data[0] = (char**)malloc(sizeof(char*) * size * col);

    for (int i = 0; i < size; i++) {
        if (i > 0) {
            // move point to next row
            data[i] = data[i - 1] + col;
        }

        for (int j = 0; j < col; j++) {
            data[i][j] = randomStr(rand() % 100);
        }
    }

    dataSet = data;
    dataUpdatedAt = timeInMilliseconds();
    toFreeRow = size;
    toFreeCol = col;
}

char* randomStr(size_t size) {
    char* str = new char[size + 1];
    for (size_t i = 0; i < size; ++i) {
        str[i] = '0' + rand() % 72;
    }
    str[size] = '\0';
    return str;
}

long long timeInMilliseconds() {
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return (((long long)tv.tv_sec) * 1000) + (tv.tv_usec / 1000);
}