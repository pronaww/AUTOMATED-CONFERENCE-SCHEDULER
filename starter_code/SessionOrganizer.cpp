/* 
 * File:   SessionOrganizer.cpp
 * Author: Kapil Thakkar
 * 
 */

#include "SessionOrganizer.h"
#include "Util.h"

#include <ctime>
#include <cstdlib>


SessionOrganizer::SessionOrganizer ( )
{
    parallelTracks = 0;
    papersInSession = 0;
    sessionsInTrack = 0;
    processingTimeInMinutes = 0;
    tradeoffCoefficient = 1.0;
}

SessionOrganizer::SessionOrganizer ( string filename )
{
    readInInputFile ( filename );
    conference = new Conference ( parallelTracks, sessionsInTrack, papersInSession );
}

void SessionOrganizer::swap (int a, int b, int c, int i, int j, int k){
    int temp = conference->getTrack( j ).getSession ( i ).getPaper ( k );

    conference->setPaper ( j, i, k, conference->getTrack( b ).getSession ( a ).getPaper ( c ) );
    conference->setPaper ( b, a, c, temp );
}

void SessionOrganizer::organizePapers ( timer t )
{
	//Start the clock
	// clock_t cl;
    // cl = clock();

	int n = conference->getSessionsInTrack()*conference->getPapersInSession()*conference->getParallelTracks();

    int paperCounter = 0;
    // int arr[] = {6,1,10,3,2,5,0,8,11,9,4,7};
    for ( int i = 0; i < conference->getSessionsInTrack ( ); i++ )
    {
        for ( int j = 0; j < conference->getParallelTracks ( ); j++ )
        {
            for ( int k = 0; k < conference->getPapersInSession ( ); k++ )
            {
                conference->setPaper ( j, i, k, n-1 - paperCounter );
                paperCounter++;
            }
        }
    }


    // double old_max_score = -1;
    // double max_score = scoreOrganization();
	// double new_score=-1;
    // double current_max = max_score;
	double current_delta = 0;
			    
    int remember_i=-1, remember_j=-1, remember_k=-1;
    int temp;
    //The code is written here

    int a,b,c,i,j,k;
    double delta;

    int noOfExchanges = 0;

    double processingTimeInSeconds = processingTimeInMinutes*60.0*0.99;
    // cout << "************" << endl << processingTimeInMinutes << endl << processingTimeInSeconds << endl << "************" << endl;
    while(true){

    	noOfExchanges = 0;

        if(t.elapsedTime() >= processingTimeInSeconds)
            return;

        // cout << t.elapsedTime() << endl;
	    // if(old_max_score==max_score)
	    // 	break;

	    for ( a = 0; a < conference->getSessionsInTrack ( ); a++ )
	    {
	        for ( b = 0; b < conference->getParallelTracks ( ); b++ )
	        {
	            for ( c = 0; c < conference->getPapersInSession ( ); c++ )
	            {
	                current_delta = 0;
	            	//exchange with every single element in the conference
	                //i = a because of leaving a rows above it
	            	for ( i = a; i < conference->getSessionsInTrack ( ); i++ )
				    {
				        for ( j = 0; j < conference->getParallelTracks ( ); j++ )
				        {	
			                //exchange
	                        if(a==i&&b<=j){
	                            continue;
	                	    }

				            for ( k = 0; k < conference->getPapersInSession ( ); k++ )
				            {
				                    // cout << t.elapsedTime() << endl;
	    
							// cout << "organizePapers" << endl;
					            	// int paperFromPointer = conference->getTrack( b ).getSession ( a ).getPaper ( c );
					            	// int temp = conference->getTrack( j ).getSession ( i ).getPaper ( k );
					                
					             //    conference->setPaper ( j, i, k, paperFromPointer );
					             //    conference->setPaper ( b, a, c, temp );

	                                delta = deltaScore(a,b,c,i,j,k);
	                                // cout << "Delta: " << delta << endl; 
					                // new_score = scoreOrganization();
	                                if(delta>0.000000001 && delta>=current_delta){
	                                    remember_i=i;
	                                    remember_j=j;
	                                    remember_k=k;
	                                    current_delta = delta;
	                                }
					                // if(new_score>max_score && new_score>current_max){
					                // 	remember_i=i;
					                // 	remember_j=j;
					                // 	remember_k=k;
					                // 	current_max = new_score;
					                // }

					                // conference->setPaper ( j, i, k, temp );
					                // conference->setPaper ( b, a, c, paperFromPointer );

					                // cout << remember_i << " " << remember_j << " " << remember_k << endl;
					                // cout << "New Score " << max_score << endl;

					                if(t.elapsedTime() >= processingTimeInSeconds)
					                	return;

				            }
				        }
				    }

				    if(current_delta>0){
				    	// old_max_score = max_score;
				    	// max_score += current_delta;

				    	// int paperFromPointer = conference->getTrack( b ).getSession ( a ).getPaper ( c );
		            	temp = conference->getTrack( remember_j ).getSession ( remember_i ).getPaper ( remember_k );
		                
		                conference->setPaper ( remember_j, remember_i, remember_k, conference->getTrack( b ).getSession ( a ).getPaper ( c ) );
		                conference->setPaper ( b, a, c, temp );

		                noOfExchanges++;
				    }

				}
			}
		}

		if(noOfExchanges==0)
			sessionChanger(t, processingTimeInSeconds);

    	// cl = clock() - cl;
    }

	// cout << "Time req. in seconds is " << t.elapsedTime() << endl;
}

