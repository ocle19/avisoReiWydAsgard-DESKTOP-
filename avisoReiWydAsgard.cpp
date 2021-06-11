#include <iostream>
#include <Windows.h>
#include <string>
#include <TlHelp32.h>
#include <fstream>  
#include <conio.h>

using namespace std;

DWORD pid;
CHAR target[250];
boolean nasceu = false;
int addressNickname;
int addressCoordenadasReiTauron;
CHAR nickname[250];
CHAR coodenadasReiTauron[250];
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

void cls(HANDLE hConsole)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    SMALL_RECT scrollRect;
    COORD scrollTarget;
    CHAR_INFO fill;

    // Get the number of character cells in the current buffer.
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
    {
        return;
    }

    // Scroll the rectangle of the entire buffer.
    scrollRect.Left = 0;
    scrollRect.Top = 0;
    scrollRect.Right = csbi.dwSize.X;
    scrollRect.Bottom = csbi.dwSize.Y;

    // Scroll it upwards off the top of the buffer with a magnitude of the entire height.
    scrollTarget.X = 0;
    scrollTarget.Y = (SHORT)(0 - csbi.dwSize.Y);

    // Fill with empty spaces with the buffer's default text attribute.
    fill.Char.UnicodeChar = TEXT(' ');
    fill.Attributes = csbi.wAttributes;

    // Do the scroll
    ScrollConsoleScreenBuffer(hConsole, &scrollRect, NULL, scrollTarget, &fill);

    // Move the cursor to the top left corner too.
    csbi.dwCursorPosition.X = 0;
    csbi.dwCursorPosition.Y = 0;

    SetConsoleCursorPosition(hConsole, csbi.dwCursorPosition);
}



std::wstring s2ws(const std::string& s)
{
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}

