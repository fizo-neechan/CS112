#include <iostream>
#include <math.h>
#include <Windows.h>

#define WIDTH 45
#define HEIGHT 45
#define FPS 60
#define PI 3.14

#define R1 1
#define R2 2

class Screen{
    char surface[WIDTH][HEIGHT];

    public:
        /**
         * @brief  constructor: clears screen and initializes surface with spaces
         * 
         */
        Screen(){
            std::cout << "\x1b[2J"; //Flush screen and set cursor to home position
            std::cout << "\x1b[H";

            clearDisplay();
        }

        /**
         * @brief loading bar for the opening screen
         * 
         */
        void Loading(){
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            system("CLS");
            std::cout << "\n\n\n\n\n\n\n\n\n\tLoading...\n";
            for(int i = 0; i< 10; i++){
                Sleep(100);
                SetConsoleTextAttribute(hConsole, 20 + (15*i));
                std::cout << "  ";
                SetConsoleTextAttribute(hConsole, 8);
                std::cout << " ";      
            }
        }

        /**
         * @brief puts character into surface array at [x][y]
         * 
         * @param x x posisition
         * @param y y posisition
         * @param a input character
         */
        void putChar(int x, int y, char a){
            surface[x][y] = a;
        }   
        
        /**
         * @brief clears the current screen and draws what ever is on the surface array
         * 
         */
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

        /**
         * @brief clears the surface array
         * 
         */
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
    int K1, K2;
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

        /**
         * @brief Construct a new Donut object
         * 
         * @param size initial size of donut
         */
        Donut(int size){
            K1 = size;
            K2 = 5;
            A = 0;
            B = 0;
            clearBuffer();
        }

        /**
         * @brief increase K1 by i
         * 
         * @param i increase K1 by i
         */
        void increaseSize(int i){
            K1 += i;
        }
       
        /**
         * @brief add vel to A and B for changing spin position
         * 
         * @param vel spin_velocity
         * 
         */
        void update(float vel){
            A += vel;
            B += vel;

            clearBuffer();
        }

        /**
         * @brief clears z buffer and fills with zeros
         * 
         */
        void clearBuffer(){
            for(int i = 0; i < HEIGHT; i++)
                for(int j = 0; j < WIDTH; j++)
                    z_buffer[i][j] = 0;
        }

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
         * in other equations --> K1 is the stretch factor -- the x stretch is half of y stretch to make the perfect donut shape
         * 
         * some parts which have luminance less than zero are also a part of the render
         * they are rendered as darkest character
         * 
         * @param display on which display to draw the donut 
         * @param type 1 displays other equations, 0 displays sir's equations
         *
         */
        void drawDonut(Screen &display, bool type){

            sinA = sin(A); 
            cosA = cos(A); 
            sinB = sin(B); 
            cosB = cos(B); 

            for (theta = 0; theta < 6.28; theta += 0.1){

                sintheta = sin(theta); 
                costheta = cos(theta);
                shiftedCircle = R1*costheta + R2;  // (R2 + R1cos(theta))
                 
                for (phi = 0; phi < 6.28; phi += 0.05){
                    sinphi = sin(phi); 
                    cosphi = cos(phi); 
                    
                    if(type){
                        // other equations
                        z = sinphi * shiftedCircle * sinA + sintheta * cosA;
                        ooz = 1 / (z + K2);

                        x = HEIGHT/2 + K1 * 0.5 * ooz * (cosphi * shiftedCircle * cosB - (sinphi * shiftedCircle * cosA - sintheta * sinA) * sinB);
                        y = HEIGHT/2 + K1 * ooz * (cosphi * shiftedCircle * sinB + (sinphi * shiftedCircle * cosA - sintheta * sinA)* cosB);
                    } else{
                        // sir's equations
                        z = cosA * shiftedCircle * sinphi + R1 * sinA * sintheta;
                        ooz = 1 / (z + K2);

                        x = HEIGHT/2 + K1 * ooz * ((R2+R1*costheta)*(cosB*cosphi + sinA*sinB*sinphi) - (R1*cosA*sinB*sintheta));
                        y = HEIGHT/2 + K1 * ooz * ((R2+R1*costheta)*(cosphi*sinB - cosB*sinA*sinphi) + (R1*cosA*cosB*sinB)); 
                    }

                    // combined 8*n into one as luminance
                    luminance = 8 * ((sintheta * sinA - sinphi * costheta * cosA) * cosB - sinphi * costheta * sinA - sintheta * cosA - cosphi * costheta * sinB);
                   
                    // only renders the parts which are inside the screen size = [width][height] and closest to veiwer
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

    // initialize instances of donut (size = 0) and screen
    Donut donut(0);
    Screen display;
    
    float spin_vel = 0.05;

    display.Loading();
    Sleep(1000);
    int i = 0;

// MAIN LOOP
    while(1){
        donut.drawDonut(display, 1); // draw donut on the display
        donut.update(spin_vel);

        if(i++/2 < 15)
            donut.increaseSize(+1);

// EVENTS
        // left mouse btn increases spin speed
        // right mouse btn decreases spin speed
        if(GetAsyncKeyState(VK_LBUTTON))
            spin_vel += 0.05;
        if(GetAsyncKeyState(VK_RBUTTON))
            spin_vel -= 0.05;
        // up arrow increases donut size
        // down arrow decreases donut size;
        if(GetAsyncKeyState(VK_DOWN))
            donut.increaseSize(-2);
        if(GetAsyncKeyState(VK_UP))
            donut.increaseSize(2);
        // escape key ends loop
        if(GetAsyncKeyState(VK_ESCAPE))
            break;
        

        // flip display to show on screen
        display.flip();
        // controlling FPS using sleep
        Sleep(1000/FPS);
    }

    return 0;
}
