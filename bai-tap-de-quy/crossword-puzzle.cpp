#include <bits/stdc++.h>

using namespace std;

string ltrim(const string&);
string rtrim(const string&);

/*
 * Complete the 'crosswordPuzzle' function below.
 *
 * The function is expected to return a STRING_ARRAY.
 * The function accepts following parameters:
 *  1. STRING_ARRAY crossword
 *  2. STRING words
 */

vector<string> listofWord(string words)
{
    vector<string> outp{};
    int l = words.length();
    string w = "";
    for (int i = 0; i < l; i++)
    {
        if (words[i] != ';')
        {
            w = w + words[i];
        }
        else
        {
            outp.push_back(w);
            w = "";
        }
    }
    outp.push_back(w);
    return outp;
}


bool fillHWord(vector<string>& crossword, const string s, int i0, int j0)
{
    vector<string> tmpCrossword = crossword;
    for (int t = 0; t < j0; t++)
    {
        if (tmpCrossword[i0][t] == s[0])
        {
            if (t + s.length() > 10) { return false; }
            for (int j = 0; j < s.length(); j++)
            {
                if (tmpCrossword[i0][t + j] == '-' || tmpCrossword[i0][t + j] == s[j])
                {
                    tmpCrossword[i0][t + j] = s[j];
                    if (j + 1 == s.length()) {
                        crossword = tmpCrossword;
                        return true;
                    }
                }
                else
                {
                    tmpCrossword = crossword;
                    break;
                }
            }
        }
    }

    if (j0 + s.length() > 10) { return false; }
    for (int j = 0; j < s.length(); j++)
    {
        if (tmpCrossword[i0][j0 + j] == '-' || tmpCrossword[i0][j0 + j] == s[j])
        {
            tmpCrossword[i0][j0 + j] = s[j];
            for (size_t k = 0; k < crossword.size(); k++) {
            }
        }
        else
        {
            return false;
        }
    }
    crossword = tmpCrossword;
    return true;
}

bool fillVWord(vector<string>& crossword, const string s, int i0, int j0)
{
    vector<string> tmpCrossword = crossword;
    for (int t = 0; t < i0; t++)
    {
        if (tmpCrossword[t][j0] == s[0])
        {
            if (t + s.length() > 10) { return false; }
            for (int i = 0; i < s.length(); i++)
            {
                if (tmpCrossword[t + i][j0] == '-' || tmpCrossword[t + i][j0] == s[i])
                {
                    tmpCrossword[t + i][j0] = s[i];
                    if (i + 1 == s.length()) {
                        crossword = tmpCrossword;
                        return true;
                    }
                }
                else
                {
                    tmpCrossword = crossword;
                    break;
                }
            }
        }
    }
    if (i0 + s.length() > 10) { return false; }
    for (int i = 0; i < s.length(); i++)
    {
        if (tmpCrossword[i0 + i][j0] == '-' || tmpCrossword[i0 + i][j0] == s[i])
        {
            tmpCrossword[i0 + i][j0] = s[i];
        }
        else
        {
            return false;
        }
    }
    crossword = tmpCrossword;
    return true;
}

bool solvePuzzle(vector<string>& crossword, vector<string> wordsList, int layer)
{
    vector<string> tmpCrossword = crossword;
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (crossword[i][j] == '-')
            {

                int nl = wordsList.size();
                bool f1 = false;
                for (int t = 0; t < nl; t++) {
                    string aWord = wordsList[t];
                    f1 = fillHWord(crossword, aWord, i, j);
                    if (!f1)
                    {
                        f1 = fillVWord(crossword, aWord, i, j);
                    }
                    if (f1)
                    {
                        vector<string> newWordsList = wordsList;
                        newWordsList.erase(newWordsList.begin() + t);
                        if (newWordsList.size() == 0)
                        {
                            return true;
                        }
                        f1 = solvePuzzle(crossword, newWordsList, layer + 1);
                    }
                    if (!f1)
                    {
                        crossword = tmpCrossword;
                    }
                    else
                    {
                        return true;
                    }
                }
                if (!f1)
                {
                    return false;
                }
            }
        }
    }
    return true;
}

vector<string> crosswordPuzzle(vector<string> crossword, string words) {
    vector<string> crossword0 = crossword;
    vector<string> wordsList = listofWord(words);
    bool f0 = false;
    while (!f0)
    {
        f0 = solvePuzzle(crossword, wordsList, 1);
    }
    return crossword;
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    vector<string> crossword(10);

    for (int i = 0; i < 10; i++) {
        string crossword_item;
        getline(cin, crossword_item);

        crossword[i] = crossword_item;
    }

    string words;
    getline(cin, words);

    vector<string> result = crosswordPuzzle(crossword, words);

    for (size_t i = 0; i < result.size(); i++) {
        fout << result[i];

        if (i != result.size() - 1) {
            fout << "\n";
        }
    }

    fout << "\n";

    fout.close();

    return 0;
}

string ltrim(const string& str) {
    string s(str);

    s.erase(
        s.begin(),
        find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace)))
    );

    return s;
}

string rtrim(const string& str) {
    string s(str);

    s.erase(
        find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(),
        s.end()
    );

    return s;
}
