#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#define u8 uint8_t
#define u16 uint16_t
#define STACK_SIZE 32
#define HEAP_SIZE STACK_SIZE * 4
#define HEADER 4

static u16 IN_USE = 0;

typedef struct virtual_memory
{
	u8 stack[STACK_SIZE];
	char** unmapped;
	u8 heap[HEAP_SIZE];

	struct {
		char** data;
		char** bss;
		char* text;	
	}data_t;
}virtual_memory_t;

typedef struct entity
{
	u8* ptr;
	u16 size;
}entity_t;

entity_t LIST[40];
void LOG()
{
	printf("OUR LOG\n");
	for (unsigned i=0; i < IN_USE; i++)
	{
		printf("Data + HEADER. [%p]. Memory of our heap free:[%u]\n", LIST[i].ptr, LIST[i].size);
	}
	printf("Entity in use:[%ld]\n", (sizeof(LIST)/sizeof(LIST[0]) - IN_USE));
}

entity_t* new_entity(size_t size)
{
	if (LIST[0].ptr == NULL && LIST[0].size == 0)
	{
		static virtual_memory_t vm;
		LIST[0].ptr = vm.heap;
		LIST[0].size = HEAP_SIZE;
		IN_USE++;
		LOG();
	}

	entity_t* best = LIST;
	for (unsigned i=0; i<IN_USE; i++)
	{
		if(LIST[i].size >= size && LIST[i].size < best->size) 
		{
			best = &LIST[i];
		} 
	}

	return best;
}

void* d_malloc(size_t size) 
{
	assert(size <= HEAP_SIZE);
	size += HEADER;
	entity_t* e = new_entity(size);

	u8* start = e->ptr;
	u8* user_ptr = start + HEADER;

	*start = size;

	e->ptr += size;
	e->size -= size;

	// assert()
	LOG();

	return user_ptr;
} 
void d_free(void* ptr) 
{
	u8* start = ptr - HEADER;
	LIST[IN_USE].ptr = &(*start); 
	LIST[IN_USE].size = (u8)*((u8*)ptr - HEADER); 
	IN_USE++;
	LOG();
}

void test() 
{
	typedef struct foo
	{
		int a;
		int b;
	}foot_t;
	
	foot_t* foo;

	char* bar;
	int* bazz;

	foo = d_malloc(sizeof(foot_t));
	bar = d_malloc(5);
	bazz = d_malloc(sizeof(int));

	foo->a = 5;
	foo->b = 10;
	strcpy(bar, "bar");
	memcpy(bazz, &foo->a, sizeof(int));

	printf("Address: [%p], data: [%d] [%d]\n", foo, foo->a, foo->b);
	printf("Address: [%p], data: [%s]\n", bar, bar);
	printf("Address: [%p], data: [%d] [%d]\n", bazz, *bazz);

	d_free(foo);
	d_free(bar);
}

int main(int argc, char** argv) 
{
	test();
	return 0;
}
