#include <iostream>
#include <math.h>

#define WIDTH 80
#define HEIGHT 80
#define FPS 30
#define PI 3.14

#define R1 1
#define R2 2

class Screen{
    char surface[WIDTH][HEIGHT];

    public:
        Screen(){
            // system("CLS"); //Flush screen
            clearDisplay();
        }

        void putChar(int x, int y, char a){
            surface[x][y] = a;
        }   
        
        void flip(){
            std::cout << "\x1b[H";
            for(int i = 0; i < HEIGHT; i++){
                for(int j = 0; j < WIDTH; j++){
                    std::cout << surface[i][j];
                }
                std::cout << std::endl;
            }
            // system("CLS");

            clearDisplay();
        }

        void clearDisplay(){
            for(int i = 0; i < HEIGHT; i++){
                for(int j = 0; j < WIDTH; j++){
                    surface[i][j] = ' ';
                }
            }
        }
};

class Donut{
    Screen display;
    float A, B, theta, phi;
    float z_buffer[WIDTH][HEIGHT];

    public:
        Donut(){
            A = 0;
            B = 0;
        }

        void update(float vel){
            A += vel;     //increase x rotation speed
            B += vel;  //increase z rotation speed

            clearBuffer();
        }

        void clearBuffer(){
            for(int i = 0; i < HEIGHT; i++)
                for(int j = 0; j < WIDTH; j++)
                    z_buffer[i][j] = 0;
        }

        void drawDonut(){
            std::cout << "\x1b[2J";

            for (float theta = 0; theta < 6.28; theta += 0.07){
                for (float phi = 0; phi < 6.28; phi += 0.02){

                    float sintheta = sin(theta); 
                    float costheta = cos(theta); 

                    float sinphi = sin(phi); 
                    float cosphi = cos(phi); 
                    
                    float sinA = sin(A); 
                    float cosA = cos(A); 
                    
                    float sinB = sin(B); 
                    float cosB = cos(B); 
                    
                    float shiftedCircle = R1*costheta + R2;  // (R2 + R1cos(theta))

                    float z = sinphi * shiftedCircle * sinA + sintheta * cosA;
                    
                    float ooz = 1 / (z + 5); // 1/(z + K2)   K2 = 5
                    
                    // K1 is the stretch factor -- the x stretch is half of y stretch to make the perfect donut shape
                    int x = WIDTH/2 + 25 * ooz * (cosphi * shiftedCircle * cosB - (sinphi * shiftedCircle * cosA - sintheta * sinA) * sinB); //K1 = 25 here
                    int y = HEIGHT/2 + 50 * ooz * (cosphi * shiftedCircle * sinB + (sinphi * shiftedCircle * cosA - sintheta * sinA)* cosB); // K1 = 50 here

                    int luminance = 8 * ((sintheta * sinA - sinphi * costheta * cosA) * cosB - sinphi * costheta * sinA - sintheta * cosA - cosphi * costheta * sinB);
                   
                    if (HEIGHT > y && y > 0 && WIDTH > x && x > 0 && ooz > z_buffer[x][y]){
                        z_buffer[x][y] = ooz;
                        char a = ".,-~:;=!*#$@"[luminance > 0 ? luminance : 0];
                        display.putChar(x, y, a);
                    }
                }
            }
            display.flip();
        }
};

int main(){
    Donut donut;
    
// MAIN LOOP
    while(1){
        donut.drawDonut();
        donut.update(0.5);

        _sleep(1000/FPS);
    }
    return 0;
}
