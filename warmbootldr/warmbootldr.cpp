// warmboot.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "pm.h"

volatile DWORD *ICMR = (DWORD*) 0xacd00004;
volatile DWORD *sleep_state_checksum = (DWORD*) 0xacf00008;
volatile DWORD *sleep_state = (DWORD*) 0xa4031000;

const int SLEEP_STATE_LEN = 0x59;
const LPWSTR WARMBOOT_PATH = L"\\Storage Card\\warmboot.bin";

// Midway thru OEMPowerOff - kernel build dependent
void (*enter_sleep)(void) = (void(*)(void))0x8007dc84;

DWORD checksum(DWORD *data, int count)
{
	DWORD sum = 0x5a72; // magic
	for (int i=0; i<count; i++) {
		sum += data[i];
		sum = (sum << 1) | (sum >> 0x1f); // rotate left 1
	}
	return sum;
}

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPTSTR    lpCmdLine,
                   int       nCmdShow)
{
	HANDLE hFile = CreateFile(WARMBOOT_PATH,
	                          GENERIC_READ,
	                          FILE_SHARE_READ,
	                          NULL,
	                          OPEN_EXISTING,
	                          FILE_ATTRIBUTE_NORMAL,
	                          NULL);

	if (hFile == INVALID_HANDLE_VALUE) {
		printf("[-] Failed to open '%ls' for reading.\n", WARMBOOT_PATH);
		getchar();
		return 0;
	}

	DWORD warmboot_len = GetFileSize(hFile, NULL);
	DWORD bytes_read;

	void * warmboot = malloc(warmboot_len);

	if (warmboot == NULL) {
		printf("[-] Failed to malloc buffer for warmboot code. (0x%lx bytes)\n", warmboot_len);
		getchar();
		return 0;
	}

	if (ReadFile(hFile, warmboot, warmboot_len, &bytes_read, NULL) == FALSE) {
		printf("[-] Failed to read '%ls'.\n", WARMBOOT_PATH);
		getchar();
		return 0;
	}

	if (bytes_read != warmboot_len) {
		printf("[-] Only read 0x%lx/0x%lx bytes.\n", bytes_read, warmboot_len);
		getchar();
		return 0;
	}

	printf("[+] Read 0x%lx bytes from '%ls'.\n", warmboot_len, WARMBOOT_PATH);
	CloseHandle(hFile);
	
	printf("[+] Setting warmboot entrypoint: 0x%08lx\n", warmboot);
	sleep_state[8] = (DWORD) warmboot;

	printf("[*] Checksum: 0x%08lx\n", *sleep_state_checksum);
	printf("[+] Fixing up sleep state checksum...\n");
	*sleep_state_checksum = checksum((DWORD*)sleep_state, SLEEP_STATE_LEN);
	printf("[*] Checksum: 0x%08lx\n", *sleep_state_checksum);
	
	printf("[+] Going to sleep...\n");
	Sleep(1000); // wait a bit, so we actually get to see the logs

	*ICMR = 0; // disable interrupts
	
	// TODO: relocate warmboot to fixed location?
	
	enter_sleep();

	printf("[!] Oh no, we woke up again. This shouldn't have happened...\n");
	getchar();
	return 0;
}

