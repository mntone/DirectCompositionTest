#include "framework.h"
#include "window.h"

#include "resource.h"

window_base::window_base(const wchar_t* className, const wchar_t* title) noexcept
    : className_(className)
    , title_(title)
    , initialized_(false)
{ }

window_base::~window_base() noexcept
{
    BOOL result = UnregisterClassW(className_, hInstance_);
    if (result == FALSE)
    {
        DWORD err = GetLastError();
    }
}

HRESULT window_base::init() noexcept
{
    HRESULT hr = S_OK;
    if (initialized_) return hr;

    hr = init_before();
    if (FAILED(hr)) {
        return hr;
    }

    HINSTANCE hInstance = GetModuleHandleW(nullptr);
    if (hInstance == nullptr)
    {
        DWORD err = GetLastError();
        hr = HRESULT_FROM_WIN32(err);
        return hr;
    }

    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = window_base::window_procdure_static;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCEW(IDI_DIRECTCOMPOSITIONTEST));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_DIRECTCOMPOSITIONTEST);
    wcex.lpszClassName = className_;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCEW(IDI_SMALL));

    ATOM atom = RegisterClassExW(&wcex);
    if (atom == FALSE)
    {
        DWORD err = GetLastError();
        hr = HRESULT_FROM_WIN32(err);
        return hr;
    }

    HWND hWnd = CreateWindowExW(
        WS_EX_APPWINDOW | WS_EX_NOREDIRECTIONBITMAP,
        className_,
        title_,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0,
        nullptr,
        nullptr,
        hInstance,
        this);
    if (hWnd == nullptr) {
        DWORD err = GetLastError();
        hr = HRESULT_FROM_WIN32(err);
        return hr;
    }

    // Set variables
    hInstance_ = hInstance;
    hWnd_ = hWnd;

    hr = init_after();
    if (FAILED(hr)) {
        return hr;
    }

    initialized_ = true;
    return hr;
}

HRESULT window_base::show() noexcept {
    HRESULT hr = init();
    if (SUCCEEDED(hr)) {
        ShowWindow(hWnd_, SW_SHOW);
        UpdateWindow(hWnd_);
    }
    return S_OK;
}

int window_base::run() noexcept {
    HACCEL hAccelTable = LoadAccelerators(hInstance_, MAKEINTRESOURCEW(IDC_DIRECTCOMPOSITIONTEST));
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return static_cast<int>(msg.wParam);
}

LRESULT window_base::window_procdure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept {
    return DefWindowProcW(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK window_base::window_procdure_static(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept {
    LRESULT result = 0;
    if (message == WM_CREATE) {
        window_base* that = static_cast<window_base*>(reinterpret_cast<LPCREATESTRUCT>(lParam)->lpCreateParams);
        LONG_PTR ret = SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(that));
        if (ret == 0) {
            return ret;
        }
        result = that->window_procdure(hWnd, message, wParam, lParam);
    }
    else {
        window_base* that = reinterpret_cast<window_base*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        if (that) {
            result = that->window_procdure(hWnd, message, wParam, lParam);
        }
        else {
            result = DefWindowProcW(hWnd, message, wParam, lParam);
        }
    }
    return result;
}

LRESULT window::window_procdure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept {
    switch (message)
    {
    case WM_SIZE: {
        size size;
        size.width = GET_X_LPARAM(lParam);
        size.height = GET_Y_LPARAM(lParam);
        HRESULT hr = size_changed(wParam, size);
        return FAILED(hr) ? -1 : 0;
    }
        break;

    case WM_DESTROY:
        destory();
        break;
    }
    return DefWindowProcW(hWnd, message, wParam, lParam);
}

HRESULT window::size_changed(WPARAM wParam, size size) noexcept {
    return S_OK;
}

void window::destory() noexcept {
    PostQuitMessage(0);
}
