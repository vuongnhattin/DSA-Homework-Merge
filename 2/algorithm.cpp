#include "algorithm.h"
#include <math.h>

int stringSearch(std::ifstream& f, std::string pattern, int algNum)
{
    ALG alg; //alg is the function pointer pointing to the corresponding algorithm 
    switch (algNum)
    {
        case BF:
            alg = BruteForce;
            break;

        case RK:
            alg = RapinKarp;
            break;

        case KMP:
            alg = KnuthMorrisPath;
            break;

        case BM:
            alg = BoyerMoore;
            break;
    }

    return alg(f, pattern);
}

bool stringCompare(std::string& s1, int s1_start, std::string& s2, int s2_start, int numCmp) //simulate std::compare in std::string 
                                                                                             //because C++ might optimize the std::compare
{
    int i = s1_start;
    int len_s1 = s1.length();
    int j = s2_start;
    int len_s2 = s2.length();
    while (numCmp > 0 && i < len_s1 && j < len_s2)
    {
        if (s1[i] < s2[j])
            return -1;
        
        if (s1[i] > s2[j])
            return 1;
        
        numCmp--;
        i++;
        j++;
    }
    return 0;
}


int BruteForce(std::ifstream& f, std::string pattern)
{
    int res = 0;
    int patLen = pattern.length();
    while (!f.eof()) //file with many lines, string searching on each line 
                     //and return number of pattern founded in all of the lines
    {
        std::string text;
        getline(f, text);
        
        res += BruteForce(text, pattern);
    }
    return res;
}

int BruteForce(std::string text, std::string pattern)
{
    int res = 0;
    int texLen = text.length();
    int patLen = pattern.length();

    for (int i = 0; i < texLen - patLen + 1; i++)
        if (stringCompare(text, i, pattern, 0 , patLen) == 0) //compare pattern and text at every text position 
            res++;
    return res;
}

int RapinKarp(std::ifstream& f, std::string pattern)
{
    int res = 0;

    //tính trước hash number cho xâu pattern
    int mod = pow(10, 9) + 7; //lấy số mod nguyên tố làm giảm đụng độ
    //lấy hash number cho xâu pattern
    int patLen = pattern.length();
    int patHash = 0;
    for (int i = 0; i < patLen; i++)
        patHash = (long long)(patHash + pattern[i]) * MAXCHARACTER % mod; //MAXCHARACTER contains number of maximum number of characters
                                                                          //might appear in both pattern and text
    while (!f.eof()) //file with many lines, string searching on each line 
                     //and return number of pattern founded in all of the lines
    {
        std::string text;
        getline(f, text);

        res += RapinKarp(text, pattern, patLen, patHash);
    }
    return res;                                                     
}

int RapinKarp(std::string text, std::string pattern, int patLen, int patHash)
{
    int res = 0;
    int mod = pow(10, 9) + 7;                                        
    //lấy hash number cho từng vị trí xâu text
    int texLen = text.length();
    int texHash = 0;
    int window = 0; //save number of calculated characters in texHash 
                    //if window > patLen, remove a character in textHash
                    //call deleted charater as a, formula: hash - a * MAXCHARACTER ^ patLen

    int leadingBase = 1; //save MAXCHARACTER ^ patLen (cannot use pow because it might be really large)
    for (int i = 0; i <= patLen; i++)
        leadingBase = ((long long)leadingBase * MAXCHARACTER) % mod;  

    //string searching
    for (int i = 0; i < texLen; i++)
    {
        texHash = ((long long)texHash + text[i]) * MAXCHARACTER % mod;
        window++;
        if (window > patLen)
        {
            texHash = texHash - (text[i - patLen] * (long long)leadingBase) % mod;
            if (texHash < 0)
                texHash = texHash + mod;
            window--;
        }

        if (texHash == patHash && window == patLen) //if two hash numbers are equal, check every character (there might be collision)
        {
            if (stringCompare(text, i - patLen + 1, pattern, 0 , patLen) == 0)
                res++;
        }
    }        
    return res;
}

int KnuthMorrisPath(std::ifstream& f, std::string pattern)
{
    int res = 0;
    int patLen = pattern.length();
    int* patSuf = new int[patLen]; //create an array contains longest suffix ended at position i-1 (not the whole substring [0..i-1])
    
    patSuf[0] = 0;
    for (int i = 1; i < patLen; i++)
    {
        int sufLen = patSuf[i - 1]; //sufLen contains longest suffix which are currently considered
        while (sufLen != 0 && pattern[sufLen] != pattern[i]) //if suffix does not exist or it has been found, exit while
            sufLen = patSuf[sufLen - 1];

        if (sufLen == 0 && pattern[0] != pattern[i]) //if suffix doea not exist, take the first character to compare
            patSuf[i] = 0;
        else
            patSuf[i] = sufLen + 1;
    }

    while (!f.eof()) //file with many lines, string searching on each line 
                     //and return number of pattern founded in all of the lines
    {
        std::string text;
        getline(f, text);

        res += KnuthMorrisPath(text, pattern, patSuf);
    }

    delete[] patSuf;
    return res;
}