void SessionOrganizer::swapSession( int a, int b, int i, int j )
{
	Session tmpSession1 = conference->getTrack ( b ).getSession ( a );
	conference->getTrack ( b ).setSession ( a, conference->getTrack ( j ).getSession ( i ) );
	conference->getTrack ( j ).setSession ( i, tmpSession1 );
}

double SessionOrganizer::distanceScoreSessionEval( int a, int b, int i, int j )
{
	Track tmpTrack = conference->getTrack ( j );
    Session tmpSession = tmpTrack.getSession ( i );
    double d1 = 0.0;

	//for loop for every paper
	for (int z = 0; z < tmpSession.getNumberOfPapers ( ); z++)
	{
	    int index1 = tmpSession.getPaper ( z );
	    // Get competing papers.
	    for ( int l = 0; l < conference->getParallelTracks ( ); l++ )
	    {
	        if(l!=j){
	            Track tmpTrack2 = conference->getTrack ( l );
	            Session tmpSession2 = tmpTrack2.getSession ( i );
	            for ( int m = 0; m < tmpSession2.getNumberOfPapers ( ); m++ )
	            {
	                int index2 = tmpSession2.getPaper ( m );
	                d1 += distanceMatrix[index1][index2];
	            }
	        }
	    }
		
	}

    tmpTrack = conference->getTrack ( b );
    tmpSession = tmpTrack.getSession ( a );
    double d2 = 0.0;

	//for loop for every paper
	for (int z = 0; z < tmpSession.getNumberOfPapers ( ); z++)
	{
	    int index1 = tmpSession.getPaper ( z );
	    // Get competing papers.
	    for ( int l = 0; l < conference->getParallelTracks ( ); l++ )
	    {
	        if(l!=b){
	            Track tmpTrack2 = conference->getTrack ( l );
	            Session tmpSession2 = tmpTrack2.getSession ( a );
	            for ( int m = 0; m < tmpSession2.getNumberOfPapers ( ); m++ )
	            {
	                int index2 = tmpSession2.getPaper ( m );
	                d2 += distanceMatrix[index1][index2];
	            }
	        }
	    }
		
	}

	double value = d1 + d2;
	return value;
}

double SessionOrganizer::deltaScoreSession( int a, int b, int i, int j )
{

	double before = distanceScoreSessionEval( a, b, i, j );

	swapSession( a, b, i, j );

	double after = distanceScoreSessionEval( a, b, i, j );

	return after-before;
}

void SessionOrganizer::sessionChanger( timer t, double processingTimeInSeconds )
{
	// cout << "sessionChanger";
    for ( int b = 0; b < conference->getParallelTracks ( ); b++ )
    {
    	Track tmpTrack1 = conference->getTrack ( b );
        for ( int a = 0; a < tmpTrack1.getNumberOfSessions ( ); a++ )
        {
		    for ( int j = 0; j < conference->getParallelTracks ( ); j++ )
		    {
		    	Track tmpTrack2 = conference->getTrack ( j );
		        for ( int i = a+1; i < tmpTrack2.getNumberOfSessions ( ); i++ )
		        {
		            //delta score for swap session
		            double delta = deltaScoreSession( a, b, i, j );
		            if(delta>0){
		            	swapSession( a, b, i, j );
		            	return;
		            }

		            if(t.elapsedTime() >= processingTimeInSeconds)
            			return;
		        }
		    }
		}
	}

}

