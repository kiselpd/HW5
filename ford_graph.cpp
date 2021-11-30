#include <iostream>
#include <string>
#include <fstream>
#include <limits>

using namespace std;


const double INF = numeric_limits<double>::infinity();


struct EDGES
{
    int first_top;
    int second_top;
    double edge;
};


class Matrix
{
private:
    double** matrix;
    int size_matrix;
public:
    Matrix()
    {
        string str;
        getline(cin, str);
        size_matrix = 0;
        for (int i = 0; i < str.length(); i++)
        {
            if (str[i] == ' ')
                size_matrix++;
        }
        size_matrix++;

        matrix = new double* [size_matrix];
        for (int i = 0; i < size_matrix; i++)
        {
            matrix[i] = new double[size_matrix];
        }

        double number = 0;

        int index = 0, check_pow = 0, pow = 10, minus = 1;
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
            else if (str[i] == ' ')
            {
                matrix[0][index++] = number * minus;
                number = 0;
                check_pow = 0;
                pow = 10;
                minus = 1;
            }
            else if (str[i] == '.')
            {
                check_pow = 1;
            }
            else if (str[i] == '-')
            {
                minus *= -1;
            }
        }
        matrix[0][size_matrix - 1] = number * minus;

        for (int i = 1; i < size_matrix; i++)
        {
            for (int j = 0; j < size_matrix; j++)
            {
                cin >> matrix[i][j];
            }
        }
    }

    ~Matrix()
    {
        for (int i = 0; i < size_matrix; i++)
        {
            delete[] matrix[i];
        }

        delete[] matrix;
    }

    double** SetMatrix()
    {
        return matrix;
    }

    int SetSizeMatrix()
    {
        return size_matrix;
    }

    int ErrorMatrix()
    {
        for (int i = 0; i < size_matrix; i++)
        {
            for (int j = i; j < size_matrix; j++)
            {
                if (matrix[i][j] != matrix[j][i])
                {
                    return 1;
                }
            }
        }
        return 0;
    }

    void PrintMatrix()
    {
        for (int i = 0; i < size_matrix; i++)
        {
            for (int j = 0; j < size_matrix; j++)
            {
                cout << matrix[i][j] << " ";
            }
            cout << endl;
        }
    }
    void DeleteMatrix()
    {
        for (int i = 0; i < size_matrix; i++)
        {
            delete[] matrix[i];
        }
        delete[] matrix;
        size_matrix = 0;
    }
};


class Graph
{
private:
    int sum_edge;
    EDGES* e;
    int sum_top;
public:
    Graph(double** matrix, int size_matrix)
    {
        sum_edge = 0;
        sum_top = size_matrix;

        for (int i = 0; i < size_matrix; i++)
        {
            for (int j = i; j < size_matrix; j++)
            {
                if (matrix[i][j] != 0)
                {
                    sum_edge++;
                }
            }
        }

        e = new EDGES[sum_edge];
        int y = 0;
        for (int i = 0; i < size_matrix; i++)
        {
            for (int j = i; j < size_matrix; j++)
                if (matrix[i][j])
                {
                    e[y].edge = matrix[i][j];
                    e[y].first_top = i;
                    e[y].second_top = j;
                    y += 1;
                }
        }
    }

    ~Graph()
    {
        delete[] e;
    }

    void PrintGraph()
    {
        for (int i = 0; i < sum_edge; i++)
        {

            cout << "Weight:" << e[i].edge << " First_top:" << e[i].first_top << " Second_top:" << e[i].second_top << endl;
        }
    }

    void GraphDot()
    {
        ofstream outf("graph.dot");
        if (!outf)
        {
            cerr << "Uh oh, Test.txt could not be opened for writing!" << endl;
            exit(1);
        }

        outf << "graph new{" << endl;
        for (int i = 0; i < sum_edge; i++)
        {
            outf << e[i].first_top << "--" << e[i].second_top << "[label = " << e[i].edge << "]" << endl;
        }
        outf << "}" << endl;
        system("dot graph.dot -Tpng -o graph.png");
    }
    void FordBellman(int start_top)
    {
        double dis[sum_top];

        for (int i = 0; i < sum_top; i++)
        {
            dis[i] = INF;
        }

        dis[start_top] = 0;

        for (int i = 0; i < sum_top - 1; i++) {



            for (int j = 0; j < sum_edge; j++) {

                if (dis[e[j].first_top] + e[j].edge < dis[e[j].second_top])
                {
                    dis[e[j].second_top] = dis[e[j].first_top] + e[j].edge;
                }
            }

        }

        for (int i = 0; i < sum_edge; i++) {

            int x = e[i].first_top;

            int y = e[i].second_top;

            int weight = e[i].edge;

            if (dis[x] != INF && dis[x] + weight < dis[y])
            {
                cout << "Graph contains negative weight cycle" << endl;
            }
        }

        cout << "Vertex Distance from Source" << endl;

        for (int i = 0; i < sum_top; i++)
        {
            cout << i << "\t\t" << dis[i] << endl;
        }
    }
};

int main()
{
    Matrix M;

    if (M.ErrorMatrix())
    {
        cout << "Try again!" << endl;
        return 0;
    }
    else
    {
        M.PrintMatrix();
    }

    Graph G(M.SetMatrix(), M.SetSizeMatrix());
    G.PrintGraph();
    G.FordBellman(0);
    G.GraphDot();
    return 0;
}
