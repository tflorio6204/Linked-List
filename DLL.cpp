// Ryan Vink & Thomas Florio
#include "DNode.hpp"
#include "DLL.hpp"
#include <iostream>
#include <cstdlib>
using namespace std;

DLL::DLL(){
    numTasks = 0;
    tothrs = 0;
    totmin = 0;
    first = nullptr;
    last = nullptr;
}

DLL::DLL(string t, int p, int h, int m){
    first = new DNode(t, p, h, m);
    last = first;
    numTasks = 1;
    tothrs = 0;
    totmin = 0;
    addTime(tothrs, totmin);
}

DLL::~DLL()
{
    // if everything was deleted already
    if (first == nullptr) {
        return;
    }
    DNode* current = first;
    while(current -> next != nullptr)
    {
        current = current -> next;
        delete current -> prev;
    }
    delete last;
}

void DLL::push(string n, int p, int h, int m) {
    addTime(h, m);
    numTasks++;
    auto *insertNode = new DNode(n, p, h, m);
    // if list is empty
    if (last == nullptr && first == nullptr) {
        last = insertNode;
        first = insertNode;
    }
    else if (last->task->priority <= p) {
        last->next = insertNode;
        insertNode->prev = last;
        last = last->next;
    }
    else if (first->task->priority > p) {
        insertNode->next = first;
        first->prev = insertNode;
        first = insertNode;
    }
    else {
        DNode* currentNode = last;
        while (currentNode->task->priority > p) {
            currentNode = currentNode->prev;
        }
        insertNode->prev = currentNode;
        insertNode->next = currentNode->next;
        currentNode->next->prev = insertNode;
        currentNode->next = insertNode;
    }
}

Task *DLL::pop() { // Assuming at least 1 element exists
    DNode* temp = first;
    if (numTasks == 1) { // make it empty
        first = nullptr;
        last = nullptr;
    }
    else {
        first = first->next;
        first->prev = nullptr;
    }
    removeTime(temp->task->hr, temp->task->min);
    numTasks--;
    return temp->task;
}

int DLL::remove(int tn) {
    if (first->task->tasknum == tn) {
        Task* t = pop();
        delete t;
        return tn;
    }
    else if (last->task->tasknum == tn) {
        removeTime(last->task->hr, last->task->min);
        numTasks--;
        last = last->prev;
        last->next = nullptr;
        return tn;
    }
    DNode* currentNode = first;
    while (currentNode->task->tasknum != tn) {
        currentNode = currentNode->next;
        if (currentNode == nullptr) {
            // We reached the end and couldn't find node with task tn
            return -1;
        }
    }
    currentNode->next->prev = currentNode->prev;
    currentNode->prev->next = currentNode->next;
    removeTime(currentNode->task->hr, currentNode->task->min);
    numTasks--;
    delete currentNode;
    return tn;
}

void DLL::addTime(int h, int m) {
    tothrs += h;
    totmin += m;
    if (totmin >= 60) { // adjusting time format
        tothrs = tothrs + (int)(totmin/60);
        totmin = totmin % 60;
    }
}

void DLL::removeTime(int h, int m) {
    tothrs -= h;
    totmin -= m;
    if (totmin < 0) { // adjusting for time being negative
        tothrs--;
        totmin = 60 + totmin;
    }
}

void DLL::moveUp(int t) { // moving a task to the top of list
    if (first->task->tasknum == t) { // first element is being moved to end
        first->task->priority = last->task->priority; // change the priority
        last->next = first;
        first->prev = last;
        last = last->next;
        first = first->next;
        first->prev = nullptr;
        last->next = nullptr;
    }
    else if (first->next->task->tasknum == t) {
        if (first->next->task->priority > first->task->priority) {
            first->next->task->priority = first->task->priority;
        }
        first->prev = first->next;
        first->next->prev = nullptr;
        first->next = first->next->next;
        first->prev->next = first;
        first = first->prev;
    }
    else if (last->task->tasknum == t)
    {
        if (last->task->priority > last->prev->task->priority)
        {
            last->task->priority = last->prev->task->priority;
        }
        DNode * temp = last -> prev;
        last -> prev -> prev -> next = last;
        last -> next = temp;
        temp -> prev = last;
        temp -> next = nullptr;
    }
    else {
        DNode *current = first;
        while (current->task->tasknum != t) {
            current = current->next;
        }
        if (current->task->priority > current->prev->task->priority) {
            current->task->priority = current->prev->task->priority;
        }
        DNode* temp = current->prev;
        current->prev = current->prev->prev;
        current->prev->next = current;
        current->next->prev = temp;
        temp->next = current->next;
        temp->prev = current;
        current->next = temp;
    }
}

