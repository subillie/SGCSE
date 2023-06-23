/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Reallocate this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your information in the following struct.
 ********************************************************/
team_t team = {
    /* Your student ID */
    "20200422",
    /* Your full name*/
    "Subin Lee",
    /* Your email address */
    "leechlo71@sogang.ac.kr",
};

/* Basic constants and macros: */
#define WSIZE 4           /* Word and header/footer size (bytes) */
#define DSIZE 8           /* Double word size (bytes) */
#define CHUNKSIZE (1<<12) /* Extend heap by this amount (bytes) */

#define MAX(x, y) ((x) > (y) ? (x) : (y))

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc) ((size) | (alloc))

/* Read and write a word at address p. */
#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (val))

/* Read the size and allocated fields from address p */
#define GET_SIZE(p)  (GET(p) & ~(DSIZE - 1)) // 0x7 = 0111
#define GET_ALLOC(p) (GET(p) & 0x1)          // 0x1 = 0001

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp) ((void *)(bp) - WSIZE)                      // Header
#define FTRP(bp) ((void *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE) // Footer

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLK(bp) ((void *)(bp) + GET_SIZE(HDRP(bp)))           // Next block
#define PREV_BLK(bp) ((void *)(bp) - GET_SIZE((void *)(bp)-DSIZE)) // Previous block

/* Given ptr in free list, get next and previous ptr in the list */
#define GET_NEXT_PTR(bp) (*(char **)(bp + WSIZE))
#define GET_PREV_PTR(bp) (*(char **)(bp))

/* Global variables */
static char *heap_listp = 0;
static char *free_listp = 0;

/* Function prototypes */
static void *first_fit(size_t adj_size);
static void *extend_heap(size_t words);
static void *coalesce(void *bp);
static void allocate(void *bp, size_t adj_size);
static void insert(void *bp);
static void delete(void *bp);

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void) {
	// Create the initial empty heap.
	if ((heap_listp = mem_sbrk(8 * WSIZE)) == (void *)-1)
		return -1;

	PUT(heap_listp, 0);							   // Alignment padding
	PUT(heap_listp + (1 * WSIZE), PACK(DSIZE, 1)); // Prologue header
	PUT(heap_listp + (2 * WSIZE), PACK(DSIZE, 1)); // Prologue footer
	PUT(heap_listp + (3 * WSIZE), PACK(0, 1));	   // Epilogue header
	free_listp = heap_listp + 2 * WSIZE;           // Set free list pointer

	// Extend the empty heap with a free block of minimum possible block size
	if (extend_heap(CHUNKSIZE / WSIZE) == NULL)
		return -1;
	return 0;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size) {
	size_t adj_size; // Adjusted block size
	size_t ext_size; // Amount to extend heap if no fit
	void *bp;

    // Ignore spurious requests
	if (size == 0)
		return (NULL);

	// Adjust block size to include overhead and alignment reqs
	if (size <= DSIZE)
		adj_size = 2 * DSIZE;
	else
		adj_size = DSIZE * ((size + DSIZE + (DSIZE - 1)) / DSIZE);

	// Search the free list for a fit
    if ((bp = first_fit(adj_size)) == NULL) {
	    // If no fit found, get more memory and allocate the block
        ext_size = MAX(adj_size, CHUNKSIZE);
        if ((bp = extend_heap(ext_size / WSIZE)) == NULL)
            return (NULL);
    }
    allocate(bp, adj_size);
	return (bp);
}

/*
 * mm_free - Freeing a block
 */
void mm_free(void *bp) {
	if (bp == NULL)
		return;

	// Free and coalesce the block
	size_t size = GET_SIZE(HDRP(bp)); // Get size of block
	PUT(HDRP(bp), PACK(size, 0));     // Set header
	PUT(FTRP(bp), PACK(size, 0));     // Set footer
	coalesce(bp);                     // Coalesce if the previous or next blocks are free
}

/*
 * mm_realloc - Reallocate a block
 */
void *mm_realloc(void *ptr, size_t size) {
    void *newptr, *oldptr = ptr;
    size_t copy_size;
    
    newptr = mm_malloc(size);
    if (newptr == NULL)
      return NULL;
    copy_size = GET_SIZE(HDRP(oldptr));
    if (size < copy_size)
      copy_size = size;
    memcpy(newptr, oldptr, copy_size);
    mm_free(oldptr);
    return newptr;
}

/* 
 * first_fit - Find a fit for a block with adj_size bytes
 */
