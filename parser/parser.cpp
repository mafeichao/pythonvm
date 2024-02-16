#include <stdio.h>

#define INIT 0
#define NUM 1

int main() {
    FILE * fp = fopen("test_token.txt", "r");
    char ch; 
    int state, num = 0;

    while ((ch = getc(fp)) != EOF) {
        if (ch == ' ' || ch == '\n') {
            if (state == NUM) {
                printf("token NUM : %d\n", num);
                state = INIT;
                num = 0;
            }   
        }   

        else if (ch >= '0' && ch <= '9') {
            state = NUM;
            num = num * 10 + ch - '0';
        }   

        else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
		    if (state == NUM) {
                printf("token NUM : %d\n", num);
                state = INIT;
                num = 0;
            }

            printf("token operator : %c\n", ch);
            state = INIT;
        }

    }

    fclose(fp);
    return 0;
}
