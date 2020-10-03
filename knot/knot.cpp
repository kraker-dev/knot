#include <windows.h>
#include <stdlib.h>
#include <string.h>

#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.system.h>
#include <winrt/windows.ui.xaml.hosting.h>
#include <windows.ui.xaml.hosting.desktopwindowxamlsource.h>
#include <winrt/windows.ui.xaml.controls.h>
#include <winrt/Windows.ui.xaml.media.h>

#include "NotificationIcon.h"
#include "Result.h"

using namespace winrt;
using namespace Windows::UI;
using namespace Windows::UI::Composition;
using namespace Windows::UI::Xaml::Hosting;
using namespace Windows::Foundation::Numerics;

using namespace knot::UI;

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

// The main window class name.
const wchar_t windowClassName[] = L"KnotDesktopApp";

HWND _hWnd = nullptr;
HWND _childhWnd = nullptr;
HWND _hWndXamlIsland = nullptr;
HINSTANCE _hInstance;

DesktopWindowXamlSource desktopSource; //TODO: Move XAML into a separate class and make it the primary way to create UI.

Win32Err CreateMainWindow(HWND& mainHWnd);
HRESULT InitXamlIsland(HWND hwnd, HWND& xamlhWnd, DesktopWindowXamlSource& desktopsource);

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    _hInstance = hInstance;

    Win32Err_RETURN_IF_FAILED(CreateMainWindow(_hWnd));
    RETURN_IF_FAILED(InitXamlIsland(_hWnd, _hWndXamlIsland, desktopSource));

    NotificationIcon notifyIcon(hInstance, _hWnd);
    Win32Err_RETURN_IF_FAILED(notifyIcon.CreateNotificationIcon());

    ShowWindow(_hWnd, nCmdShow);
    UpdateWindow(_hWnd);

    //Message loop:
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

Win32Err CreateMainWindow(HWND& reshWnd)
{
    WNDCLASSEX windowClass = { };

    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.lpfnWndProc = WindowProc;
    windowClass.hInstance = _hInstance;
    windowClass.lpszClassName = windowClassName;
    windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    windowClass.hIconSm = LoadIcon(windowClass.hInstance, IDI_APPLICATION);

    if (RegisterClassEx(&windowClass) == NULL)
    {
        MessageBox(NULL, L"Windows registration failed!", L"Error", NULL);
        return NULL;
    }

    reshWnd = CreateWindow(
        windowClassName,
        L"Knot | Create your notes anywhere",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        NULL,
        _hInstance,
        NULL
    );
    if (reshWnd == NULL)
    {
        MessageBox(NULL, L"Call to CreateWindow failed!", L"Error", NULL);
        return GetLastError();
    }

    return ERROR_SUCCESS;
}

HRESULT InitXamlIsland(HWND hwnd, HWND& xamlhWnd, DesktopWindowXamlSource& desktopsource)
{
    // Begin XAML Island section.

    // The call to winrt::init_apartment initializes COM; by default, in a multithreaded apartment.
    winrt::init_apartment(apartment_type::single_threaded);

    // Initialize the XAML framework's core window for the current thread.
    WindowsXamlManager winxamlmanager = WindowsXamlManager::InitializeForCurrentThread();

    // Get handle to the core window.
    auto interop = desktopSource.as<IDesktopWindowXamlSourceNative>();

    // Parent the DesktopWindowXamlSource object to the current window.
    RETURN_IF_FAILED (interop->AttachToWindow(hwnd));
    HWND xamlWindow;
    // Get the new child window's HWND. 
    RETURN_IF_FAILED(interop->get_WindowHandle(&xamlWindow));

    // Update the XAML Island window size because initially it is 0,0.
    RECT rect = {};
    GetClientRect(hwnd, &rect);
    SetWindowPos(xamlWindow, 0, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW);
    xamlhWnd = xamlWindow;

    // Create the XAML content.
    Windows::UI::Xaml::Controls::StackPanel xamlContainer;
    xamlContainer.Background(Windows::UI::Xaml::Media::SolidColorBrush{ Windows::UI::Colors::DimGray() });

    Windows::UI::Xaml::Controls::TextBox tb;
    tb.Text(L"Hello World from Xaml Islands!");
    tb.VerticalAlignment(Windows::UI::Xaml::VerticalAlignment::Center);
    tb.HorizontalAlignment(Windows::UI::Xaml::HorizontalAlignment::Center);

    xamlContainer.Children().Append(tb);
    xamlContainer.UpdateLayout();
    desktopSource.Content(xamlContainer);

    return S_OK;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT messageCode, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    RECT rcClient;

    switch (messageCode)
    {
    case WM_PAINT:
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

        // Create main window
    case WM_CREATE:
        return 0;

        // Main window changed size
    case WM_SIZE:
        // Get the dimensions of the main window's client
        // area, and enumerate the child windows. Pass the
        // dimensions to the child windows during enumeration.
        GetClientRect(hWnd, &rcClient);
        SetWindowPos(_hWndXamlIsland, nullptr, rcClient.left, rcClient.top, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, SWP_NOZORDER);
        return 0;

        // Process other messages.

    default:
        return DefWindowProc(hWnd, messageCode, wParam, lParam);
        break;
    }

    return 0;
}