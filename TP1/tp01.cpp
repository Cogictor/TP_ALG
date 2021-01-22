#include <iostream>
#include <string>
#include <list>
#include <vector>


using std::cin;
using std::cout;
using std::endl;
using std::string;

std::vector <std::vector <std::list <int>>> BFS( int raizes, int outros, std::vector <std::list <int>> Grafo,int nlayers){
    std::vector <std::vector <std::list <int>>> Layers; // vetor de árvores de caminhos
    int u,v,dist;
    for(int i=0;i<raizes;i++){
        int layer=0;
        std::vector <std::list <int>> L; // árvore de caminhos, possui as camadas de distância para a raiz
        std::list <int> l; // camada com os nós de mesma distância da raiz
        int *visitado = new int[raizes+outros]; // vetor que indica se um nó já foi visitado
        int *distancia = new int[raizes+outros];
        std::list <int> Fila;
        for(int m=0;m<raizes+outros;m++){
            visitado[m] = 0;
        }
        visitado[i] = 1;
        distancia[i] = 0;
        l.push_back(i);
        L.push_back(l);
        Fila.push_back(i);
        l.clear();
        dist = 1;
        while(!Fila.empty()){
            u = Fila.front();
            Fila.pop_front();
            for (std::list<int>::iterator it=Grafo[u].begin(); it != Grafo[u].end(); ++it){
                if(*it!=0){
                    v = *it+ raizes -1; // transforma v da ordenação recebida de entrada para sua posição respectiva no vetor visitado
                    if(visitado[v]==0){
                        visitado[v] = 1;
                        distancia[v] = distancia[u]+1;
                        if(distancia[v]> dist){ //passou para a proxima layer
                            dist+=1;
                            layer+=1;
                            if(layer==nlayers)
                                break;
                            L.push_back(l);
                            l.clear();
                        }
                        l.push_back(v- raizes +1); // adiciona na camada atual
                        Fila.push_back(v);
                    }
                }
            }
            if(layer==nlayers)
                break;
        }
        if(nlayers!=0)
            L.push_back(l);
        delete [] visitado;
        delete [] distancia;
        Layers.push_back(L);
    }
    return Layers;
}
void DFS_recursivo( int raiz, int raizes, std::vector <std::list <int>> Grafo,int *visitado, int *ciclos){
    visitado[raiz] = 1; // visitado
    int v;
    for (std::list<int>::iterator it=Grafo[raiz].begin(); it != Grafo[raiz].end(); ++it){
        if(*it!=0 && *ciclos==0){
            v = *it - 1 + raizes; // coloca referenciando a sua posição em visitado
            if(visitado[v]==1){
                *ciclos+=1;
            }else if(visitado[v]==0){
                DFS_recursivo( v, raizes, Grafo, visitado, ciclos);
            }
        }
    }
    visitado[raiz] = 2; // finalizado
}

int DFS(int raizes, int outros, std::vector <std::list <int>> Grafo){
    int ciclos=0;
    for(int i=0;i<raizes;i++){
        int *visitado = new int[raizes+outros];
        for(int m=0;m<raizes+outros;m++){
        visitado[m] = 0;
        }
        DFS_recursivo( i, raizes, Grafo, visitado, &ciclos);
        delete [] visitado;
        if(ciclos==1)
            return 1;
    }
    return 0;
}

int main(){
    int qtdCD, qtdPV, X,sz;
    cin >> qtdCD >> qtdPV >> X;

    string line, word;
    char l, j=' ';
    cin.ignore();

    // Colocar valores de entrada em uma lista de adjacencia
    std::vector <std::list <int>> LAdj;
    for(int k=0;k<qtdCD+qtdPV;k++){
        std::list <int> l1;
        word="";
        getline(cin,line);
        sz = line.size();
        for(int i=0;i<sz;i++){
            l = line[i];
            if(l!=j){
                word = word+ line[i];
            }
            else{
                //add numero
                l1.push_back(stoi(word));
                word = "";
            }
        }
        l1.push_back(stoi(word));
        LAdj.push_back(l1);
    }

    // Calcular quantas layers serão necessárias
    // layers = (Tmáx-Tinicial)/X  -->Tmáx=-60, Tinicial=-90
    int nlayers,ciclos,n;
    if (X>0){
        nlayers = 30/X;
    }else {
        nlayers = qtdPV;
    }
    // Vetor que armazena as Layers para cada CD, cada Layer corresponde a um vetor de listas, sendo os componentes das listas os nós de cada camada
    std::vector <std::vector <std::list <int>>> Layers;
    Layers = BFS( qtdCD, qtdPV, LAdj,nlayers);
    // Ciclos
    ciclos = DFS( qtdCD, qtdPV, LAdj);

    // Lista de postos nas nlayers
    std::list <int> postos; 

    for(int q=0;q<qtdCD;q++){ // para cada CD
        sz = Layers[q].size();
        for(int k=1;k<sz;k++){ // layers 1:nlayers
            postos.splice(postos.begin(),Layers[q][k]);
        }
    }
    postos.sort(); // ordena
    postos.unique(); // retira elementos repetidos
    sz = postos.size();
    cout<<sz<<endl;
    if(sz==0){
        cout <<"*"<<endl;
    }else{
        for(int q=0;q<sz-1;q++){
            n = postos.front();
            postos.pop_front();
            cout << n <<' ';
        }
        n = postos.front();
        postos.pop_front();
        cout << n << endl;
    }
    cout << ciclos;

    return 0;
}