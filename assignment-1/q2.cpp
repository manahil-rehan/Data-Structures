#include <iostream>
#include <string>
#include <cmath>
using namespace std;

class EmbeddingStore
{
private:
    float **items;
    int dim;
    int count;
    int cap;

    void resize(int newCap)
    {
        // TODO: allocate a new float* array of size newCap, copy existing
        
        //creating a copy
        float **temp = new float*[newCap];
        //copying the existing items to the new temp array
        for (int i = 0; i < count; i++) {
            temp[i] = items[i];
        }
        //freeing the old pointer array
        delete[]items;
        //setting the prev capacity to the new one
        cap= newCap;
        //copying the new temp array to the old array
        items = temp;
    }

public:
    EmbeddingStore(int dimension, int initialCapacity = 4)
    {
        dim = dimension;
        cap = initialCapacity;
        count = 0;
        items = new float *[cap];
    }

    ~EmbeddingStore()
    {
        for (int i = 0; i < count; i++)
            delete[] items[i];
        delete[] items;
    }

    int size() { return count; }
    int capacity() { return cap; }

    void addEmbedding(float *vec)
    {
        // TODO: if count == cap, double capacity.
        if (count == cap) {
            int newCap = cap * 2;
            resize(newCap);
        }
        //creating a temp array that will store vector
        //why? ku k vector is going to be deleted in main
        //to if any other function uses the vector after that, 
        //to wo function is accessing a dangling/ freed memory
        float *temp = new float[dim];
        //copying the vectors in temporary array
        for (int d = 0; d < dim; d++) {
            temp[d] = vec[d];
        }
        //copy the vec in items
        items[count] = temp;
        count++;
    }

    void removeEmbedding(int index)
    {
        // TODO: bounds-check. delete[] the vector at index, then shift all pointers.
        if (index < 0 || index >= count) {
            cout << "INVALID INDEX -_-" << endl;
            return;
        }
        //deleting index par jo item hai
        delete[] items[index];
        
        //shifting all the remaining elements jo count
        //se agay hein, one step backwards
        for (int i = index; i < count-1; i++) {
            items[i] = items[i + 1];
        }
        
        //decreasing count by 1 ku k ek item delete ho gya hai
        count--;
    }

    float *getEmbedding(int index)
    {
        // TODO: bounds-checked access, return pointer (or nullptr if invalid).
        if (index >= 0 && index < count) {
            return items[index];
        }
        else
            return nullptr;
    }

    float distance(float *a, float *b)
    {
        // TODO: Euclidean distance between two length-dim vectors.
        float total = 0;
        for (int i = 0; i < dim; i++) {
            total = total + pow((a[i] - b[i]), 2);
        }
        if (total > 0.0) {
            return (sqrt(total));
        }
        else 
            return 0.0f;
    }

    int nearestNeighbor(float *query, float &outDistance)
    {
        // TODO: linear scan over all stored embedding, find index with
        // minimum distance() to query. Set outDistance to that distance.
        // Return -1 if store is empty.
        if (count == 0) {
           return -1; 
        }
        
        //setting bestIndex to the first index
        //setting bestdistance to the distance between query and the first element
        int bestIndex = 0;
        float bestDis = distance(items[0], query);
        //loops through the whole array to find if 
        //any other distance is smaller than the initial best distance 
        for (int i = 1; i < count; i++) {
            float dis = distance(items[i], query); 
            if (dis < bestDis) {
                bestDis = dis;
                bestIndex = i;
            }
        }
        //sets outDistance to the best disyance and returns the best index
        outDistance = bestDis;
        return bestIndex;
 
    }

    void printState()
    {
        for (int i = 0; i < count; i++)
        {
            cout << "[" << i << "] ";
            for (int j = 0; j < dim; j++)
            {
                cout << items[i][j];
                if (j != dim - 1)
                    cout << " ";
            }
            cout << endl;
        }
    }
};

int main()
{
    int dim;
    cin >> dim;

    EmbeddingStore store(dim);

    cout << "Enter a command:\n- ADD/REMOVE/QUERY/STATE\n- EXIT to stop" << endl;
    string cmd;
    while (cin >> cmd)
    {
        if (cmd == "ADD")
        {
            float *vec = new float[dim];
            for (int i = 0; i < dim; i++)
                cin >> vec[i];
            store.addEmbedding(vec);
            delete[] vec; // addEmbedding should have made its own copy
            cout << "size=" << store.size() << " capacity=" << store.capacity() << endl;
        }
        else if (cmd == "REMOVE")
        {
            int idx;
            cin >> idx;
            store.removeEmbedding(idx);
            cout << "size=" << store.size() << " capacity=" << store.capacity() << endl;
        }
        else if (cmd == "QUERY")
        {
            float *q = new float[dim];
            for (int i = 0; i < dim; i++)
                cin >> q[i];
            float dist;
            int idx = store.nearestNeighbor(q, dist);
            cout << "nearest=" << idx << " distance=" << dist << endl;
            delete[] q;
        }
        else if (cmd == "STATE")
        {
            store.printState();
        }
        else if (cmd == "EXIT")
        {
            break;
        }
        // TODO (optional): handle malformed/unknown commands gracefully.
        else
            cout << "INVALID COMMAND -_- -_-" << endl;
    }

    return 0;
}
