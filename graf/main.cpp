#include<bits/stdc++.h>

using namespace std;

class GRAF
{
private:
    int n, m, s;
    vector<vector<int>> v, tr;
    vector<vector<pair<int,int>>> c;

    void dfs(int poz, bool viz[]);
    void pcrit(int poz, int desc[], int low[], int tata[], list<pair<int,int>>& muc, vector<vector<int>>& ccon, int& nrconex);
    void dfstc(int &poz, bool viz[], vector<vector<int>> &ctc, int &nrcomp);
    void fillst(int &poz, bool viz[], stack<int> &st);
    void dfspc(int &poz, bool viz[], int desc[], int low[], int tata[]);
    pair<int, int> parcdarb(int poz);

public:
    GRAF();
    GRAF(bool orientat, bool cost, bool arbore);
    void bfs();
    void conex();
    void bc();
    void tc();
    void stp();
    void pc();
    void apm();
    void djk();
    int darb();
    int fm();
};

GRAF::GRAF(){}

GRAF::GRAF(bool orientat, bool cost, bool arbore)
{
    //freopen("bfs.in", "r", stdin);
    //freopen("biconex.in", "r", stdin);
    //freopen("dfs.in", "r", stdin);
    //freopen("ctc.in", "r", stdin);
    //freopen("sortaret.in", "r", stdin);
    //freopen("punctecrit.in", "r", stdin);

    if(arbore == false)
        scanf("%d %d", &n, &m);
    else
    {
        scanf("%d", &n);
        m = n-1;
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

void GRAF::bfs()
{
    int d[n+1];
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
    freopen("bfs.out", "w", stdout);
    for(int i=1; i<=n; ++i)
        printf("%d ", d[i]);
    fclose(stdout);
}

void GRAF::dfs(int poz, bool viz[])
{
    viz[poz] = true;
    for(auto i : v[poz])
    {
        if(!viz[i])
            dfs(i, viz);
    }
}

void GRAF::conex()
{
    bool viz[n+1] = {false};
    int nrconex = 0;

    for(int i=1; i<=n; ++i)
    {
        if(!viz[i])
        {
            dfs(i, viz);
            nrconex++;
        }
    }
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

void GRAF::bc()
{
    int *desc = new int[n+1], *low = new int[n+1], *tata = new int[n+1];
    list<pair<int,int>>muc;
    vector<vector<int>> ccon(n);
    int nrconex = 0;
    fill_n(desc, n+1, -1);
    fill_n(low, n+1, -1);
    fill_n(tata, n+1, -1);

    for(int i=1; i<=n; ++i)
    {
        if(desc[i] == -1)
            pcrit(i, desc, low, tata, muc, ccon, nrconex);
    }

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

void GRAF::dfstc(int &poz, bool viz[], vector<vector<int>> &ctc, int &nrcomp)
{
    ctc[nrcomp].push_back(poz);
    viz[poz] = true;
    //cout<<poz<<" ";
    for(auto i : tr[poz])
        if(viz[i] == false)
            dfstc(i, viz, ctc, nrcomp);
}

void GRAF::fillst(int &poz, bool viz[], stack<int> &st)
{
    viz[poz] = true;
    for(auto i : v[poz])
        if(viz[i] == false)
            fillst(i, viz, st);


    st.push(poz);
}

void GRAF::tc()
{
    int nrcomp = 0;
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
            dfstc(st.top(), viz, ctc, nrcomp);
            //cout<<"\n";
        }
        st.pop();
    }
    freopen("ctc.out", "w", stdout);
    printf("%d\n", nrcomp);
    for(int i=1; i<=nrcomp; ++i)
    {
        for(auto j : ctc[i])
            printf("%d ", j);
        printf("\n");
    }
    fclose(stdout);
}

void GRAF::stp()
{
    stack<int> st;
    bool viz[n+1] = {false};
    for(int i=1; i<=n; ++i)
        if(!viz[i])
            fillst(i, viz, st);

    freopen("sortaret.out", "w", stdout);
    while(st.empty() == false)
    {
        printf("%d ", st.top());
        st.pop();
    }
    fclose(stdout);
}

void GRAF::dfspc(int &poz, bool viz[], int desc[], int low[], int tata[])
{
    static int time = 0;
    viz[poz] = true;
    desc[poz] = low[poz] = ++time;

    for(auto i : v[poz])
    {
        if(viz[i] == false)
        {
            tata[i] = poz;
            dfspc(i, viz, desc, low, tata);
            low[poz] = min(low[poz], low[i]);
            if(low[i] > desc[poz])
                printf("%d %d\n", poz, i);
        }
        else if(i != tata[poz])
            low[poz] = min(low[poz], desc[i]);
    }
}

void GRAF::pc()
{
    int desc[n+1] = {0};
    int low[n+1];
    bool viz[n+1] = {false};
    int tata[n+1];
    fill_n(tata, n+1, -1);

    freopen("punctecrit.out", "w", stdout);

    for(int i=0; i<n; ++i)
        if(viz[i] == false)
            dfspc(i, viz, desc, low, tata);

    fclose(stdout);
}

void hakim()
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

    while(1)
    {
        sort(sir.begin(), sir.end(), greater<>());
        if(sir[0] == 0)
        {
            cout<<"Yes";
            return;
        }

        int first = sir[0];
        sir.erase(sir.begin()+0);

        if(first > sir.size())
        {
            cout<<"No";
            return;
        }

        for(int i=0; i<first; ++i)
        {
            sir[i]--;
            if(sir[i] < 0)
            {
              cout<<"No";
              return;
            }
        }
    }
}

void GRAF::apm()
{
    freopen("apm.in", "r", stdin);

    scanf("%d %d", &n, &m);

    int x, y, z;
    c.resize(n+1);
    for(int i=0; i<m; ++i)
    {
        scanf("%d %d %d", &x, &y, &z);
        c[x].push_back(make_pair(y, z));
        c[y].push_back(make_pair(x, z));
    }
    fclose(stdin);

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    vector<int> k(n+1, INT_MAX);
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

    int ctot = 0;
    for(int i=1; i<=n; ++i)
        ctot += k[i];
    freopen("apm.out", "w", stdout);
    printf("%d %d\n", ctot, n-1);
    for(int i=2; i<=n; ++i)
        printf("%d %d\n", tata[i], i);
    fclose(stdout);
}

void GRAF::djk()
{
    freopen("dijkstra.in", "r", stdin);

    scanf("%d %d", &n, &m);

    int x, y, z;
    c.resize(n+1);
    for(int i=0; i<m; ++i)
    {
        scanf("%d %d %d", &x, &y, &z);
        c[x].push_back(make_pair(y, z));
    }
    fclose(stdin);

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
    GRAF g(false, false, true);
    freopen("darb.out", "w", stdout);
    printf("%d", g.darb());
    fclose(stdout);
}

int GRAF::fm()
{

}

void fm_infoarena()
{
    freopen("maxflow.in", "r", stdin);
    GRAF g(true, true, false);
    g.fm();

}

int main()
{
    ios_base::sync_with_stdio(false);
    darb_infoarena();
    return 0;
}