void DLL::moveDown(int tn) {
    if (last->task->tasknum == tn) { // last element is being moved to end
        last->task->priority = first->task->priority; // change the priority
        first->prev = last;
        last->next = first;
        first = first->prev;
        last = last->prev;
        last->next = nullptr;
        first->prev = nullptr;
    }
    else if (last->prev->task->tasknum == tn) { //check second to last item and move to end
        if (last->prev->task->priority > last->task->priority) {
            last->prev->task->priority = last->task->priority;
        }
        last->next = last->prev;
        last->prev->next = nullptr; // set second to last new last (nullptr)
        last->prev = last->prev->prev;
        last->next->prev = last;
        last = last->next;

    }
    else if (first->task->tasknum == tn) //check first item in list to move to end
    {
        if (first->task->priority < first->next->task->priority)
        {
            first->task->priority = first->next->task->priority;
        }
        DNode* temp = first->next; //used to save first -> next data
        first->next = first->next->next;
        first->next->prev = first;
        temp->next = first;
        first->prev = temp;
        temp->prev = nullptr;
        first = temp;
    }
    else {
        DNode *current = last;
        while (current->task->tasknum != tn) {
            current = current->prev;
        }
        if (current->task->priority < current->next->task->priority) {
            current->task->priority = current->next->task->priority;
        }
        DNode* temp = current->next;
        current->next = current->next->next;
        current->next->prev = current;
        current->prev->next = temp;
        temp->prev = current->prev;
        temp->next = current;
        current->prev = temp;
    }
}


void DLL::changePriority(int tn, int newp) {
    DNode* currentNode = first;
    while (currentNode->task->tasknum != tn) {
        currentNode = currentNode->next;
    }
    if(currentNode -> task -> priority < newp) { // checks if newp is on second half of list and moves accordingly
        while(currentNode -> task -> priority != newp) {
            moveDown(tn);
        }
        moveUp(tn);
    }
    else if(currentNode -> task -> priority > newp) {
        while(currentNode -> task -> priority != newp) {
            moveUp(tn);
        }
        moveDown(tn);
    }
    else {
        while((currentNode -> next -> task -> priority == newp) || (currentNode == last)) {
            moveDown(tn);
        }
    }
}

void DLL::listDuration(int *th, int *tm,int tp) {
    *th = *tm = 0;
    DNode* currentNode = first;
    while (currentNode != nullptr) {
        if (currentNode->task->priority == tp) {
            *th += currentNode->task->hr;
            *tm += currentNode->task->min;
        }
        currentNode = currentNode->next;
    }
    if (*tm >= 60) {
        *th = *th + (int)(*tm/60); //modify hours to minutes and minutes
        *tm = *tm % 60;
    }
}

void DLL::printList() {
    DNode *current = first;
    cout << "Total Time Required: "<<tothrs<< ":"<<totmin<<endl;
    while (current != nullptr) {
        current->task->printTask(); // print each item
        current = current->next;
    }
    cout << endl;
}

void DLL::printList(int p) { // prints list of priority p only
    int hours;
    int minutes;
    listDuration(&hours, &minutes, p);
    cout << p <<": Total Time Required: "<< hours << ":"<< minutes <<endl;
    DNode *current = first;
    while (current != nullptr) {
        if (current->task->priority == p) {
            current->task->printTask();
        }
        current = current->next;
    }
    cout << endl;
}