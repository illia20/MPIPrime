# include <cmath>
# include <cstdlib>
# include <ctime>
# include <iomanip>
# include <iostream>
# include <mpi.h>

using namespace std;

int main(int argc, char* argv[]);
int prime_number(int n, int id, int p);

const int nTests = 4;
int N[nTests] = { 1000, 10000, 100000, 1000000 };

int main(int argc, char* argv[])
{
    int id, ierr, p;

    ierr = MPI_Init(&argc, &argv);

    if (ierr != 0)
    {
        cout << "\n";
        cout << "PRIME_MPI - Error!\n";
        cout << "MPI_Init returned nonzero ierr.\n";
        exit(1);
    }


    //  Get the number of processes.
    ierr = MPI_Comm_size(MPI_COMM_WORLD, &p);

    //  Determine this processes's rank.
    ierr = MPI_Comm_rank(MPI_COMM_WORLD, &id);

    if (id == 0)
    {
        cout << "Primes MPI Program\n";
        cout << "The number of processes is " << p << "\n";
        cout << "\n";
        cout << "         N        P          Time\n";
        cout << "\n";
    }

    int n, primes, primes_part;
    double wtime;
    for (int i = 0; i < nTests; i++) {
        n = N[i];

        if (id == 0)
        {
            wtime = MPI_Wtime();
        }

        ierr = MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

        primes_part = prime_number(n, id, p);

        ierr = MPI_Reduce(&primes_part, &primes, 1, MPI_INT, MPI_SUM, 0,
            MPI_COMM_WORLD);

        if (id == 0)
        {
            wtime = MPI_Wtime() - wtime;

            cout << "  " << setw(8) << n
                << "  " << setw(8) << primes
                << "  " << setw(14) << wtime << "\n";
        }
    }

    //  Terminate MPI.
    MPI_Finalize();
    if (id == 0)
    {
        cout << "\nFinish";
        cout << '\n';
    }

    return 0;
}

int prime_number(int n, int id, int p)
{
    int i;
    int j;
    int prime;
    int total;

    total = 0;

    for (i = 2 + id; i <= n; i = i + p)
    {
        prime = 1;
        for (j = 2; j < (i / 2 + 1); j++)
        {
            if ((i % j) == 0)
            {
                prime = 0;
                break;
            }
        }
        total = total + prime;
    }
    return total;
}