static void *first_fit(size_t adj_size) {
	for (void *bp = free_listp; GET_ALLOC(HDRP(bp)) == 0; bp = GET_NEXT_PTR(bp))
		if (adj_size <= (size_t)GET_SIZE(HDRP(bp)))
			return bp;
	return NULL; // No fit
}

/* 
 * extend_heap - Extend heap with free block and return its block pointer
 */
static void *extend_heap(size_t words) {
	char *bp;
	size_t size = (words % 2) ? (words + 1) * WSIZE : words * WSIZE;

	// Allocate an even number of words to maintain alignment
	if (size < 2 * DSIZE)
		size = 2 * DSIZE;

	// Call for more memory space
	if ((long)(bp = mem_sbrk(size)) == -1)
		return NULL;

	// Initialize free block header/footer and the epilogue header
	PUT(HDRP(bp), PACK(size, 0));		 // free block header
	PUT(FTRP(bp), PACK(size, 0));		 // free block footer
	PUT(HDRP(NEXT_BLK(bp)), PACK(0, 1)); // new epilogue header

	// coalesce bp with next and previous blocks
	return coalesce(bp);
}

/* 
 * coalesce - Boundary tag coalescing. Return block pointer to coalesced block
 */
static void *coalesce(void *bp)  {
	size_t NEXT_ALLOC = GET_ALLOC(HDRP(NEXT_BLK(bp))); // Previous block's header
	size_t PREV_ALLOC = GET_ALLOC(FTRP(PREV_BLK(bp))) || PREV_BLK(bp) == bp; // Next block's footer
	size_t size = GET_SIZE(HDRP(bp)); // Current block's header

	// Case 1: Coalesce next block
	if (PREV_ALLOC && !NEXT_ALLOC) {
		size += GET_SIZE(HDRP(NEXT_BLK(bp)));
		delete(NEXT_BLK(bp));
		PUT(HDRP(bp), PACK(size, 0));
		PUT(FTRP(bp), PACK(size, 0));

	// Case 2: Coalesce previous block
	} else if (!PREV_ALLOC && NEXT_ALLOC) {
		size += GET_SIZE(HDRP(PREV_BLK(bp)));
		bp = PREV_BLK(bp);
		delete(bp);
		PUT(HDRP(bp), PACK(size, 0));
		PUT(FTRP(bp), PACK(size, 0));

	// Case 3: Coalesce both
	} else if (!PREV_ALLOC && !NEXT_ALLOC) {
		size += GET_SIZE(HDRP(NEXT_BLK(bp))) + GET_SIZE(HDRP(PREV_BLK(bp)));
		delete(NEXT_BLK(bp));
		delete(PREV_BLK(bp));
		bp = PREV_BLK(bp);
		PUT(HDRP(bp), PACK(size, 0));
		PUT(FTRP(bp), PACK(size, 0));
	}

	insert(bp); // Insert coalesced block into free list
	return bp;
}

/* 
 * allocate - Place block of adj_size bytes at start of free block bp
 *         and split if remainder would be at least minimum block size
 */
static void allocate(void *bp, size_t adj_size) {
	size_t cur_size = GET_SIZE(HDRP(bp)); // Size of free block

    // If the remainder is at least the minimum block size
	if ((cur_size - adj_size) >= 2 * DSIZE) {
		PUT(HDRP(bp), PACK(adj_size, 1));
		PUT(FTRP(bp), PACK(adj_size, 1));
		delete(bp);        // Delete free block from free list
		bp = NEXT_BLK(bp); // Set bp to next block
		PUT(HDRP(bp), PACK(cur_size - adj_size, 0));
		PUT(FTRP(bp), PACK(cur_size - adj_size, 0));
		coalesce(bp);
    
    // If the remainder is less than the minimum block size
	} else {
		PUT(HDRP(bp), PACK(cur_size, 1)); // Set header
		PUT(FTRP(bp), PACK(cur_size, 1)); // Set footer
		delete(bp); // Delete free block from free list
	}
}

/* 
 * insert - Insert block into free list
 */
static void insert(void *bp) {
    // Set next and previous free blocks
    GET_NEXT_PTR(bp) = free_listp;
    GET_PREV_PTR(free_listp) = bp;
    GET_PREV_PTR(bp) = NULL;
	free_listp = bp;
}

/* 
 * delete - Delete block from free list
 */
static void delete(void *bp) {
	// If bp is not the first block
	if (GET_PREV_PTR(bp))
        GET_NEXT_PTR(GET_PREV_PTR(bp)) = GET_NEXT_PTR(bp);

	// If bp is the first block
    else
		free_listp = GET_NEXT_PTR(bp);

    GET_PREV_PTR(GET_NEXT_PTR(bp)) = GET_PREV_PTR(bp);
}
