#include <iostream>
using namespace std;

char* Mystrcpy(char* s1, const char* s2) {
    int i;
    for (i = 0; s2[i] != '\0'; i++) {
        s1[i] = s2[i];
    }
    s1[i] = '\0';
    return s1;
}

char* Mystrncpy(char* s1, const char* s2, size_t n) {
    for (int i = 0; i < n; i++) {
        s1[i] = s2[i];
    }
    s1[n] = '\0';
    return s1;
}

char* Mystrcat(char* s1, const char* s2) {
    int i, j;
    for (i = 0; s1[i] != '\0'; i++);
    for (j = 0; s2[j] != '\0'; j++) {
        s1[i + j] = s2[j];
    }
    s1[i + j] = '\0';
    return s1;
}

char* Mystrncat(char* s1, const char* s2, size_t n) {
    int i, j;
    for (i = 0; s1[i] != '\0'; i++);
    for (j = 0; j < n; j++) {
        s1[i + j] = s2[j];
    }
    s1[i + j] = '\0';
    return s1;
}

int Mystrcmp(const char* s1, const char* s2) {
    int i = 0;
    while (1) {
        if (s1[i] > s2[i])
            return 1;
        else if (s1[i] < s2[i])
            return -1;
        else if (s1[i] == '\0' && s2[i] == '\0')
            return 0;
        i++;
    }
}

int Mystrncmp(const char* s1, const char* s2, size_t n) {
    int i = 0;
    while (1) {
        if (s1[i] > s2[i])
            return 1;
        else if (s1[i] < s2[i])
            return -1;
        else if (i == n)
            return 0;
        i++;
    }
}

size_t Mystrlen(const char* s) {
    int i;
    for (i = 0; s[i] != '\0'; i++);

    return i;
}

char* Mystrtok(char* s1, const char* s2) {
    static char *static_string = s1;

    if(static_string == NULL)
        return NULL;

    char *s3 = new char[Mystrlen(s1)];

    for(int i = 0; static_string[i] != '\0'; i++){
        int k = 0;  
        for(int j = 0; s2[j] != '\0'; j++){
            if(static_string[i] == '\0' || static_string[i] == s2[j]){
                s3[i] = '\0';
                static_string += i + 1;
                return s3;
            } else if(k == 0){
                s3[i] = static_string[i];
                k++;
            }
            
        }
    }
    return NULL;
}



int main() {

    char s1[] = "hello world my name is faizan,and i like to code "; 
    char s2[] = " ,";
    while(1){
        char * s3 = Mystrtok(s1,s2);
        if(s3 == NULL)
            break;
        else
            cout << s3 << endl;
    }



    return 0;
}