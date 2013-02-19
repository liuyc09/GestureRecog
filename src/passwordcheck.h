/*------------------------------------------------------------------------------------------*\
    Created by: Jason Carlisle Mann (on2valhalla | jcm2207@columbia.edu)

    This class supports the GesturePassword program, by analyzing sequences of 
    hand gestures provided one set at a time, and checking them against predetermined
    passwords. Gestures are defined by the Hand class

    The currently valid sequences are:
    1.  FIST | FIST | FIST
    2.  POINT | FIST | PALM
    3.  PALM | (NONE OR UNK) | PALM
    4.  (PALM & POINT) | (FIST & FIST) | (POINT & FIST)

    Also if the first three gestures are NONE or UNKOWN, the checking continues on the above,
    ignoring those inputs.

\*------------------------------------------------------------------------------------------*/


#ifndef PASSCHECK_H
#define PASSCHECK_H

#include "../include/hand.h"
#include "../src/gesturedetector.h"
#include <iostream>


class PasswordCheck
{

public:
	//default constructor
    PasswordCheck() { }

	/*
	    add a set of hands to the sequence (currently the 
	    grammar only supports up to two hands in one capture)
		@set the parameter is the vector of captures.
	*/
	void addHandSet(const std::vector<Hand> &set)
	{
        for(unsigned int i = 0; i < set.size() ; i++)
			std::cout << "adding: " << set[i].type << "\t";
        input.push_back(set);
	}

	/*
	    clears the current sequence
	*/
	void reset()
	{
		input.clear();
	}

	/*
		This function determines whether or not it is time to 
		check the stored sequence against the password grammar

		currently 2 palms, or 6 items begins the check
	*/
	bool doCheck(std::vector<Hand> &hands)
	{
		if(input.size() >= 6 ||
	    		(hands.size() == 2 && hands[0].type == PALM 
	    		&& hands[1].type == PALM))
	    	return true;
	    else
	    	return false;
	}


	/*
	    This method checks the current sequence stored
	    against a tree of conditional statements that represent
	    the password grammar hardcoded into the Class.
	    This method could be expanded into using decision trees,
	    but with only these few sequences, that is unnecessary.

	    @return returns true if the sequence matched a password, 
	    else false
	*/
	bool checkPassword()
    {
		if(input.back()[0].type == PALM && input.back()[1].type == PALM)
			input.pop_back();

        int i = -1; //start one low to allow addition early in loop
		Hand curHand;

		while (true) //loop to allow reset sequence
		{
            if(input.size() - ++i < 3) //check for too few items
                return false;

			if(input[0].size() == 1)
			{
				if(input[i][0].type == FIST){
					if(input[++i][0].type == FIST){
						if(input[++i][0].type == FIST){
							if(input.size() - i == 1){
								return true; // FIST / FIST / FIST
							}}}}
				else if(input[i][0].type == POINT){
					if(input[++i][0].type == FIST){
						if(input[++i][0].type == PALM){
							if(input.size() - i == 1){
								return true; // POINT / FIST / PALM
							}}}}
				else if(input[i][0].type == PALM){
					if(input[++i][0].type == NONE ||
						input[i][0].type == UNK){
						if(input[++i][0].type == PALM){
							if(input.size() - i == 1){
								return true; //PALM / (NONE|UNKOWN) / PALM
							}}}}
				else {
                    if(input[++i][0].type == NONE ||
						input[i][0].type == UNK){
						if(input[++i][0].type == NONE ||
								input[i][0].type == UNK){
							continue; //begin the loop again, continue
							}}}			//searching with the next element
			}
			else if(input[0].size() == 2)
			{
				if((input[i][0].type == POINT && input[i][1].type == PALM) ||
					(input[i][1].type == POINT && input[i][0].type == PALM)){
					if((input[++i][0].type == FIST && input[i][1].type == FIST)){
						if((input[++i][0].type == POINT && input[i][1].type == FIST) ||
							(input[i][1].type == POINT && input[i][0].type == FIST)){
							if(input.size() - i == 1){
								return true; //PALM & POINT / FIST & FIST / FIST & POINT
							}}}}
			}

			return false; // if you have reached this point you have failed to find the
							//correct passphrase
		}
	}


private:
	std::vector< std::vector < Hand > > input; //the sequence of hand gestures (up to 2 in one capture)


};


#endif
