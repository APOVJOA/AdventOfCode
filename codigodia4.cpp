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
    ListLinked() : first(nullptr), n(0) {}
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

    void append(T e) override { insert(n, e); }
    void prepend(T e) override { insert(0, e); }

    T remove(int pos) override {
        if (pos < 0 || pos >= n)
            throw out_of_range("Posición fuera de rango");

        T eliminado;
        if (pos == 0) {
            Node<T>* borrar = first;
            eliminado = borrar->data;
            first = first->next;
            delete borrar;
        }
        else {
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
};

class Graph {
private:
    int nVertices;
    ListLinked<int>* adj;

public:
    Graph(int n) : nVertices(n) {
        adj = new ListLinked<int>[n];
    }

    ~Graph() {
        delete[] adj;
    }

    void addEdge(int u, int v) {
        adj[u].append(v);
    }

    ListLinked<int>& neighbors(int v) {
        return adj[v];
    }

    int size() const { return nVertices; }
};


Graph* buildGraph(const vector<string>& grid, vector<pair<int,int>>& posMap) {
    int R = grid.size(), C = grid[0].size();

    // Asignar ID a cada '@'
    posMap.clear();
    for(int r = 0; r < R; r++)
        for(int c = 0; c < C; c++)
            if(grid[r][c] == '@')
                posMap.push_back({r, c});

    Graph* G = new Graph(posMap.size());


    vector<vector<int>> id(R, vector<int>(C, -1));
    for(int i = 0; i < (int)posMap.size(); i++)
        id[posMap[i].first][posMap[i].second] = i;

    // 8 direcciones
    int dx[8]={-1,-1,-1,0,0,1,1,1};
    int dy[8]={-1,0,1,-1,1,-1,0,1};

    // Construir adyacencias
    for(int i = 0; i < (int)posMap.size(); i++) {
        int r = posMap[i].first;
        int c = posMap[i].second;

        for(int k=0;k<8;k++){
            int nr = r + dx[k], nc = c + dy[k];
            if(nr>=0 && nc>=0 && nr<R && nc<C && grid[nr][nc]=='@') {
                int j = id[nr][nc];
                G->addEdge(i, j);
            }
        }
    }

    return G;
}

/* PART 1 USANDO GRAPH */
int part1(Graph* G) {
    int acc = 0;

    for(int v = 0; v < G->size(); v++) {
        if(G->neighbors(v).size() < 4)
            acc++;
    }
    return acc;
}

/*PART 2 USANDO GRAPH + BFS */
int part2(Graph* G) {
    int N = G->size();
    vector<bool> removed(N, false);
    vector<int> deg(N);

    // grados iniciales
    for(int i=0;i<N;i++)
        deg[i] = G->neighbors(i).size();

    ListLinked<int> queue;

    // inicializar cola
    for(int i=0;i<N;i++)
        if(deg[i] < 4)
            queue.append(i);

    int total = 0;

    while(!queue.empty()) {
        int v = queue.get(0);
        queue.remove(0);

        if(removed[v]) continue;
        removed[v] = true;
        total++;

        // actualizar vecinos
        ListLinked<int>& neigh = G->neighbors(v);
        for(int i=0;i<neigh.size();i++){
            int u = neigh.get(i);
            if(!removed[u]) {
                deg[u]--;
                if(deg[u] < 4)
                    queue.append(u);
            }
        }
    }

    return total;
}

int main() {
    vector<string> grid;
    string line;

    ifstream fin("rompecabezas4.txt");
    if(!fin){
        cerr << "No se pudo abrir puzzle.txt\n";
        return 1;
    }

    while(getline(fin, line))
        if(!line.empty())
            grid.push_back(line);

    fin.close();

    vector<pair<int,int>> posMap;
    Graph* G = buildGraph(grid, posMap);

    cout << "Part 1: " << part1(G) << endl;
    cout << "Part 2: " << part2(G) << endl;

    delete G;
    return 0;
}


