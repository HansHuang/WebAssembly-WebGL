#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include <random>
#include <string>

#include "imgui.h"

using namespace ImGui;

namespace App {

static void ShowDemoBlotter(bool* p_open);

}
static void writeTable(int start, int count);
static char* randomStr(size_t len);
static void gerenateData(int size, int col);

static bool isVirtualization = true;
static bool loadData = false;
static bool realtimeData = false;
static int updateCounter = 0;
// static time_t t = time(NULL);
// static struct tm dataUpdatedTm = *localtime(&t);

static int dataSize = 10000;
static int colCount = 10;
static char*** dataSet = NULL;

void App::ShowDemoBlotter(bool* p_open) {
    SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);

    if (!Begin("Demo Blotter", p_open)) {
        End();
        return;
    }

    PushItemWidth(-GetWindowWidth() * 0.25f);
    Text("Currently all data is generated from memory");
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
        char title[32];
        for (int i = 0; i < colCount; i++) {
            sprintf(title, "Col_%d", i);
            TableSetupColumn(title, ImGuiTableColumnFlags_WidthStretch);
        }

        TableHeadersRow();
        if (loadData) {
            if (!dataSet) {
                gerenateData(dataSize, colCount);
            }
            if (realtimeData) {
                if (updateCounter >= 30) { //30 PFS/s
                    gerenateData(dataSize, colCount);
                    updateCounter = 0;
                }
                updateCounter++;
                // struct tm tm = *localtime(&t);
                // if (tm.tm_min >= dataUpdatedTm.tm_min && tm.tm_sec - dataUpdatedTm.tm_sec >= 1) {
                //     gerenateData(dataSize, colCount);
                //     dataUpdatedTm = *localtime(&t);
                // }
                // gerenateData(dataSize, colCount);
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
    // const char* str = dynamicData ? randomStr(rand() & 20) : "Cell";
    for (int row = start; row < count; row++) {
        TableNextRow();
        for (int column = 0; column < colCount; column++) {
            TableSetColumnIndex(column);
            // Text("%s %d %d", str, column, row);
            Text(dataSet[row][column]);
        }
    }
}

void gerenateData(int size, int col) {
    if (dataSet) {
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
            data[i][j] = randomStr(rand() & 10);
        }
    }
    dataSet = data;
}

char* randomStr(size_t len) {
    static char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";
    char* result = NULL;

    if (!len) return result;

    result = (char*)malloc(sizeof(char) * (len + 1));
    if (!result) return result;

    for (size_t i = 0; i < len; i++) {
        int key = rand() % (int)(sizeof(charset) - 1);
        result[i] = charset[key];
    }
    return result;
}