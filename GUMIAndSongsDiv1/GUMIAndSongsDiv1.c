#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include "GUMIAndSongsDiv1.h"

static void show_dataUnity(const data_unity* dataUnity, int size)
{
    int i;
    for (i = 0; i < size; i++) {
        printf("%d: duraiton=%d, tone=%d\n", i, dataUnity[i].duration, dataUnity[i].tone);
    }
}

static void init_dataUnity(const int *duration, const int *tone, data_unity *dataUnity, int size)
{
    int i;
    for (i = 0; i < size; i++) {
        dataUnity[i].duration = duration[i];
        dataUnity[i].tone = tone[i];
    }
}

static int compare_dataUnity(const void *dataUnity1, const void *dataUnity2)
{
    return (((data_unity *)dataUnity1)->duration > ((data_unity *)dataUnity2)->duration);
}

static void sort_dataUnity(data_unity *dataUnity, int size)
{
    qsort((void *)dataUnity, size, sizeof(data_unity), compare_dataUnity);
}

static bool test_dataUnity(const data_unity *dataUnity, int size, int count, int T)
{
    int *stack = (int *)calloc(count, sizeof(int));
    int *max_tone_stack = (int *)calloc(count, sizeof(int));
    int *min_tone_stack = (int *)calloc(count, sizeof(int));
    int stack_head = -1, pre_stack_head_value = -1;
    int max_tone_stack_head = -1, min_tone_stack_head = -1;
    int sum_duration = 0;
    int current_tone = 0, pre_max_tone= 0, pre_min_tone = 0;
    int i;
    bool res = false;
    assert(stack != NULL && max_tone_stack != NULL && min_tone_stack != NULL);
    while ((stack_head != -1) || (pre_stack_head_value != size - count)) {
        if (stack_head == count - 1) {
            if (sum_duration + max_tone_stack[max_tone_stack_head] - min_tone_stack[min_tone_stack_head] <= T) {
                res = true;
                goto function_end;
            }
            else {
                pre_stack_head_value = stack[stack_head];
                stack_head--;
                //recaculate sum_duration, max_tone, min_tone
                sum_duration -= dataUnity[pre_stack_head_value].duration;
                max_tone_stack_head--;
                min_tone_stack_head--;
            }
            continue;
        }
        if (pre_stack_head_value == size - 1) {
            pre_stack_head_value = stack[stack_head];
            stack_head--;
            //recaculate sum_duration, max_tone, min_tone
            sum_duration -= dataUnity[pre_stack_head_value].duration;
            max_tone_stack_head--;
            min_tone_stack_head--;
            continue;
        }
        if (stack_head != -1 && stack[stack_head] == size - 1) {
            pre_stack_head_value = stack[stack_head];
            stack_head--;
            //recaculate sum_duration, max_tone, min_tone
            sum_duration -= dataUnity[pre_stack_head_value].duration;
            max_tone_stack_head--;
            min_tone_stack_head--;
            continue;
        }
        stack_head++;
        max_tone_stack_head++;
        min_tone_stack_head++;
        stack[stack_head] = pre_stack_head_value + 1;
        pre_stack_head_value = stack[stack_head];
        sum_duration += dataUnity[pre_stack_head_value].duration;
        //caculate max_tone and min_tone
        current_tone = dataUnity[pre_stack_head_value].tone;
        if (stack_head == 0) {
            max_tone_stack[max_tone_stack_head] = min_tone_stack[min_tone_stack_head] = current_tone;
        }
        else {
            pre_max_tone = max_tone_stack[max_tone_stack_head-1]; 
            pre_min_tone = min_tone_stack[min_tone_stack_head-1];
            max_tone_stack[max_tone_stack_head] = current_tone > pre_max_tone ? current_tone : pre_max_tone;
            min_tone_stack[min_tone_stack_head] = current_tone < pre_min_tone ? current_tone : pre_min_tone;
        }
        if (sum_duration > T) {
            goto function_end;
        }
    }
function_end:
    free(stack);
    free(min_tone_stack);
    free(max_tone_stack);
    return res;
}

int max_songs(int duration[], int tone[], int size, int T)
{
    int i, sum = 0, max_count = size, count = 0;
    data_unity *data = (data_unity *)calloc(size, sizeof(data_unity));
    assert(data != NULL);
    init_dataUnity(duration, tone, data, size);
    //show_dataUnity(data, size);
    sort_dataUnity(data, size);
    //show_dataUnity(data, size);
    for (i = 0; i < size; i++) {
        sum += data[i].duration;
        if (sum > T) {
            max_count = i;
            break;
        }
    }
    printf("max_count = %d\n", max_count);
    for (i = max_count; i > 0; i--) {
        if (test_dataUnity(data, size, i, T)) {
            count = i;
            break;
        }
    }
    free(data);
    return count;
}

int main(int argc, const char *argv[])
{
    int duration[] = {87,21,20,73,97,57,12,80,86,97,98,85,41,12,89,15,41,17,68,37,21,1,9,65,4,
        67,38,91,46,82,7,98,21,70,99,41,21,65,11,1,8,12,77,62,52,69,56,33,98,97};
    int tone[] = {88,27,89,2,96,32,4,93,89,50,58,70,15,48,31,2,27,20,31,3,23,86,69,12,59,
        61,85,67,77,34,29,3,75,42,50,37,56,45,51,68,89,17,4,47,9,14,29,59,43,3};
    int T = 212;
    int size = sizeof(duration)/sizeof(int);
    int counts = max_songs(duration, tone, size, T); 
    printf("%d\n", counts);
    return 0;
}
