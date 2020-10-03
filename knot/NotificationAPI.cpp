#include "NotificationIcon.h"
#include "framework.h"
#include "Result.h"
#include "Resource.h"

#include <shellapi.h>
#include <commctrl.h>

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "comctl32.lib")

class __declspec(uuid("04154841-ABDC-478D-822E-047E9FF23A70")) KnotIcon;

namespace knot::UI
{
	NotificationIcon::NotificationIcon(HINSTANCE hInstance, HWND hWnd)
	{
		m_hInstance = hInstance;
		m_hWnd = hWnd;
		m_iconData = {};

		m_iconData.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE | NIF_SHOWTIP | NIF_GUID;
		m_iconData.guidItem = __uuidof(KnotIcon);
		m_iconData.uCallbackMessage = WMAppNotifyIconCallback;
		wcscpy_s(m_iconData.szTip, L"Create Notes quickly");

		LoadIconMetric(m_hInstance, MAKEINTRESOURCE(IDI_KNOT), LIM_SMALL, &m_iconData.hIcon);
	}

	Win32Err NotificationIcon::CreateNotificationIcon()
	{
		BOOL_RETURN_E_IF_FAILED(Shell_NotifyIcon(NIM_ADD, &m_iconData));

		m_iconData.uVersion = NOTIFYICON_VERSION_4;
		BOOL_RETURN_E_IF_FAILED(Shell_NotifyIcon(NIM_SETVERSION, &m_iconData));
		return ERROR_SUCCESS;
	}
}