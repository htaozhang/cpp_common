#include <cstdio>
#include <cstring>
#include <iostream>

using namespace std;

int main() {
    int size = 127;
    char *buf = (char*)malloc(sizeof(char) * size);
    printf("before strtok, buf addr: %p\n", buf);

    for (char i = 0; i < size; i++) {
        buf[i] = 'a' + i % 5;
    }

    //cout << p << endl;
    
    char *p = strtok(buf, "e");
    while (p) {
        cout << p << endl;
        p = strtok(NULL, "e");
    }
    printf("after strtok, buf addr: %p\n", buf);

    free(buf);
    
    for (int i = 0; i < size; i++) {
        cout << buf + i << "|" <<  endl;
    }

    return 0;
}
