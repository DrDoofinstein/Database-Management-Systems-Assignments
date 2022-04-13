#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int pageSize;

class page
{
public:
    page *prevPage;
    page *nextPage;
    int recordCount;
    vector<int> primaryKeys;
    int capacity;
    int pageId;

    page()
    {
        prevPage = nullptr;
        nextPage = nullptr;
        recordCount = 0;
        capacity = pageSize - 16;
        pageId = 0;
    }
};

class heapFile
{
public:
    page *head;
    int pageCount;

    heapFile()
    {
        head = nullptr;
        pageCount = 0;
    }

    void insert(int recordSize, int val)
    {
        page *currPtr = head;
        while (currPtr != nullptr)
        {
            if (currPtr->capacity >= (recordSize + 4))
            {
                currPtr->primaryKeys.push_back(val);
                currPtr->capacity = currPtr->capacity - 4 - recordSize;
                currPtr->recordCount++;
                return;
            }
            currPtr = currPtr->nextPage;
        }

        page *lastPtr = head;
        page *newPage = new page();
        newPage->primaryKeys.push_back(val);
        newPage->capacity = newPage->capacity - 4 - recordSize;
        newPage->recordCount++;
        pageCount++;
        if (head == nullptr)
        {
            head = newPage;
        }
        else
        {
            while (lastPtr->nextPage != nullptr)
            {
                lastPtr = lastPtr->nextPage;
            }
            lastPtr->nextPage = newPage;
            newPage->prevPage = lastPtr;
            newPage->pageId = lastPtr->pageId + 1;
        }
        return;
    }

    void search(int val)
    {
        page *currPtr = head;
        while (currPtr != nullptr)
        {
            for (int i = 0; i < currPtr->recordCount; i++)
            {
                if (val == currPtr->primaryKeys[i])
                {
                    cout << currPtr->pageId << " " << i << endl;
                    return;
                }
            }
            currPtr = currPtr->nextPage;
        }
        cout << -1 << " " << -1 << endl;
        return;
    }

    void status()
    {
        cout << pageCount << " ";
        page *currPtr = head;
        while (currPtr != nullptr)
        {
            cout << currPtr->recordCount << " ";
            currPtr = currPtr->nextPage;
        }
        cout << endl;
    }
};

int main()
{
    cin >> pageSize;
    heapFile file;
    int choice;
    cin >> choice;
    while (choice != 4)
    {
        if (choice == 1)
        {
            int recSize, primaryVal;
            cin >> recSize >> primaryVal;
            file.insert(recSize, primaryVal);
        }
        else if (choice == 2)
        {
            file.status();
        }
        else if (choice == 3)
        {
            int priVal;
            cin >> priVal;
            file.search(priVal);
        }
        else
        {
            cout << "Invalid Choice Code" << endl;
        }
        cin >> choice;
    }
    return 0;
}