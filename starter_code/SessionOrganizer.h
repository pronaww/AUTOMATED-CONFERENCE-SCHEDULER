/* 
 * File:   SessionOrganizer.h
 * Author: Kapil Thakkar
 *
 */

#ifndef SESSIONORGANIZER_H
#define	SESSIONORGANIZER_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include <time.h>

#include "Conference.h"
#include "Track.h"
#include "Session.h"

using namespace std;

class timer {
    private:
        //double begTime;
	struct timespec begin;
	struct timespec end;
	double elapsedSeconds;
    public:
        void start() {
            //begTime = clock();
	    clock_gettime(CLOCK_MONOTONIC, &begin);
        }

        double elapsedTime() {
	    clock_gettime(CLOCK_MONOTONIC, &end);
	    elapsedSeconds = TimeSpec(&end)-TimeSpec(&begin);
	    return elapsedSeconds;
            //return ((double) clock() - begTime) / (double) CLOCKS_PER_SEC;
        }

        bool isTimeout(double seconds) {
            return seconds >= elapsedTime();
        }

	static double TimeSpec(struct timespec* ts) {
	    return (double)ts->tv_sec + (double)ts->tv_nsec / 1000000000.0;
	}
};


/**
 * SessionOrganizer reads in a similarity matrix of papers, and organizes them
 * into sessions and tracks.
 * 
 * @author Kapil Thakkar
 *
 */
class SessionOrganizer {
private:
    double ** distanceMatrix;

    int parallelTracks ;
    int papersInSession ;
    int sessionsInTrack ;

    Conference *conference;

    double processingTimeInMinutes ;
    double tradeoffCoefficient ; // the tradeoff coefficient


public:
    SessionOrganizer();
    SessionOrganizer(string filename);
    
    
    /**
     * Read in the number of parallel tracks, papers in session, sessions
     * in a track, and the similarity matrix from the specified filename.
     * @param filename is the name of the file containing the matrix.
     * @return the similarity matrix.
     */
    void readInInputFile(string filename);
    
    
    
    /**
     * Organize the papers according to some algorithm.
     */
    void organizePapers(timer);
    
    
    /**
     * Get the distance matrix.
     * @return the distance matrix.
     */
    double** getDistanceMatrix();
    
    
    /**
     * Score the organization.
     * @return the score.
     */
    double scoreOrganization();
    
    
    void printSessionOrganiser(char *);

    //new defined functions
    void swap(int, int, int, int, int, int);
    double deltaScore(int, int, int, int, int, int);
    void swapSession(int, int, int, int);
    void sessionChanger(timer, double);
    double deltaScoreSession(int, int, int, int);
    double distanceScoreSessionEval(int, int, int, int);
};

#endif	/* SESSIONORGANIZER_H */

