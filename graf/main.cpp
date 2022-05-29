#include<bits/stdc++.h>

using namespace std;

class GRAF
{
private:
    int n, m, s;
    vector<vector<int>> v, tr;
    vector<vector<pair<int,int>>> c;

    void dfs_conex(int poz, bool viz[]);
    void pcrit(int poz, int desc[], int low[], int tata[], list<pair<int,int>>& muc, vector<vector<int>>& ccon, int& nrconex);
    void dfs_ctc(int &poz, bool viz[], vector<vector<int>> &ctc, int &nrcomp);
    void fillst(int &poz, bool viz[], stack<int> &st);
    void dfs_pc(int &poz, bool viz[], int desc[], int low[], int tata[], vector<vector<int>>& sol_pc);
    pair<int, int> parcdarb(int poz);

public:
    GRAF();
    GRAF(bool orientat, bool cost, bool arbore, bool start);
    int getN();
    void bfs(int* d);
    int conex();
    vector<vector<int>> bc(int &nrconex);
    vector<vector<int>> ctc(int &nrcomp);
    stack<int> stp();
    vector<vector<int>> pc();
    vector<int> apm(int &cst, int &nrmuc);
    vector<int> djk();
    int darb();
    int fm();
    vector<int> bellmanford();
};

GRAF::GRAF(){}

GRAF::GRAF(bool orientat, bool cost, bool arbore, bool start)
{
    if(start == true)
    {
        scanf("%d %d %d", &n, &m, &s);
    }
    else
    {
        if(arbore == false)
        scanf("%d %d", &n, &m);
        else
        {
            scanf("%d", &n);
            m = n-1;
        }
    }

    int x, y, z;
    v.resize(n+1);
    tr.resize(n+1);
    c.resize(n+1);
    for(int i=0; i<m; ++i)
    {
        if(cost == true)
        {
            scanf("%d %d %d", &x, &y, &z);
            c[x].push_back(make_pair(y, z));

            if(!orientat)
                c[y].push_back(make_pair(x, z));
        }
        else
        {
            scanf("%d %d", &x, &y);
            v[x].push_back(y);
            tr[y].push_back(x);

            if(!orientat)
            {
                v[y].push_back(x);
                tr[x].push_back(y);
            }
        }
    }
    fclose(stdin);
}

int GRAF::getN()
{
    return this->n;
}

void GRAF::bfs(int* d)
{
    fill_n(d, n+1, -1);
    d[s] = 0;
    queue<int> q;
    q.push(s);
    while(!q.empty())
    {
        int poz = q.front();
        for(auto i : v[poz])
        {
            if(d[i] == -1)
            {
                q.push(i);
                d[i]=d[poz]+1;
            }
        }
        q.pop();
    }
}

void bfs_infoarena()
{
    freopen("bfs.in", "r", stdin);
    GRAF g(true, false, false, true);
    int n = g.getN();
    int rezultat_bfs[n];
    g.bfs(rezultat_bfs);
    freopen("bfs.out", "w", stdout);
    for(int i=1; i<=n; ++i)
        printf("%d ", rezultat_bfs[i]);
    fclose(stdout);
}

void GRAF::dfs_conex(int poz, bool viz[])
{
    viz[poz] = true;
    for(auto i : v[poz])
    {
        if(!viz[i])
            dfs_conex(i, viz);
    }
}

int GRAF::conex()
{
    bool viz[n+1] = {false};
    int nrconex = 0;

    for(int i=1; i<=n; ++i)
    {
        if(!viz[i])
        {
            dfs_conex(i, viz);
            nrconex++;
        }
    }
    return nrconex;
}

void dfs_infoarena()
{
    freopen("dfs.in", "r", stdin);
    GRAF g(false, false, false, false);
    int nrconex;
    nrconex = g.conex();
    return;
    freopen("dfs.out", "w", stdout);
    printf("%d", nrconex);
    fclose(stdout);
}

void GRAF::pcrit(int poz, int desc[], int low[], int tata[], list<pair<int,int>>& muc, vector<vector<int>>& ccon, int& nrconex)
{
    static int time = 0;
    int cop = 0;
    desc[poz] = low[poz] = ++time;
    for(auto i : v[poz])
    {
        if(desc[i] == -1)
        {
            cop++;
            tata[i] = poz;
            muc.push_back(make_pair(poz,i));
            pcrit(i, desc, low, tata, muc, ccon, nrconex);

            low[poz] = min(low[poz], low[i]);
            if((tata[poz] == -1 && cop > 1) || (desc[poz] != -1 && low[i] >= desc[poz]))
            {
                while(muc.back().first != poz || muc.back().second != i)
                {
                    ccon[nrconex].push_back(muc.back().second);
                    muc.pop_back();
                }
                ccon[nrconex].push_back(muc.back().second);
                ccon[nrconex].push_back(muc.back().first);
                muc.pop_back();
                nrconex++;
            }

        }
        else if(i != tata[poz])
            low[poz] = min(low[poz], desc[i]);
    }
}

