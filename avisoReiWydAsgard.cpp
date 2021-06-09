#include <iostream>
#include <Windows.h>
#include <string>
#include <TlHelp32.h>
using namespace std;

DWORD pid;
CHAR target[400];
boolean nasceu = false;

std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return str;
}

DWORD GetModuleBase(const wchar_t* ModuleName, DWORD ProcessId) {
    // Esta estrutura contém muitas coisas boas sobre um módulo
    MODULEENTRY32 ModuleEntry = { 0 };
    // Obtenha um instantâneo de todos os módulos no processo especificado
    HANDLE SnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, ProcessId);

    if (!SnapShot)
        return NULL;

    // Você tem que inicializar o tamanho, caso contrário não funcionará
    ModuleEntry.dwSize = sizeof(ModuleEntry);

    // Obtenha o primeiro módulo do processo
    if (!Module32First(SnapShot, &ModuleEntry))
        return NULL;

    do {
        // Verifique se o nome do módulo corresponde ao que estamos procurando
        if (!wcscmp(ModuleEntry.szModule, ModuleName)) {
            // Em caso afirmativo, feche o identificador de instantâneo e retorne o endereço de base
            CloseHandle(SnapShot);
            return (DWORD)ModuleEntry.modBaseAddr;
        }
        // Pegue o próximo módulo no instantâneo
    } while (Module32Next(SnapShot, &ModuleEntry));

    // Não foi possível encontrar o módulo especificado, então retorne NULL
    CloseHandle(SnapShot);
    return NULL;
}

BOOL IsProcessRunning(DWORD pid)
{
    HANDLE process = OpenProcess(SYNCHRONIZE, FALSE, pid);
    DWORD ret = WaitForSingleObject(process, 0);
    CloseHandle(process);
    return ret == WAIT_TIMEOUT;
}

int main()
{
    std::wstring windowName;
    std::wcout << "Entre no canal desejado e fique dentro do deserto para iniciar.\n";

    std::wcout << "Para iniciar, digite o nome da janela: ";
    std::getline(std::wcin, windowName);
    int nrRand = rand() % 10 + 1;
    char letters[] = "abcdefghijklmnopqrstuvwxyz";
    char letRand = letters[rand() % 26];

    HWND hWnd = FindWindow(0, windowName.c_str());

    GetWindowThreadProcessId(hWnd, &pid);
    string newWindowName = letRand + std::to_string(nrRand);
    SetWindowTextA(hWnd, newWindowName.c_str());
    HANDLE pHandle = OpenProcess(PROCESS_VM_READ, FALSE, pid);
    DWORD rf_client = GetModuleBase(L"DoNPatch.dll", pid); 
    DWORD baseAddress = rf_client + 0x005642C;          
    DWORD address = 0; 
    ReadProcessMemory(pHandle, (void*)baseAddress, &address, sizeof(address), 0); 
    address += 0x59C;
    ReadProcessMemory(pHandle, (void*)address, &address, sizeof(address), 0);
    address += 0x0;

    while (true) {

        ReadProcessMemory(pHandle, (LPVOID)address, &target, 250, NULL);
        std::string s(target);
        s = ReplaceAll(s, std::string("Taurons!"), std::string(""));
        s = ReplaceAll(s, std::string("Faltam"), std::string(""));
        s = ReplaceAll(s, std::string(" "), std::string(""));
        std::string c(target);
        int numeroTauros = atoi(target);
        if (IsProcessRunning(pid)) {

            if (numeroTauros > 2) {
                cout << "Faltam" << s << "Taurons!" << endl;
                nasceu = false;
            }
            if (numeroTauros <= 10) {
                cout << target << endl;
                nasceu = true;
            }
        }
        else {
            cout << "A janela digitada encontra-se fechada!" << endl;
            Sleep(2000);
            main();

        }
        Sleep(100);
        system("CLS");
        if (nasceu) {
            cout << "O rei pode ter nascido!" << endl;
            //Sleep(60000);
            ///nasceu = false;
        }
    }
    system("Pause");
}
