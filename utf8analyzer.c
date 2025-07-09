#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

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