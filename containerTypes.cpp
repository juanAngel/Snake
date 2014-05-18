#include "containerTypes.h"


void addNodeBefore(linkList* first,linkList* next){
    if(first && next){
        linkList* after = first->next;
        first->next = next;
        next->next = after;
    }
}


linkList* delNextNode(linkList *node){
    if(node){
        linkList* next = node->next;
        if(next)
            node->next = next->next;
        return next;
    }
    return nullptr;
}


void insertNode(linkList *listNode, linkList *node){
    if(listNode && node){
        node->next = listNode->next;
        listNode->next = node;
    }
}


void initVector(vector *v, ulong nElementMax, ulong typeSize){
    v->typeSize = typeSize;
    v->nElementMax = nElementMax;
    v->nElementUsaje = 0;
    v->buff = (byte*)calloc(nElementMax,typeSize);
}


void uinitVector(vector *v){
    free(v->buff);
}


void resizeVector(vector *v, ulong nElementMax){
    v->buff = (byte*)realloc(v->buff,(v->nElementMax = nElementMax)*v->typeSize);
    if(v->nElementMax > v->nElementUsaje)v->nElementUsaje = v->nElementMax;
}
