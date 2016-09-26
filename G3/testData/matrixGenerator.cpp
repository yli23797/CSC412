#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

using namespace std;

int randomInt()
{
    int temp = rand() % 5000;
    return temp;
}

int main( int argc, char** argv )
{
    int counter = 0;
    string test = "test";
    ifstream fin;
    ofstream fout;

    fin.open( "matrixSize" );
    if(!fin)
    {
        cout << "can't open inputfile\n";

    }

    int size;
    while( fin >> size )
    {
        fout.open( test + to_string(counter) + "A" );
        cout << test + to_string(counter) + "A" << endl;
        fout << size << " ";
        fout << size << "\n";
        for(int i = 0; i < size; i++)
        {
            for( int j = 0; j < size; j++)
            {
                fout << randomInt() << " ";
            }
            fout << "\n";
        }
        fout.close();

        fout.open( test + to_string(counter) + "B");
        fout << size << " ";
        fout << size << "\n";
        for(int i = 0; i < size; i++)
        {
            for( int j = 0; j < size; j++)
            {
                fout << randomInt() << " ";
            }
            fout << "\n";
        }
        counter++;
        fout.close();
    }
    fin.close();
}