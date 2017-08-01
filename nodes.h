#include <vector>

class Nodes {
    public:
        int m_id;
        std::vector<int> links;

        Nodes (int id);
        ~Nodes ();

        void addLink (int linkId);
        void removeLink (int linkId);
        void print ();
};