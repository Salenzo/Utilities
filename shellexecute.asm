; fasm shellexecute.asm
format PE GUI 4.0
include "win32wx.inc"

.code
start:
	invoke GetModuleHandle, NULL
	mov ebx, eax
	invoke LoadString, ebx, 2, buffer, 32767
	lea edi, [buffer+eax*2]

	invoke GetCommandLine
	mov esi, eax
	xor eax, eax
	xor ecx, ecx
	.repeat
		lodsw
		cmp eax, '"'
		sete ch
		xor cl, ch
	.until ZERO? & eax = ' ' | eax = 0
	add esi, -2

	xor eax, eax
	mov ecx, 32767
copy:
	lodsw
	stosw
	test eax, eax
	loopnz copy

	push 0
	mov eax, esp
	invoke LoadString, ebx, 0, eax, 0
	pop eax
	invoke ShellExecute, NULL, "open", eax, buffer, NULL, SW_SHOWNORMAL
	.if eax <= 32
		invoke GetLastError
		mov ebx, eax
		push 0
		mov esi, esp
		; FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS
		invoke FormatMessage, 0x00001300, NULL, ebx, LANG_NEUTRAL, esi, 0, NULL
		invoke MessageBox, HWND_DESKTOP, dword [esi], buffer, MB_ICONWARNING
	.else
		xor ebx, ebx
	.endif
	invoke ExitProcess, ebx
.end start

.data
	buffer rw 65536

section ".rsrc" resource data readable
	directory RT_ICON, icons, RT_GROUP_ICON, group_icons, RT_STRING, strings, RT_VERSION, versions

	resource icons, 1, LANG_NEUTRAL, icon_data
	resource group_icons, 1, LANG_NEUTRAL, main_icon
	icon main_icon, icon_data, "shellexecute.ico"

	resource strings, 1, LANG_NEUTRAL, string_data
	resdata string_data
		dw 17
		du "C:\Windows\py.exe"
		dw 0
		dw 17
		du "<arg1> <arg2> ..."
		rw 13
	endres

	resource versions, 1, LANG_NEUTRAL, version
	versioninfo version, VOS__WINDOWS32, VFT_APP, VFT2_UNKNOWN, LANG_NEUTRAL, 0, \
		"FileDescription", "<replace this with description>", \
		"OriginalFilename", "ShellExecute.exe"
