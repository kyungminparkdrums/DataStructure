#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include<time.h>


#define	MAX_CACHE_SIZE		8192

// File where the data stream is stored
#define TRACE_FILE_NAME		"ref_stream.txt"


// Node type of doubly-link list
struct buffer {
	unsigned long blkno;
	struct buffer* next, * prev;
    struct buffer* hash_next, * hash_prev;
};

// Space for data storage
// No more memory alloc after this
// Need to maintain LRU list or FIFO list with the allocated nodes
struct buffer cache_buffer[MAX_CACHE_SIZE];

// hash table
# define HASH_SIZE   6144 // 75% of MAX_CACHE_SIZE; 75% of 8192
struct buffer hash_table[HASH_SIZE];

// Head node for maintaining lrulist (in case of LRU simulator) or fifolist
#if 1
struct buffer lrulist;
#else
struct buffer fifolist;
#endif

unsigned long curtime, hit, miss;

void lruin(struct buffer* bp) {
    struct buffer* dp = &lrulist;
    
    bp->next = dp->next;
    bp->prev = dp;
    (dp->next)->prev = bp;
    dp->next = bp;
}

struct buffer* lruout() {
    struct buffer* bp;
    
    bp = lrulist.prev;
    
    // when deleting a buffer from the cache, delete its hash value from the hash table
    if (bp->hash_prev)
        (bp->hash_prev)->hash_next = bp->hash_next;
    if (bp->hash_next)
        (bp->hash_next)->hash_prev = bp->hash_prev;
    
    (bp->prev)->next = bp->next;
    (bp->next)->prev = bp->prev;
    
    return bp;
}

void reorder(struct buffer* bp) {
    (bp->prev)->next = bp->next;
    (bp->next)->prev = bp->prev;

    //printf("%lu reordered next to head\n", bp->blkno);
    
    lruin(bp);
}

void init() {
    int i;
    
    lrulist.next = lrulist.prev = &lrulist;
    
    for (i=0; i<MAX_CACHE_SIZE;i++){
        cache_buffer[i].blkno = ~0;
        cache_buffer[i].next = cache_buffer[i].prev = NULL;
        cache_buffer[i].hash_next = cache_buffer[i].hash_prev = NULL;
        
        lruin(&cache_buffer[i]);
    }

    return;
}

// hashing
int hash_function(unsigned long blkno){
    return blkno % HASH_SIZE;
}

// hash
struct buffer* findblk(unsigned long blkno, struct buffer *hash_table[]) {
    /*
    struct buffer* dp;
    struct buffer* bp;
    
    dp = &lrulist;
    for (bp = dp->next; bp != dp; bp = bp->next) {
        if (bp->blkno == blkno) {
            return bp;
        }
    }
    */
    int hash_value = hash_function(blkno);
    
    struct buffer* node;
    struct buffer* bp;
    for (node = hash_table[hash_value]; node ; node = node->hash_next) {
        if (node->blkno == blkno) {
            //printf("Found the key %lu with hash %d\n", node->blkno, hash_value);
            return node;
        }
    }
    
    return NULL;
}

// add to hash table
void hash_chain_add(unsigned long blkno, struct buffer* bufp, struct buffer* hash_table[]) {
    int hash_value = hash_function(blkno);
    
    //printf("Hashvalue %d\n",hash_value);
    
    struct buffer* node = hash_table[hash_value];
    bufp->blkno = blkno;
    bufp->hash_next = NULL;
    
    if (node) {
        node->hash_prev = bufp;
        hash_table[hash_value] = bufp;
        bufp->hash_next = node;
    }
    else {
        hash_table[hash_value] = bufp;
    }
}

void pgref(unsigned long blkno, struct buffer* hash_table[]) {
    struct buffer* bufp;
    
    // search blkno in lrulist
    bufp = findblk(blkno, hash_table);
    //printf("SEARCH\n");
    
    if (bufp) {
        // hit
        hit++;
        //printf("Found the key %lu, hit %d\n", bufp->blkno, hit);
        reorder(bufp);
    }
    else {
        // miss
        miss++;
        //printf("Key %lu with hash %d NOT found\n", blkno, hash_function(blkno));
        
        bufp = lruout();
        bufp->blkno = blkno;
        lruin(bufp);
        //printf("LRUIN\n");
        
        hash_chain_add(blkno, bufp, hash_table);
    }
}

int main(int argc, char* argv[])
{
	int	ret;
	unsigned long blkno;
	FILE* fp = NULL;

	init();
    
    curtime = miss = hit = 0;

	if ((fp = fopen(TRACE_FILE_NAME, "r")) == NULL) {
		printf("%s trace file open fail.\n", TRACE_FILE_NAME);

		return 0;
	}
	else {
		printf("start simulation!\n");
	}

    clock_t begin = clock();
	////////////////////////////////////////////////
	// Simulation
	////////////////////////////////////////////////
    while ((ret = fscanf(fp, "%lu\n", &blkno)) != EOF) {
        curtime++;
        pgref(blkno, hash_table);
    }
    clock_t end = clock();
    float time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    
	fclose(fp);

    printf("total access = %lu, hit = %lu, miss = %lu\n", curtime, hit, miss);
    printf("hit ratio = %f, miss ratio = %f\n", (float)hit/(float)curtime, (float)miss/(float)curtime);
    printf("time = %f\n", time_spent);
	return 0;
}