vector<vector<int>> GRAF::bc(int &nrconex)
{
    int *desc = new int[n+1], *low = new int[n+1], *tata = new int[n+1];
    list<pair<int,int>>muc;
    vector<vector<int>> ccon(n);
    nrconex = 0;
    fill_n(desc, n+1, -1);
    fill_n(low, n+1, -1);
    fill_n(tata, n+1, -1);

    for(int i=1; i<=n; ++i)
    {
        if(desc[i] == -1)
            pcrit(i, desc, low, tata, muc, ccon, nrconex);
    }
    return ccon;
}

void bc_infoarena()
{
    freopen("biconex.in", "r", stdin);
    GRAF g(false, false, false, false);
    int nrconex;
    vector<vector<int>> ccon = g.bc(nrconex);
    freopen("biconex.out", "w", stdout);
    printf("%d\n", nrconex);
    for(int i=0; i<nrconex; ++i)
    {
        for(auto j : ccon[i])
            printf("%d ", j);
        printf("\n");
    }
    fclose(stdout);
}

void GRAF::dfs_ctc(int &poz, bool viz[], vector<vector<int>> &ctc, int &nrcomp)
{
    ctc[nrcomp].push_back(poz);
    viz[poz] = true;
    //cout<<poz<<" ";
    for(auto i : tr[poz])
        if(viz[i] == false)
            dfs_ctc(i, viz, ctc, nrcomp);
}

void GRAF::fillst(int &poz, bool viz[], stack<int> &st)
{
    viz[poz] = true;
    for(auto i : v[poz])
        if(viz[i] == false)
            fillst(i, viz, st);


    st.push(poz);
}

vector<vector<int>> GRAF::ctc(int &nrcomp)
{
    nrcomp = 0;
    vector<vector<int>> ctc(n+1);
    stack<int> st;
    bool viz[n+1] = {false};

    for(int i=1; i<=n; ++i)
        if(!viz[i])
            fillst(i, viz, st);

    fill_n(viz, n+1, false);

    while(st.empty() == false)
    {
        if(viz[st.top()] == false)
        {
            nrcomp++;
            dfs_ctc(st.top(), viz, ctc, nrcomp);
            //cout<<"\n";
        }
        st.pop();
    }
    return ctc;
}

void ctc_infoarena()
{
    freopen("ctc.in", "r", stdin);
    GRAF g(true, false, false, false);
    int nrcomp;
    vector<vector<int>> comp = g.ctc(nrcomp);
    freopen("ctc.out", "w", stdout);
    printf("%d\n", nrcomp);
    for(int i=1; i<=nrcomp; ++i)
    {
        for(auto j : comp[i])
            printf("%d ", j);
        printf("\n");
    }
    fclose(stdout);
}

stack<int> GRAF::stp()
{
    stack<int> st;
    bool viz[n+1] = {false};
    for(int i=1; i<=n; ++i)
        if(!viz[i])
            fillst(i, viz, st);
    return st;
}

void stp_infoarena()
{
    freopen("sortaret.in", "r", stdin);
    GRAF g(true, false, false, false);
    stack<int> st = g.stp();
    freopen("sortaret.out", "w", stdout);
    while(st.empty() == false)
    {
        printf("%d ", st.top());
        st.pop();
    }
    fclose(stdout);
}

void GRAF::dfs_pc(int &poz, bool viz[], int desc[], int low[], int tata[], vector<vector<int>>& sol_pc)
{
    static int time = 0;
    viz[poz] = true;
    desc[poz] = low[poz] = ++time;

    for(auto i : v[poz])
    {
        if(viz[i] == false)
        {
            tata[i] = poz;
            dfs_pc(i, viz, desc, low, tata, sol_pc);
            low[poz] = min(low[poz], low[i]);
            if(low[i] > desc[poz])
            {
                vector<int> con;
                con.push_back(poz);
                con.push_back(i);
                sol_pc.push_back(con);
            }
        }
        else if(i != tata[poz])
            low[poz] = min(low[poz], desc[i]);
    }
}

