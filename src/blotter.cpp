#include <stdint.h>
#include <stdlib.h>

#include "imgui.h"

static void ShowDemoBlotter(bool* p_open);
static void writeTable(int start, int count);

void ShowDemoBlotter(bool* p_open) {
    static bool loadData = false;
    static bool isVirtualization = false;
    static int dataSize = 10000;

#ifndef __EMSCRIPTEN__
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowViewport(rand());
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 120, main_viewport->WorkPos.y + 120), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);
#endif

    if (!ImGui::Begin("Demo Blotter", p_open)) {
        ImGui::End();
        return;
    }

    ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.25f);

    ImGui::Text("Currently all data is generated from memory");

    ImGui::NewLine();
    ImGui::Checkbox("Load Data", &loadData);
    ImGui::SameLine(0, 20);
    ImGui::Checkbox("Table Virtualization", &isVirtualization);

    static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable | ImGuiTableFlags_ScrollY;

    if (ImGui::BeginTable("table1", 3, flags)) {
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableSetupColumn("AAA", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("BBB", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("CCC", ImGuiTableColumnFlags_WidthStretch);
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
        for (int column = 0; column < 3; column++) {
            ImGui::TableSetColumnIndex(column);
            ImGui::Text("Hello %d,%d", column, row);
        }
    }
}