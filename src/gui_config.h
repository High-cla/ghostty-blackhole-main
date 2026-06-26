// gui_config.h  ImGui config panel for blackhole parameters
#pragma once

struct BlackholeConfig {
    int   mode       = 0;     // 0=always, 1=idle
    int   idleSec    = 300;
    float holeRadius = -1.0f; // -1 = shader default
    float diskGain   = -1.0f;
    float diskTemp   = -1.0f;
    float exposure   = -1.0f;
    float speed      = -1.0f;
    float starGain   = -1.0f;
    float diskIncl   = -1.0f;
    bool  confirmed  = false;
};

bool GUI_ShowConfigPanel(BlackholeConfig& cfg);