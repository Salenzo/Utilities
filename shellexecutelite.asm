; fasm shellexecutelite.asm
format PE GUI 4.0
include "win32wx.inc"

.code
start:
	invoke MultiByteToWideChar, CP_UTF8, 0, filename_utf8, -1, filename, 2047
	invoke MultiByteToWideChar, CP_UTF8, 0, parameters_utf8, -1, parameters, 2047
	invoke ShellExecute, NULL, "open", filename, parameters, NULL, SW_SHOWNORMAL
	.if eax <= 32
		invoke GetLastError
		mov ebx, eax
		push 0
		mov esi, esp
		; FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS
		invoke FormatMessage, 0x00001300, NULL, ebx, LANG_NEUTRAL, esi, 0, NULL
		invoke MessageBox, HWND_DESKTOP, dword [esi], filename, MB_ICONWARNING
	.else
		xor ebx, ebx
	.endif
	invoke ExitProcess, ebx
.end start

.data
	align 16
	filename_utf8 db "C:\Windows\system32\notepad.exe"
	rb 2017
	parameters_utf8 db "C:\Windows\win.ini"
	rb 2030
	db "END"
	filename rw 2048
	parameters rw 2048
