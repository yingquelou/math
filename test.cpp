#include <iostream>
#include <vector>
struct Node
{
    using type = std::pair<int, int>;
    type cur;
    std::vector<type> box;
    Node(const int &i, const int &j) : cur(i, j) {}
    void push(const int &i, const int &j) { box.push_back(type(i, j)); }
};
struct Tree
{
    Node node;
    Tree *next;
};

std::ostream &operator<<(std::ostream &_os_, const Node &node)
{
    return _os_ << node.cur.first << '\t' << node.cur.second;
}
int main(int argc, char *argv[])
{
    // 史-0 t-1 w-2
    int arr[][4]{{0, 2, 1, 0}, {1, 0, 2, 2}, {2, 0, 1, 1}};
    Node head(1, 1);
    auto &os = std::cout;
    os << head;
    return 0;
}
template <size_t r, size_t c>
Tree *start(const int (&arr)[r][c], const int &i, const int &j)
{


}