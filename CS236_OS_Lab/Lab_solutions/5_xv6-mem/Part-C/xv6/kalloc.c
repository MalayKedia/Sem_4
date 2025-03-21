// Physical memory allocator, intended to allocate
// memory for user processes, kernel stacks, page table pages,
// and pipe buffers. Allocates 4096-byte pages.

#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "spinlock.h"

void freerange(void *vstart, void *vend);
extern char end[]; // first address after kernel loaded from ELF file
                   // defined by the kernel linker script in kernel.ld

struct run {
  struct run *next;
};

struct refCount {
  int refs[PHYSTOP/PGSIZE]; // to track number of processes using each physical frame
};

struct {
  struct spinlock lock;
  int use_lock;
  struct run *freelist;
} kmem;

struct refCount count;

// Initialization happens in two phases.
// 1. main() calls kinit1() while still using entrypgdir to place just
// the pages mapped by entrypgdir on free list.
// 2. main() calls kinit2() with the rest of the physical pages
// after installing a full page table that maps them on all cores.
void
kinit1(void *vstart, void *vend)
{
  for (int i=0; i<PHYSTOP/PGSIZE; i++) {
    count.refs[i] = 0; // simple initialisation
  }
  initlock(&kmem.lock, "kmem");
  kmem.use_lock = 0;
  freerange(vstart, vend);
}

void
kinit2(void *vstart, void *vend)
{
  freerange(vstart, vend);
  kmem.use_lock = 1;
}

void
freerange(void *vstart, void *vend)
{
  char *p;
  p = (char*)PGROUNDUP((uint)vstart);
  for(; p + PGSIZE <= (char*)vend; p += PGSIZE)
    kfree(p);
}
//PAGEBREAK: 21
// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(char *v)
{
  struct run *r;
  
  if((uint)v % PGSIZE || v < end || V2P(v) >= PHYSTOP)
    panic("kfree");
  r = (struct run*)v;
  if (kmem.use_lock) {
    if (count.refs[V2P((char*)r)/PGSIZE] > 1) {
      count.refs[V2P((char*)r)/PGSIZE]--;
      return;
    }
    else if (count.refs[V2P((char*)r)/PGSIZE] < 1) {
      panic("Shouldn't be possible to free a page having no reference!\n");
    } 
  }
  // Fill with junk to catch dangling refs.
  memset(v, 1, PGSIZE);

  if(kmem.use_lock)
    acquire(&kmem.lock);
  
  r->next = kmem.freelist;
  kmem.freelist = r;
  if(kmem.use_lock)
    release(&kmem.lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
char*
kalloc(void)
{
  struct run *r;

  if(kmem.use_lock)
    acquire(&kmem.lock);
  r = kmem.freelist;
  if(r) {
    kmem.freelist = r->next;
    count.refs[V2P(r)/PGSIZE] = 1;
  }
    
  if(kmem.use_lock)
    release(&kmem.lock);
  return (char*)r;
}

int freePage() {

  // function to return number of free pages, which is just the size of our freelist
  struct run *r;
  int freeCount = 0;

  if(kmem.use_lock)
    acquire(&kmem.lock);
  r = kmem.freelist;
  
  while (r) {
    freeCount++;
    r = r->next;
  }

  if(kmem.use_lock)
    release(&kmem.lock);

  return freeCount;  
}

int getRefCount(uint pa) {
  return count.refs[pa/PGSIZE];
}

void increaseRefCount (uint pa) {
  count.refs[pa/PGSIZE]++;
}
void decreaseRefCount (uint pa) {
  count.refs[pa/PGSIZE]--;
}