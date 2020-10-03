/*
*	Copyright (C) 2020 Sidhin S Thomas
*
*	This file is part of knot.
*
*   knot is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   knot is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with knot.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "framework.h"

#include <shellapi.h>

const UINT WMAppNotifyIconCallback = WM_APP + 1;

namespace knot::UI
{
	class NotificationIcon
	{
	public:
		NotificationIcon(HINSTANCE hInstance, HWND hWnd);
		Win32Err CreateNotificationIcon();
	private:
		NOTIFYICONDATA m_iconData;
		HINSTANCE m_hInstance;
		HWND m_hWnd;
	};
}