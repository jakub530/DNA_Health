#include <codecvt>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include <iostream>
#include <fstream>


using namespace std;

vector<string> split_string(string);

class Gene
{
public:
    int index;
    long long value;

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
    int from_index;
    int to_index;
    string DNA;
    DNA_Strand(vector<string> t_case)
    {
        from_index = stoi(t_case[0]);
        to_index = stoi(t_case[1]);
        DNA = t_case[2];
    }
};


void create_tree(Node* root, vector<Node*> &all_nodes,const vector<string> &genes, const vector<int> &health)
{
    
    for (int g_i = 0; g_i < genes.size(); g_i++)
    {
        Node* active_Node = root;
        for (int c = 0; c < genes[g_i].size(); c++)
        {
            if (active_Node->gene_map.find(genes[g_i][c]) == active_Node->gene_map.end())
            {
                //Key Not Found Option
                Node* tmp_node = new Node;
                all_nodes.push_back(tmp_node);
                active_Node->gene_map[genes[g_i][c]] = all_nodes.back();
            }
            active_Node = active_Node->gene_map[genes[g_i][c]];
            if (c == genes[g_i].size() - 1)
            {
                Gene tmp_gene(g_i, health[g_i]);
                active_Node->genes.push_back(tmp_gene);
            }
        }
    }
}

void progress_node(vector<Node*> &active_nodes,const int &node_ind,const DNA_Strand& t_case,long long &health, vector<int> nodes_to_delete, const char &c)
{
    Node* active_Node = active_nodes[node_ind];
    if (active_Node->gene_map.find(c) != active_Node->gene_map.end())
    {
        active_nodes[node_ind] = active_Node->gene_map[c];
        Node* node = active_nodes[node_ind];
        for (const auto &gene: node->genes)
        {
            if (gene.index >= t_case.from_index && gene.index <= t_case.to_index)
            {
                health += gene.value;
            }
            else if (gene.index > t_case.to_index)
            {
                break;
            }
        }
    }
    else
    {
        nodes_to_delete.push_back(node_ind);
    }
}

void delete_nodes(const vector<int> &nodes_to_delete, vector<Node*> &active_nodes)
{
    if (nodes_to_delete.size() > 0)
    {
        for (int i = nodes_to_delete.size() - 1; i >= 0; i--)
        {
            active_nodes.erase(active_nodes.begin() + nodes_to_delete[i]);
        }
    }
}

void dna_health(vector<string> genes, vector<int> health, vector<vector<string>> first_last)
{
    vector<Node*> all_nodes;
    Node* root = new Node;
    all_nodes.push_back(root);
    create_tree(root,all_nodes,genes,health);

    long long min_health =  std::numeric_limits<long long>::max();
    long long max_health = 0;
    for (auto const& t_case : first_last)
    {
        vector<Node*> active_nodes;
        DNA_Strand t_case(t_case);
        long long health = 0;
        for (auto const& c : t_case.DNA)
        {
            vector<int> nodes_to_delete;
            active_nodes.push_back(root);
            for (int node = 0; node < active_nodes.size(); node++)
            {
                progress_node(active_nodes, node, t_case, health, nodes_to_delete,c);
            }
            delete_nodes(nodes_to_delete, active_nodes);
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
