#include <bits/stdc++.h>     

#define mp make_pair

using namespace std;
typedef pair<int, int> pii;

struct code_node
{
    string name;
    string type;
    vector<int> childs;
};                    


string input_string;
vector<code_node> node_vector;
map<char, bool> skip_char, for_name;
bool read_error;

bool substring_equale(string s1, int start1, string s2, int start2, int len)
{
    if (s1.length() - start1 < len || s2.length() < len || start1 < 0 || start2 < 0)
        return false;
    bool res = true;
    for (int i = 0; i < len; ++i)
        res = res || s1[i + start1] == s2[i + start2];
    return res;    
}

string add_extension(string s)
{
    bool is_exist = false;
    is_exist = is_exist;
    string ext = ".clj", ext_script = ".cljs";
    if (substring_equale(s, s.length() - ext.length(), ext, 0, ext.length()) ||
        substring_equale(s, s.length() - ext_script.length(), ext_script, 0, ext_script.length()))
        return s;
    return s + ext;
}

void update_itt(int &index)
{
    while (index < input_string.length() && !skip_char[input_string[index]])
        ++index;
}

int read_tree(int &index)
{
    if (read_error)
        return -1;
    update_itt(index);
    int now_char = input_string[index];
    if (index >= input_string.length())
    {
        read_error = true;
        return -1;
    }
    code_node this_node;
    node_vector.push_back(this_node);
    int cur = node_vector.size() - 1;
    //go deep
    if (now_char == '(' || now_char == '[' || now_char == '{')
    {
        node_vector[cur].type = "collection";
        char close_br;
        if (now_char == '(')
        {
            close_br = ')';
            node_vector[cur].name = "code list";
        }
        if (now_char == '[')
        {
            close_br = ']';
            node_vector[cur].name = "vector";
        }
        if (now_char == '{')
        {
            close_br = '}';
            node_vector[cur].name = "dict";
        }
        ++index;
        update_itt(index);            
        while (index < input_string.length() && input_string[index] != close_br)
        {
            node_vector[cur].childs.push_back(read_tree(index));
        }
        if (index >= input_string.length())
        {
            read_error = true;
            return -1;
        }
        ++index;
        update_itt(index);
        return cur;       
    }
    //read string.  need add regular string
    if (now_char == '"')
    {
        node_vector[cur].type = "string";
        string s = "";
        ++index;
        bool back_slash = false;
        while (index < input_string.length() && (back_slash || input_string[index] != '"'))
        {
            s.push_back(input_string[index]);
            back_slash = input_string[index] == '\\';
            ++index;            
        }
        if (index >= input_string.length())
        {
            read_error = true;
            return -1;    
        }
        node_vector[cur].name = s;
        ++index;
        return cur;
    }
    //reed list of item
    if (now_char == '\'')
    {
        node_vector[cur].type = "const list";
        ++index;
        if (index >= input_string.length() || input_string[index] != '(')
        {
            read_error = true;
            return -1;
        }
        ++index;
        string s = "'(";
        int balans = 1;
        while (index < input_string.length() && balans > 0)
        {
            s.push_back(input_string[index]);
            if (input_string[index] == '(')
                ++balans;
            else if (input_string[index] == ')')
                --balans;
            ++index;            
        }
        if (index >= input_string.length() && balans > 0)
        {
            read_error = true;
            return -1;    
        }                
        node_vector[cur].name = s;
        return cur;        
    }
    //read const
    if (true)
    {
        node_vector[cur].type = "const";
        string s;
        while (index < input_string.length() && for_name[input_string[index]])
        {
            s.push_back(input_string[index]);
            ++index;    
        }
        if (index >= input_string.length())
        {
            read_error = true;
            return -1;
        }
        node_vector[cur].name = s;
        return cur;
    }
    return -1;        
}

void write_tree(int root)
{
    if (node_vector[root].type == "collection")
    {
        int i = 0;
        if (node_vector[root].name == "code list" && node_vector[root].childs.size() > 0)
        {
            write_tree(node_vector[root].childs[i++]);
            printf("(");
        }
        else if (node_vector[root].name == "vector")
            printf("[");
        else if (node_vector[root].name == "dict")
            printf("{");
        for (; i < node_vector[root].childs.size(); ++i)
        {
            write_tree(node_vector[root].childs[i]);
            if (i + 1 < node_vector[root].childs.size())
                printf(", ");
        }
        if (node_vector[root].name == "code list")
            printf(")\n");
        else if (node_vector[root].name == "vector")
            printf("]");
        else if (node_vector[root].name == "dict")
            printf("}");
    }
    else if (node_vector[root].type == "const list")
    {
        cout << node_vector[root].name;
    }
    else if (node_vector[root].type == "string")
    {
        cout << '"' + node_vector[root].name + '"';
    }
    else if (node_vector[root].type == "const")
    {
        cout << node_vector[root].name;
    }
}

void run()
{
    int index = 0;   
    cout << "GO";
    int root = read_tree(index);
    cout << "READ ACC\n";
    if(read_error)
    {
        printf("read ERROR\n");
        return;
    }
    write_tree(root);       
}

void init()
{
    for (int i = 0; i < 256; ++i)
        skip_char[(char)i] = for_name[(char)i] = 0;
    string no_skip = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM0123456789:!+-*/_<=>'\"\\#()[]{}";
    for (char i : no_skip)
        skip_char[i] = 1;
    string char_name = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOOPASDFGHJKLZXCVBNM0123456789:!+-*/_<=>";
    for (char i : char_name)
        for_name[i] = 1;
    read_error = false;
}


int main()
{
    init();
    //cout << "Enter file name for continue\n";
    string file_name;
    //cin >> file_name;
    file_name = add_extension(file_name);
    freopen("1.clj", "r", stdin);
    string read_string;
    input_string = ("(do ");
    while (cin >> read_string)
    {
        for (char i : read_string)
            input_string.push_back(i);
        input_string.push_back('\n');    
    }
    input_string.push_back(')');
    run(); 
    return 0;
}