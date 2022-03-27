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

            /**
             * @brief  constructor: clears screen and initializes surface with spaces
             * 
             */

            std::cout << "\x1b[2J"; //Flush screen and set pos to home position
            std::cout << "\x1b[H";

            clearDisplay();
        }

        void putChar(int x, int y, char a){
            surface[x][y] = a;
        }   
        
        void flip(){

            /**
             * @brief clears the current screen and draws what ever is on the surface array
             * 
             */

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

            /**
             * @brief clears the surface array
             * 
             */

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
    float sintheta, costheta;
    float sinphi, cosphi;
    float sinA, cosA;
    float sinB, cosB;
    float z, ooz;
    float shiftedCircle;
    int x, y, luminance;

    public:
        Donut(){

            /**
             * @brief constructor: initializes A & B with zero
             * 
             */

            A = 0;
            B = 0;
        }
       
        void update(float vel){

            /**
             * @brief add vel to A and B for changing spin position
             * 
             * @param vel spin_velocity
             * 
             */

            A += vel;
            B += vel;

            clearBuffer();
        }

        void clearBuffer(){

            /**
             * @brief clears z buffer and fills with zeros
             * 
             */

            for(int i = 0; i < HEIGHT; i++)
                for(int j = 0; j < WIDTH; j++)
                    z_buffer[i][j] = 0;
        }

        void drawDonut(Screen &display){

            /**
             * @brief Draws the donut on display
             * 
             * z_buffer: only renders the parts which are closest to the viewer, not the ones that are far as those are not seen
             * 
             * theta and phi range from 0 - 2PI
             * draws a point of donut on every iteration
             * +=0.1 and +=0.05 are chosen as optimal values for drawing complete donut and not getting too many extra iterations
             * 
             * using other x,y,z equations for making the donut, as sir's equations donot work properly
             * combining the equations for x prime and y prime into one, so dont have to compute twice
             * K1 is the stretch factor -- the x stretch is half of y stretch to make the perfect donut shape
             * 
             * some parts which have luminance less than zero are also a part of the render
             * they are rendered as darkest character
             * 
             * @param display on which display to draw the donut 
             *
             */
            
            for (theta = 0; theta < 6.28; theta += 0.1){
                for (phi = 0; phi < 6.28; phi += 0.05){

                    sintheta = sin(theta); 
                    costheta = cos(theta); 

                    sinphi = sin(phi); 
                    cosphi = cos(phi); 
                    
                    sinA = sin(A); 
                    cosA = cos(A); 
                    
                    sinB = sin(B); 
                    cosB = cos(B); 
                    
                    shiftedCircle = R1*costheta + R2;  // (R2 + R1cos(theta))

                    z = sinphi * shiftedCircle * sinA + sintheta * cosA;
                    
                    ooz = 1 / (z + 5); // 1/(z + K2)   // K2 = 5
                    
                    x = HEIGHT/2 + K1 * 0.5 * ooz * (cosphi * shiftedCircle * cosB - (sinphi * shiftedCircle * cosA - sintheta * sinA) * sinB);
                    y = HEIGHT/2 + K1 * ooz * (cosphi * shiftedCircle * sinB + (sinphi * shiftedCircle * cosA - sintheta * sinA)* cosB); 

                    // combined 8*n into one as luminance
                    luminance = 8 * ((sintheta * sinA - sinphi * costheta * cosA) * cosB - sinphi * costheta * sinA - sintheta * cosA - cosphi * costheta * sinB);
                   
                    // only renders the parts which are inside the screen size = [width][height]
                    if (HEIGHT > y && y > 0 && WIDTH > x && x > 0 && ooz > z_buffer[x][y]){
                        z_buffer[x][y] = ooz;

                        char a = ".,-~:;=!*#$@"[luminance > 0 ? luminance : 0];
                        display.putChar(x, y, a);
                    }
                }
            }
        }
};

int main(){

    // initialize instances of donut and screen
    Donut donut;
    Screen display;
    
    float spin_vel = 0.05;

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
