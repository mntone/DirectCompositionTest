#pragma once

class window_base
{
public:
    window_base(const wchar_t* className, const wchar_t* title) noexcept;
    ~window_base() noexcept;

    HRESULT init() noexcept;
    HRESULT show() noexcept;
    int run() noexcept;

    virtual HRESULT init_before() noexcept = 0;
    virtual HRESULT init_after() noexcept = 0;

    virtual LRESULT window_procdure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept;
    static LRESULT CALLBACK window_procdure_static(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept;

    HWND hwnd() const noexcept { return hWnd_; }

protected:
    HINSTANCE hInstance_;
    HWND hWnd_;

private:
    const wchar_t* className_;
    const wchar_t* title_;
    bool initialized_;
};

struct size {
    int width, height;
};

class window : public window_base
{
public:
    window(const wchar_t* className, const wchar_t* title) noexcept
        : window_base(className, title)
    { }

    LRESULT window_procdure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept;

    virtual HRESULT size_changed(WPARAM wParam, size size) noexcept;
    virtual void destory() noexcept;
};
