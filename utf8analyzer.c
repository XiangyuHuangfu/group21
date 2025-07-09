#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

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

void print_codepoints(const char *str) {
    printf("Code points as decimal numbers:");
    for (int i = 0; str[i] != '\0'; ) {
        unsigned char byte = (unsigned char)str[i];
        uint32_t codepoint = 0;
        int char_len = 0;

        if (byte <= 0x7F) {
            char_len = 1;
            codepoint = byte;
        } else if ((byte & 0xE0) == 0xC0) {
            char_len = 2;
            codepoint = byte & 0x1F;
        } else if ((byte & 0xF0) == 0xE0) {
            char_len = 3;
            codepoint = byte & 0x0F;
        } else if ((byte & 0xF8) == 0xF0) {
            char_len = 4;
            codepoint = byte & 0x07;
        } else {
            i++;
            continue;
        }

        for (int j = 1; j < char_len; j++) {
            byte = (unsigned char)str[i + j];
            codepoint = (codepoint << 6) | (byte & 0x3F);
        }

        printf(" %u", codepoint);
        i += char_len;
    }
    printf("\n");
}

void print_bytes_per_codepoint(const char *str) {
    printf("Bytes per code point:");
    for (int i = 0; str[i] != '\0'; ) {
        unsigned char byte = (unsigned char)str[i];
        int char_len = 0;

        if (byte <= 0x7F) {
            char_len = 1;
        } else if ((byte & 0xE0) == 0xC0) {
            char_len = 2;
        } else if ((byte & 0xF0) == 0xE0) {
            char_len = 3;
        } else if ((byte & 0xF8) == 0xF0) {
            char_len = 4;
        } else {
            i++;
            continue;
        }

        printf(" %d", char_len);
        i += char_len;
    }
    printf("\n");
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