void SessionOrganizer::readInInputFile ( string filename )
{
    vector<string> lines;
    string line;
    ifstream myfile ( filename.c_str () );
    if ( myfile.is_open ( ) )
    {
        while ( getline ( myfile, line ) )
        {
            //cout<<"Line read:"<<line<<endl;
            lines.push_back ( line );
        }
        myfile.close ( );
    }
    else
    {
        cout << "Unable to open input file";
        exit ( 0 );
    }

    if ( 6 > lines.size ( ) )
    {
        cout << "Not enough information given, check format of input file";
        exit ( 0 );
    }

    processingTimeInMinutes = atof ( lines[0].c_str () );
    papersInSession = atoi ( lines[1].c_str () );
    parallelTracks = atoi ( lines[2].c_str () );
    sessionsInTrack = atoi ( lines[3].c_str () );
    tradeoffCoefficient = atof ( lines[4].c_str () );

    int n = lines.size ( ) - 5;
    double ** tempDistanceMatrix = new double*[n];
    for ( int i = 0; i < n; ++i )
    {
        tempDistanceMatrix[i] = new double[n];
    }


    for ( int i = 0; i < n; i++ )
    {
        string tempLine = lines[ i + 5 ];
        string elements[n];
        splitString ( tempLine, " ", elements, n );

        for ( int j = 0; j < n; j++ )
        {
            tempDistanceMatrix[i][j] = atof ( elements[j].c_str () );
        }
    }
    distanceMatrix = tempDistanceMatrix;

    int numberOfPapers = n;
    int slots = parallelTracks * papersInSession*sessionsInTrack;
    if ( slots != numberOfPapers )
    {
        cout << "More papers than slots available! slots:" << slots << " num papers:" << numberOfPapers << endl;
        exit ( 0 );
    }
}

double** SessionOrganizer::getDistanceMatrix ( )
{
    return distanceMatrix;
}

void SessionOrganizer::printSessionOrganiser ( char * filename)
{
    conference->printConference ( filename);
}

double SessionOrganizer::deltaScore(int a, int b, int c, int i, int j, int k){
    int noOfSessions;

    //same session
    if(a==i) {
        double s1 = 0.0;
        Track tmpTrack = conference->getTrack ( j );
        Session tmpSession = tmpTrack.getSession ( i );
        int index1 = tmpSession.getPaper ( k );
        noOfSessions = tmpSession.getNumberOfPapers ( );
        for (int z = 0; z < tmpSession.getNumberOfPapers ( ); z++)
        {
            int index2 = tmpSession.getPaper ( z );
            if(z!=k)
                s1 += 1 - distanceMatrix[index1][index2];
        }

        double d1 = 0.0;
        // Get competing papers.
        for ( int l = 0; l < conference->getParallelTracks ( ); l++ )
        {
            if(l!=j){
                Track tmpTrack2 = conference->getTrack ( l );
                Session tmpSession2 = tmpTrack2.getSession ( i );
                for ( int m = 0; m < tmpSession2.getNumberOfPapers ( ); m++ )
                {
                    int index2 = tmpSession2.getPaper ( m );
                    d1 += distanceMatrix[index1][index2];
                }
            }
        }

        double s2 = 0.0;
        tmpTrack = conference->getTrack ( b );
        tmpSession = tmpTrack.getSession ( a );
        index1 = tmpSession.getPaper ( c );
        for (int z = 0; z < tmpSession.getNumberOfPapers ( ); z++)
        {
            int index2 = tmpSession.getPaper ( z );
            if(z!=c)
                s1 += 1 - distanceMatrix[index1][index2];
        }

        double d2 = 0.0;
        // Get competing papers.
        for ( int l = 0; l < conference->getParallelTracks ( ); l++ )
        {
            if(l!=b){
                Track tmpTrack2 = conference->getTrack ( l );
                Session tmpSession2 = tmpTrack2.getSession ( a );
                for ( int m = 0; m < tmpSession2.getNumberOfPapers ( ); m++ )
                {
                    int index2 = tmpSession2.getPaper ( m );
                    d2 += distanceMatrix[index1][index2];
                }
            }
        }

        double delScore = (tradeoffCoefficient+1)*(2*noOfSessions-(s1+s2+d1+d2));
        return delScore;
    }
    //different session
    double s1 = 0.0;
    Track tmpTrack = conference->getTrack ( j );
    Session tmpSession = tmpTrack.getSession ( i );
    int index1 = tmpSession.getPaper ( k );
    for (int z = 0; z < tmpSession.getNumberOfPapers ( ); z++)
    {
        int index2 = tmpSession.getPaper ( z );
        if(z!=k)
            s1 += 1 - distanceMatrix[index1][index2];
    }

    double d1 = 0.0;
    // Get competing papers.
    for ( int l = 0; l < conference->getParallelTracks ( ); l++ )
    {
        if(l!=j){
            Track tmpTrack2 = conference->getTrack ( l );
            Session tmpSession2 = tmpTrack2.getSession ( i );
            for ( int m = 0; m < tmpSession2.getNumberOfPapers ( ); m++ )
            {
                int index2 = tmpSession2.getPaper ( m );
                d1 += distanceMatrix[index1][index2];
            }
        }
    }

    double s2 = 0.0;
    tmpTrack = conference->getTrack ( b );
    tmpSession = tmpTrack.getSession ( a );
    index1 = tmpSession.getPaper ( c );
    for (int z = 0; z < tmpSession.getNumberOfPapers ( ); z++)
    {
        int index2 = tmpSession.getPaper ( z );
        if(z!=c)
            s1 += 1 - distanceMatrix[index1][index2];
    }

    double d2 = 0.0;
    // Get competing papers.
    for ( int l = 0; l < conference->getParallelTracks ( ); l++ )
    {
        if(l!=b){
            Track tmpTrack2 = conference->getTrack ( l );
            Session tmpSession2 = tmpTrack2.getSession ( a );
            for ( int m = 0; m < tmpSession2.getNumberOfPapers ( ); m++ )
            {
                int index2 = tmpSession2.getPaper ( m );
                d2 += distanceMatrix[index1][index2];
            }
        }
    }
    double before = s1+s2+tradeoffCoefficient*(d1+d2);
    
    //swap
    swap(a,b,c,i,j,k);

    s1 = 0.0;
    tmpTrack = conference->getTrack ( j );
    tmpSession = tmpTrack.getSession ( i );
    index1 = tmpSession.getPaper ( k );
    for (int z = 0; z < tmpSession.getNumberOfPapers ( ); z++)
    {
        int index2 = tmpSession.getPaper ( z );
        if(z!=k)
            s1 += 1 - distanceMatrix[index1][index2];
    }

    d1 = 0.0;
    // Get competing papers.
    for ( int l = 0; l < conference->getParallelTracks ( ); l++ )
    {
        if(l!=j){
            Track tmpTrack2 = conference->getTrack ( l );
            Session tmpSession2 = tmpTrack2.getSession ( i );
            for ( int m = 0; m < tmpSession2.getNumberOfPapers ( ); m++ )
            {
                int index2 = tmpSession2.getPaper ( m );
                d1 += distanceMatrix[index1][index2];
            }
        }
    }

    s2 = 0.0;
    tmpTrack = conference->getTrack ( b );
    tmpSession = tmpTrack.getSession ( a );
    index1 = tmpSession.getPaper ( c );
    for (int z = 0; z < tmpSession.getNumberOfPapers ( ); z++)
    {
        int index2 = tmpSession.getPaper ( z );
        if(z!=c)
            s1 += 1 - distanceMatrix[index1][index2];
    }

    d2 = 0.0;
    // Get competing papers.
    for ( int l = 0; l < conference->getParallelTracks ( ); l++ )
    {
        if(l!=b){
            Track tmpTrack2 = conference->getTrack ( l );
            Session tmpSession2 = tmpTrack2.getSession ( a );
            for ( int m = 0; m < tmpSession2.getNumberOfPapers ( ); m++ )
            {
                int index2 = tmpSession2.getPaper ( m );
                d2 += distanceMatrix[index1][index2];
            }
        }
    }

    double after = s1+s2+tradeoffCoefficient*(d1+d2);
    swap(a,b,c,i,j,k);

    return after-before;
}

