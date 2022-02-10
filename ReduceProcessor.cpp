#include "defs.hpp"

using namespace std;

struct Word
{
    string key;
    int value;
};

vector<string> read_from_map_process(int number_of_map_process);
vector<Word> merge_words(vector<string> inp, int number_of_map_process);
string result_vector_to_string(vector<Word> words);
void send_result_to_main_process(string result, int write_pipe_id);

int main(int argc, char *argv[])
{
    string number_of_testcase;
    string write_pipe_id;
    stringstream ss(argv[1]);
    getline(ss, number_of_testcase, DOLLORSIGN);
    getline(ss, write_pipe_id, DOLLORSIGN);
    vector<string> words = read_from_map_process(atoi(number_of_testcase.c_str()));
    vector<Word> merged_words = merge_words(words, atoi(number_of_testcase.c_str()));
    string merged_words_string = result_vector_to_string(merged_words);
    send_result_to_main_process(merged_words_string, atoi(write_pipe_id.c_str()));
    return 0;
}

vector<string> read_from_map_process(int number_of_map_process)
{
    vector<string> result;
    string line;
    for (int i = 0; i < number_of_map_process; i++)
    {
        ifstream fd;
        fd.open((MAP_PIPE_NAME + to_string(i + 1)), std::fstream::in);
        getline(fd, line);
        result.push_back(line);
        fd.close();
    }
    return result;
}

vector<Word> merge_words(vector<string> inp, int number_of_map_process)
{
    vector<Word> result;
    for(int i = 0; i < number_of_map_process; i++)
    {
        string line = inp[i];
        stringstream ss(line);
        string data;
        while(getline(ss, data, COMMA))
        {
            string key;
            string value;
            stringstream ss2(data);
            getline(ss2, key, COLON);
            getline(ss2, value, COLON);
            if(result.size() == 0)
            {
                Word temp;
                temp.key = key;
                temp.value = atoi(value.c_str());
                result.push_back(temp);
            }
            else
            {
                bool r = false;
                for(int j = 0; j < result.size(); j++)
                    if(key == result[j].key)
                    {
                        result[j].value += atoi(value.c_str());
                        r = true;
                        break;
                    }
                if(r != true)
                {
                    Word temp;
                    temp.key = key;
                    temp.value = atoi(value.c_str());
                    result.push_back(temp);
                }
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

void send_result_to_main_process(string result, int write_pipe_id)
{
    write(write_pipe_id, result.c_str(), result.length());
    close(write_pipe_id);
}