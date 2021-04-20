#include <stdint.h>
#include <stdlib.h>

#include <random>
#include <string>

#include "imgui.h"

namespace App {

static void ShowDemoBlotter(bool* p_open);

}
static void writeTable(int start, int count);
static char* randomStr(size_t len);

static bool loadData = false;
static bool dynamicData = false;
static bool isVirtualization = false;
static int dataSize = 10000;
static int colCount = 10;

void App::ShowDemoBlotter(bool* p_open) {
    ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin("Demo Blotter", p_open)) {
        ImGui::End();
        return;
    }

    ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.25f);

    ImGui::Text("Currently all data is generated from memory");

    ImGui::NewLine();
    ImGui::Checkbox("Load Data", &loadData);
    ImGui::SameLine(0, 20);
    ImGui::Checkbox("Dynamic Data", &dynamicData);
    ImGui::SameLine(0, 20);
    ImGui::Checkbox("Table Virtualization", &isVirtualization);
    ImGui::SliderInt("Data Size", &dataSize, 100, 100000, "%d rows");

    static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable | ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollX;

    if (ImGui::BeginTable("table1", colCount, flags)) {
        ImGui::TableSetupScrollFreeze(0, 1);
        char title[32];
        for (int i = 0; i < colCount; i++) {
            sprintf(title, "Col_%d", i);
            ImGui::TableSetupColumn(title, ImGuiTableColumnFlags_WidthStretch);
        }

        ImGui::TableHeadersRow();
        if (loadData) {
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
        ImGui::EndTable();
    }

    ImGui::PopItemWidth();
    ImGui::End();
}

void writeTable(int start, int count) {
    for (int row = start; row < count; row++) {
        ImGui::TableNextRow();
        for (int column = 0; column < colCount; column++) {
            ImGui::TableSetColumnIndex(column);
            const char* str = dynamicData ? randomStr(rand() & 20) : "Cell";
            ImGui::Text("%s %d %d", str, column, row);
        }
    }
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