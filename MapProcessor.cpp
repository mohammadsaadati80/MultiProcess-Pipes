#include "defs.hpp"

using namespace std;

struct Word
{
    string key;
    int value;
};

vector<string> read_file(string filename);
vector<Word> count_word(vector<string> file_data);
string result_vector_to_string(vector<Word> words);
void send_result_to_map_pipe(string result ,string map_index);

int main(int argc, char *const argv[])
{
    string testcase_dir;
    string testcase_index;
    stringstream ss(argv[1]);
    getline(ss, testcase_dir, DOLLORSIGN);
    getline(ss, testcase_index, DOLLORSIGN);
    vector<string> file_data = read_file(testcase_dir + SLASH + testcase_index + CSV_EXT);
    vector<Word> result = count_word(file_data);
    string result_string = result_vector_to_string(result);
    send_result_to_map_pipe(result_string, testcase_index);
    return 0;
}

vector<string> read_file(string filename)
{
    vector<string> file_words;
    string line;
    ifstream file(filename.c_str());
    while (getline(file, line, NEW_LINE))
    {
        stringstream ss(line);
        string word;
        while(getline(ss, word, COMMA))
            file_words.push_back(word);   
    }
    file.close();
    return file_words;
}

vector<Word> count_word(vector<string> file_data)
{
    vector<Word> result;
    for (int i = 0; i < file_data.size(); i++)
    {
        if(result.size() == 0)
        {
            Word temp;
            temp.key = file_data[i];
            temp.value = 1;
            result.push_back(temp);
        }
        else
        {
            bool r = false;
            for(int j = 0; j < result.size(); j++)
                if(file_data[i] == result[j].key)
                {
                    result[j].value++;
                    r = true;
                    break;
                }
            if(r != true)
            {
                Word temp;
                temp.key = file_data[i];
                temp.value = 1;
                result.push_back(temp);
            }
            
        } 
    }
    return result;
}

string result_vector_to_string(vector<Word> words)
{
    string result = "";
    for (int i = 0; i < words.size(); i++)
        result += (words[i].key + COLON + to_string(words[i].value) + COMMA);
    result.pop_back();
    return result;
}

void send_result_to_map_pipe(string result ,string map_index)
{
    int fd;
    fd = open((MAP_PIPE_NAME + map_index).c_str(), O_WRONLY);
    write(fd, result.c_str(), result.length());
    close(fd);
}