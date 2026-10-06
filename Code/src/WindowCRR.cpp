#include "WindowCRR.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>

#include <algorithm>
#include <cmath>
#include <string>
#include <vector>
#include <iostream> // TODO REMOVE

struct WindowState {
    int x = 100;
    int y = 100;
    int w = 800;
    int h = 600;

    int lastX = 0, lastY = 0, lastW = 0, lastH = 0;
    int stableFrames = 0;
    int cooldown = 0;
    bool wasMaximized = false;
    bool fullscreen = false;
};

WindowCRR::WindowCRR(CRR crr) {
    this->crr = crr;
}

static void updateWindowState(GLFWwindow* win, WindowState& st) {
    if (st.fullscreen || glfwGetWindowAttrib(win, GLFW_ICONIFIED))
        return;

    const bool maximized = glfwGetWindowAttrib(win, GLFW_MAXIMIZED) == GLFW_TRUE;

    if (st.wasMaximized && !maximized) {
        glfwSetWindowSize(win, st.w, st.h);
        glfwSetWindowPos(win, st.x, st.y);
        st.cooldown = 30;
        st.stableFrames = 0;
    }

    st.wasMaximized = maximized;

    if (maximized) {
        st.stableFrames = 0;
        return;
    }

    int x, y, w, h;
    glfwGetWindowPos(win, &x, &y);
    glfwGetWindowSize(win, &w, &h);

    const bool same = (x == st.lastX && y == st.lastY && w == st.lastW && h == st.lastH);
    st.stableFrames = same ? st.stableFrames + 1 : 0;
    st.lastX = x; st.lastY = y; st.lastW = w; st.lastH = h;

    if (st.cooldown > 0) {
        st.cooldown--;
        return;
    }

    if (st.stableFrames >= 15) {
        st.x = x; 
        st.y = y;
        st.w = w; 
        st.h = h;
    }
}

static void toggleFullscreen(GLFWwindow* window, WindowState& st) {
    if (!st.fullscreen) {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    } else {
        glfwSetWindowMonitor(window, nullptr, st.x, st.y, st.w, st.h, 0);
    }

    st.fullscreen = !st.fullscreen;
    st.wasMaximized = false;
    st.stableFrames = 0;
    st.cooldown = 30;
    glfwSwapInterval(1);
}

void WindowCRR::drawUI() {
    const ImGuiViewport* vp = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(vp->WorkPos);
    ImGui::SetNextWindowSize(vp->WorkSize);

    ImGui::Begin("Binomial Model", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

    ImGui::Text("Pricer d'option - Modele binomial (CRR)");

    ImGui::SeparatorText("Option");

    int oType = crr.getOType() == PUT ? 1 : 0;
    ImGui::RadioButton("Call", &oType, 0);
    ImGui::SameLine();
    ImGui::RadioButton("Put", &oType, 1);
    OptionType optionType = (oType == 1) ? PUT : CALL;
    crr.setOType(optionType);

    int eType = crr.getEType() == AMERICAN ? 1 : 0;
    ImGui::RadioButton("Europeenne", &eType, 0);
    ImGui::SameLine();
    ImGui::RadioButton("Americaine", &eType, 1);
    ExerciseType exerciseType = (eType == 1) ? AMERICAN : EUROPEAN;
    crr.setEType(exerciseType);

    ImGui::SeparatorText("Settings");

    ImGui::PushItemWidth(180.0f);

    double S = crr.getS0();
    ImGui::InputDouble("Spot (S)", &S, 1.0, 10.0, "%.5f");
    crr.setS0(S);

    double K = crr.getK();
    ImGui::InputDouble("Strike (K)", &K, 1.0, 10.0, "%.5f");
    crr.setK(K);

    double T = crr.getT();
    ImGui::InputDouble("Expiration (ans)", &T, 0.25, 1.0, "%.5f");
    crr.setT(T);

    double r = crr.getR();
    ImGui::InputDouble("Taux r", &r, 0.01, 0.05, "%.5f");
    crr.setR(r);
    
    double sigma = crr.getSigma();
    ImGui::InputDouble("Volatilite sigma", &sigma, 0.01, 0.05, "%.5f");
    crr.setSigma(sigma);

    int N = crr.getN();
    ImGui::InputInt("Nombre de pas N", &N, 10, 100);
    crr.setN(N);

    ImGui::PopItemWidth();

    crr.setN(std::clamp(crr.getN(), 1, 5000));

    ImGui::Spacing();
    if (ImGui::Button("Calculer", ImVec2(140, 30))) {
        // TODO
        std::cout << crr.crrOptionPrice() << std::endl;
    }

    ImGui::SameLine();
    if (ImGui::Button("Reinitialiser", ImVec2(140, 30)))
    {
       // TODO REINITIALISER
    }

    ImGui::SeparatorText("Results");

    if (crr.getComputed()) {
        if (ImGui::BeginTable("results", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit)) {
            ImGui::TableSetupColumn("Grandeur", ImGuiTableColumnFlags_WidthFixed, 200.0f);
            ImGui::TableSetupColumn("Valeur", ImGuiTableColumnFlags_WidthFixed, 160.0f);
            ImGui::TableHeadersRow();

            auto row = [](const char* label, double value)
            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn(); ImGui::TextUnformatted(label);
                ImGui::TableNextColumn(); ImGui::Text("%.6f", value);
            };

            row("Prix de l'option", crr.getResult());
            row("Delta", crr.getDt());
            row("Facteur de hausse u", crr.getU());
            row("Facteur de baisse d", crr.getD());
            row("Proba risque-neutre p", 67); //TODO

            ImGui::EndTable();
        }
    } else {
        ImGui::TextDisabled("Clique sur \"Calculer\" pour lancer le calcul.");
    }

    ImGui::End();
}

void WindowCRR::render(double spot, double strike, double priceResult) {
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(800, 600, "Cox-Ross-Rubinstein Model", nullptr, nullptr
    );

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    WindowState winState;
    bool f11WasDown = false;
    glfwGetWindowPos(window, &winState.x, &winState.y);
    glfwGetWindowSize(window, &winState.w, &winState.h);


    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        updateWindowState(window, winState);

        bool f11Down = glfwGetKey(window, GLFW_KEY_F11) == GLFW_PRESS;
        if (f11Down && !f11WasDown)
            toggleFullscreen(window, winState);
        f11WasDown = f11Down;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        drawUI();

        ImGui::Render();

        int width;
        int height;
        glfwGetFramebufferSize(window, &width, &height);

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}