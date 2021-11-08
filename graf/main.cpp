#include<bits/stdc++.h>

using namespace std;

class GRAF
{
private:
    int n, m, s;
    vector<vector<int>> v, tr;

    void dfs(int poz, bool viz[]);
    void pcrit(int poz, int desc[], int low[], int tata[], list<pair<int,int>>& muc, vector<vector<int>>& ccon, int& nrconex);
    void dfstc(int &poz, bool viz[], vector<vector<int>> &ctc, int &nrcomp);
    void fillst(int &poz, bool viz[], stack<int> &st);
    GRAF trans();
    void dfspc(int &poz, bool viz[], int desc[], int low[], int tata[]);

public:
    GRAF(int n);
    GRAF(bool orientat);
    void bfs();
    void conex();
    void bc();
    void tc();
    void stp();
    void pc();
};

GRAF::GRAF(int n)
{
    this->n = n;
    v.resize(n+1);
}

GRAF::GRAF(bool orientat)
{
    //freopen("bfs.in", "r", stdin);
    //freopen("biconex.in", "r", stdin);
    //freopen("dfs.in", "r", stdin);
    //freopen("ctc.in", "r", stdin);
    //freopen("sortaret.in", "r", stdin);
    freopen("punctecrit.in", "r", stdin);

    //scanf("%d %d %d", &n, &m, &s);
    scanf("%d %d", &n, &m);

    int x, y;
    v.resize(n+1);
    tr.resize(n+1);
    for(int i=0; i<m; ++i)
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

GRAF GRAF::trans()
{
    GRAF gt(n);
    for(int i=1; i<=n; ++i)
        for(auto j : v[i])
            gt.v[j].push_back(i);
    return gt;
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

int main()
{
    GRAF g(true);
    g.pc();
    return 0;
}
