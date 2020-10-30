#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <stdlib.h>

#define	MAX_CACHE_SIZE		8192

// File where the data stream is stored
#define TRACE_FILE_NAME		"ref_stream.txt"

// Node type for double linked list
typedef struct buffer {
	unsigned long blkno;
	struct buffer* next, * prev;
    int used;
} buffer;

// Space for data storage
// No more memory alloc after this
// Need to maintain LRU list or FIFO list with the allocated nodes
struct buffer cache_buffer[MAX_CACHE_SIZE];

// Head node for maintaining lrulist (in case of LRU simulator) or fifolist: not used in this implementation
#if 1
struct buffer lrulist;
#else
struct buffer fifolist;
#endif

void init() {
    // initialize all values to 0 (set 0 as the "empty" buffer)
    for (int i=0; i<MAX_CACHE_SIZE; i++)
        cache_buffer[i].blkno = 0;
}

// insert buffer after [pos] in cache_buffer.
void insert(int pos, unsigned long data, int called) {
    struct buffer* new_node = &cache_buffer[0] + pos;

    new_node->blkno = data;
    new_node->used = called;
    
    if (pos != 0)
        new_node->prev = &cache_buffer[0] + pos;
    else
        new_node->prev = &cache_buffer[0] + (MAX_CACHE_SIZE-1);
    
    if (pos < MAX_CACHE_SIZE-1)
        new_node->next = &cache_buffer[0] + (pos+1);
    else
        new_node->next = &cache_buffer[0];
    
    //printf("%lu added in %d \n", data, pos);
    
}

// delete buffer at [pos] in cache_buffer.
void delete(int pos) {
    struct buffer* removed = &cache_buffer[0] + pos;
    
    removed->prev->next = removed->next;
    removed->next->prev = removed->prev;
    
    removed->blkno = 0;
}

buffer* search(unsigned long data) {
    for (int i=0; i<MAX_CACHE_SIZE; i++)
        if ((&cache_buffer[0]+i)->blkno == data)
            return &cache_buffer[0]+i;
    return NULL;
}

int main(int argc, char* argv[])
{
	int	ret;
	unsigned long blkno;
	FILE* fp = NULL;

    size_t read;
    char* line = NULL;
    size_t len = 0;
    
    init();

	if ((fp = fopen(TRACE_FILE_NAME, "r")) == NULL) {
		printf("%s trace file open fail.\n", TRACE_FILE_NAME);

		return 0;
	}
	else {
		printf("\nstart simulation!\n\n");
	}

	////////////////////////////////////////////////
	// Start the simulation and print the output
	////////////////////////////////////////////////

    // read ref data from stream
    int nline = 0;
    int del_index = 0;
    int called = 0;
    
    float hit = 0;
    float miss = 0;
    float hit_ratio = 0.;
    
    while ((read = getline(&line, &len, fp)) != -1) {
        //printf("%s", line);

        // everytime a data is called (whether it is present in the cache or not), add up the variable "called" and store it in the buffer. This will be used to determine the least recently used ref data.
        
        // for the first 8192 ref data
        if (nline < MAX_CACHE_SIZE) {
            // if the ref data is in the cache
            if (search(atol(line)) != NULL) {
                search(atol(line))->used = called++;
                //printf("[%d] is found from cache, used = %d\n", atol(line),search(atol(line))->used);
                hit++;
            }
            // otherwise, insert the data in the cache
            else {
                insert(nline,atol(line),called++);
                //printf("[%d] is inserted, used = %d\n", atol(line),(cache_buffer+nline)->used);
                nline++;
                miss++;
            }
        }
        else {
            // if the ref data is in the cache
            if (search(atol(line)) != NULL) {
                search(atol(line))->used = called++;
                //printf("[%d] is found from cache, used = %d\n", atol(line),search(atol(line))->used);
                hit++;
            }
            // otherwise, insert the data in the cache
            else {
                del_index = 0;
                // find the ref data in the cache that is least recently used
                for (int i=0; i<MAX_CACHE_SIZE; i++) {
                    if ((&cache_buffer[0]+i)->used <= (&cache_buffer[0]+del_index)->used) del_index = i;
                    //printf("%d <= %d \n", del_index, (cache_buffer+i));
                }
                // delete the least recently used ref data from the cache
                //printf("[%d] at %d is to be deleted, used = %d\n", (&cache_buffer[0]+del_index)->blkno, del_index, (&cache_buffer[0]+del_index)->used);
                delete(del_index);
                //printf("[%d] at %d is deleted, used = %d\n", (&cache_buffer[0]+del_index)->blkno, del_index, (&cache_buffer[0]+del_index)->used);
                
                // in the deleted place, insert the new data
                insert(del_index, atol(line), called++);
                //printf("[%d] at %d is inserted, used = %d\n", (&cache_buffer[0]+del_index)->blkno, del_index, (&cache_buffer[0]+del_index)->used);
                miss++;
            }
        }
    }
    
	fclose(fp);

    hit_ratio = 100*hit/(hit+miss);
    
    printf("Hit: %d out of %d\n", (int)hit, (int)(hit+miss));
    printf("Miss: %d out of %d\n", (int)miss, (int)(hit+miss));
    printf("Hit Ratio: %.2f percent\n", hit_ratio);
    
	return 0;
    
}
