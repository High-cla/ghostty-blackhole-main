// screen_capture.cpp ? DXGI Desktop Duplication + staging readback
// 
// Critical: scInit() must be called AFTER the OpenGL context is current.
// Creating a GL context on the same GPU can invalidate pre-existing D3D11 resources.
#include "screen_capture.h"
#include <cstring>
#include <windows.h>

bool scInit(ScreenCapture& sc) {
    sc.width  = GetSystemMetrics(SM_CXSCREEN);
    sc.height = GetSystemMetrics(SM_CYSCREEN);

    D3D_FEATURE_LEVEL featLevel;
    HRESULT hr = D3D11CreateDevice(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
        D3D11_CREATE_DEVICE_BGRA_SUPPORT,
        nullptr, 0, D3D11_SDK_VERSION,
        &sc.d3dDev, &featLevel, &sc.d3dCtx);
    if (FAILED(hr)) {
        fprintf(stderr, "[DXGI] D3D11CreateDevice failed: 0x%08X\n", (unsigned)hr);
        return false;
    }

    IDXGIDevice* dxgiDev = nullptr;
    hr = sc.d3dDev->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDev);
    if (FAILED(hr)) { scShutdown(sc); return false; }

    IDXGIAdapter* adapter = nullptr;
    hr = dxgiDev->GetAdapter(&adapter);
    dxgiDev->Release();
    if (FAILED(hr)) { scShutdown(sc); return false; }

    IDXGIOutput* output = nullptr;
    hr = adapter->EnumOutputs(0, &output);
    adapter->Release();
    if (FAILED(hr)) { scShutdown(sc); return false; }

    IDXGIOutput1* output1 = nullptr;
    hr = output->QueryInterface(__uuidof(IDXGIOutput1), (void**)&output1);
    output->Release();
    if (FAILED(hr)) { scShutdown(sc); return false; }

    hr = output1->DuplicateOutput(sc.d3dDev, &sc.dupl);
    output1->Release();
    if (FAILED(hr)) {
        fprintf(stderr, "[DXGI] DuplicateOutput failed: 0x%08X\n", (unsigned)hr);
        scShutdown(sc);
        return false;
    }

    fprintf(stderr, "[DXGI] Desktop capture ready: %dx%d\n", sc.width, sc.height);
    return true;
}

bool scInitStaging(ScreenCapture& sc) {
    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width          = sc.width;
    desc.Height         = sc.height;
    desc.MipLevels      = 1;
    desc.ArraySize      = 1;
    desc.Format         = DXGI_FORMAT_B8G8R8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage          = D3D11_USAGE_STAGING;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    desc.BindFlags      = 0;

    HRESULT hr = sc.d3dDev->CreateTexture2D(&desc, nullptr, &sc.stagingTex);
    if (FAILED(hr)) {
        fprintf(stderr, "[DXGI] Staging texture failed: 0x%08X\n", (unsigned)hr);
        return false;
    }

    sc.pixelBuf = new unsigned char[sc.width * sc.height * 4];
    fprintf(stderr, "[DXGI] Staging ready: %.1f MB\n",
            (sc.width * sc.height * 4) / 1048576.0);
    return true;
}

const unsigned char* scCapture(ScreenCapture& sc) {
    if (!sc.dupl || !sc.stagingTex || !sc.pixelBuf) return nullptr;

    // Release previous frame before acquiring new one
    sc.dupl->ReleaseFrame();

    IDXGIResource* frameRes = nullptr;
    DXGI_OUTDUPL_FRAME_INFO frameInfo = {};
    HRESULT hr = sc.dupl->AcquireNextFrame(25, &frameInfo, &frameRes);

    if (hr == DXGI_ERROR_WAIT_TIMEOUT)
        return nullptr;  // No new frame; caller reuses previous data
    if (FAILED(hr)) {
        // ACCESS_LOST or other error: silently skip, try again next frame
        return nullptr;
    }

    ID3D11Texture2D* desktopTex = nullptr;
    hr = frameRes->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&desktopTex);
    frameRes->Release();

    if (FAILED(hr) || !desktopTex) {
        sc.dupl->ReleaseFrame();
        return nullptr;
    }

    // GPU copy: desktop -> staging
    sc.d3dCtx->CopyResource(sc.stagingTex, desktopTex);
    desktopTex->Release();

    // Release dupl frame immediately after CopyResource
    sc.dupl->ReleaseFrame();

    // CPU map: staging -> pixel buffer
    D3D11_MAPPED_SUBRESOURCE mapped;
    hr = sc.d3dCtx->Map(sc.stagingTex, 0, D3D11_MAP_READ, 0, &mapped);
    if (FAILED(hr)) return nullptr;

    int rowSize = sc.width * 4;
    if (mapped.RowPitch == (UINT)rowSize) {
        memcpy(sc.pixelBuf, mapped.pData, rowSize * sc.height);
    } else {
        for (int y = 0; y < sc.height; y++)
            memcpy(sc.pixelBuf + y * rowSize,
                   (unsigned char*)mapped.pData + y * mapped.RowPitch, rowSize);
    }
    sc.d3dCtx->Unmap(sc.stagingTex, 0);

    return sc.pixelBuf;
}

void scShutdown(ScreenCapture& sc) {
    if (sc.pixelBuf)   { delete[] sc.pixelBuf;   sc.pixelBuf   = nullptr; }
    if (sc.stagingTex) { sc.stagingTex->Release(); sc.stagingTex = nullptr; }
    if (sc.dupl)       { sc.dupl->Release();       sc.dupl       = nullptr; }
    if (sc.d3dCtx)     { sc.d3dCtx->Release();     sc.d3dCtx     = nullptr; }
    if (sc.d3dDev)     { sc.d3dDev->Release();     sc.d3dDev     = nullptr; }
}
