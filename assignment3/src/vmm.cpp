#include "../include/vmm.h"

#define DEBUG 1

vmm::vmm(int size)
{
  max_size = size;
}

// This instruction stores the given variableId and its value in the first unassigned spot in the memory
int vmm::memStore(std::string variableId, unsigned int value)
{
  FILE* fp;
  // Update timestamp for last access
  variable_t tmp;
  time_t rawtime;
  time (&rawtime);
  tmp.lastAccessTime = rawtime;
  tmp.variableId = variableId;
  tmp.value = value;
  struct tm * timeinfo;
  timeinfo = localtime (&rawtime);
  
  // Verify we have enough space in main memory
  if(page_table.size() >= max_size)
  {
    // No more space, append to disk space
    if(DEBUG) {
      printf("Writing to disk %s %d %s", variableId.c_str(), value, asctime(timeinfo));
    }
    fp = fopen("vm.txt", "a");
    fprintf(fp, "%s %d\n", variableId.c_str(), value);
  }
  else {
    // We have enough space!
    page_table.push_back(tmp);
  }
  return 1;
}

// This instruction removes the variable Id and its value from the memory , so the page which was holding this variable becomes available for storage.
void vmm::memFree(std::string variableId)
{
  //TODO: Should this be removed from disk space as well?

  // Search in memory for variableId
  for (int i = 0; i < page_table.size(); i++) {
    if(page_table[i].variableId == variableId) {
      page_table.erase(page_table.begin() + i);
    }
  }
}

// If the variableId exists in the main memory it returns its value. If the variableId is not in the main memory but exists in disk space (i.e. page fault occurs), then it should move this variable in to the memory.
// Notice that if no spot is available in the memory, program needs to swap this variable with the least recently accessed variable , i.e. the variable with smallest Last Access time, in the main memory.
// Finally, if the variableId does not exist in the main memory and disk space, the API should return - 1.
int vmm::memLookup(std::string variableId)
{
  // Look up in disk space, handle page fault
  for (int i = 0; i < page_table.size(); i++) {
    if(page_table[i].variableId == variableId) {
      time_t rawtime;
      time (&rawtime);
      page_table[i].lastAccessTime = rawtime;
      return page_table[i].value;
    }
  }
  return 1;
}

// This function will find the least recently accessed variable in main memory and swap it with the element with variableId in disk
void vmm::swap_memory(std::string variableId)
{
  // Least recently accessed variable, or smallest last access time should be swapped
  variable_t tmp;
  time_t rawtime;
  time (&rawtime);
  tmp.lastAccessTime = rawtime;




}

void vmm::handle_page_fault(void)
{
  // Look up in vm.txt
}
