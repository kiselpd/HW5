#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <vector>

using namespace std;


const double INF = numeric_limits<double>::infinity();


struct EDGES
{
    int first_top;
    int second_top;
    double edge;
};



class Graph
{
private:
    int sum_edge;
    vector <EDGES> e;
    int sum_top;
public:
    Graph()
    {
        cout << "Enter a weighted oriented graph in the format \"0->1 23\" " << endl;
        string str;
        int minus = 1, index = 0, check_pow = 0, pow = 10, sum_top = 0;;
        double number = 0;
        e.resize(index+1);
        getline(cin, str);
        while (!str.empty())
        {
            for (int i = 0; i < str.length(); i++)
            {
                if ((str[i] >= '0') && (str[i] <= '9'))
                {
                    if (!check_pow)
                    {
                        number = number * 10 + (int)(str[i] - '0');
                    }
                    else
                    {
                        number = number + (double)(str[i] - '0') / pow;
                        pow *= 10;
                    }
                }
                if ((str[i - 1] == '-') && (str[i] != '>'))
                {
                    minus = -1;
                }
                    e[index].first_top = number * minus;
                    number = 0;
                    minus = 1;
                    check_pow = 0;
                }
                if (str[i] == ' ')
                {
                    e[index].second_top = number * minus;
                    number = 0;
                    minus = 1;
                    check_pow = 0;
                }
                if (str[i] == '.')
                {
                    check_pow = 1;
                }

            }
            e[index].edge = number * minus;
            minus = 1;
            number = 0;
            index++;
            check_pow = 0;
            pow = 10;
            e.resize(index + 1);
            getline(cin, str);
        }

        sum_edge = index;
    }

    ~Graph()
    {
        e.clear();
    }

    void PrintGraph()
    {
        for (int i = 0; i < sum_edge; i++)
        {
            cout << "Weight:" << e[i].edge << " First_top:" << e[i].first_top << " Second_top:" << e[i].second_top << endl;
        }
        cout << sum_edge << endl << sum_top << endl;
    }

    /*int SumTop(double number)
    {
        for (int i = 0; i < sum_edge; i++)
        {
            if ((number == e[i].first_top) || (number == e[i].second_top))
            {
                return 0;
            }
        }
        return 1;
    }*/

    void GraphDotTopEdge()
    {
        ofstream outf("graph.dot");
        if (!outf)
        {
            cout << "Uh oh, Test.txt could not be opened for writing!" << endl;
            exit(1);
        }

        outf << "digraph new{" << endl;
        for (int i = 0; i < sum_edge; i++)
        {
            outf << e[i].first_top << "->" << e[i].second_top << "[label = " << e[i].edge << "]" << endl;
        }
        outf << "}" << endl;
        system("dot graph.dot -Tpng -o graph.png");
    }


    /*void FordBellman(int start_top)
    {
        double dis[sum_top];
     
        for (int i = 0; i < sum_top; i++)
        {
            dis[i] = INF;
        }

        dis[start_top] = 0;

        for (int i = 0; i < sum_top - 1; i++) {
            for (int j = 0; j < sum_edge; j++) {
                if ((dis[e[j].first_top] + e[j].edge < dis[e[j].second_top]))
                {
                    dis[e[j].second_top] = dis[e[j].first_top] + e[j].edge;
                    //cout << e[j].second_top << " ";
                }
                if (dis[e[j].second_top] + e[j].edge < dis[e[j].first_top])
                {
                    dis[e[j].first_top] = dis[e[j].second_top] + e[j].edge;
                    //cout << e[j].second_top << " ";
                }
            }
            cout << endl;
        }

        for (int i = 0; i < sum_edge; i++) {

            int x = e[i].first_top;
            int y = e[i].second_top;
            int weight = e[i].edge;

            if ((dis[x] != INF) && ((dis[x] + weight) < dis[y]))
            {
                cout << "Graph contains negative weight cycle" << endl;
            }
        }

        cout << "Vertex Distance from Source" << endl;

        for (int i = 0; i < sum_top; i++)
        {
            cout << i << "\t\t" << dis[i] << endl;
        }
    
    
    }*/
};

int main()
{
    /*Matrix M;

    if (M.ErrorMatrix())
    {
        cout << "Try again!" << endl;
        return 0;
    }
    else
    {
        M.PrintMatrix();
    }
    */
    Graph G;
    G.PrintGraph();
    //G.FordBellman(1);
    G.GraphDotTopEdge();
    return 0;
}
