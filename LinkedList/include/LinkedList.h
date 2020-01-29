#pragma once

#include <iostream>
#include "Block.h"

struct Node_s {
    Block m_Block;
    Node_s *pNext;
    Node_s *pPrev;
    
    Node_s()
    : pNext(nullptr), pPrev(nullptr) {}
};

class List
{
public:
    List();
    ~List();
    
private:
    Node_s *pHead;
    Node_s *pTail;
};
