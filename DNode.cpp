/*
 * DNode.cpp
 *
 *  Created on: Apr 2, 2020
 *      Author: 13027
 */

#include <cstdlib>
#include <iostream>
#include "DNode.hpp"
using namespace std;

DNode:: DNode()
{
    prev = nullptr;
    next = nullptr;
    task = nullptr;
}

DNode:: DNode(string t, int p, int lenhr, int lenmin)
{
    task = new Task(t, p, lenhr, lenmin);
    prev = nullptr;
    next = nullptr;
}

DNode:: ~DNode()
{
    delete task;
}

