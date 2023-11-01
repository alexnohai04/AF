// TemaAF.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <queue>
#include <vector>
#include <unordered_map>

using namespace std;

class Solution {
public:
    bool possibleBipartition(int n, vector<vector<int>>& dislikes) {

        if (dislikes.empty())
            return true;

        vector<int> colors(n + 1, -1);
        queue<int> q;
        q.push(dislikes[0][0]);
        //  Primul nod primeste culoarea 0
        colors[dislikes[0][0]] = 0;

        while (!q.empty()) {

            int nod = q.front();
            q.pop();
            // Extragem primul element din coada
            for (const auto& dislike : dislikes) {
                int nod1 = dislike[0];
                int nod2 = dislike[1];
                // Verificam daca unul din cele doua noduri este nodul curent
                if (nod1 == nod || nod2 == nod) {
                    int vec;
                    // Identificam celalalt nod din pereche
                    if (nod1 == nod) {
                        vec = nod2;
                    }
                    else if (nod2 == nod) {
                        vec = nod1;
                    }
                    // Verificam daca celalalt nod nu a fost colorat inca
                    if (colors[vec] == -1) {
                        // Atribuim culoarea opusa nodului curent
                        colors[vec] = 1 - colors[nod];
                        // Adaugam nodul in coada pentru a fi verificat
                        q.push(vec);
                    }
                    else if (colors[vec] == colors[nod]) {
                        // Daca cele doua noduri au aceeasi culoare, atunci returnam false
                        return false;
                    }
                }
            }
        }


        return true;
    }

    ///
  
    vector<int> findOrder(int numCourses, vector<vector<int>>& prerequisites) {
        // Lista de adiacenta
        vector<vector<int>> adj(numCourses, vector<int>());
        // Gradurile interioare 
        vector<int> gradInt(numCourses, 0);

        // Construim graful
        for (auto i : prerequisites) {
            adj[i[1]].push_back(i[0]);
            gradInt[i[0]]++;
        }

        // Coada in care bagam cursurile care pot fi facute direct
        queue<int> q;
        for (int i = 0; i < numCourses; i++) {
            if (gradInt[i] == 0) {
                q.push(i);
            }
        }

        vector<int> rez;
        // Parcurgem coada pentru a gasi ordinea cursurilor
        while (!q.empty()) {
            // Extragem cursul curent din coada
            int curent = q.front();
            q.pop();
            // Adaugam cursul la lista de cursuri 
            rez.push_back(curent);

            // Actualizam gradul de intrare pentru fiecare curs care depinde de cursul curent
            for (int next : adj[curent]) {
                gradInt[next]--;
                // Daca cursul curent nu mai are alte cursuri care sa depinda de el il adaugam in coada
                if (gradInt[next] == 0) {
                    q.push(next);
                }
            }
        }

        // Verificam daca s-au adaugat toate cursurile in lista rezultat, daca nu returnam un vector gol
        if (rez.size() != numCourses) {
            return {};
        }
        else {
            return rez;
        }
    }

    ///
    vector<vector<int>> criticalConnections(int n, vector<vector<int>>& connections) {
        vector<vector<int>> adj(n, vector<int>());
        // Construim lista de adiacenta
        for (auto& i : connections) {
            int from = i[0];
            int to = i[1];
            adj[from].push_back(to);
            adj[to].push_back(from);
        } 

        vector<int> descoperit(n, -1);
        vector<int> timpMin(n, -1);
        vector<vector<int>> critice;

        int timp = 0;
        // Pornim parcurgerea DFS de la primul nod
        dfs(0, -1, timp, adj, descoperit, timpMin, critice);

        return critice;
    }

