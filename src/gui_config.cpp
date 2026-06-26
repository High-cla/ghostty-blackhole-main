// gui_config.cpp  ImGui config panel (Chinese, all parameters)
#include "gui_config.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

bool GUI_ShowConfigPanel(BlackholeConfig& cfg) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DECORATED, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* win = glfwCreateWindow(440, 540, "Black Hole Config", nullptr, nullptr);
    if (!win) return true;
    glfwMakeContextCurrent(win);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.IniFilename = nullptr;
    ImGui::StyleColorsDark();
    static const ImWchar ranges[] = { 0x0020, 0x00FF, 0x4E00, 0x9FFF, 0 };
    io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/msyh.ttc", 18.0f, nullptr, ranges);
    io.Fonts->Build();
    ImGui_ImplGlfw_InitForOpenGL(win, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    const char* modes[] = { "始终显示", "空闲检测" };

    bool done = false;
    while (!glfwWindowShouldClose(win) && !done) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(440, 540));
        ImGui::Begin("黑洞设置", nullptr,
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

        ImGui::TextColored(ImVec4(0.4f, 0.7f, 1.0f, 1.0f), "黑洞桌面叠加效果");
        ImGui::Separator();
        ImGui::Spacing();

        if (ImGui::CollapsingHeader("模式", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Combo("##mode", &cfg.mode, modes, 2);
            if (cfg.mode == 1) {
                ImGui::SliderInt("空闲超时(秒)", &cfg.idleSec, 10, 1800);
                ImGui::Text("  = %d 分钟", cfg.idleSec / 60);
            }
        }

        if (ImGui::CollapsingHeader("黑洞参数", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::SliderFloat("黑洞大小", &cfg.holeRadius, -1.0f, 0.10f, "%.4f");
            ImGui::SliderFloat("吸积盘亮度", &cfg.diskGain, -1.0f, 8.0f, "%.2f");
            ImGui::SliderFloat("色温 (K)", &cfg.diskTemp, -1.0f, 20000.0f, "%.0f");
            ImGui::SliderFloat("曝光度", &cfg.exposure, -1.0f, 5.0f, "%.2f");
            ImGui::SliderFloat("飘移速度", &cfg.speed, -1.0f, 5.0f, "%.2f");
            ImGui::SliderFloat("星空亮度", &cfg.starGain, -1.0f, 2.0f, "%.3f");
            ImGui::SliderFloat("盘面倾角", &cfg.diskIncl, -1.0f, 1.57f, "%.2f");
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::SetCursorPosX(150);
        if (ImGui::Button("启  动", ImVec2(140, 45))) {
            cfg.confirmed = true;
            done = true;
        }

        ImGui::Spacing();
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "ESC 退出  |  点击启动运行黑洞");

        ImGui::End();
        ImGui::Render();

        glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(win);

        if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            break;
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(win);

    return cfg.confirmed;
}