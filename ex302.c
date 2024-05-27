#include <stdio.h>

void escape(char s[], char t[]) ;
void unescape(char t[], char v[]) ;

int main(void)
{
    char s1[1000] = "abcdefghijk";
    char s2[1000] = "abcde\n\nfghijk\tlmnop";
    char t1[1000];
    char t2[1000];
    char v1[1000];
    char v2[1000];
    printf("escaping...\n");
    escape(s1, t1);
    escape(s2, t2);
    printf("%s\n", t1);
    printf("%s\n", t2);
    printf("\nunescaping...\n");
    unescape(t1, v1);
    unescape(t2, v2);
    printf("%s\n", v1);
    printf("%s\n", v2);
    return 0;
}

void escape(char s[], char t[])
{
    int j = 0;
    for (int i = 0; s[i] != '\0'; ++i) {
        switch (s[i]) {
            case '\a':
                t[j] = '\\';
                t[j+1] = 'a';
                j += 2;
                break;
            case '\t':
                t[j] = '\\';
                t[j+1] = 't';
                j += 2;
                break;
            case '\n':
                t[j] = '\\';
                t[j+1] = 'n';
                j += 2;
                break;
            case '\f':
                t[j] = '\\';
                t[j+1] = 'f';
                j += 2;
                break;
            default:
                t[j] = s[i];
                j += 1;
                break;
        }
    }
    t[j] = '\\';
    t[j+1] = '0';
    t[j+2] = '\0';
}

void unescape (char t[], char v[])
{
    int j = 0;
    for (int i = 0; t[i] != '\0'; ++i) {
        if (t[i] == '\\') {
            switch (t[i+1]) {
                case 'a':
                    v[j++] = '\a';
                    break;
                case 'f':
                    v[j++] = '\f';
                    break;
                case 't':
                    v[j++] = '\t';
                    break;
                case 'n':
                    v[j++] = '\n';
                    break;
                default:
                    break;
            }
            ++i;
        }
        else {
            v[j++] = t[i];
        }
    }
    v[j] = '\0';
}
