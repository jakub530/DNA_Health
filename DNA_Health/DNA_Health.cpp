#include <codecvt>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include <iostream>
#include <fstream>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

using namespace std;
vector<string> split_string(string);

class Data
{
public:
    int index =0;
    int value =0;
    string gene="";
    Data( int _index,  int _value,  string _gene)
    {
        index = _index;
        value = _value;
        gene = _gene;
    }
    Data()
    {
    }
};

class Gene
{
public:
    int index;
    int value;

    Gene(int _index, long long _value)
    {
        index = _index;
        value = _value;
    }
};

class Node
{
public:
    std::map<char,Node*> gene_map;
    std::vector<Gene> genes = {};
};

class DNA_Strand
{
public:
    int start;
    int end;
    string DNA;
    DNA_Strand(vector<string> t_case)
    {
        start = stoi(t_case[0]);
        end = stoi(t_case[1]);
        DNA = t_case[2];
    }
};


void create_tree(Node* root, vector<Node*> &all_nodes,const vector<Data> &input_data)
{ 
    for (const auto &data:input_data)
    {
        Node* active_Node = root;
        for (int c = 0; c < data.gene.size(); c++)
        {
            if (active_Node->gene_map.find(data.gene[c]) == active_Node->gene_map.end())
            {
                //Key Not Found Option
                Node* new_node = new Node;
                all_nodes.push_back(new_node);
                active_Node->gene_map[data.gene[c]] = all_nodes.back();
            }
            active_Node = active_Node->gene_map[data.gene[c]];
            if (c == data.gene.size() - 1)
            {
                Gene tmp_gene(data.index,data.value);
                active_Node->genes.push_back(tmp_gene);
            }
        }
    }
}

void process_data(vector<int> &health, vector<string> &genes, vector<Data> &input_data)
{
    for (int i = 0; i < health.size(); i++)
    {
        Data tmp_data(i, health[i], genes[i]);
        input_data[i] = tmp_data;
    }
}

void dna_health(vector<string> genes, vector<int> health, vector<vector<string>> first_last)
{
    vector<Node*> all_nodes;
    vector<Data> input_data(genes.size());
    process_data(health,genes,input_data);
    Node* root = new Node;
    all_nodes.push_back(root);
    create_tree(root,all_nodes,input_data);

    long long min_health =  std::numeric_limits<long long>::max();
    long long max_health = 0;
    for (auto const& t_case : first_last)
    {
        vector<Node*> active_nodes;
        DNA_Strand strand(t_case);
        long long health = 0;
        for (auto const& c : strand.DNA)
        {
            vector<int> nodes_to_delete;
            active_nodes.push_back(root);
            vector<Node*>::iterator it = active_nodes.begin();
            while (it != active_nodes.end())
            {
                Node* active_Node = *it;
                if (active_Node->gene_map.find(c) != active_Node->gene_map.end())
                {
                    *it = active_Node->gene_map[c];
                    Node* node = *it;
                    for (const auto &gene: node->genes)
                    {
                        health = (gene.index >= strand.start && gene.index <= strand.end) ? health + gene.value : health;
                        if (gene.index > strand.end)
                        {
                            break;
                        }
                    }
                    ++it;
                }
                else
                {
                    it = active_nodes.erase(it);
                }
            }
        }
        min_health = min(health, min_health);
        max_health = max(health, max_health);    
    }
    std::cout << "Max health is " << max_health << "\n";
    std::cout << "Min health is " << min_health << "\n";
    std::cout << "Size of Nodes is " << all_nodes.size() << "\n";

    for (int i = 0; i < all_nodes.size(); i++)
    {
        delete all_nodes[i];
    }

    std::cout << " " << "\n";
    std::cout << " " << "\n";
    std::cout << " " << "\n";
    std::cout << " " << "\n";
    std::cout << " " << "\n";
    std::cout << " " << "\n";
}

int main()
{
    int n;
    fstream inFile;
    inFile.open("testcase_2.txt");
    string n_s;
    getline(inFile, n_s);
    //cin >> n;
    //cin.ignore(numeric_limits<streamsize>::max(), '\n');
    n = stoi(n_s);
    string genes_temp_temp;
    getline(inFile, genes_temp_temp);

    vector<string> genes_temp = split_string(genes_temp_temp);

    vector<string> genes(n);

    for (int i = 0; i < n; i++) {
        string genes_item = genes_temp[i];

        genes[i] = genes_item;
    }

    string health_temp_temp;
    getline(inFile, health_temp_temp);

    vector<string> health_temp = split_string(health_temp_temp);

    vector<int> health(n);

    for (int i = 0; i < n; i++) {
        int health_item = stoi(health_temp[i]);

        health[i] = health_item;
    }

    int s;
    string s_s;
    getline(inFile, s_s);
    s = stoi(s_s);
    //cin.ignore(numeric_limits<streamsize>::max(), '\n');

    vector<vector<string>> first_last(s);
    for (int s_itr = 0; s_itr < s; s_itr++) {
        string firstLastd_temp;
        getline(inFile, firstLastd_temp);

        vector<string> firstLastd = split_string(firstLastd_temp);
        first_last[s_itr] = firstLastd;
        int first = stoi(firstLastd[0]);

        int last = stoi(firstLastd[1]);

        string d = firstLastd[2];
    }
    dna_health(genes, health, first_last);
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    return 0;
}

vector<string> split_string(string input_string) {
    string::iterator new_end = unique(input_string.begin(), input_string.end(), [](const char& x, const char& y) {
        return x == y and x == ' ';
        });

    input_string.erase(new_end, input_string.end());

    while (input_string[input_string.length() - 1] == ' ') {
        input_string.pop_back();
    }

    vector<string> splits;
    char delimiter = ' ';

    size_t i = 0;
    size_t pos = input_string.find(delimiter);

    while (pos != string::npos) {
        splits.push_back(input_string.substr(i, pos - i));

        i = pos + 1;
        pos = input_string.find(delimiter, i);
    }

    splits.push_back(input_string.substr(i, min(pos, input_string.length()) - i + 1));

    return splits;
}
