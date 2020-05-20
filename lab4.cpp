#Compiler principle lab code,lab4
#include<cstring>
#include<algorithm>
#include<iostream>



#include <stdio.h>
#include <ctype.h>
#include <map>
#include <vector>
#include <string>
#define ID 12
#define ADD 14
#define SUB 15
#define MUL 16
#define DIV 17
#define LP 19
#define RP 20
#define EOI 31
#define SHARP 32
#define EQ 0
#define BT 1
#define LT 2
#define UD 3
#define N_Base 1000
using namespace std;

FILE *fp;
int lookahead, yylineno;
bool success;
map<int, int> intToint;
map<char, int> charToint;
map<int, char> intTochar;
string grammer[8] = {
        "E+T", "E-T", "T",
        "T*F", "T/F", "F",
        "(E)", "i"
};
int prior_matrix[9][9] = {
        {BT, BT, LT, LT, LT, BT, LT, BT},
        {BT, BT, LT, LT, LT, BT, LT, BT},
        {BT, BT, BT, BT, LT, BT, LT, BT},
        {BT, BT, BT, BT, LT, BT, LT, BT},
        {LT, LT, LT, LT, LT, EQ, LT, UD},
        {BT, BT, BT, BT, UD, BT, UD, BT},
        {BT, BT, BT, BT, UD, BT, UD, BT},
        {LT, LT, LT, LT, LT, UD, LT, EQ}
};

void init() {
    success = true;
    yylineno = 0;
    intToint[ADD] = 0; intToint[SUB] = 1;
    intToint[MUL] = 2; intToint[DIV] = 3;
    intToint[LP] = 4; intToint[RP] = 5;
    intToint[ID] = 6; intToint[SHARP] = 7;
    charToint['+'] = ADD; charToint['-'] = SUB;
    charToint['*'] = MUL; charToint['/'] = DIV;
    charToint['('] = LP; charToint[')'] = RP;
    charToint['i'] = ID;
    intTochar[ADD] = '+'; intTochar[SUB] = '-';
    intTochar[MUL] = '*'; intTochar[DIV] = '/';
    intTochar[LP] = '('; intTochar[RP] = ')';
    intTochar[ID] = 'i'; intTochar[SHARP] = '#';
    intTochar[N_Base] = 'N';
}

bool isVt(int a) {
    if (a >= N_Base) return false;
    else return true;
}

int comp(int a, int b) {
    int x = intToint[a];
    int y = intToint[b];
    return prior_matrix[x][y];
}

void advance() {
    if (fscanf(fp, "(%d", &lookahead) == EOF) {
        lookahead = SHARP;
    } else {
        char ch;
        do {
            ch = fgetc(fp);
            if (ch == '\n' || ch == EOF) break;
        } while (true);
    }
    yylineno++;
}

void parser() {
    int stack[100], top = 0;
    int i, j, k, ii, jj;

    stack[top++] = SHARP;
    advance();
    do {
        for (i = 0; i < top; i++) {
            printf("%c", intTochar[stack[i]]);
        }
        printf("\t%c\n", intTochar[lookahead]);
        for (i = top - 1; i >= 0; i--) {
            if (isVt(stack[i])) break;
        }
        int res = comp(stack[i], lookahead);
        if (res == LT || res == EQ) {
            stack[top++] = lookahead;
            advance();
        } else if (res == BT) {
            int temp = stack[i];
            for (j = i - 1; j >= 0; j--) {
                if (isVt(stack[j])) {
                    if (comp(stack[j], temp) == LT) {
                        break;
                    } else {
                        temp = stack[j];
                    }
                }
            }
            for (k = 0; k < 8; k++) {
                if ((int)grammer[k].length() == top - 1 - j) {
                    ii = j + 1;
                    jj = 0;
                    do {
                        if (grammer[k].at(jj) >= 'A' && grammer[k].at(jj) <= 'Z') {
                            if (isVt(stack[ii])) break;
                        } else {
                            if (charToint[grammer[k].at(jj)] != stack[ii]) break;
                        }
                        ii++;
                        jj++;
                    } while (ii < top && jj < (int)grammer[k].length());
                    if (ii >= top) break;
                }
            }
            if (k >= 8) {
                success = false;
                return ;
            }
            top = j + 1;
            stack[top++] = N_Base;
        } else {
            success = false;
            return ;
        }
        if (stack[0] == SHARP && stack[1] == N_Base
            && stack[2] == SHARP) {
            printf("#N#\n");
            break;
        }
    } while (true);

    success = true;
}

int main(int argc, char *argv[]) {
        fp = fopen("//Users//yuanlaihe//OuThing//input.txt", "r");
        init();
        parser();
        if (success) printf("YES.");
        else printf("NO.");
    return 0;
}

