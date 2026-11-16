#include "gui.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include "imgui.h"
#include "implot.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"

void run_gui(SignalData &data) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#ifdef __APPLE__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
#endif
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_Window *window = SDL_CreateWindow("SDR",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 550,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_GLContext context = SDL_GL_CreateContext(window);
    SDL_GL_SetSwapInterval(1);

    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGui::GetIO().IniFilename = nullptr;
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForOpenGL(window, context);
    ImGui_ImplOpenGL3_Init("#version 330");

    const ImPlotFlags plot_flags = ImPlotFlags_NoMenus | ImPlotFlags_NoInputs;

    while (data.running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT ||
                (event.type == SDL_WINDOWEVENT &&
                 event.window.event == SDL_WINDOWEVENT_CLOSE &&
                 event.window.windowID == SDL_GetWindowID(window))) {
                data.running = false;
            }
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        const ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::Begin("Plots", nullptr, ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);

        const ImVec2 available = ImGui::GetContentRegionAvail();
        const float width = (available.x - ImGui::GetStyle().ItemSpacing.x) / 2;
        const int count = static_cast<int>(data.samples.size());
        if (ImPlot::BeginPlot("Constellation", ImVec2(width, available.y),
                             plot_flags | ImPlotFlags_NoLegend | ImPlotFlags_Equal)) {
            ImPlot::SetupAxes("I", "Q");
            ImPlot::SetupAxesLimits(-2500, 2500, -2500, 2500);
            ImPlot::PlotScatterG("IQ", [](int index, void *ptr) {
                auto &signal = *static_cast<SignalData *>(ptr);
                return ImPlotPoint(signal.samples[index].real(), signal.samples[index].imag());
            }, &data, count);
            ImPlot::EndPlot();
        }

        ImGui::SameLine();
        ImPlot::SetNextAxesToFit();
        if (ImPlot::BeginPlot("Amplitude vs time", ImVec2(width, available.y), plot_flags)) {
            ImPlot::SetupAxes("Time, ms", "Amplitude");
            ImPlot::PlotLineG("I", [](int index, void *ptr) {
                auto &signal = *static_cast<SignalData *>(ptr);
                return ImPlotPoint(1000.0 * index / signal.sample_rate, signal.samples[index].real());
            }, &data, count);
            ImPlot::PlotLineG("Q", [](int index, void *ptr) {
                auto &signal = *static_cast<SignalData *>(ptr);
                return ImPlotPoint(1000.0 * index / signal.sample_rate, signal.samples[index].imag());
            }, &data, count);
            ImPlot::PlotLineG("|IQ|", [](int index, void *ptr) {
                auto &signal = *static_cast<SignalData *>(ptr);
                return ImPlotPoint(1000.0 * index / signal.sample_rate, std::abs(signal.samples[index]));
            }, &data, count);
            ImPlot::EndPlot();
        }
        ImGui::End();

        ImGui::Render();
        int width_pixels, height_pixels;
        SDL_GL_GetDrawableSize(window, &width_pixels, &height_pixels);
        glViewport(0, 0, width_pixels, height_pixels);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
