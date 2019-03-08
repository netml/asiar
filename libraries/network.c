#include <stdint.h>
#include "network.h"

// prints the contents of a uint32_t IP array
void print_uint32_array(uint32_t *array, int size) {
    printf("=============\n");

    for (int i = 0; i < size; i++)
        printf("%s\n", convert_int_to_ip(array[i]));

    printf("=============\n");
}

uint32_t get_abs_uint32(uint32_t input1, uint32_t input2) {
    if (input1 > input2)
        return (input1 - input2);
    else
        return (input2 - input1);
}

void showbits(uint32_t x) {
    for(int i=(sizeof(int)*8)-1; i>=0; i--) {
        (x&(1u<<i))?putchar('1'):putchar('0');
        if (((i)%8 == 0) && i > 0)
            printf(".");
    }

    printf("\n");
}

int get_no_of_traces_in_file(char const * file_path) {
    int no_of_traces = 0;

    FILE *infile = fopen(file_path, "r");

    if (infile == NULL) {
        printf("Error: file pointer is null.\n");
        exit(1);
    }

    // Read file
    char line[MAXSTRING]; // contains the line

    while (fgets(line, MAXSTRING, infile) != NULL) { // for each entry
        if (line[0] != '\n' && line[0] != '#') {
            no_of_traces++;
            while (fgets(line, MAXSTRING, infile) != NULL && line[0] != '#') {};
        }
    }

    fclose(infile);

    return no_of_traces;
}

bool ips_equal(uint32_t IP1, uint32_t IP2) {
    if ((IP1 < 16777216) && (IP2 < 16777216))
        return true;

    if (IP1 == IP2)
        return true;
    else
        return false;
}

bool ip_is_zero(uint32_t IP) {
    if (IP < 16777216)
        return true;
    else
        return false;
}

uint32_t convert_ip_to_int (char* ip) {
    unsigned v = 0;
    const char * start;

    start = ip;
    for (int i = 0; i < 4; i++) {
        char c;
        int n = 0;
        while (1) {
            c = * start;
            start++;
            if (c >= '0' && c <= '9') {
                n *= 10;
                n += c - '0';
            }

            else if ((i < 3 && c == '.') || i == 3)
                break;
            else
                return 0;
        }

        if (n >= 256)
            return 0;

        v *= 256;
        v += n;
    }

    return v;
}

char* convert_int_to_ip (uint32_t ip) {
    char *result = malloc(sizeof(char)*15);
    sprintf(result, "%d.%d.%d.%d",
        (ip >> 24) & 0xFF,
        (ip >> 16) & 0xFF,
        (ip >>  8) & 0xFF,
        (ip      ) & 0xFF);
    return result;
}

bool is_local(uint32_t ip) {
    if (ip >= convert_ip_to_int("192.168.0.0") && ip <= convert_ip_to_int("192.168.255.255"))
        return true;

    if (ip >= convert_ip_to_int("10.0.0.0") && ip <= convert_ip_to_int("10.255.255.255"))
        return true;

    return false;
}

// build masks one
void build_masks_ones(uint32_t *mask_one) {
    mask_one[0] = 0;
	for (int j = 0; j<32; j++) {
        uint32_t mask1 = (-1) << (31 - j);//(0xFFFFFFFF << (j)) & 0xFFFFFFFF;
        mask_one[j+1] = mask1;
    }
}

// build masks zero
void build_masks_zero(uint32_t *mask_zero) {
    for (int i = 0; i<32; i++) {
        uint32_t mask = (0xFFFFFFFF) >> i;
        mask_zero[i] = mask;
    }
    mask_zero[32] = 0;
}

// check if IP is special IP
bool is_special(uint32_t IP, int mask, uint32_t *mask_zero) {
    uint32_t zeromasked = IP & (uintptr_t)(mask_zero[mask]);

    if ((zeromasked == 0) || (zeromasked == mask_zero[mask]))
        return true;

    return false;
}

// check if IP is within the annoucements of an AS
bool contains(uint32_t network, int mask, uint32_t IP, uint32_t *mask_one) {
    uint32_t zeromasked = network & (uintptr_t)(mask_one[mask]);
    uint32_t zeromasked2 = IP & (uintptr_t)(mask_one[mask]);
    uint32_t xored = zeromasked ^ zeromasked2;

    if (xored == 0)
        return true;
    else
       return false;
}

// find and return the largest mask match between 2 IPs
int get_mask_matches(int IP1_pos, int IP2_pos, uint32_t *mask_one, uint32_t *ips_t_array) {
    int mask = 0; // mask

    uint32_t IP1 = ips_t_array[IP1_pos];
    uint32_t IP2 = ips_t_array[IP2_pos];

    for (int i = 32; i > 0; i--) {
        if (contains(IP1, i, IP2, mask_one)) {
            mask = i;
            break;
        }
    }

    return mask;
}

// find and return the largest mask match between 2 IPs
int get_mask_matches2(uint32_t IP1, uint32_t IP2, uint32_t *mask_one) {
    int mask = 0; // mask

    for (int i = 32; i > 0; i--) {
        if (contains(IP1, i, IP2, mask_one)) {
            mask = i;
            break;
        }
    }

    return mask;
}

bool isValidIp4(const char *str) {
    int segs = 0;   /* Segment count. */
    int chcnt = 0;  /* Character count within segment. */
    int accum = 0;  /* Accumulator for segment. */

    /* Catch NULL pointer. */

    if (str == NULL)
        return false;

    /* Process every character in string. */

    while (*str != '\0') {
        /* Segment changeover. */

        if (*str == '.') {
            /* Must have some digits in segment. */

            if (chcnt == 0)
                return false;

            /* Limit number of segments. */

            if (++segs == 4)
                return false;

            /* Reset segment values and restart loop. */

            chcnt = accum = 0;
            str++;
            continue;
        }
        /* Check numeric. */

        if ((*str < '0') || (*str > '9'))
            return false;

        /* Accumulate and check segment. */

        if ((accum = accum * 10 + *str - '0') > 255)
            return false;

        /* Advance other segment specific stuff and continue loop. */

        chcnt++;
        str++;
    }

    /* Check enough segments and enough characters in last segment. */

    if (segs != 3)
        return false;

    if (chcnt == 0)
        return false;

    /* Address okay. */

    return true;
}
