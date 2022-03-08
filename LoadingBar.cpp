#include <iostream>
#include <windows.h>
using namespace std;

void Loading(){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    system("CLS");
    cout << "\n\n\n\n\n\tLoading...\n";
    for(int i = 0; i< 10; i++){
        Beep(1500, 150);
        Sleep(50);
        SetConsoleTextAttribute(hConsole, 20 + (15*i));
        cout << "  ";
        SetConsoleTextAttribute(hConsole, 8);
        cout << " ";      
    }

}