vector<vector<int>> GRAF::pc()
{
    int desc[n+1] = {0};
    int low[n+1];
    bool viz[n+1] = {false};
    int tata[n+1];
    fill_n(tata, n+1, -1);
    vector<vector<int>> sol_pc;

    for(int i=0; i<n; ++i)
        if(viz[i] == false)
            dfs_pc(i, viz, desc, low, tata, sol_pc);
    return sol_pc;
}

void pc_afisare()
{
    freopen("punctecrit.in", "r", stdin);
    GRAF g(false, false, false, false);

    vector<vector<int>> sol_pc = g.pc();

    freopen("punctecrit.out", "w", stdout);
    for(auto i : sol_pc)
        printf("%d %d\n", i[0], i[1]);
    fclose(stdout);
}

bool havel_hakimi(vector<int> sir)
{
    while(1)
    {
        sort(sir.begin(), sir.end(), greater<>());
        if(sir[0] == 0)
        {
            return true;
        }

        int first = sir[0];
        sir.erase(sir.begin()+0);

        if(first > sir.size())
        {
            return false;
        }

        for(int i=0; i<first; ++i)
        {
            sir[i]--;
            if(sir[i] < 0)
            {
              return false;
            }
        }
    }
}

void havel_hakimi_afisare()
{
    freopen("hakim.in", "r", stdin);
    int nr, x;
    scanf("%d", &nr);
    vector<int> sir(nr+1);
    for(int i=0; i<nr; ++i)
    {
        scanf("%d", &x);
        sir.push_back(x);
    }
    fclose(stdin);

    if(havel_hakimi(sir))
        cout<<"Este graf simplu";
    else
        cout<<"Nu este graf simplu";
}

vector<int> GRAF::apm(int &cst, int &nrmuc)
{
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    vector<int> k(n+1, INT_MAX); //costuri minime
    vector<int> tata(n+1, -1);
    vector<bool> viz(n+1, false);

    pq.push(make_pair(0,1));
    k[1] = 0;

    while(pq.empty() == false)
    {
        int poz = pq.top().second;
        pq.pop();

        if(viz[poz] == true)
        {
            continue;
        }
        viz[poz] = true;

        for(auto i : c[poz])
        {
            if(viz[i.first] == false && k[i.first] > i.second)
            {
                k[i.first] = i.second;
                pq.push(make_pair(k[i.first], i.first));
                tata[i.first] = poz;
            }
        }
    }

    cst = 0, nrmuc = n-1;
    for(int i=1; i<=n; ++i)
        cst += k[i];

    return tata;
}

void apm_infoarena()
{
    freopen("apm.in", "r", stdin);
    GRAF g(false, true, false, false);
    int cst, nrmuc;
    vector<int> tata = g.apm(cst, nrmuc);

    freopen("apm.out", "w", stdout);
    printf("%d\n%d\n", cst, nrmuc);
    for(int i=2; i<=nrmuc+1; ++i)
        printf("%d %d\n", tata[i], i);
    fclose(stdout);
}

vector<int> GRAF::djk()
{
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    vector<int> k(n+1, INT_MAX);
    vector<bool> viz(n+1, false);

    pq.push(make_pair(0,1));
    k[1] = 0;

    while(pq.empty() == false)
    {
        int poz = pq.top().second;
        pq.pop();

        if(viz[poz] == true)
        {
            continue;
        }
        viz[poz] = true;

        for(auto i : c[poz])
        {
            if(k[poz] + i.second < k[i.first])
            {
                k[i.first] = k[poz] + i.second;
                pq.push(make_pair(k[i.first], i.first));
            }
        }
    }
    return k;
}

void djk_infoarena()
{
    freopen("dijkstra.in", "r", stdin);
    GRAF g(true, true, false, false);
    int n = g.getN();
    vector<int> k = g.djk();

    freopen("dijkstra.out", "w", stdout);
    for(int i=2; i<=n; ++i)
    {
        if(k[i] != INT_MAX)
            printf("%d ", k[i]);
        else
            printf("%d ", 0);
    }
    fclose(stdout);
}

void rf(int n, int m[101][101])
{
    for(int i=1; i<=n; ++i)
        for(int j=1; j<=n; ++j)
            for(int k=1; k<=n; ++k)
                if(m[j][i] + m[i][k] < m[j][k])
                    m[j][k] = m[j][i] + m[i][k];
    return;
}

