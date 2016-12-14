#include <stdio.h>
#define TRUE 1
#define FALSE 0
#define DIRECT 0
#define NWAY 1
#define FULL 2

const int ADDRESSES[] = {0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48,
                         52, 56, 60, 64, 68, 72, 76, 80, 0, 4, 8, 12, 16,
                         71, 3, 41, 81, 39, 38, 71, 15, 39, 11, 51, 57, 41};
const int ADDR_LEN = sizeof(ADDRESSES) / sizeof(int);

struct Address{
    int address;
    int index;
    int set;
    int offset;
    int tag;
};
// a struct to store tag, set, and valid so that they don't need to be recalculated from address
struct CacheLine {
	int tag;
	int set;
	int valid;
};
// cache is an array of CacheLine structs
struct CacheLine cache[1024]; // arbitrary large size, should be sufficient for this assignment
// helper functions for testing cache and set fullness, probably could be better than these
// obvious solutions, like keeping another flag, but this will work for now
int isCacheFull(int numLines) {
	for(int i = 0; i < numLines; i++)
		if(cache[i].valid == 0) return FALSE;
	return TRUE;
}
int isSetFull(int numLines, int set) {
	for(int i = 0; i < numLines; i++)
		if(cache[i].valid == 0 && cache[i].set == set) return FALSE;
	return TRUE;
}

// a function for traversing through addresses in an associate cache (fully or nway)
void associative(struct Address addresses[], int sets[], int numSets,  int numLines) {
	int setSize = numLines / numSets;
	int firstIn[numSets], orig[numSets];

	for(int i = 0; i < numSets; i++){
		firstIn[i] = i * setSize;
		orig[i] = firstIn[i];
    }
    int hit, earliest;
	for(int i = 0; i < ADDR_LEN; i++) {
        hit = FALSE;
 		earliest = firstIn[sets[i]];

 		for(int j = 0; j < numLines; j++) {
 			if(cache[j].valid) {
 				if(cache[j].tag == addresses[i].tag && cache[j].set == sets[i]) {
 					hit = TRUE;
 					break;
 				}
            }
 		}
        char* hit_print = (hit ? "HIT" : "MISS");
        printf(" %d: %s (Tag/Set#/Offset: %d/%d/%d)\n", addresses[i].address, hit_print, addresses[i].tag, sets[i], addresses[i].offset);

 		if (!hit) {
            // if the set is full, remove by FIFO
 			if(isSetFull(numLines, sets[i])) {
 				cache[earliest].tag = addresses[i].tag;
 				cache[earliest].valid = TRUE;
                // move the first in value by incrementing or looping back to the first line in the set
 				earliest == setSize + orig[sets[i]] - 1 ? firstIn[sets[i]] = orig[sets[i]] : firstIn[sets[i]]++;
 			}
            // else place in the set
 			else {
 				for(int j = 0; j < numLines; j++) {
 					if(cache[j].valid == FALSE && cache[j].set == sets[i]) {
 						cache[j].tag = addresses[i].tag;
 						cache[j].valid = TRUE;
 						break;
 					}
 				}
 			}
 		}
 	}

	for(int i = 0; i < numLines; i++)
		cache[i].valid ? printf("%d: Valid: True; Tag %d (Set #: %d)\n", i, cache[i].tag, cache[i].set) :
                         printf("%d: Valid: False; Tag %d (Set #: %d)\n", i, cache[i].tag, cache[i].set);
}

// a function for traversing through ADDRESSES in a direct mapped cache
void direct(struct Address addresses[], int numLines) {
	for(int i = 0; i < ADDR_LEN; i++) {
		if(cache[addresses[i].index].valid && addresses[i].tag == cache[addresses[i].index].tag)
			    printf(" %d: HIT (Tag/Index/Offset: %d/%d/%d)\n", ADDRESSES[i], addresses[i].tag, addresses[i].index, addresses[i].offset);
		else {
			printf(" %d: MISS (Tag/Index/Offset: %d/%d/%d)\n", ADDRESSES[i], addresses[i].tag, addresses[i].index, addresses[i].offset);
			cache[addresses[i].index].valid = TRUE;
			cache[addresses[i].index].tag = addresses[i].tag;
		}
	}

	printf("Cache contents:\n");
	for(int j = 0; j < numLines; j++)
		cache[j].valid ? printf("%d: VALID: True; Tag: %d\n", j, cache[j].tag) :
                         printf("%d: VALID: False; Tag: -\n", j);
}

int main(int argc, char* argv[]) {

    printf("Please change constant addresses at top of file to reconfigure\n");
    struct Address addresses[ADDR_LEN];
	int sel, s, l;
    int selecting = TRUE;
    // loop until we get successful inputs
    while (selecting){
        printf("Cache type:\n"
                "0 for direct-mapped\n"
                "1 for n-way associative\n"
                "2 for fully associative\n: ");
        scanf("%i", &sel);
        if (sel > -1 && sel < 3) selecting=FALSE;
    }
    // get s and l (as defined in hw description)
    printf("Cache size: ");
    scanf("%d", &s);
    printf("Block size: ");
    scanf("%d", &l);
    // since we have set addresses, lets just store the set, tag, index, and offset for each
    // I should've put them in one address struct array, but it's working so I don't want to change it
    int sets[ADDR_LEN];
	int numLines = s / l;
    int tagIdx;
	switch(sel){
        case DIRECT:
        {
            // instantiate the cache mappings
            for(int i = 0; i < ADDR_LEN; i++) {
                addresses[i].address = ADDRESSES[i];
                tagIdx = ADDRESSES[i] / l;
                addresses[i].tag = tagIdx / numLines;
                addresses[i].index = tagIdx % numLines;
                addresses[i].offset = ADDRESSES[i] % l;
                cache[i].valid = FALSE;
            }
            direct(addresses, numLines);
            break;
        }
        case NWAY:
        {
            int n;
            printf("N: ");
            scanf("%d", &n);

            int numSets = numLines / n;
            // instantiate the cache mappings
            for(int i = 0; i < ADDR_LEN; i++) {
                addresses[i].address = ADDRESSES[i];
                tagIdx = ADDRESSES[i] / l;
                addresses[i].tag = tagIdx / numSets;
                addresses[i].offset = ADDRESSES[i] % l;
                sets[i] = tagIdx % numSets;
            }
            int setSize = numLines / numSets;
            for(int i = 0; i < numLines; i++) {
                cache[i].valid = FALSE;
                cache[i].set = i / setSize;
            }
            associative(addresses, sets, numSets, numLines);
            break;
        }
        case FULL:
        {
            // instantiate the cache mappings
            for(int i = 0; i < ADDR_LEN; i++) {
                addresses[i].address = ADDRESSES[i];
                addresses[i].offset = ADDRESSES[i] % l;
                addresses[i].tag = ADDRESSES[i] / l;
                sets[i] = 0;
            }
            for(int i = 0; i < numLines; i++) {
                cache[i].valid = FALSE;
            }
            // call associative with 1 for the number of sets
            associative(addresses, sets, 1, numLines);
            break;
        }
	}
	return 0;
}
