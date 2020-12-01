/*
 * Task #6
 *
 * Azim Muradov, group #171
 */

#include "sortings.h"

#include "error_handling.h"

#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>


/*------------------------------------------------------- Utils ------------------------------------------------------*/

#define CHAR_COUNT (UCHAR_MAX + 1)

static inline void swap_lines(char **ptr_string_a, char **ptr_string_b) {
    if (ptr_string_a == ptr_string_b) return;

    char *temp = *ptr_string_a;
    *ptr_string_a = *ptr_string_b;
    *ptr_string_b = temp;
}


/*---------------------------------------------------- Bubble Sort ---------------------------------------------------*/

void bubble(strings_array_t lines, array_size_t count, comparator_func_t comp) {
    long long n = count;

    for (long long i = n - 1; i > 0; --i) {
        bool is_sorted = true;

        for (long long j = 0; j < i; ++j) {
            if (comp(lines[j], lines[j + 1]) > 0) {
                is_sorted = false;
                swap_lines(&lines[j], &lines[j + 1]);
            }
        }

        if (is_sorted) break;
    }
}


/*-------------------------------------------------- Insertion Sort --------------------------------------------------*/

void insertion(strings_array_t lines, array_size_t count, comparator_func_t comp) {
    long long n = count;

    for (long long i = 1; i < n; ++i) {
        char *min = lines[i];

        long long j = i - 1;
        while (j >= 0 && comp(lines[j], min) > 0) {
            lines[j + 1] = lines[j];
            --j;
        }
        lines[j + 1] = min;
    }
}


/*---------------------------------------------------- Merge Sort ----------------------------------------------------*/

static void merge_merge_part(
    strings_array_t lines, strings_array_t buf,
    array_size_t begin, array_size_t mid, array_size_t end,
    comparator_func_t comp
) {
    array_size_t i = begin, j = mid;
    array_size_t k = begin;

    while (i < mid && j < end) {
        if (comp(lines[i], lines[j]) <= 0) buf[k++] = lines[i++];
        else buf[k++] = lines[j++];
    }

    while (i < mid) buf[k++] = lines[i++];
    while (j < end) buf[k++] = lines[j++];

    for (k = begin; k < end; ++k) lines[k] = buf[k];
}

static void merge_core_part(
    strings_array_t lines, strings_array_t buf,
    array_size_t begin, array_size_t end,
    comparator_func_t comp
) {
    if (begin + 1 >= end) return;

    array_size_t mid = (begin + end) / 2;

    merge_core_part(lines, buf, begin, mid, comp);
    merge_core_part(lines, buf, mid, end, comp);

    merge_merge_part(lines, buf, begin, mid, end, comp);
}

void merge(strings_array_t lines, array_size_t count, comparator_func_t comp) {
    strings_array_t buf = malloc(count * sizeof *lines);
    if (buf == 0) {
        curr_error = TASK_6_E_MEM_ALLOC;
        return;
    }

    merge_core_part(lines, buf, 0, count, comp);

    free(buf);
}


/*---------------------------------------------------- Quick Sort ----------------------------------------------------*/

static void quick_core_part(strings_array_t lines, array_size_t begin, array_size_t end, comparator_func_t comp) {
    if (begin + 1 >= end) return;

    array_size_t less_i = begin;
    array_size_t eq_i = begin;
    array_size_t more_i = begin;

    char *pivot = lines[begin + rand() % (end - begin)];

    for (array_size_t i = begin; i < end; ++i) {
        int comp_val = comp(lines[i], pivot);

        if (comp_val < 0) swap_lines(&lines[less_i++], &lines[i]);
        if (comp_val <= 0) {
            if (less_i < eq_i + 1) swap_lines(&lines[eq_i], &lines[i]);
            eq_i++;
        }
        more_i++;
    }

    quick_core_part(lines, begin, less_i, comp);
    quick_core_part(lines, eq_i, more_i, comp);
}

void quick(strings_array_t lines, array_size_t count, comparator_func_t comp) {
    srand(time(NULL));

    quick_core_part(lines, 0, count, comp);
}


/*---------------------------------------------------- Radix Sort ----------------------------------------------------*/

static void radix_core_part(
    strings_array_t lines, array_size_t count, comparator_func_t comp,
    array_size_t l, array_size_t r, array_size_t d
) {
    if (d >= MAX_INPUT_STRING_SIZE || l + 1 >= r) return;

    size_t cmp_proxy[CHAR_COUNT];
    if (comp("a", "b") < 0) for (size_t i = 0; i < CHAR_COUNT; ++i) cmp_proxy[i] = i;
    else for (size_t i = 0; i < CHAR_COUNT; ++i) cmp_proxy[i] = CHAR_COUNT - i - 1;

    int cnt[CHAR_COUNT] = {0};
    for (array_size_t i = l; i < r; ++i) cnt[cmp_proxy[(unsigned char) lines[i][d]]]++;
    for (int i = 1; i < CHAR_COUNT; ++i) cnt[i] += cnt[i - 1];
    int cnt_copy[CHAR_COUNT];
    memcpy(cnt_copy, cnt, (CHAR_COUNT) * sizeof *cnt);

    strings_array_t c = malloc((r - l) * sizeof *c);
    for (array_size_t i = l; i < r; ++i) {
        c[--cnt[cmp_proxy[(unsigned char) lines[i][d]]]] = lines[i];
    }

    memcpy(lines + l, c, (r - l) * sizeof *c);

    radix_core_part(lines, count, comp, l, l + cnt_copy[0], d + 1);
    for (int i = 1; i < CHAR_COUNT; ++i) {
        radix_core_part(lines, count, comp, l + cnt_copy[i - 1], l + cnt_copy[i], d + 1);
    }

    free(c);
}

void radix(strings_array_t lines, array_size_t count, comparator_func_t comp) {
    radix_core_part(lines, count, comp, 0, count, 0);
}


/*------------------------------------------------------ Undefs ------------------------------------------------------*/

#undef CHAR_COUNT
