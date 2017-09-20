#ifndef _NOTIFYWINDOW_H_
#define _NOTIFYWINDOW_H_

#include <boost/noncopyable.hpp>
#include "Win_ManualResetEvent.h"
#include "Win_Thread.h"

namespace DeviceDetectLibrary
{
	class IDeviceChanged;

	class CNotifyWindow : boost::noncopyable
	{
	public:
		explicit CNotifyWindow(IDeviceChanged& deviceChanged);
		virtual ~CNotifyWindow(void);

		HWND GetHWND(void) const;

	private:
		void NotifyRegisterClass(HINSTANCE hInstance);
		bool InitInstance(HINSTANCE hInstance);

	private:
		static void ThreadProc(void* context);

	private:
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		HWND hWnd_;
		bool destroy_;
		CThread thread_;
		CManualResetEvent started_;
		IDeviceChanged& deviceChanged_;
	};
}

#endif // _NOTIFYWINDOW_H_