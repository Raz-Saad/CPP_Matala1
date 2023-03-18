#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AdptArray.h"

typedef struct AdptArray_
{
    PElement *ArrElements; //array of elements
    int Arrsize; // size of the array
    COPY_FUNC CopyFunc;//pointer to a copy function
    DEL_FUNC DelFunc;//pointer to a delete function
    PRINT_FUNC PrintFunc;//pointer to a print function
}AdptArray;


PAdptArray CreateAdptArray(COPY_FUNC CopyFunc, DEL_FUNC DelFunc,PRINT_FUNC PrintFunc)
{
    PAdptArray pAdptAry;
    pAdptAry = (PAdptArray)malloc(sizeof(AdptArray));//allocate memory
    if(pAdptAry!=NULL)//checks if the allocate done successfully
    { //initializing
        pAdptAry->ArrElements = NULL;
        pAdptAry->Arrsize=0;
        pAdptAry->CopyFunc=CopyFunc;
        pAdptAry->DelFunc=DelFunc;
        pAdptAry->PrintFunc=PrintFunc;
    }
    else
    {
        return NULL;
    }
    return pAdptAry;
}

void DeleteAdptArray(PAdptArray Pa)
{//free the struct and all the elements in the array
    if(Pa==NULL)
        return;
    for(int i=0; i< Pa->Arrsize;i++)
    { //free each element from the array that isnt null
        if(Pa->ArrElements[i]!=NULL)
            Pa->DelFunc(Pa->ArrElements[i]); 
    }
    free(Pa->ArrElements); // free the elements array
    free(Pa);

}

Result SetAdptArrayAt(PAdptArray Pa, int index, PElement element)
{//store an element in the array at a specific index, if the index isnt exist so we resize the array
    PElement *temp;
    if(Pa==NULL || index < 0)
        return FAIL;

    if(index >= Pa->Arrsize)// in this case we need to resize the array
    {
        temp = (PElement)calloc((index+1),sizeof(PElement)); // reallocating the memory into a bigger array
        if(temp==NULL)
            return FAIL;   
        memcpy(temp, Pa->ArrElements, sizeof(PElement)* (Pa->Arrsize));//copy data to new array
        free(Pa->ArrElements);
        Pa->ArrElements = temp;
        Pa->Arrsize=index+1;// update array size
    }
    
    if(Pa->ArrElements[index]!=NULL)//deletes previous element if exists
        Pa->DelFunc(Pa->ArrElements[index]);
    
    if(element!=NULL)
        Pa->ArrElements[index]=Pa->CopyFunc(element);//store a new element in the wanted spot if isnt null
        
    return SUCCESS;
}


PElement GetAdptArrayAt(PAdptArray Pa, int index)
{//returns pelement from the array from a specific index
    if(Pa==NULL || index >= Pa->Arrsize || index < 0)
        return NULL;
    else
    {
        if(Pa->ArrElements[index]==NULL)
            return NULL;
        return Pa->CopyFunc(Pa->ArrElements[index]);
    }

}

int GetAdptArraySize(PAdptArray Pa)
{//returns array size
    if(Pa==NULL || Pa->ArrElements==NULL)
        return -1;
    else
        return Pa->Arrsize;
}

void PrintDB(PAdptArray Pa)
{//prints all the elements in the array
    if(Pa==NULL || Pa->ArrElements==NULL) // checks if the array is null or the struct is null, if so we dont want to print it
        return;
    
    for(int i=0; i< Pa->Arrsize;i++)
    {
        if(Pa->ArrElements[i]!=NULL) // prints only spots that isnt null
            Pa->PrintFunc(Pa->ArrElements[i]);
    }
}