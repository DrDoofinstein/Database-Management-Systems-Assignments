#include <iostream>
#include <string>
#include <stdlib.h>
#include <bitset>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

using namespace std;

int n;
int candidateKeys[10000] = {};

bool lexico(int x, int y)
{
    if (__builtin_popcount(x) != __builtin_popcount(y))
    {
        return __builtin_popcount(x) < __builtin_popcount(y);
    }

    for (int i = 0; i < n; i++)
    {
        if ((x & (1 << i)) == (y & (1 << i)))
        {
            continue;
        }
        else
        {
            if ((x & (1 << i)))
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
    }
    return 1;
}

bool isCandidateKey(int k, int fd_l[10000], int fd_r[10000], int b)
{
    int attributeClosure_curr = b;
    int attributeClosure_prev = b;
    while (1)
    {
        for (int i = 0; i < k; i++)
        {
            if ((fd_l[i] & attributeClosure_curr) == fd_l[i])
            {
                attributeClosure_curr |= fd_r[i];
                // cout << b << " " << attributeClosure_curr << endl;
                if (__builtin_popcount(attributeClosure_curr) == n)
                {
                    break;
                }
            }
        }
        if (__builtin_popcount(attributeClosure_curr) == n)
        {
            break;
        }
        if (attributeClosure_curr != attributeClosure_prev)
        {
            attributeClosure_prev = attributeClosure_curr;
        }
        else
        {
            break;
        }
    }
    // cout << b << " " << attributeClosure_curr << endl;
    if (__builtin_popcount(attributeClosure_curr) == n)
    {
        return true;
    }
    return false;
}

int main()
{

    int k = 10; // We will take input later
    int fd_l[10000] = {};
    int fd_r[10000] = {};
    for (int i = 0; i < 2 * k + 2; i++)
    {
        string str;
        getline(cin, str);
        istringstream ss(str);
        int x;
        while (ss >> x)
        {
            if (i == 0)
            {
                n = x;
            }
            if (i == 1)
            {
                k = x;
            }
            else
            {
                if (i % 2 == 0)
                {
                    fd_l[(i / 2) - 1] |= (1 << (x - 1));
                }
                else
                {
                    fd_r[(i / 2) - 1] |= (1 << (x - 1));
                }
            }
        }
    }
    // cout<< fd_l[0] << " "<<fd_r[0]<<endl;
    // cout << fd_l[3] << " " << fd_r[3] << endl;
    int count = 0;

    for (int b = 1; b < (1 << n); b++)
    {
        bool isSuperset = false;
        for (int i = 0; i < count; i++)
        {
            if ((candidateKeys[i] & b) == candidateKeys[i])
            {
                isSuperset = true;
                break;
            }
        }
        if (!isSuperset)
        {
            if (isCandidateKey(k, fd_l, fd_r, b))
            {
                candidateKeys[count] = b;
                count++;
            }
        }
    }
    cout << count << endl;
    sort(candidateKeys, candidateKeys + count, lexico);
    for (int i = 0; i < count; i++)
    {

        for (int y = 0; y < n; y++)
        {
            if ((candidateKeys[i] & (1 << y)))
            {
                cout << y + 1 << " ";
            }
        }
        cout << endl;
    }
    return 0;
}