#pragma once
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <bitset>
#include <cmath>
#include "SHA_1.h"
using namespace std;

class RandomNumberGen
{
    string* DataArray;

public:
    RandomNumberGen()
    {
        srand(time(0));
        DataArray = new string[200];
        for (int i = 0; i < 200; i++)
        {
            DataArray[i] = "";
            for (int j = 0; j < 20; j++)
            {
                DataArray[i] += rand() % 128;
            }
        }
    }

    string getID(int bits)
    {
        int chooseString = rand() % 200;
        SHA1 temp;
        string output = temp.from_String(DataArray[chooseString]);
        char* hextoBin = new char[160];
        for (int i = 0, j = 0; i < 40; i++)
        {
            string hexString(1, output[i]);
            int result = std::stoi(hexString, nullptr, 16);
            string binary = bitset<4>(result).to_string();
            for (int k = 0; k < 4; j++, k++)
            {
                hextoBin[j] = binary[k];
            }
        }
        int index = 160 - bits;
        string final = "";
        for (int i = index; i < 160; i++)
        {
            final += hextoBin[i];
        }
        int totalbits = 160 - index;
        int size2 = 4 * (((bits - 1) / 4) + 1);
        char* binTohex = new char[size2];
        for (int i = 0; i < size2; i++)
        {
            binTohex[i] = '0';
        }
        for (int i = size2, j = bits; j >= 0 && i >= 0; j--, i--)
        {
            binTohex[i] = final[j];
        }
        string returnwali = "";
        for (int i = 0, z = 0; i < ((bits - 1) / 4) + 1; i++)
        {
            int y = 0;
            for (int j = 3; j >= 0; j--, z++)
            {
                y += (binTohex[z] - '0') * pow(2, j);
            }

            if (y < 10)
            {
                returnwali += to_string(y);
            }
            else
            {
                char hexChar = 'a' + (y - 10);
                returnwali += hexChar;
            }
        }

        return returnwali;
    }

    bool AdjustBits(string& str, int bits)
    {
        string temp = "";
        temp += str[0];

        int num = 0;
        num = std::stoi(temp, nullptr, 16);

        int shift_bits = pow(2, bits);

        if (shift_bits >= num)
        {
            return 0;
        }

        return 1;
    }
    string findTheMax(int bits)
    {
        int totalsize = 4 * (((bits - 1) / 4) + 1);

        char* bitArray = new char[totalsize] {'0'};
        for (int i = 0; i < totalsize; i++)
        {
            bitArray[i] = '0';
        }
        for (int i = totalsize - bits; i < totalsize; i++)
        {
            bitArray[i] = '1';
        }

        string returnwali = "";
        for (int i = 0, z = 0; i < ((bits - 1) / 4) + 1; i++)
        {
            int y = 0;
            for (int j = 3; j >= 0; j--, z++)
            {
                y += (bitArray[z] - '0') * pow(2, j);
            }

            if (y < 10)
            {
                returnwali += to_string(y);
            }
            else
            {
                char hexChar = 'a' + (y - 10);
                returnwali += hexChar;
            }
        }
        return returnwali;
    }

    //string subtractFromMax(string s, int bits)
    //{
    //    string x = findTheMax(bits);
    //    string h1;
    //    string h2;

    //    if (x.length() > s.length())
    //    {
    //        return s;
    //    }
    //    else if (x.length() == s.length() && x >= s)
    //    {
    //        return s;
    //    }

    //    h1 = s;
    //    h2 = x;
    //    int borrow = 0;
    //    string result = "";
    //    int i = h1.length() - 1, j = h2.length() - 1;
    //    for (; j >= 0; j--, i--)
    //    {
    //        string h11(1, h1[i]);
    //        int d1 = std::stoi(h11, nullptr, 16);

    //        string h22(1, h2[j]);
    //        int d2 = std::stoi(h22, nullptr, 16);

    //        d1 -= (d2 + borrow);
    //        borrow = 0;