double SessionOrganizer::scoreOrganization ( )
{
    // Sum of pairwise similarities per session.
    double score1 = 0.0;
    for ( int i = 0; i < conference->getParallelTracks ( ); i++ )
    {
        Track tmpTrack = conference->getTrack ( i );
        for ( int j = 0; j < tmpTrack.getNumberOfSessions ( ); j++ )
        {
            Session tmpSession = tmpTrack.getSession ( j );
            for ( int k = 0; k < tmpSession.getNumberOfPapers ( ); k++ )
            {
                int index1 = tmpSession.getPaper ( k );
                for ( int l = k + 1; l < tmpSession.getNumberOfPapers ( ); l++ )
                {
                    int index2 = tmpSession.getPaper ( l );
                    score1 += 1 - distanceMatrix[index1][index2];
                }
            }
        }
    }

    // Sum of distances for competing papers.
    double score2 = 0.0;
    for ( int i = 0; i < conference->getParallelTracks ( ); i++ )
    {
        Track tmpTrack1 = conference->getTrack ( i );
        for ( int j = 0; j < tmpTrack1.getNumberOfSessions ( ); j++ )
        {
            Session tmpSession1 = tmpTrack1.getSession ( j );
            for ( int k = 0; k < tmpSession1.getNumberOfPapers ( ); k++ )
            {
                int index1 = tmpSession1.getPaper ( k );

                // Get competing papers.
                for ( int l = i + 1; l < conference->getParallelTracks ( ); l++ )
                {
                    Track tmpTrack2 = conference->getTrack ( l );
                    Session tmpSession2 = tmpTrack2.getSession ( j );
                    for ( int m = 0; m < tmpSession2.getNumberOfPapers ( ); m++ )
                    {
                        int index2 = tmpSession2.getPaper ( m );
                        score2 += distanceMatrix[index1][index2];
                    }
                }
            }
        }
    }
    double score = score1 + tradeoffCoefficient*score2;
    return score;
}
