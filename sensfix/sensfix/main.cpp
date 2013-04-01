#include <Windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <fstream>
#include <string>

static void WINAPI Load();
void vp(DWORD address, int s);

float Sensitivity = 0.0125000002f;

HMODULE g_hMod = NULL;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		g_hMod = hModule;
		if ( CreateThread( 0, 0, (LPTHREAD_START_ROUTINE)Load, NULL, 0, 0) == NULL ) {
			ExitProcess(GetLastError());
			return FALSE; 
		}


	}
	return TRUE;
}

template < class T, class U >
void MemPutFast ( U ptr, const T value )
{
    *(T*)ptr = value;
}


static void WINAPI Load() {

	std::ifstream ifile("sensfix.cfg");
	if (ifile) {
		float sens;
		std::string key = "";
		while(ifile >> key >> sens) {
			if(key.compare("mainsens") == 0) {
				Sensitivity = sens;
			}
		}
	} else {
		std::ofstream ofile("sensfix.cfg");
		ofile << "mainsens 0.0125000002";

		ofile.close();
	}
	if(GetModuleHandle(L"gta-vc.exe") != NULL) {
		MemPutFast<float>(0x068B000, Sensitivity);
	} else if(GetModuleHandle(L"gta3.exe") != NULL) {
		MemPutFast<float>(0x05F063C, Sensitivity);
	}

	FreeLibraryAndExitThread(g_hMod, 0);
}