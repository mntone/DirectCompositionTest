#include "framework.h"
#include "app_window.h"

#include "resource.h"

using namespace winrt;

HRESULT app_window::init_before() noexcept {
    HRESULT hr = S_OK;

    //hr = CreateDXGIFactory1(IID_IDXGIFactory3, dxgiFactory.put_void());

    com_ptr<ID3D11Device> device;
    hr = D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        D3D11_CREATE_DEVICE_BGRA_SUPPORT,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        device.put(),
        nullptr,
        nullptr);
    if (FAILED(hr)) return hr;

    //hr = device.as(IID_ID3D11Device2, d3d11Device.put_void());

    /*DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.BufferCount = 2;
    swapChainDesc.Scaling = DXGI_SCALING_NONE;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
    swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_PREMULTIPLIED;*/

    com_ptr<IDXGIDevice2> dxgiDevice;
    hr = device.as(IID_IDXGIDevice2, dxgiDevice.put_void());
    if (FAILED(hr)) return hr;

#if _DEBUG
    D2D1_DEBUG_LEVEL debugLevel = D2D1_DEBUG_LEVEL_ERROR;
#else
    D2D1_DEBUG_LEVEL debugLevel = D2D1_DEBUG_LEVEL_NONE;
#endif
    D2D1_CREATION_PROPERTIES props = D2D1::CreationProperties(
        D2D1_THREADING_MODE_SINGLE_THREADED,
        debugLevel,
        D2D1_DEVICE_CONTEXT_OPTIONS_NONE);

    hr = D2D1CreateDevice(dxgiDevice.get(), props, d2d1Device.put());
    if (FAILED(hr)) return hr;

    hr = DCompositionCreateDevice3(d2d1Device.get(), __uuidof(IDCompositionDesktopDevice), dCompositionDesktopDevice.put_void());
    if (FAILED(hr)) return hr;

    hr = dCompositionDesktopDevice.as(__uuidof(IDCompositionDevice3), dCompositionDevice.put_void());
    return hr;
}

HRESULT app_window::init_after() noexcept {
    HRESULT hr = S_OK;

    /*com_ptr<IDCompositionSurface> desktop;

    // WPF no HWND wo torikomu tokini tukau
    com_ptr<IDCompositionVisual2> desktopVisual;
    hr = dCompositionDesktopDevice->CreateVisual(desktopVisual.put());
    hr = dCompositionDesktopDevice->CreateSurfaceFromHwnd(GetDesktopWindow(), reinterpret_cast<IUnknown**>(desktop.put()));
    hr = desktopVisual->SetContent(dCompositionSurface.get());*/

    com_ptr<IDCompositionVisual2> outputVisual;
    hr = dCompositionDesktopDevice->CreateVisual(outputVisual.put());

    com_ptr<IDCompositionTarget> outputTarget;
    hr = dCompositionDesktopDevice->CreateTargetForHwnd(hWnd_, FALSE, outputTarget.put());
    hr = outputTarget->SetRoot(outputVisual.get());

    com_ptr<IDCompositionVisual2> backgroundVisual;
    hr = dCompositionDesktopDevice->CreateVisual(backgroundVisual.put());
    hr = dCompositionDesktopDevice->CreateSurface(256, 256, DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ALPHA_MODE_PREMULTIPLIED, dCompositionSurface.put());
    hr = dCompositionDesktopDevice->CreateScaleTransform(backgroundTransform.put());
    hr = backgroundVisual->SetTransform(backgroundTransform.get());

    /*com_ptr<IDCompositionGaussianBlurEffect> blurEffect;
    hr = dCompositionDevice->CreateGaussianBlurEffect(blurEffect.put());
    hr = blurEffect->SetStandardDeviation(16.F);

    com_ptr<IDCompositionBlendEffect> blendEffect;
    hr = dCompositionDevice->CreateBlendEffect(blendEffect.put());
    hr = blendEffect->SetMode(D2D1_BLEND_MODE_LINEAR_BURN);
    hr = blendEffect->SetInput(0, nullptr, 0);
    hr = blendEffect->SetInput(1, blendEffect.get(), 0);

    com_ptr<IDCompositionVisual2> blurVisual;
    hr = dCompositionDesktopDevice->CreateVisual(blurVisual.put());
    hr = blurVisual->SetContent(dCompositionSurface.get());*/
    //hr = backgroundVisual->SetEffect(blendEffect.get());

    //hr = dCompositionSourceVisual->SetEffect(blendEffect.get());

    //hr = dCompositionSourceVisual->AddVisual(desktopVisual.get(), FALSE, nullptr);
    hr = outputVisual->AddVisual(backgroundVisual.get(), FALSE, nullptr);
    //hr = dCompositionSourceVisual->AddVisual(blurVisual.get(), FALSE, nullptr);

    com_ptr<ID2D1DeviceContext> d2d1DeviceContext;
    POINT offset;
    hr = dCompositionSurface->BeginDraw(nullptr, IID_ID2D1DeviceContext, d2d1DeviceContext.put_void(), &offset);
    d2d1DeviceContext->Clear(D2D1::ColorF(0x666666, 1.0));
    hr = dCompositionSurface->EndDraw();
    hr = dCompositionDesktopDevice->Commit();
    if (FAILED(hr)) return hr;

    hr = dCompositionDesktopDevice->WaitForCommitCompletion();
    return hr;
}

HRESULT app_window::size_changed(WPARAM wParam, size size) noexcept {
    HRESULT hr = S_OK;
    hr = backgroundTransform->SetScaleX(static_cast<float>(size.width));
    if (FAILED(hr)) return hr;

    hr = backgroundTransform->SetScaleY(static_cast<float>(size.height));
    if (FAILED(hr)) return hr;

    hr = dCompositionDesktopDevice->Commit();
    if (FAILED(hr)) return hr;

    hr = dCompositionDesktopDevice->WaitForCommitCompletion();
    return hr;
}

LRESULT app_window::window_procdure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept {
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    /*case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: HDC を使用する描画コードをここに追加してください...
        EndPaint(hWnd, &ps);
    }
    break;*/
    }
    return window::window_procdure(hWnd, message, wParam, lParam);
}