void rf_infoarena()
{
    freopen("royfloyd.in", "r", stdin);
    int n, m[101][101];
    scanf("%d", &n);
    for(int i=1; i<=n; ++i)
        for(int j=1; j<=n; ++j)
        {
            scanf("%d", &m[i][j]);
            if(m[i][j] == 0)
                m[i][j] = 1001;
        }


    fclose(stdin);
    rf(n, m);
    freopen("royfloyd.out", "w", stdout);
    for(int i=1; i<=n; ++i)
    {
         for(int j=1; j<=n; ++j)
         {
             if(i == j)
                printf("0 ");
             else
                printf("%d ", m[i][j]);
         }

        printf("\n");
    }
    fclose(stdout);
}

pair<int, int> GRAF::parcdarb(int poz)
{
    int d[n+1];
    fill_n(d, n+1, -1);
    d[poz] = 1;

    queue<int> q;
    q.push(poz);

    int last;

    while(q.empty() == false)
    {
        poz = q.front();
        q.pop();
        for(auto i : v[poz])
        {
            if(d[i] == -1)
            {
                q.push(i);
                d[i]=d[poz]+1;
                last = i;
            }
        }
    }
    return {last, d[last]};
}

int GRAF::darb()
{
    pair<int, int> rez;
    rez = parcdarb(1);
    rez = parcdarb(rez.first);
    return rez.second;
}

void darb_infoarena()
{
    freopen("darb.in", "r", stdin);
    GRAF g(false, false, true, false);
    freopen("darb.out", "w", stdout);
    printf("%d", g.darb());
    fclose(stdout);
}

vector<int> GRAF::bellmanford()
{
    queue<int> q;
    vector<int> k(n+1, INT_MAX);
    vector<bool> inq(n+1, false);
    vector<int> negativ(n+1, 0);

    q.push(1);
    k[1] = 0;

    while(q.empty() == false)
    {
        int poz = q.front();
        q.pop();
        inq[poz] = false;

        for(auto i : c[poz])
        {
            if(k[poz] + i.second < k[i.first])
            {
                k[i.first] = k[poz] + i.second;
                negativ[i.first]++;

                if(negativ[i.first] == n)
                {
                    vector<int> ciclu_negativ;
                    ciclu_negativ.push_back(2000);
                    return ciclu_negativ;
                }

                if(inq[i.first] == false)
                {
                    q.push(i.first);
                    inq[i.first] = true;
                }
            }
        }
    }
    return k;
}

void bellmanford_infoarena()
{
    freopen("bellmanford.in", "r", stdin);
    GRAF g(true, true, false, false);
    int n = g.getN();
    vector<int> k = g.bellmanford();

    freopen("bellmanford.out", "w", stdout);
    if(k[0] == 2000)
    {
        printf("%s", "Ciclu negativ!");
    }
    else
    {
        for(int i=2; i<=n; ++i)
        {
            if(k[i] != INT_MAX)
                printf("%d ", k[i]);
            else
                printf("%d ", 0);
        }
    }
    fclose(stdout);
}

class Disjoint
{
    private:
        int* tata;
        int* h;
        int n;

        int radacina(int x);

    public:
        Disjoint(int n)
        {
            this->n = n;
            h = new int[n];
            tata = new int[n];
            for(int i=0; i<n; ++i)
            {
                tata[i]=i;
            }
        }

        void reuniune(int x, int y);
        bool frati(int x, int y);
};

int Disjoint::radacina(int x)
{
    if(tata[x] != x)
        tata[x] = radacina(tata[x]);
    return tata[x];
}

void Disjoint::reuniune(int x, int y)
{
    int rad_x = radacina(x);
    int rad_y = radacina(y);

    if(rad_x == rad_y)
        return;

    if(h[rad_x] < h[rad_y])
    {
        tata[rad_x] = rad_y;
    }
    else if(h[rad_x] > h[rad_y])
    {
        tata[rad_y] = rad_x;
    }
    else
    {
        tata[rad_x] = rad_y;
        h[rad_y]++;
    }
}

bool Disjoint::frati(int x, int y)
{
    if(radacina(x) == radacina(y))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void disjoint_infoarena()
{
    freopen("disjoint.in", "r", stdin);
    freopen("disjoint.out", "w", stdout);
    int n, m;
    scanf("%d %d", &n, &m);
    Disjoint d(n);

    for(int i=0; i<m; ++i)
    {
        int cod, x, y;
        scanf("%d %d %d", &cod, &x, &y);
        if(cod == 1)
        {
            d.reuniune(x, y);
        }
        else
        {
            if(d.frati(x, y))
                printf("%s", "DA\n");
            else
                printf("%s", "NU\n");
        }
    }
    fclose(stdin);
    fclose(stdout);
}

int main()
{
    disjoint_infoarena();
    return 0;
}