    void dfs(int nodCurent, int parinte, int& timp, vector<vector<int>>& adj, vector<int>& descoperit, vector<int>& timpMin, vector<vector<int>>& critice) {
        // Daca nodul curent a fost deja vizitat, iesim 
        if (descoperit[nodCurent] != -1) {
            return;
        }

        // Setam timpul de descoperire si timpul minim pentru nodul curent
        descoperit[nodCurent] = timp;
        timpMin[nodCurent] = timp;
        timp++;

        // Parcurgem vecinii nodului curent
        for (int vec : adj[nodCurent]) {
            // Daca vecinul nu a fost descoperit continuam parcurgerea
            if (descoperit[vec] == -1) {
                dfs(vec, nodCurent, timp, adj, descoperit, timpMin, critice);
                // Actualizam timpul minim pentru nodul curent bazat pe timpul minim al vecinilor sai
                timpMin[nodCurent] = min(timpMin[nodCurent], timpMin[vec]);
                // Verificam daca exista o conexiune critica intre nodul curent si vecin
                if (timpMin[vec] > descoperit[nodCurent]) {
                    // Adaugam conexiunea critica in rezultat
                    critice.push_back({ nodCurent, vec });
                }
            }
            // Daca vecinul nu este parintele nodului curent, actualizam timpul minim
            else if (vec != parinte) {
                timpMin[nodCurent] = min(timpMin[nodCurent], descoperit[vec]);
            }
        }
    }



    ///
    vector<int> eventualSafeNodes(vector<vector<int>>& graph) {
        int n = graph.size();
        vector<int> gradOut(n, 0); // gradurile de iesire
        unordered_map<int, vector<int>> adj; 
        // Calculam gradul de iesire pt fiecare nod si construim adj
        for (int i = 0; i < n; i++)
        {
            for (auto it : graph[i])
            {
                adj[it].push_back(i);
            }
            gradOut[i] = graph[i].size();
        }
        vector<int> sigure;
        vector<int> vizit(n, 0);
        // Cautam in mod repetat noduri sigure 
        while (true)
        {
            bool ok = 0;
            for (int i = 0; i < n; i++)
            {
                // Verificam daca nodul nu a fost vizitat si daca are gradul de iesire 0
                if (!vizit[i] && gradOut[i] == 0)
                {
                    // Adaugam nodul la lista de noduri sigure
                    sigure.push_back(i);
                    // Marcam nodul ca vizitat
                    vizit[i] = 1;
                    ok = 1;
                    // Scadem gradul de iesire pentru fiecare nod vecin al nodului curent
                    for (auto it : adj[i])
                        gradOut[it]--;
                }
            }
            if (!ok)
                break;
        }
        sort(sigure.begin(), sigure.end());
        return sigure;
    }

    ///
    bool equationsPossible(vector<string>& equations) {
        vector<vector<int>> listaAd(26); 
        vector<int> color(26, -1); 
        // Construim lista de adiacenta 
        for (string& ec : equations) {
            if (ec[1] == '=') { // Verificam daca ecuatia este de tipul "==" pentru a adauga nodurile in graf
                int x = ec[0] - 'a'; 
                int y = ec[3] - 'a';
                listaAd[x].push_back(y); 
                listaAd[y].push_back(x);
            }
        }
        // Pt fiecare nod facem DFS pentru a atribui culorile 
        for (int i = 0; i < 26; i++) {
            dfs(i, i, color, listaAd);
        }
        // Verificam daca ecuatiile "!=" respecta regulile de colorare
        for (string& eqn : equations) {
            if (eqn[1] == '!') {
                int x = eqn[0] - 'a';
                int y = eqn[3] - 'a';
                if (color[x] == color[y]) { 
                    return false;
                }
            }
        }
        return true;
    }
    void dfs(int node, int clr, vector<int>& color, vector<vector<int>>& listaAd) {
        if (color[node] == -1) { // Daca culoarea nodului nu este inca atribuita, o atribuim
            color[node] = clr;
            for (int neighbor : listaAd[node]) { // Parcurgem vecinii nodului curent pentru a le atribui aceeasi culoare
                dfs(neighbor, clr, color, listaAd);
            }
        }
    }


    ///


};


int main()
{
    std::cout << "Hello World!\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
