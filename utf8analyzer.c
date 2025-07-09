#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

bool is_ascii(const char *str);
void uppercase_ascii(char *str);
int utf8_strlen(const char *str);
void print_codepoints(const char *str);
void print_bytes_per_codepoint(const char *str);
void print_first_6_codepoints(const char *str);
void print_animal_emojis(const char *str);
void print_next_codepoint_at_index_3(const char *str);
int utf8_charlen(unsigned char byte);
uint32_t utf8_decode_cp(const char *str, int *len);
int utf8_encode_cp(uint32_t cp, char out[4]);
bool is_animal_emoji(uint32_t cp);

bool is_ascii(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if ((unsigned char)str[i] > 127) {
            return false;
        }
    }
    return true;
}

void uppercase_ascii(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            str[i] = str[i] - ('a' - 'A');
        }
    }
}

int utf8_strlen(const char *str) {
    int count = 0;
    for (int i = 0; str[i] != '\0'; ) {
        unsigned char byte = (unsigned char)str[i];
        if (byte <= 0x7F) {
            i += 1;
        } else if ((byte & 0xE0) == 0xC0) {
            i += 2;
        } else if ((byte & 0xF0) == 0xE0) {
            i += 3;
        } else if ((byte & 0xF8) == 0xF0) {
            i += 4;
        } else {
            i++;
        }
        count++;
    }
    return count;
}

int utf8_charlen(unsigned char byte) {
    if (byte <= 0x7F) {
        return 1;
    } else if ((byte & 0xE0) == 0xC0) {
        return 2;
    } else if ((byte & 0xF0) == 0xE0) {
        return 3;
    } else if ((byte & 0xF8) == 0xF0) {
        return 4;
    }
    return 1; // Invalid, but treat as single byte
}

uint32_t utf8_decode_cp(const char *str, int *len) {
    unsigned char byte = (unsigned char)str[0];
    uint32_t codepoint = 0;
    
    if (byte <= 0x7F) {
        *len = 1;
        return byte;
    } else if ((byte & 0xE0) == 0xC0) {
        *len = 2;
        codepoint = byte & 0x1F;
    } else if ((byte & 0xF0) == 0xE0) {
        *len = 3;
        codepoint = byte & 0x0F;
    } else if ((byte & 0xF8) == 0xF0) {
        *len = 4;
        codepoint = byte & 0x07;
    } else {
        *len = 1;
        return 0xFFFD; // Replacement character
    }
    
    for (int i = 1; i < *len; i++) {
        byte = (unsigned char)str[i];
        codepoint = (codepoint << 6) | (byte & 0x3F);
    }
    
    return codepoint;
}

int utf8_encode_cp(uint32_t cp, char out[4]) {
    if (cp <= 0x7F) {
        out[0] = cp;
        return 1;
    } else if (cp <= 0x7FF) {
        out[0] = 0xC0 | (cp >> 6);
        out[1] = 0x80 | (cp & 0x3F);
        return 2;
    } else if (cp <= 0xFFFF) {
        out[0] = 0xE0 | (cp >> 12);
        out[1] = 0x80 | ((cp >> 6) & 0x3F);
        out[2] = 0x80 | (cp & 0x3F);
        return 3;
    } else if (cp <= 0x10FFFF) {
        out[0] = 0xF0 | (cp >> 18);
        out[1] = 0x80 | ((cp >> 12) & 0x3F);
        out[2] = 0x80 | ((cp >> 6) & 0x3F);
        out[3] = 0x80 | (cp & 0x3F);
        return 4;
    }
    return 0;
}

void print_codepoints(const char *str) {
    printf("Code points as decimal numbers:");
    for (int i = 0; str[i] != '\0'; ) {
        int len;
        uint32_t codepoint = utf8_decode_cp(str + i, &len);
        printf(" %u", codepoint);
        i += len;
    }
    printf("\n");
}

void print_bytes_per_codepoint(const char *str) {
    printf("Bytes per code point:");
    for (int i = 0; str[i] != '\0'; ) {
        int len = utf8_charlen((unsigned char)str[i]);
        printf(" %d", len);
        i += len;
    }
    printf("\n");
}

void print_first_6_codepoints(const char *str) {
    printf("Substring of the first 6 code points: \"");
    int count = 0;
    for (int i = 0; str[i] != '\0' && count < 6; ) {
        int len = utf8_charlen((unsigned char)str[i]);
        for (int j = 0; j < len; j++) {
            putchar(str[i + j]);
        }
        i += len;
        count++;
    }
    printf("\"\n");
}

bool is_animal_emoji(uint32_t cp) {
    return (cp >= 0x1F400 && cp <= 0x1F43F) || 
           (cp >= 0x1F980 && cp <= 0x1F9AE);
}

void print_animal_emojis(const char *str) {
    printf("Animal emojis:");
    for (int i = 0; str[i] != '\0'; ) {
        int len;
        uint32_t codepoint = utf8_decode_cp(str + i, &len);
        if (is_animal_emoji(codepoint)) {
            for (int j = 0; j < len; j++) {
                putchar(str[i + j]);
            }
            putchar(' ');
        }
        i += len;
    }
    printf("\n");
}

void print_next_codepoint_at_index_3(const char *str) {
    int index = 0;
    for (int i = 0; str[i] != '\0'; ) {
        int len;
        uint32_t codepoint = utf8_decode_cp(str + i, &len);
        
        if (index == 3) {
            codepoint++;
            char out[4];
            int out_len = utf8_encode_cp(codepoint, out);
            for (int j = 0; j < out_len; j++) {
                putchar(out[j]);
            }
            printf("\n");
            return;
        }
        
        i += len;
        index++;
    }
    printf("(Index out of range)\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: utf8analyzer \"<UTF-8 encoded string>\"\n");
        return 1;
    }

    char *str = argv[1];
    char *uppercased = strdup(str);
    uppercase_ascii(uppercased);

    printf("Valid ASCII: %s\n", is_ascii(str) ? "true" : "false");
    printf("Uppercased ASCII: %s\n", uppercased);
    printf("Length in bytes: %zu\n", strlen(str));
    printf("Number of code points: %d\n", utf8_strlen(str));
    print_codepoints(str);
    print_bytes_per_codepoint(str);
    print_first_6_codepoints(str);
    print_animal_emojis(str);
    printf("Next character of Codepoint at index 3: ");
    print_next_codepoint_at_index_3(str);

    free(uppercased);
    return 0;
}