int KnuthMorrisPath(std::string text, std::string pattern, int patSuf[])
{
    int res = 0;

    int sufLen = 0; //variable contains longest suffix ended at position i-1 in text
    int texLen = text.length();
    int patLen = pattern.length();

    if (text[0] == pattern[0])
        sufLen = 1;
    for (int i = 1; i < texLen; i++)
    {
        while (sufLen != 0 && pattern[sufLen] != text[i])
            sufLen = patSuf[sufLen - 1];
        
        if (pattern[sufLen] == text[i]) 
            sufLen++;
        
        if (sufLen == patLen) //the length of suffix equals the length of pattern string -> a pattern is found in text string
            res++;
    } 
    return res;
}

int BoyerMoore(std::ifstream& f, std::string pattern)
{
    int res = 0;

    int patLen = pattern.length();
    int* badShift = new int[MAXCHARACTER];
    int* goodShift = new int[patLen];

    buildBadShift(badShift, pattern); //build shifting table in bad rule character case
    buildGoodShift(goodShift, pattern); //build shifting table in good rule character case

    while (!f.eof()) //file with many lines, string searching on each line 
                     //and return number of pattern founded in all of the lines
    {
        std::string text;
        getline(f, text);

        res += BoyerMoore(text, pattern, badShift, goodShift);
    }

    delete[] badShift;
    delete[] goodShift;
    return res;
}

void buildBadShift(int badShift[], std::string pattern)
{
    int patLen = pattern.length(); //build badShift table with Horspool algorithm
    for (int i = 0; i < MAXCHARACTER; i++)
        badShift[i] = patLen;
    for (int i = 0; i < patLen - 1; i++)
        badShift[pattern[i]] = patLen - i - 1;
}

void buildGoodShift(int goodShift[], std::string pattern)
{
    int patLen = pattern.length();
    for (int i = 0; i < patLen; i++)
        goodShift[i] = patLen; //initialize value
    goodShift[0] = 1; //no information -> shift 1

    //rule 1: VD: given a[bcd] with "bcd" is the matched substring of pattern
    //            find another "bcd" in pattern with the condition: the following character is not "a"
    //            -> use KMP from right to left
    int* patPre = new int[patLen];
    patPre[patLen - 1] = 0;
    for (int i = patLen - 2; i >= 0; i--)
    {
        int len = patPre[i + 1]; 

        while (len > 0 && pattern[patLen - len - 1] != pattern[i])
            len = patPre[patLen - len];

        if (len == 0 && pattern[patLen - 1] != pattern[i])
            patPre[i] = 0;
        else
            patPre[i] = len + 1;
    }

    for (int i = 0; i < patLen; i++) 
    {
        int len = patPre[i];
        
        while (len > 0) //tồn tại tiền tố 
        {
            if (i == 0 || pattern[i - 1] != pattern[patLen - len - 1]) //check the condition that following character has to be different 
                goodShift[len] = patLen - len - i;
            len = patPre[patLen - len];
        }
    }

    //rule 2: VD: given a[bcd] with bcd is the matched substring of pattern
    //            another [bcd] in pattern is not found
    //            find suffix in matched substring satisfied this suffix matches pattern prefix
    int len = patPre[0];
    for (int i = patLen - 1; i >= 0; i--)
    {
        if (len == 0) 
            break;
        if (len > i)
            len = patPre[patLen - len];
        goodShift[i] = std::min(goodShift[i], patLen - len);
    }
    delete[] patPre;
}

int BoyerMoore(std::string text, std::string pattern, int badShift[], int goodShift[])
{
    int res = 0;
    int texLen = text.length();
    int patLen = pattern.length();
    
    int i = patLen - 1; //point to text
    while (i < texLen)
    {
        int j = patLen - 1; //trỏ vào pattern
        
        while (j > -1 && text[i + j - (patLen - 1)] == pattern[j]) //check every character
            j--;
        if (j == -1)
        {
            res++;
            i++;
        }
        else //error is founded -> shift i
            i += std::max(badShift[text[i + j - (patLen - 1)]], goodShift[patLen - j - 1]);
    }
    return res;
}