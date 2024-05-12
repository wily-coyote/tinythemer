#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <dwmapi.h>

int revert = 0;

int setTheme(HWND hwnd, int dwm){
	BOOL iconic = !dwm;
	int k = DWMNCRP_DISABLED;
	if(dwm == 1){
		k = DWMNCRP_USEWINDOWSTYLE;
	}
	DwmSetWindowAttribute(
		hwnd,
		DWMWA_NCRENDERING_POLICY,
		&k,
		sizeof(DWMNCRP_DISABLED)
	);
	DwmSetWindowAttribute(
		hwnd,
		DWMWA_FORCE_ICONIC_REPRESENTATION,
		&iconic,
		sizeof(iconic)
	);
	return 0;
}

BOOL CALLBACK windowProc(HWND hwnd, LPARAM lparam){
	setTheme(hwnd, revert);
	return TRUE;
}

void CALLBACK eventProc(
	HWINEVENTHOOK hWinEventHook,
	DWORD event,
	HWND hwnd,
	LONG idObject,
	LONG idChild,
	DWORD idEventThread,
	DWORD dwmsEventTime){
	if (idObject != OBJID_WINDOW || idChild != CHILDID_SELF || event == EVENT_OBJECT_CREATE || hwnd == NULL) return;
	setTheme(hwnd, revert);
	return;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd){
	const char* revertCmdLine = "-reverse";
	revert = 0;
	if(strstr(lpCmdLine, revertCmdLine) != NULL){
		revert = 1;
	}
	EnumWindows(windowProc, 0);
	HWINEVENTHOOK createEvt = SetWinEventHook(EVENT_OBJECT_CREATE, EVENT_OBJECT_CREATE, 0, eventProc, 0, 0, WINEVENT_OUTOFCONTEXT|WINEVENT_SKIPOWNPROCESS);
	HWINEVENTHOOK fgEvt = SetWinEventHook(EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND, 0, eventProc, 0, 0, WINEVENT_OUTOFCONTEXT|WINEVENT_SKIPOWNPROCESS);
	MSG msg;
	while(GetMessage(&msg, NULL, 0, 0) > 0){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	UnhookWinEvent(createEvt);
	UnhookWinEvent(fgEvt);
	return 0;
}