int main()
{
    setlocale(LC_ALL, "Portuguese");

    std::wstring windowName;
    std::string newWindowName;

    std::wcout << "Entre no canal desejado e fique dentro do deserto para iniciar.\n";
    std::wcout << "Para iniciar, digite o nome da janela: ";
    std::getline(std::wcin, windowName);



    /// LPCWSTR windowName = L"c";

   /// HWND hWnd = FindWindow(0, windowName.c_str());


   /// LPCWSTR gameone = L"c";

    HWND hWnd = FindWindow(0, windowName.c_str());

    GetWindowThreadProcessId(hWnd, &pid);
    HANDLE pHandle = OpenProcess(PROCESS_VM_READ, FALSE, pid);
    DWORD rf_client = GetModuleBase(L"DoNPatch.dll", pid);
    DWORD baseAddress = rf_client + 0x005642C;
    DWORD address = 0;
    ReadProcessMemory(pHandle, (void*)baseAddress, &address, sizeof(address), 0);
    address += 0x59C;
    ReadProcessMemory(pHandle, (void*)address, &address, sizeof(address), 0);
    address += 0x0;

    DWORD baseAddressCoordenadasReiTauron = rf_client + 0x005642C;
    DWORD addressCoordenadasReiTauron = 0;
    ReadProcessMemory(pHandle, (void*)baseAddressCoordenadasReiTauron, &addressCoordenadasReiTauron, sizeof(addressCoordenadasReiTauron), 0);
    addressCoordenadasReiTauron += 0x61C;
    ReadProcessMemory(pHandle, (void*)addressCoordenadasReiTauron, &addressCoordenadasReiTauron, sizeof(addressCoordenadasReiTauron), 0);
    addressCoordenadasReiTauron += 0x0;

    DWORD rf_client2 = GetModuleBase(L"SD Asgard.exe", pid);
    DWORD baseAddressNickname = rf_client2 + 0x01EDD54;
    DWORD addressNickname = 0;
    ReadProcessMemory(pHandle, (void*)baseAddressNickname, &addressNickname, sizeof(addressNickname), 0);
    addressNickname += 0x0;

    if (IsProcessRunning(pid)) {

        std::wcout << "Agora digite o NOVO nome para a janela: ";
        std::getline(std::cin, newWindowName);
        std::wstring windowName = s2ws(newWindowName.c_str());
        LPCWSTR result = windowName.c_str();
        SetWindowText(hWnd, result);

        HWND hWnd = FindWindow(0, result);

        GetWindowThreadProcessId(hWnd, &pid);
        HANDLE pHandle = OpenProcess(PROCESS_VM_READ, FALSE, pid);
        DWORD rf_client = GetModuleBase(L"DoNPatch.dll", pid);
        DWORD baseAddress = rf_client + 0x005642C;
        DWORD address = 0;
        ReadProcessMemory(pHandle, (void*)baseAddress, &address, sizeof(address), 0);
        address += 0x59C;
        ReadProcessMemory(pHandle, (void*)address, &address, sizeof(address), 0);
        address += 0x0;

        DWORD baseAddressCoordenadasReiTauron = rf_client + 0x005642C;
        DWORD addressCoordenadasReiTauron = 0;
        ReadProcessMemory(pHandle, (void*)baseAddressCoordenadasReiTauron, &addressCoordenadasReiTauron, sizeof(addressCoordenadasReiTauron), 0);
        addressCoordenadasReiTauron += 0x61C;
        ReadProcessMemory(pHandle, (void*)addressCoordenadasReiTauron, &addressCoordenadasReiTauron, sizeof(addressCoordenadasReiTauron), 0);
        addressCoordenadasReiTauron += 0x0;

        DWORD rf_client2 = GetModuleBase(L"SD Asgard.exe", pid);
        DWORD baseAddressNickname = rf_client2 + 0x01EDD54;
        DWORD addressNickname = 0;
        ReadProcessMemory(pHandle, (void*)baseAddressNickname, &addressNickname, sizeof(addressNickname), 0);
        addressNickname += 0x0;

    


    }
    else {
        cout << "A janela digitada encontra-se fechada!" << endl;
        Sleep(2000);
        main();
    }

    while (true) {

        ReadProcessMemory(pHandle, (LPVOID)address, &target, 250, NULL);
        ReadProcessMemory(pHandle, (LPVOID)addressCoordenadasReiTauron, &coodenadasReiTauron, 250, NULL);
        ReadProcessMemory(pHandle, (LPVOID)addressNickname, &nickname, 250, NULL);
        /// cout << target << endl;
        std::string s(target);
        std::string sNickname(nickname);
        std::string sCoordenada(coodenadasReiTauron);
        std::string MENSAGEM;
        s = ReplaceAll(s, std::string("Taurons!"), std::string(""));
        s = ReplaceAll(s, std::string("Faltam"), std::string(""));
        s = ReplaceAll(s, std::string(" "), std::string(""));

        int numeroTauros = atoi(std::string(s).c_str());
        // cout << "Faltam" << s << "Taurons!   - JANELA: " << newWindowName << endl;
        if (IsProcessRunning(pid)) {
            if (sNickname.length() >= 3) {

                if (numeroTauros > 2) {

                    cout << "\n\nFaltam " << numeroTauros << " Taurons!  " << sCoordenada << "  \nJANELA: " << newWindowName <<  "\nPERSONAGEM: " << sNickname << endl;
                    nasceu = false;
                    MENSAGEM = s;
                }
                if (numeroTauros <= 10) {
                    nasceu = true;
                    if (nasceu) {
                        cout << "\nO Rei Tauron pode ter nascido! "<< sCoordenada << "\nJANELA: " << newWindowName << "\nPERSONAGEM : " << sNickname << endl;
                        cout << "\n\nAguardando o Rei Tauron morrer... \n Use o pergaminho para o deserto caso o Rei esteja morto." << endl;
                        Beep(523, 3000); // 523 hertz (C5) por 500 milissegundos (0,5 segundos)
                    ///cin.get(); // espera tocar o som
                        nasceu = false;
                        MENSAGEM = "Rei Tauron pode ter nascido no canal " + newWindowName;
                    }
                }

                if (newWindowName == "ODIN-1") {
                    std::ofstream ofs("ODIN-1.txt", std::ofstream::out);
                    ofs << MENSAGEM;
                    ofs.close();
                }
                else if (newWindowName == "ODIN-2") {
                    std::ofstream ofs("ODIN-2.txt", std::ofstream::out);
                    ofs << MENSAGEM;
                    ofs.close();
                }
                else if (newWindowName == "ODIN-3") {
                    std::ofstream ofs("ODIN-3.txt", std::ofstream::out);
                    ofs << MENSAGEM;
                    ofs.close();
                }
                else
                {

                }
                

            }
            else {
                cout << "Conecte-se em algum personagem e use um pergaminho para o deserto." << endl;
            }


        }
        else {
            cout << "A janela digitada encontra-se fechada!" << endl;
            Sleep(2000);
            main();

        }
        Sleep(500);
        HANDLE hStdout;

        hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

        cls(hStdout);
    }
    system("Pause");
}