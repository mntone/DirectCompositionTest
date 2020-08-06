#pragma once
#include "window.h"

class app_window final : public window {
public:
    app_window(const wchar_t* className, const wchar_t* title) noexcept
        : window(className, title)
    { }
    HRESULT init_before() noexcept;
    HRESULT init_after() noexcept;

    LRESULT window_procdure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept;
    HRESULT size_changed(WPARAM wParam, size size) noexcept;

private:
    //winrt::com_ptr<IDXGIFactory3> dxgiFactory;
    //winrt::com_ptr<ID3D11Device2> d3d11Device;
    winrt::com_ptr<ID2D1Device> d2d1Device;
    winrt::com_ptr<IDCompositionDesktopDevice> dCompositionDesktopDevice;
    winrt::com_ptr<IDCompositionDevice3> dCompositionDevice;

    winrt::com_ptr<IDCompositionSurface> dCompositionSurface;
    winrt::com_ptr<IDCompositionScaleTransform> backgroundTransform;
};
