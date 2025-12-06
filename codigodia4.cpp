#include <iostream>
#include <vector>
#include <string>
#include <fstream>  
using namespace std;


template <typename T>
class Node {
public:
    T data;
    Node<T>* next;

    Node(const T& data, Node<T>* next = nullptr)
        : data(data), next(next) {}

    friend ostream& operator<<(ostream& out, const Node<T>& node) {
        out << node.data;
        return out;
    }
};


template <typename T>
class List {
public:
    virtual void insert(int pos, T e) = 0;
    virtual void append(T e) = 0;
    virtual void prepend(T e) = 0;
    virtual T remove(int pos) = 0;
    virtual T get(int pos) = 0;
    virtual int search(T e) = 0;
    virtual bool empty() = 0;
    virtual int size() = 0;
};


template <typename T>
class ListLinked : public List<T> {
private:
    Node<T>* first;
    int n;

public:
    ListLinked() {
        first = nullptr;
        n = 0;
    }

    ~ListLinked() {
        Node<T>* aux;
        while (first != nullptr) {
            aux = first->next;
            delete first;
            first = aux;
        }
    }

    void insert(int pos, T e) override {
        if (pos < 0 || pos > n)
            throw out_of_range("Posición fuera de rango");

        if (pos == 0)
            first = new Node<T>(e, first);
        else {
            Node<T>* actual = first;
            for (int i = 0; i < pos - 1; i++)
                actual = actual->next;

            actual->next = new Node<T>(e, actual->next);
        }
        n++;
    }

    void append(T e) override {
        insert(n, e);
    }

    void prepend(T e) override {
        insert(0, e);
    }

    T remove(int pos) override {
        if (pos < 0 || pos >= n)
            throw out_of_range("Posición fuera de rango");

        T eliminado;
        if (pos == 0) {
            Node<T>* borrar = first;
            eliminado = borrar->data;
            first = first->next;
            delete borrar;
        } else {
            Node<T>* actual = first;
            for (int i = 0; i < pos - 1; i++)
                actual = actual->next;

            Node<T>* borrar = actual->next;
            eliminado = borrar->data;
            actual->next = borrar->next;
            delete borrar;
        }

        n--;
        return eliminado;
    }

    T get(int pos) override {
        if (pos < 0 || pos >= n)
            throw out_of_range("Posición fuera de rango");

        Node<T>* actual = first;
        for (int i = 0; i < pos; i++)
            actual = actual->next;

        return actual->data;
    }

    int search(T e) override {
        Node<T>* actual = first;
        int i = 0;
        while (actual != nullptr) {
            if (actual->data == e)
                return i;
            actual = actual->next;
            i++;
        }
        return -1;
    }

    bool empty() override { return n == 0; }
    int size() override { return n; }

    T operator[](int pos) { return get(pos); }

    friend ostream& operator<<(ostream& out, const ListLinked<T>& list) {
        Node<T>* actual = list.first;
        out << "[";
        while (actual != nullptr) {
            out << actual->data;
            if (actual->next != nullptr) out << ", ";
            actual = actual->next;
        }
        out << "]";
        return out;
    }
};


class Graph {
private:
    int nVertices;
    ListLinked<int>* adj; // array de listas enlazadas

public:
    Graph(int n) {
        nVertices = n;
        adj = new ListLinked<int>[n];
    }

    ~Graph() {
        delete[] adj;
    }

    void addEdge(int origen, int destino) {
        adj[origen].append(destino);
    }

    ListLinked<int>& neighbors(int v) {
        return adj[v];
    }
};



const int dx[8] = {-1,-1,-1, 0,0, 1,1,1};
const int dy[8] = {-1, 0, 1,-1,1,-1,0,1};

bool valid(int x, int y, int R, int C) {
    return x>=0 && y>=0 && x<R && y<C;
}

int countNeighbors(const vector<string>& grid, int r, int c) {
    int cnt=0;
    for(int k=0;k<8;k++){
        int nr=r+dx[k], nc=c+dy[k];
        if(valid(nr,nc,grid.size(),grid[0].size()) && grid[nr][nc]=='@')
            cnt++;
    }
    return cnt;
}

int part1(const vector<string>& grid) {
    int R = grid.size(), C = grid[0].size();
    int acc=0;
    for(int r=0;r<R;r++){
        for(int c=0;c<C;c++){
            if(grid[r][c]=='@' && countNeighbors(grid,r,c)<4)
                acc++;
        }
    }
    return acc;
}

int part2(vector<string> grid) {
    int R = grid.size(), C = grid[0].size();
    int totalRemoved=0;

    while(true){
        vector<pair<int,int>> removable;

        for(int r=0;r<R;r++){
            for(int c=0;c<C;c++){
                if(grid[r][c]=='@' && countNeighbors(grid,r,c)<4)
                    removable.push_back({r,c});
            }
        }

        if(removable.empty())
            break;

        totalRemoved += removable.size();
        for(auto &p : removable)
            grid[p.first][p.second]='.';
    }

    return totalRemoved;
}



int main() {
    vector<string> grid;
    string line;

    ifstream fin("puzzle.txt");
    if (!fin) {
        cerr << "Error: No se pudo abrir puzzle.txt\n";
        return 1;
    }

    while (getline(fin, line)) {
        if (!line.empty())
            grid.push_back(line);
    }
    fin.close();

    cout << "Part 1: " << part1(grid) << endl;
    cout << "Part 2: " << part2(grid) << endl;

    return 0;
}
