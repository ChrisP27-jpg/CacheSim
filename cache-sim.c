#include "cache-sim.h"

#include "memalloc.h"

#include <stdbool.h>
#include <stddef.h>

struct CacheSimImpl{
  unsigned numSetBits;
  unsigned numLinesPerSet;
  unsigned numLineBits;
  unsigned numMemAddrBits;
  Replacement replace;

  int size;
  MemAddr **cacheAddr;
  bool **valid;
};
/** Create and return a new cache-simulation structure for a
 *  cache for main memory withe the specified cache parameters params.
 *  No guarantee that *params is valid after this call.
 */
CacheSim *
new_cache_sim(const CacheParams *params)
{
  int size=1;
  for (int y=0;y< params->nSetBits; y++){size=size*2;}
  CacheSim *a=malloc(sizeof(CacheSim)+sizeof(bool **)+sizeof(MemAddr **));
  a->size=size;
  a->numSetBits = params->nSetBits;
  a->numLinesPerSet = params->nLinesPerSet;
  a->numLineBits = params->nLineBits;
  a->numMemAddrBits = params->nMemAddrBits;
  a->replace = params->replacement;

  a->valid = malloc(sizeof(bool*) * size);
  for (int z=0; z<size; z++){
    a->valid[z]=malloc( a->numLinesPerSet *sizeof(bool));
    for (int y=0; y< a->numLinesPerSet; y++){a->valid[z][y]=false;}
  }

  a->cacheAddr = malloc(size *sizeof(MemAddr*));
  for (int i = 0; i<size; i++){
    a->cacheAddr[i]=malloc( a->numLinesPerSet *sizeof(MemAddr));
    for (int j=0; j< a->numLinesPerSet; j++){ a->cacheAddr[i][j]=0;}
  }
  return a;
}

/** Free all resources used by cache-simulation structure *cache */
void
free_cache_sim(CacheSim *cache)
{
  for(int j=0;j< cache->numLinesPerSet; j++){ free(cache->valid[j]);}
  free(cache->valid);
  for(int i=0;i< cache->size; i++){
    free(cache->cacheAddr[i]);
  }
  free(cache->cacheAddr);
  free(cache);
}

/** Return result for requesting addr from cache */
CacheResult
cache_sim_result(CacheSim *cache, MemAddr addr)
{
  if(cache->replace == LRU_R){
  CacheResult result={0,0};
  int s;
  MemAddr temp;
  addr &= (~0)<< cache->numLineBits;
  s = addr>> cache->numLineBits;
  s &= ~((~0) << cache->numSetBits);
  for(int i=0; i< cache->size; i++){
    for(int j=0; j< cache->numLinesPerSet; j++){
      if(cache->cacheAddr[i][j]== addr){
        result.status = CACHE_HIT;
        result.replaceAddr=0;
	if (cache->valid[s][cache->numLinesPerSet -1]==true){
	  temp = cache->cacheAddr[i][j];
	  for (int t=1; t< cache->numLinesPerSet; t++){
    	    cache->cacheAddr[s][t-1] = cache->cacheAddr[s][t];
	  }
	  cache->cacheAddr[s][cache->numLinesPerSet -1]=temp;
	}
        return result;
      }
    }
  }

  for(int r=0; r< cache->numLinesPerSet; r++){
    if(cache->valid[s][r]==false){
      cache->valid[s][r]=true;
      cache->cacheAddr[s][r]=addr;
      result.status = CACHE_MISS_WITHOUT_REPLACE;
      return result;
    }
  }

  result.replaceAddr= cache->cacheAddr[s][0];
  for (int t=1; t< cache->numLinesPerSet; t++){
    cache->cacheAddr[s][t-1] = cache->cacheAddr[s][t];
  }
  cache->cacheAddr[s][cache->numLinesPerSet -1]=addr;
  result.status = CACHE_MISS_WITH_REPLACE;
  return result;
}

else{
  CacheResult result={0,0};
  int s;
  MemAddr temp;
  addr &= (~0)<< cache->numLineBits;
  s = addr>> cache->numLineBits;
  s &= ~((~0) << cache->numSetBits);
  for(int i=0; i< cache->size; i++){
    for(int j=0; j< cache->numLinesPerSet; j++){
      if(cache->cacheAddr[i][j]== addr){
        result.status = CACHE_HIT;
        result.replaceAddr=0;
	if (cache->valid[s][cache->numLinesPerSet -1]==true){
	  temp = cache->cacheAddr[i][j];
	  for (int t=1; t< cache->numLinesPerSet; t++){
    	    cache->cacheAddr[s][t-1] = cache->cacheAddr[s][t];
	  }
	  cache->cacheAddr[s][cache->numLinesPerSet -1]=temp;
	}
        return result;
      }
    }
  }

  for(int r=0; r< cache->numLinesPerSet; r++){
    if(cache->valid[s][r]==false){
      cache->valid[s][r]=true;
      cache->cacheAddr[s][r]=addr;
      result.status = CACHE_MISS_WITHOUT_REPLACE;
      return result;
    }
  }

  result.replaceAddr= cache->cacheAddr[s][cache->numLinesPerSet - 1];
  cache->cacheAddr[s][cache->numLinesPerSet -1]=addr;
  result.status = CACHE_MISS_WITH_REPLACE;
  return result;
}
}