    //        if (d1 < 0)
    //        {
    //            d1 += 16;
    //            borrow++;
    //        }
    //        if (d1 <= 9)
    //        {
    //            result = to_string(d1) + result;
    //        }
    //        else
    //        {
    //            char x = d1 + 'a' - 10;
    //            result = x + result;
    //        }
    //    }
    //    while (i >= 0)
    //    {
    //        string h11(1, h1[i--]);
    //        int d1 = std::stoi(h11, nullptr, 16);
    //        d1 -= borrow;
    //        borrow = 0;
    //        if (d1 < 0)
    //        {
    //            d1 += 16;
    //            borrow++;
    //        }
    //        if (d1 <= 9)
    //        {
    //            result = to_string(d1) + result;
    //        }
    //        else
    //        {
    //            char x = d1 + 'a' - 10;
    //            result = x + result;
    //        }
    //    }
    //    while (result[0] == '0' && result.length() > ((bits - 1) / 4) + 1)
    //    {
    //        result = result.substr(1, result.length() - 1);
    //    }
    //    result = subtractFromMax(result, bits);

    //    return result;
    //}

    string mod(string s, int bits)
    {
        string x = findTheMax(bits);
        char* h1 = nullptr;
        char* h2 = nullptr;

        if (x.length() > s.length())
        {
            return s;
        }
        else if (x.length() == s.length() && x >= s)
        {
            return s;
        }
        h1 = new char[s.length() * 4];
        h2 = new char[x.length() * 4];
        for (int i = 0, y = 0; i < s.length(); i++)
        {
            string h11(1, s[i]);
            int d1 = std::stoi(h11, nullptr, 16);
            string bit = bitset<4>(d1).to_string();
            for (int j = 0; j < 4; j++, y++)
            {
                h1[y] = bit[j];
            }
        }
        for (int i = 0, y = 0; i < x.length(); i++)
        {
            string h11(1, x[i]);
            int d1 = std::stoi(h11, nullptr, 16);
            string bit = bitset<4>(d1).to_string();
            for (int j = 0; j < 4; j++, y++)
            {
                h2[y] = bit[j];
            }
        }
        char* h3 = new char[x.length() * 4];
        for (int i = x.length() * 4 - 1, j = s.length() * 4 - 1; i >= 0; i--, j--)
        {
            if (h2[i] == '1')
            {
                h3[i] = h1[j];
            }
            else
            {
                h3[i] = '0';
            }
        }
        string returnwali = "";
        for (int i = 0, z = 0; i < ((bits - 1) / 4) + 1; i++)
        {
            int y = 0;
            for (int j = 3; j >= 0; j--, z++)
            {
                y += (h3[z] - '0') * pow(2, j);
            }

            if (y < 10)
            {
                returnwali += to_string(y);
            }
            else
            {
                char hexChar = 'a' + (y - 10);
                returnwali += hexChar;
            }
        }
        return returnwali;
    }

    string AddDectoHex(string hexstr, unsigned long long int data)
    {
        ostringstream out;
        out << hex << data;
        string hexstr2 = out.str();
        int carry = 0;
        int i = hexstr.length() - 1, j = hexstr2.length() - 1;
        string result = "";
        for (; i >= 0 && j >= 0; i--, j--)
        {
            string h1(1, hexstr[i]);
            int d1 = std::stoi(h1, nullptr, 16);
            string h2(1, hexstr2[j]);
            int d2 = std::stoi(h2, nullptr, 16);
            d1 += d2 + carry;
            carry = 0;

            if (d1 > 15)
            {
                d1 -= 16;
                carry++;
            }
            if (d1 <= 9)
            {
                result = to_string(d1) + result;
            }
            else
            {
                char x = d1 + 'a' - 10;
                result = x + result;
            }
        }
        while (i >= 0)
        {
            string h1(1, hexstr[i--]);
            int d1 = std::stoi(h1, nullptr, 16);
            d1 += carry;
            carry = 0;
            if (d1 > 15)
            {
                d1 -= 16;
                carry++;
            }
            if (d1 <= 9)
            {
                result = to_string(d1) + result;
            }
            else
            {
                char x = d1 + 'a' - 10;
                result = x + result;
            }
        }
        while (j >= 0)
        {
            string h2(1, hexstr2[j--]);
            int d1 = std::stoi(h2, nullptr, 16);
            d1 += carry;
            carry = 0;
            if (d1 > 15)
            {
                d1 -= 16;
                carry++;
            }
            if (d1 <= 9)
            {
                result = to_string(d1) + result;
            }
            else
            {
                char x = d1 + 'a' - 10;
                result = x + result;
            }
        }
        if (carry)
        {
            result = to_string(carry) + result;
        }
        return result;
    }
};