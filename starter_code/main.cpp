/* 
 * File:   main.cpp
 * Author: Kapil Thakkar
 *
 */

#include <cstdlib>

#include "SessionOrganizer.h"

using namespace std;

/*
 * 
 */
int main ( int argc, char** argv )
{
    // Parse the input.
    if ( argc < 3 )
    {   
        cout<<"Missing arguments\n";
        cout<<"Correct format : \n";
        cout << "./main <input_filename> <output_filename>";
        exit ( 0 );
    }
    string inputfilename ( argv[1] );
    
    // Initialize the conference organizer.
    SessionOrganizer *organizer  = new SessionOrganizer( inputfilename );

    // Organize the papers into tracks based on similarity.
    timer t;
    t.start();

    organizer->organizePapers ( t );
    organizer->printSessionOrganiser ( argv[2]);

    // Score the organization against the gold standard.
    // double score = organizer->scoreOrganization ( );
    // cout<< "score:"<<score<<endl;
    // cout << "Time req. in seconds is " << t.elapsedTime() << endl;

    return 0;
}

