#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

bool can_openDoor(int doorR, int doorG, int roomR, int roomG, int roomW, int keys[])
{
    if (keys[0] + keys[2] < doorR) {
        return false;
    }
    else {
        keys[2] -= (doorR-keys[0] > 0) ? doorR-keys[0]:0;
        keys[0] -= (doorR-keys[0] > 0) ? keys[0] : doorR;
    }
    if (keys[1] + keys[2] < doorG) {
        return false;
    }
    else {
        keys[2] -= (doorG-keys[1] > 0) ? doorG-keys[1]:0;
        keys[1] -= (doorG-keys[1] > 0) ? keys[1] : doorG;
    }
    keys[0] += roomR;
    keys[1] += roomG;
    keys[2] += roomW;
    return true;
}

int count = 0;
int cal_max_keys(const int doorR[], const int doorG[], const int roomR[], const int roomG[], const int roomW[], int size, const int keys[], int mask)
{
    int i;
    int res = 0, restmp = 0;
    int *keytmp = (int *)calloc(3, sizeof(int));
    count ++;
    assert(keytmp != NULL);
    memcpy(keytmp, keys, 3*sizeof(int));
    res = keys[0] + keys[1] + keys[2];
    if (!mask) {
        return res;
    }
    for (i = 0; i < size; i++) {
        if (mask&(1<<i)) {
            if (can_openDoor(doorR[i], doorG[i], roomR[i], roomG[i], roomW[i], keytmp)) {
                restmp = cal_max_keys(doorR, doorG, roomR, roomG, roomW, size, keytmp, mask^(1<<i));
                res = restmp > res ? restmp : res;
            }
            memcpy(keytmp, keys, 3*sizeof(int));
        }
    }
    free(keytmp);
    return res;
}

int max_keys(int doorR[], int doorG[], int roomR[], int roomG[], int roomW[], int size, int keys[])
{
    int i, mask = 0;
    for (i = 0; i < size; i++) {
        mask |= (1<<i);
    }
    return cal_max_keys(doorR, doorG, roomR, roomG, roomW, size, keys, mask);
}

int main(int argc, const char *argv[])
{
    int doorR[] = {9,5,10,8,4,3,0,8,4,1,3,9};
     int doorG[] = {9,10,0,8,9,4,3,8,1,8,10,4};
      int roomR[] = {1,2,0,2,3,3,5,3,1,3,0,5};
       int roomG[] = {5,2,5,0,5,2,3,4,0,0,5,2};
        int roomW[] = {1,5,1,2,0,4,4,0,3,3,1,3};
         int keys[] = {5,0,1};
         
    int size = sizeof(doorR)/sizeof(int);
    printf("max_keys = %d\n", max_keys(doorR, doorG, roomR, roomG, roomW, size, keys));
    printf("%d\n", count);
    return 0;
}
