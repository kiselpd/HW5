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
    vector <double> name_top;
    double *dis;
public:
    Graph()
    {
        cout << "Enter a weighted oriented graph in the format \"0->1 23\" " << endl;
        string str;
        int minus = 1, index = 0, check_pow = 0, pow = 10;
        double number = 0;
        e.resize(index+1);
        getline(cin, str);
        while (!str.empty())
        {
            for (long unsigned int i = 0; i < str.length(); i++)
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
                if ((str[i] == '-') && (str[i+1] == '>'))
                {
                    e[index].first_top = this->NewTop(name_top, number * minus);
                    number = 0;
                    minus = 1;
                    check_pow = 0;
                }
                if (str[i] == ' ')
                {
                    e[index].second_top = this->NewTop(name_top, number * minus);
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
        sum_top = name_top.size();
        dis = new double[sum_top];
    }

    ~Graph()
    {
        e.clear();
        name_top.clear();
    }

    void PrintGraph()
    {
        for (int i = 0; i < sum_edge; i++)
        {
            cout << "Weight:" << e[i].edge << " First_top:" << e[i].first_top << " Second_top:" << e[i].second_top << endl;
        }
        cout << sum_edge << endl << sum_top << endl;
        for (int i = 0; i < sum_top; i++)
        {
            cout << "All names of top: " << name_top[i] << " ";
        }
    }

    double NewTop(vector<double> &vect, double name)
    {
        for (long unsigned int i = 0; i < vect.size(); i++)
        {
            if (name == vect[i])
                return i;
        }
        vect.push_back(name);
        return (vect.size() - 1);
    }

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

    void GraphMakeDot(string name_file, double start_top)
    {
        string str1 = "dot " + name_file + " -Tpng -o " + name_file;
        str1 = str1.substr(0, str1.length() - 3);
        str1 = str1 + "png";

        string str2 = "explorer.exe " + name_file;
        str2 = str2.substr(0, str2.length() - 3);
        str2 = str2 + "png";

        const char* command1 = str1.c_str();
        const char* command2 = str2.c_str();

        ofstream outf(name_file);
        if (!outf)
        {
            cout << "File" << name_file << "can\'t be opened!" << endl;
            exit(1);
        }

        outf << "digraph new{" << endl;
        for (int i = 0; i < sum_edge; i++)
        {
            outf << name_top[e[i].first_top] << "->" << name_top[e[i].second_top]<< "[label = " << e[i].edge << "]" << endl;
        }
        outf.close();

        if ((this->NegativeWeightCycle()) == 0)
        {
            PrintDistance(name_file, start_top);
        }
        else
        {
            ofstream out(name_file, std::ios::binary | std::ios::app);
            out << "label=\"Graph contains negative weight cycle!\"" << endl;
            out.close();
        }
        ofstream oute(name_file, std::ios::binary | std::ios::app);
        oute << "}" << endl;
        oute.close();
        system(command1);
        system(command2);
    }

    void FordBellman(double start_top)
    {
        int x;
        for (int i = 0; i < sum_top; i++)
        {
            dis[i] = INF;
        }
       
        for (long unsigned int i = 0; i < name_top.size(); i++)
        {
            if (start_top == name_top[i])
                x = i;
        }

        dis[x] = 0;

        for (int i = 0; i < sum_top - 1; i++) {
            for (int j = 0; j < sum_edge; j++) {
                if ((dis[e[j].first_top] + e[j].edge < dis[e[j].second_top]))
                {
                    dis[e[j].second_top] = dis[e[j].first_top] + e[j].edge;
                }
            }
        }
    }

    int NegativeWeightCycle()
    {
        for (int i = 0; i < sum_edge; i++) {

            int x = e[i].first_top;
            int y = e[i].second_top;
            int weight = e[i].edge;

            if ((dis[x] != INF) && ((dis[x] + weight) < dis[y]))
            {
                return 1;
            }
        }
        return 0;
    }

    void PrintDistance(string name_file, double start_top)
    {
        ofstream outd(name_file, std::ios::binary | std::ios::app);
        outd << "\n" << "label=\"Vertex Distance from " << start_top  << endl;
        for (int i = 0; i < sum_top; i++)
        {
            if (dis[i] != INF)
            {
                outd << start_top << "-->" << name_top[i] << " = " << dis[i] << endl;
            }
            else
            {
                outd << start_top << "-->" << name_top[i] << " don't connect" << endl;
            }
        }

        outd << "\"";
        outd.close();
    }
};

class Window
{
private:
    string file;
    double start;
public:
    Window()
    {
   
        Graph G;

        cout << "Enter the file where you want to save the work of the Bellman-Ford algorithm: ";
        getline(cin, file);
        if (file.empty())
        {
            file = "test.dot";
        }

        cout << "Enter the start in the graph: ";
        cin >> start;

        G.FordBellman(start);
        G.GraphMakeDot(file, start);
    }
};


int main()
{

    Window Start;

    return 0;
}
