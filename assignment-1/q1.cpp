#include <iostream>
#include <string>
using namespace std;

class ImageGrid
{
private:
    int **data;
    int rows;
    int cols;

public:
    ImageGrid(int r, int c)
    {
        rows = r;
        cols = c;
        data = new int *[rows];
        for (int i = 0; i < rows; i++)
        {
            data[i] = new int[cols];
            for (int j = 0; j < cols; j++)
                data[i][j] = 0;
        }
    }

    ~ImageGrid()
    {
        for (int i = 0; i < rows; i++)
            delete[] data[i];
        delete[] data;
    }

    int getRows() { return rows; }
    int getCols() { return cols; }

    bool inBounds(int r, int c)
    {
        return r >= 0 && r < rows && c >= 0 && c < cols;
    }

    int get(int r, int c)
    {
        // TODO: bounds-checked read. Return -1 (or throw) if out of bounds.
        if (inBounds(r, c))
            return data[r][c];
        else
            return -1;
    }

    void set(int r, int c, int value)
    {
        // TODO: bounds-checked write.
        if (inBounds(r, c))
            data[r][c] = value;
            
    }

    void rotate90Clockwise()
    {
        // TODO: rotate the grid 90 degrees clockwise.
        
        //a new temporary grid allocation
        int **rotated = new int*[cols];
        for (int i = 0; i < cols; i++)
            rotated[i] = new int[rows];
            
        for (int r = 0; r < rows; r++){
            for (int c = 0; c < cols; c++){
                //new rows and columns for the rotated grid
                int new_r = c;
                int new_c = rows - 1 - r;
                rotated[new_r][new_c] = data[r][c];
            }
        }
        
        //deletion of previous data
        for (int i = 0; i < rows; i++)
            delete[] data[i];
        delete[] data;
        
        //copying the new rotated grid to the old one
        data = rotated;
        int temp = rows;
        rows = cols;
        cols = temp;
        
    }

    void flipHorizontal()
    {
        // TODO: mirror the image left-right, in place (no extra grid needed).
        //simply loop through the mid of array, swap lateral elements
        for (int r = 0; r < rows; r++){
            for (int c = 0; c < cols/2; c++){
                //using temp variable to swap
                int temp;
                temp = data[r][c];
                data[r][c] = data[r][cols - 1 - c];
                data[r][cols - 1 - c] = temp;
            }
        }
    }

    void histogram(int *outCounts /* size 256, pre-zeroed by caller */)
    {
        // TODO: count occurrences of each intensity value (0-255) into outCounts.
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                outCounts[data[r][c]]++;
            }
        }
    }

    void applyBoxBlur()
    {
        // TODO: replace each pixel with the integer average of its EXISTING
        
        //creating a temporary array
        int **blur = new int* [rows];
        for (int r = 0; r < rows; r++){
            blur[r] = new int [cols];
        }
        
        // copying the existing data to the temporary array
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                blur[r][c] = data[r][c];
            }
        }
        
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                int sum = 0;
                int neighborCount = 0;
                //this loop sums the neighboring pixels and the current one
                for (int i = -1; i <= 1; i++) {
                    for (int j = -1; j <= 1; j++) {
                        if (inBounds(r + i, c + j)) {
                            sum += blur[r + i][c + j];
                            neighborCount++;
                        }
                    }
                }
        
                data[r][c] = sum / neighborCount;
            }
        }
    }

    void print()
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                cout << data[i][j];
                if (j != cols - 1)
                    cout << " ";
            }
            cout << endl;
        }
    }
};

int main()
{
    int rows, cols;
    cout << "Enter the number of rows and columns"<< endl;
    cin >> rows >> cols;

    ImageGrid img(rows, cols);
    cout << "Enter the elements of grid." << endl;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            int val;
            cin >> val;
            img.set(i, j, val);
        }
    }

    string cmd;
    cout << "Enter a command:\n- ROTATE/FLIP/HIST/PRINT/BLUR\n- EXIT to end" << endl;
    while (cin >> cmd)
    {
        
        if (cmd == "ROTATE")
        {
            img.rotate90Clockwise();
        }
        else if (cmd == "FLIP")
        {
            img.flipHorizontal();
        }
        else if (cmd == "HIST")
        {
            int counts[256] = {0};
            img.histogram(counts);
            for (int v = 0; v < 256; v++)
            {
                if (counts[v] > 0)
                    cout << v << ": " << counts[v] << endl;
            }
        }
        else if (cmd == "BLUR")
        {
            img.applyBoxBlur();
        }
        else if (cmd == "PRINT")
        {
            img.print();
        }
        else if (cmd == "EXIT")
        {
            break;
        }
        // TODO (optional): handle malformed/unknown commands gracefully.
        else {
            cout << "UNKNOWN COMMAND -_- -_-\nTERMINATING!!" << endl;
            break;
        }
    }

    return 0;
}
