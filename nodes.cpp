#include <iostream>
#include "nodes.h"

Nodes::Nodes (int id) {
    m_id = id;
}

Nodes::~Nodes () {
}

void Nodes::addLink(int linkId) {
    links.push_back(linkId);
}

void Nodes::removeLink(int linkId) {
    // TODO: sort links to use binary_search instead of find
    ptrdiff_t pos = std::distance(links.begin(), find(links.begin(), links.end(), linkId));
    if (pos < links.size()) {
        links.erase(links.begin() + pos);
    }
}

void Nodes::print() {
    std::cout << "id " << m_id << std::endl;  

    for(int i = 0; i < links.size(); i++) {
        std::cout << "link " << links[i] << std::endl;       
    }
}

// #include "nodes.h"
// #include <iostream>

// int main () {
//     Nodes *nodo = new Nodes(1);
//     std::cout << "Nodo " << nodo->m_id << std::endl;

//     nodo->addLink(3);
//     nodo->addLink(4);
//     nodo->addLink(5);
//     nodo->print();
//     nodo->removeLink(4);
//     nodo->print();    
    
//     return 0;
// }