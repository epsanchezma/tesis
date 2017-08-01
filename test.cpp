#include "nodes.h"
#include <iostream>

int main () {
    Nodes *nodo = new Nodes(1);
    std::cout << "Nodo " << nodo->m_id << std::endl;

    nodo->addLink(3);
    nodo->addLink(4);
    nodo->addLink(5);
    nodo->print();
    nodo->removeLink(4);
    nodo->print();    
    
    return 0;
}