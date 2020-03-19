#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
ifstream f("date.in");

struct automat
{
    int n, * q, m, q0, nf, nt, nc;
    char lambda = 'l';
    bool* fi;
    int M = 0;//cea mai mare stare
    string l; //alfabetul de intrare
    string o; //alfabetul de iesire
    map <pair<int, char>, vector<pair<int, char> >> delta; //tranzitiile
};

vector<string> traduceri;
string currentTranslation;

void citire(automat &A)
{
    f >> A.n;
    A.q = new int[A.n + 1];
    int i;
    for (i = 0; i < A.n; i++) ///starile
    {
        f >> A.q[i];
        if (A.q[i] > A.M)
            A.M = A.q[i]; ///starea de val maxima

    }

    f >> A.m;
    f >> A.l;///alfabetul
    A.l.push_back(A.lambda);
    A.m++;
    f >> A.o;
    f >> A.q0; /// starea initiala
    f >> A.nf; ///nr de stari finale
    A.fi = new bool[A.M + 1];
    for (int i = 0; i < A.M; i++) {
        A.fi[i] = 0;
    }
    for (i = 0; i < A.nf; i++)
    {
        int x;
        f >> x;
        A.fi[x] = 1;    //i este stare finala daca fi[i]=1
    }
    A.nf = A.M;
    f >> A.nt; ///nr de tranzitii
    for (i = 0; i < A.nt; i++) ///tranzitiile
    {
        int x, r;
        char a;
        char b;
        f >> x >> a >> r >> b;
        A.delta[{x, a}].push_back({r, b});

    }
}

void afisare(automat A)
{
    cout << A.n << "\n";
    cout << A.l << "\n";
    cout << A.o << "\n";
    cout << A.M << "\n";
    for (int i = 0; i < A.n; i++)
        for (int j = 0; j < A.m; j++)
            for(int k = 0; k < A.delta[{A.q[i], A.l[j]}].size(); k++)
            {
                cout << A.q[i] << " " <<  A.l[j] << " " << A.delta[{A.q[i], A.l[j]}][k].first << " " << A.delta[{A.q[i], A.l[j]}][k].second << "\n";
            }
    for (int i = 1; i <= A.M; i++) {
        cout << A.fi[i] << " ";
    }
    cout << "\n\n";
}

void gasesteTraducere(automat A, string input, int start, int stareCurenta) {
    if ((start >= input.size() + 1) && A.fi[stareCurenta] == 1)
    {
        traduceri.push_back(currentTranslation);
        return;
    }
    if (currentTranslation.size() == 10 || traduceri.size() == 10)
    {
        return;
    }
        
    vector<pair<int, char>> temp = A.delta[{stareCurenta, input[start - 1]}];

    for(int i = 0; i < temp.size(); i++)
    {
        currentTranslation.push_back(temp[i].second);
        gasesteTraducere(A, input, start + 1, temp[i].first);
        if (!currentTranslation.empty())
        {
            currentTranslation.pop_back();
        }
    }

    temp = A.delta[{stareCurenta, A.lambda}];

    for (int i = 0; i < temp.size(); i++)
    {
        currentTranslation.push_back(temp[i].second);
        if (temp[i].first != 0)
        {
            gasesteTraducere(A, input, start, temp[i].first);
            if (!currentTranslation.empty())
            {
                currentTranslation.pop_back();
            }
        }
    }
}

int main()
{
    automat A;
    citire(A);
    afisare(A);

    string cuvant;
    while (f >> cuvant)
    {
        cout << "Cuvant " << cuvant << "\n" << "Traduceri: " << "\n";
        
        gasesteTraducere(A, cuvant, 1, A.q0);

        for (int i = 0; i < traduceri.size(); i++)
        {
            cout << traduceri[i] << "\n";
        }
        cout << "\n";
        traduceri.erase(traduceri.begin(), traduceri.end());
    }
}
