#include <iostream>
#include <math.h>
#include <Windows.h>

#define WIDTH 45
#define HEIGHT 45
#define FPS 60
#define PI 3.14

#define R1 1
#define R2 2
#define K1 30

class Screen{
    char surface[WIDTH][HEIGHT];

    public:
        Screen(){
            std::cout << "\x1b[2J"; //Flush screen and set pos to home position
            std::cout << "\x1b[H";

            clearDisplay();
        }

        void putChar(int x, int y, char a){
            surface[x][y] = a;
        }   
        
        void flip(){
            std::cout << "\x1b[2J"; //Flush screen
            std::cout << "\x1b[H";

            for(int i = 0; i < HEIGHT; i++){
                for(int j = 0; j < WIDTH; j++){
                    std::cout << surface[i][j];
                }
                std::cout << std::endl;
            }
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
    // declare variables 
    float A, B, theta, phi;
    float z_buffer[WIDTH][HEIGHT];

    public:
        Donut(){
            A = 0;
            B = 0;
        }

        void update(float vel){
            A += vel;     //increase x rotation 
            B += vel;     //increase z rotation 

            clearBuffer();
        }

        void clearBuffer(){
            for(int i = 0; i < HEIGHT; i++)
                for(int j = 0; j < WIDTH; j++)
                    z_buffer[i][j] = 0;
        }

        void drawDonut(Screen &display){

            // theta and phi range from 0 - 2PI
            // draws a point of donut on every iteration
            // 0.1 and 0.05 are chosen as optimal values for drawing complete donut and not getting too many extra iterations
            for (theta = 0; theta < 6.28; theta += 0.1){
                for (phi = 0; phi < 6.28; phi += 0.05){

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
                    
                    float ooz = 1 / (z + 5); // 1/(z + K2)   // K2 = 5
                    
                    // using other x,y,z equations for making the donut, as sir's equations donot work properly
                    // combining the equations for x prime and y prime into one, so dont have to compute twice
                    // K1 is the stretch factor -- the x stretch is half of y stretch to make the perfect donut shape
                    int x = HEIGHT/2 + K1 * 0.5 * ooz * (cosphi * shiftedCircle * cosB - (sinphi * shiftedCircle * cosA - sintheta * sinA) * sinB);
                    int y = HEIGHT/2 + K1 * ooz * (cosphi * shiftedCircle * sinB + (sinphi * shiftedCircle * cosA - sintheta * sinA)* cosB); 

                    // combined 8*n into one as luminance
                    int luminance = 8 * ((sintheta * sinA - sinphi * costheta * cosA) * cosB - sinphi * costheta * sinA - sintheta * cosA - cosphi * costheta * sinB);
                   
                    // only renders the parts which are inside the screen size = [width][height]
                    // z_buffer: only renders the parts which are closest to the viewer, not the ones that are far as those are not seen
                    if (HEIGHT > y && y > 0 && WIDTH > x && x > 0 && ooz > z_buffer[x][y]){
                        z_buffer[x][y] = ooz;

                        // some parts which have luminance less than zero are also a part of the render
                        char a = ".,-~:;=!*#$@"[luminance > 0 ? luminance : 0];  // they are rendered as darkest character
                        display.putChar(x, y, a);
                    }
                }
            }
        }
};

int main(){
    float spin_vel = 0.05;

    // initialize instances of donut and screen
    Donut donut;
    Screen display;
    

// MAIN LOOP
    while(1){
        donut.drawDonut(display); // draw donut on the display
        donut.update(spin_vel);

        // EVENTS
        // left mouse btn increases spin speed
        // right mouse btn decreases spin speed
        if(GetAsyncKeyState(VK_LBUTTON))
            spin_vel += 0.05;
        if(GetAsyncKeyState(VK_RBUTTON))
            spin_vel -= 0.05;
        if(GetAsyncKeyState(VK_ESCAPE))
            break;

        // flip display to show on screen
        display.flip();
        // controlling FPS using sleep
        _sleep(1000/FPS);
    }
    
    return 0;
}
