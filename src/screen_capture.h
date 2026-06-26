// screen_capture.h ? DXGI Desktop Duplication wrapper
#pragma once
#include <d3d11.h>
#include <dxgi1_2.h>
#include <cstdio>

struct ScreenCapture {
    ID3D11Device*           d3dDev     = nullptr;
    ID3D11DeviceContext*    d3dCtx     = nullptr;
    IDXGIOutputDuplication* dupl       = nullptr;
    ID3D11Texture2D*        stagingTex = nullptr;
    unsigned char*          pixelBuf   = nullptr;
    int width  = 0;
    int height = 0;
};

// Initialize D3D11 device + DXGI Desktop Duplication (call AFTER GL context is current)
bool scInit(ScreenCapture& sc);

// Create staging texture + pixel buffer for CPU readback
bool scInitStaging(ScreenCapture& sc);

// Acquire latest frame + copy to staging + return CPU pixel pointer (BGRA)
// Combines AcquireNextFrame, CopyResource, Map in one call.
// Returns nullptr on timeout or error.
const unsigned char* scCapture(ScreenCapture& sc);

// Release all resources
void scShutdown(ScreenCapture& sc);
