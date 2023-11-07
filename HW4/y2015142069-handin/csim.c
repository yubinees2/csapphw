/* 2015142069 안유빈*/

#include "cachelab.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <getopt.h>
#include "cachelab.h"

struct cacheblock{
	int valid; // v = 1bit;
	int tag; // tag = t bit;
	//int data; // data = 32-1-t bit; we don't need it because it is simulator.
	int time; // start from E(line size) and reduced when not used
};

struct cacheset{
	struct cacheblock *blocks;
};

struct cache{
	struct cacheset *sets;
} cache;

struct parameter{
	int t;
	int s;
	int b;
	//address = t+s+b bit
	int S;//S=2^s, number of set
	int E;//E=2^e, number of block/set
	int B;//B=2^b, bytes/block
	//cache : v(1) + tag(t) + offset(B) bit = 32bit
	char *filename;
} parameter;

struct cnt{
	int hit;
	int miss;
	int eviction;
} cnt;

FILE *file;

int parse(int argc, char **argv){
	char option;
	while((option = getopt(argc, argv, "s:E:b:t:"))!=-1){
		switch(option){
			case 's':
				parameter.s = atoi(optarg);
				parameter.S = 2<<parameter.s;
				break;
			case 'E':
				parameter.E = atoi(optarg);
				break;
			case 'b':
				parameter.b = atoi(optarg);
				parameter.B = 2<<parameter.b;
				break;
			case 't':
				parameter.filename = optarg;
				break;
			default : return 1;
		}
    	}
	parameter.t = 32-parameter.s-parameter.b;
	if (!parameter.s|!parameter.E|!parameter.b|!parameter.t) return 1;
	return 0;
}

void initialize(){
	cache.sets = malloc(sizeof(struct cacheset)*parameter.S);
	for(int i=0; i<parameter.S; i++){
		cache.sets[i].blocks = malloc(sizeof(struct cacheblock)*(parameter.E));
		memset(cache.sets[i].blocks, 0, parameter.E);
	}
	// cache = array[S][E], size(arrary[S][E])=cacheblock, size(array[S])=cacheset
	return;
}
int sim(int addr);
void simulate(){
	char readline[32];
	char *inst_line;
       	int addr_line;
	//int size_line;
	while(fgets(readline, 32, file) != NULL){ // fget *file or file?
	       //readline = " L 13fc81a0, 8"; 32 is enough to save each line. 
		if (readline[0] != 'I'){ // if instruction type is L, M, S
			//parse instruction type, address to test, size.
			inst_line = strtok(&readline[1], " ");
			addr_line = strtol(strtok(NULL, ", "), NULL, 16);
			strtok(NULL, "");
			//size_line = atoi(strtok(NULL, " "));
			//do simulation. if M, take two times.
			sim(addr_line);
			if ('M'==*inst_line) sim(addr_line);
			
		}
	}
}


int sim(int addr){
	int idx_tag = (addr<0)? ((addr&0x7fffffff)>>(32-parameter.t))+(1<<(parameter.t-1)): addr>>(32-parameter.t);
	int idx_set = (addr&(0x7fffffff>>(parameter.t-1)))>>parameter.b;
		
	struct cacheset set = cache.sets[idx_set];
	
	int i, j;
	struct cacheblock *block;
	int valid_cnt = 0;

	struct cacheblock *oldblock;

	//find hit
	for (i=0; i<parameter.E; i++){
		block = &set.blocks[i];
		if (block->valid && (block->tag==idx_tag)){
			cnt.hit += 1;
			if (block->time) block->time -= 1;
			return 1; // fix : find hit
		}
	}
	//miss case
	cnt.miss +=1;

	//if empty cache exist -> save
	for (i=0; i<parameter.E; i++){
		block = &set.blocks[i];
		
		if (block->valid){
			if (block->time) block->time -= 1;
			valid_cnt +=1;
		}
		else{
			block->valid = 1;
			block->tag = idx_tag;
			//block->data = data;
			//actual cache needs data update in this line.
			block->time = parameter.E;
			return 2; // fix : find empty cache and save
		}
		//if there is no empty cache(all are valid) : eviction case
		if (valid_cnt==parameter.E){
			cnt.eviction += 1; // eviction count up

			oldblock = &set.blocks[0];
			for (j=0; j<parameter.E; j++)	if (oldblock->time >= set.blocks[j].time) oldblock = &set.blocks[j];
			// finc longest lived block and replace it to new one.
			oldblock->valid=1;
			oldblock->tag = idx_tag;
			//actual cache need data update in this line.
			oldblock->time = parameter.E;
			return 3; // fix : find eviction
		}
	}
	return 0;
}



void clear(){
	fclose(file);
	free(cache.sets);
	for(int i=0; i<parameter.S; i++){
		free(cache.sets[i].blocks);
	}
}


int main(int argc, char **argv)
{

		
	int parse_result = parse(argc, argv);
	if (parse_result){ // if not parsed correctly
	       	printf("%s:  Missing required command line argument\n", argv[0]);
	       	return 1;
	}
	
	initialize();
	file = fopen(parameter.filename, "r");
	if (!file){
		printf("%s: No such file or directory\n", parameter.filename);
	}

	simulate(file);

	clear();
	



	printSummary(cnt.hit, cnt.miss, cnt.eviction);
	return 0;
}
