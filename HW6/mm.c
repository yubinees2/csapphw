/*
 * Student ID : 2015142069
 * Name : 안유빈
 *
 * */


/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
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
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "유비니와쪄염뿌우'ㅅ'-3",
    /* First member's full name */
    "안유빈",
    /* First member's email address */
    "dksdbqls23@naver.com",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};


/*  based on minimum block size 8 */
#define WSIZE 4	// size of the header
#define DSIZE 8 // default block size
#define CHUNKSIZE 512 // when extending heap

/* pack size bit and allocated bit into same word */
#define PACK(size, alloc)((size) | (alloc))

/* read or write to mem addr p */
#define GET(p)	(* (unsigned int *)(p))
#define PUT(p, val)	(*(unsigned int *)(p) = (val))

/* given addr p, read size bit or allocated bit */
#define GET_SIZE(p)	(GET(p) & ~0x7) 
#define GET_ALLOC(p) (GET(p) & 0x1)

/* given block ptr bp */
#define HDRP(bp)	((char *)(bp) - WSIZE) //HeaderPointer
#define FTRP(bp)	((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE) //FooterPointer
#define NEXT_BLKP(bp)	((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp)	((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))


/* take root */
static void *root = NULL; /* points to the first block past */


/* function prototypes */
static void *coalesce(void *bp);
static void *extend_heap(size_t words);
static void *find_fit(size_t asize);
static void place(void *bp, size_t asize);
static void copy_block(void *src, void *dest);
size_t adjust_size(size_t size);
#define MAX(x, y) ((x) > (y)? (x) : (y))

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
	/* create the initial empty heap, heap_listp holds */
	void *heap_listp = mem_sbrk(4*WSIZE);
	if (heap_listp == (void *)(-1)){ return -1;}

	PUT(heap_listp, 0); /* for alignment */
	heap_listp += WSIZE;
	PUT(heap_listp, PACK(DSIZE, 1)); // header
	heap_listp += WSIZE;
	PUT(heap_listp, PACK(DSIZE, 1)); // footer
	heap_listp += WSIZE;
	PUT(heap_listp, PACK(0, 1)); /* epiprologue header */
	
	if (extend_heap(CHUNKSIZE/WSIZE) == NULL) {return -1;}
	root = heap_listp - WSIZE; /* changes root to first block after init*/
    return 0;
}
		

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{

	if (size == 0)	return NULL; // test validation
	
	size_t asize;	/* adjusted block size */
	size_t extendsize;	/* amount to extend heap if no fit */
	char *bp;
	
	/* adjust block size to include overhead and alignment reqs. */
	asize = adjust_size(size);
	
	/* search the free list*/
	if ((bp = find_fit(asize)) != NULL) {
		place(bp, asize);
		return bp;
	}

	/* no fit found. get more memory and place the block */
	extendsize = MAX(asize,CHUNKSIZE);
	if ((bp = extend_heap(extendsize/WSIZE)) != NULL){
		place(bp, asize);
		return bp;
	}
	
	return NULL;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *bp)
{
	size_t size = GET_SIZE(HDRP(bp));
	PUT(HDRP(bp), PACK(size, 0)); // put not allocated to header
	PUT(FTRP(bp), PACK(size, 0)); // put not allocated to header
	coalesce(bp); // try to coalesce
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
	/* check if not allocated */
	if(ptr == NULL) return mm_malloc(size);

	/* check if just want to free */
	else if(size == 0)
	{
		mm_free(ptr);
		return NULL;
	}


	/* compare sizes */
	else
	{
		void* new_ptr = ptr; /* for possible relocation */
		size_t old_size = GET_SIZE(HDRP(ptr));
		size_t old_next_size = GET_SIZE(HDRP(NEXT_BLKP(ptr)));
		size_t new_size;
		size_t tmp_size;

		/* adjust block size to include overhead and alignment reqs. */
		new_size = adjust_size(size);

		/* if new size is greater, find space for new block size */
		if(new_size > old_size)
		{
			/* check if possible to coalesce with next block to make enough space */
			size_t tmp_size = old_next_size + old_size; /* hypothetical combined block size */
			if(!GET_ALLOC(HDRP(NEXT_BLKP(ptr))) && (tmp_size >= new_size))
			{
				PUT(HDRP(ptr), PACK(tmp_size, 0)); /* trick place into thinking it is one contiguous block */
				place(ptr, new_size); /* place will split the next block if necessary */
				return ptr;
			}
			/* since not possible, find another place to reallocate to, note that this leaves out possibility of merging backwards */
			else if ((new_ptr = find_fit(new_size)) != NULL) 
			{
				place(new_ptr, new_size);
				copy_block(ptr, new_ptr);
				mm_free(ptr);
				return new_ptr;
			}
			else /* if no space, extend heap */
			{
				size_t extendsize = MAX(new_size,CHUNKSIZE);
				if ((new_ptr = extend_heap(extendsize/WSIZE)) == NULL){return NULL;}
				else {
					place(new_ptr, new_size);
					copy_block(ptr, new_ptr);
					mm_free(ptr);
					return new_ptr;
				}
			}
		}
			
		/* new size is smaller or equal */	
		else 
		{
			size_t extra_space = old_size - new_size; /* because old_size and new_size are both DWORD-Aligned, extra-space is a multiple of DWORD */
			size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(ptr)));
			size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(ptr)));
			if (extra_space < DSIZE) {return ptr;} /* reduction in size too little to make any changes */
			else if (prev_alloc && next_alloc && (extra_space < 2*DSIZE)) {return ptr;} /* no free neighbours and extra space less than minimum block size */
			else if (!next_alloc) /* give extra space to next block */
			{
				PUT(HDRP(ptr), PACK(new_size, 1));
				PUT((ptr + new_size - DSIZE), PACK(new_size, 1));
				PUT((ptr + new_size - WSIZE), PACK(old_next_size + extra_space, 0));
				PUT(FTRP(NEXT_BLKP(ptr)), PACK(old_next_size + extra_space, 0));
				return ptr;
			}
			else if (!prev_alloc) /* copy data into free previous block so that data is not lost before being copied, free block is now moved forward */
			{
				new_ptr = PREV_BLKP(ptr);
				size_t old_prev_size = GET_SIZE(HDRP(new_ptr));
				PUT(FTRP(ptr), PACK(old_prev_size + extra_space, 0)); /* before memory location of header is erased */
				copy_block(ptr, new_ptr);
				PUT(HDRP(new_ptr), PACK(new_size, 1));
				PUT(FTRP(new_ptr), PACK(new_size, 1));
				PUT(HDRP(NEXT_BLKP(new_ptr)), PACK(old_prev_size + extra_space, 0));
				return new_ptr;
			}
			else { /* no free neighbours but extra space enough to split block in 2 */
				PUT(HDRP(ptr), PACK(new_size, 1));
				PUT(FTRP(ptr), PACK(new_size, 1));
				PUT(HDRP(NEXT_BLKP(ptr)), PACK(extra_space, 0));
				PUT(FTRP(NEXT_BLKP(ptr)), PACK(extra_space, 0));
				return ptr;
			}
		}
	}
}


