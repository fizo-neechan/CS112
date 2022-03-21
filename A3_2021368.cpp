#include <iostream>
#include <random>
#include <Windows.h>
#include <time.h>
#include <stdio.h>
#include <math.h>


class Donut{
    private:
        float A, B;
        float z[1760];
        char b[1760];

    public:
        Donut(){
            A = 0;
            B = 0;
        }

        void print(){
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole , 252);
            std::cout << "\x1b[H";
            for (int k = 0; k < 1761; k++)
            { 
                std::cout << (char)(k % 80 ? b[k] : 10);
            }
        }

        void Update(){
            std::cout << "\x1b[2J";
            while (true)
            {
                memset(b, 32, 1760);
                memset(z, 0, 7040);
                for (float theta = 0; theta < 6.28; theta += 0.07)
                {
                    for (float phi = 0; phi < 6.28; phi += 0.02)
                    {
                        float c = sin(phi); // sin(phi)
                        float d = cos(theta); // cos(theta)
                        float e = sin(A); // sin(A)
                        float f = sin(theta); // sin(theta)
                        float g = cos(A); // cos(A)
                        float h = d + 2;  // (R2 + R1cos(theta)) //R2 is taken as 2 here
                        float D = 1 / (c * h * e + f * g + 5); // 1/(z + K2) //K2 is taken as 5
                        float l = cos(phi); // cos(phi)
                        float m = cos(B); // cos(B)
                        float n = sin(B); // sin(B)
                        float t = c * h * g - f * e;
                        int x = 40 + 30 * D * (l * h * m - t * n);
                        int y = 12 + 15 * D * (l * h * n + t * m);
                        int o = x + 80 * y;
                        int N = 8 * ((f * e - c * d * g) * m - c * d * e - f * g - l * d * n);
                        if (22 > y && y > 0 && x > 0 && 80 > x && D > z[o])
                        {
                            z[o] = D;
                            b[o] = ".,-~:;=!*#$@"[N > 0 ? N : 0];
                        }
                    }
                }
                A += 0.0704;
                B += 0.0352;
                print();
            }
        }

        

};

int main(){
    Donut D;
    D.Update();
    return 0;
}