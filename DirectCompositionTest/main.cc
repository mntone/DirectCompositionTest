#include "framework.h"
#include "app_window.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    app_window wnd(L"DIRECT_COMPOSITION_TEST", L"Direct Composition Test");
    HRESULT hr = wnd.show();
    if (FAILED(hr)) return -1;
    return wnd.run();
}
