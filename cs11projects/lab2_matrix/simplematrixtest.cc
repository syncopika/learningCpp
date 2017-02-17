#include "Matrix.hh"
#include <iostream>

using namespace std;

int main()
{
    // Create a new matrix with two rows and three columns.
    Matrix a(2, 3);

    // Print out some information about the size of this matrix.
    cout << "Matrix a is " << a.getrows() << " rows tall." << endl;
    cout << "Matrix a is " << a.getcols() << " columns wide." << endl;

    // Print out the contents of this matrix (should be all zeroes!).
    for (int r = 0; r < a.getrows(); r++)
    {
        for (int c = 0; c < a.getcols(); c++)
        {
            cout << "Element (" << r << ", " << c << ") = "
                 << a.getelem(r, c) << endl;
        }
    }

    // Fill in a few values.
    a.setelem(1, 2, -5280);  // Bottom right element is now -5280.
    a.setelem(0, 1, 123);    // Top center element is now 123.

    // Create an identical copy of this matrix.
    Matrix b(a);

    // Change the original matrix some more.
    a.setelem(1, 2, 555);  // Bottom right element is now 555.

    // Examine some elements of each matrix.
    cout << "(1, 2) of a = " << a.getelem(1, 2) 
         << " [should be 555]" << endl;
    cout << "(1, 2) of b = " << b.getelem(1, 2) 
         << " [should be -5280]" << endl;

    // So if a and b are different, let's copy a into a new matrix
    // and add b to it.
    Matrix c(a);
    c.add(b);

    // Now let's copy c into another new matrix, d, and subtract a from it.
    Matrix d(c);
    d.subtract(a);

    // Hmm...  that means d should be b, no?
    if (d.equals(b))
    {
        cout << "Woo hoo!  d = b!" << endl;
    }
    else
    {
        cout << "Oh oh!  Something went wrong, d isn't b!" << endl;
    }
	
	//* I added this test
	//try to set invalid coordinate to value. program should terminate here
	//d.setelem(100, 100, 50); 

    // Let's create a tiny 0 by 0 matrix using the default constructor.
    Matrix e;
    cout << "0x0 matrix e is "
         << e.getrows() << " by " << e.getcols() << endl;

    // Of course, e and d are different, since they have different sizes!
    if (!e.equals(d))
    {
        cout << "e and d are indeed different!" << endl;
    }
    else
    {
        cout << "Oh well, back to the drawing board...." << endl;
    }

    // Okay, enough of this; destroy all those matrices and end the program.
    return 0;
}
