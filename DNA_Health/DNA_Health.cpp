#include <codecvt>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <chrono>

using namespace std;
vector<string> split_string(string);

class Data
{
public:
    int index{ 0 }, value{ 0 };
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
    long long value;

    Gene(int _index, long long _value)
    {
        index = _index;
        value = _value;
    }
};

bool gene_comp(const Gene& v1, const Gene& v2)
{
    return v1.index < v2.index;
}

class Node
{
public:
    std::map<char,Node*> gene_map;
    std::vector<Gene> genes = {};
};

class DNA_Strand
{
public:
    int start{0}, end{0};
    string DNA;
    DNA_Strand(vector<string> t_case)
    {
        start = stoi(t_case[0]);
        end = stoi(t_case[1]);
        DNA = t_case[2];
    }
};


void create_tree(Node* root,const vector<Data> &input_data)
{ 
    for (const auto &data:input_data)
    {
        Node* active_node = root;
        for (const auto &c:data.gene)
        {
            if (active_node->gene_map.find(c) == active_node->gene_map.end())
            {
                active_node->gene_map[c] = new Node;
            }
            active_node = active_node->gene_map[c];
        }
        active_node->genes.push_back(Gene(data.index, data.value));
    }
}

void process_data(vector<int> &health, vector<string> &genes, vector<Data> &input_data)
{
    for (int i = 0; i < health.size(); i++)
    {
        input_data[i] = Data(i, health[i], genes[i]);
    }
}

void delete_node(Node* node)
{
    for(map<char, Node*>::iterator it= node->gene_map.begin(); it != node->gene_map.end();it++)
    {
        delete_node(it->second);
    }
    delete node;
}

long long find_health(DNA_Strand strand, Node* root)
{
    long long health{0};  
    vector<Node*> active_nodes;
    for (auto const& c : strand.DNA)
    {
        vector<Node*> new_active_nodes;
        new_active_nodes.reserve(active_nodes.size()+1);
        active_nodes.push_back(root);
        for(const auto &active_node:active_nodes)
        {
            if (active_node->gene_map.find(c) != active_node->gene_map.end())
            {
                new_active_nodes.push_back(active_node->gene_map[c]);
                std::vector<Gene>::iterator start, end, it;
                start = std::lower_bound (new_active_nodes.back()->genes.begin(), new_active_nodes.back()->genes.end(), Gene(strand.start,0),gene_comp); //          ^
                end  =  std::upper_bound(start, new_active_nodes.back()->genes.end(), Gene(strand.end, 0), gene_comp); //  
                for (it = start; it < end; it++)
                {
                    health += (*it).value;
                }
            }
        }
        active_nodes = new_active_nodes;
    }
    
    return health;
}

void dna_health(vector<string> genes, vector<int> health, vector<vector<string>> first_last)
{
    vector<Data> input_data(genes.size());
    process_data(health,genes,input_data);
    Node* root = new Node;
    create_tree(root,input_data);

    long long min_health =  std::numeric_limits<long long>::max();
    long long max_health = 0;
    for (auto const& t_case : first_last)
    {
        DNA_Strand strand(t_case);
        long long health = find_health(strand, root);
        min_health = min(health, min_health);
        max_health = max(health, max_health);   
    }
    std::cout << "Max health is " << max_health << "\n";
    std::cout << "Min health is " << min_health << "\n";
    delete_node(root);
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
    auto start = chrono::steady_clock::now();
    dna_health(genes, health, first_last);
    auto end = chrono::steady_clock::now();
    cout << "Elapsed time in miliseconds: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;

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