/* PRIVATE FUNCTIONS */


size_t adjust_size(size_t size){
	return (size <= DSIZE)? 2*DSIZE : DSIZE * ((size + (DSIZE) + (DSIZE-1)) / DSIZE);
}

/* checks for all cases when a block is freed and performs correct coalesce */
static void *coalesce(void *bp) 
{
	size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
	size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
	size_t size = GET_SIZE(HDRP(bp));

	if (prev_alloc && next_alloc) {			/* Case 1: no coalesce required */
		return bp;
	}

	else if (prev_alloc && !next_alloc) {		/* Case 2: coalesce with next block */
		size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
		PUT (HDRP(bp), PACK(size,0));
		PUT (FTRP(bp), PACK(size,0));
	}

	else if (!prev_alloc && next_alloc) {		/* Case 3: coalesce with prev block */
		size += GET_SIZE(HDRP(PREV_BLKP(bp)));
		PUT(FTRP(bp), PACK(size, 0));
		PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
		bp = PREV_BLKP(bp);
	}

	else {						/* Case 4: coalesce with both prev and next block*/
		size += GET_SIZE(HDRP(PREV_BLKP(bp))) +
				GET_SIZE(FTRP(NEXT_BLKP(bp)));
		PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
		PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
		bp = PREV_BLKP(bp);
	}
	return bp;
}

/* extends the heap, checks if enough memory available, and whether size requested is aligned */
static void *extend_heap(size_t words)
{
	char *bp;
	size_t size;
	
	/* allocate an even number of words to maintain alignment */
	size = (words % 2) ? (words+1) * WSIZE : words * WSIZE;
	if ((long)(bp = mem_sbrk(size)) == (-1)){return NULL;}
	
	/* initialize free block header/footer and the epilogue header */
	PUT(HDRP(bp), PACK(size, 0));		/* free block header */
	PUT(FTRP(bp), PACK(size, 0));		/* free block footer */
	PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));	/* new epilogue header */
	
	/* coalesce if the previous block was free */
	return coalesce(bp);
}

/* next-fit */
static void *find_fit(size_t asize)
{
	void *bp = root;
	while(GET_SIZE(HDRP(bp))>0){
		if (!GET_ALLOC(HDRP(bp)) && (asize <= GET_SIZE(HDRP(bp)))) {
			return bp;
		}
		else bp = NEXT_BLKP(bp);
	}

	return NULL; /* No fit case */
}

/* Actual allocation by placing size and allocated bits, memory pointer in*/ 
static void place(void *bp, size_t asize)
{
	size_t csize = GET_SIZE(HDRP(bp));

	if ((csize - asize) >= (2*DSIZE)) {
		PUT(HDRP(bp), PACK(asize, 1));
		PUT(FTRP(bp), PACK(asize, 1));
		bp = NEXT_BLKP(bp);
		PUT(HDRP(bp), PACK(csize-asize, 0));
		PUT(FTRP(bp), PACK(csize-asize, 0));
	}
	else {
		PUT(HDRP(bp), PACK(csize, 1));
		PUT(FTRP(bp), PACK(csize, 1));
	}
}

/* used by mm_realloc in case new ptr within heap needed */
static void copy_block(void *src, void *dest)
{
	size_t payload_size = GET_SIZE(HDRP(src)) - DSIZE; /* remove total size of header and footer */
	char *curr_src = (char *)src; 
	char *curr_dest = (char *)dest;
	while (curr_src <((char *)src + payload_size) ){
		*curr_dest = *curr_src;
		curr_dest ++;
		curr_src++;
	}
	return;
} 
