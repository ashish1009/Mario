#include "LinkedList.h"
#include "Logger.h"

List::List()
: pHead(nullptr), pTail(nullptr)
{
    LogInfo(BIT_LIST, "List() Constructor !!!\n");
}

List::~List()
{
    LogInfo(BIT_LIST, "List() Destructor !!!\n");
}
