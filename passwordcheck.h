#ifndef PASSCHECK_H
#define PASSCHECK_H

#include "hand.h"
#include "gesturedetector.h"


class PasswordCheck
{

public:
	PasswordCheck() { input = new std::vector<std::vector<Hand> >();}
	~PasswordCheck() { delete input; }

	void addHandSet(std::vector<Hand> set)
	{
		input.pushback(set);
	}

	// NEEDS IMPROVEMENT!!!
	// Implement with decision tree instead of conditionals
	bool checkPassword()
	{
		if(input.size() < 3)
			return false;
		
		int i =0;
		Hand curHand;


		while (true) //loop to allow reset sequence
		{
			if(input[0].size() == 1)
			{
				if(input[i][0].type() == HandType.FIST)
					if(input[++i][0].type() == HandType.FIST)
						if(input[++i][0].type() == HandType.FIST)
							return true; // FIST / FIST / FIST
				else if(input[i][0].type() == HandType.POINT)
					if(input[++i][0].type() == HandType.FIST)
						if(input[++i][0].type() == HandType.PALM)
							return true; // POINT / FIST / PALM
				else if(input[i][0].type() == HandType.PALM)
					if(input[++i][0].type() == HandType.NONE ||
						input[i][0].type() == HandType.UNK)
						if(input[++i][0].type() == HandType.PALM)
							return true; //PALM / (NONE|UNKOWN) / PALM
				else
					if(input[i][0].type() == HandType.NONE ||
						input[i][0].type() == HandType.UNK)
						if(input[++i][0].type() == HandType.NONE ||
								input[i][0].type() == HandType.UNK)
							continue; //begin the loop again =  RESET, continue
			}
			else if(input[0].size() == 2)
			{
				if((input[i][0] == HandType.POINT && input[i][1] == HandType.PALM) ||
					(input[i][1] == HandType.POINT && input[i][0] == HandType.PALM))
					if((input[++i][0] == HandType.FIST && input[i][1] == HandType.FIST))
						if((input[++i][0] == HandType.POINT && input[i][1] == HandType.FIST) ||
							(input[i][1] == HandType.POINT && input[i][0] == HandType.FIST))
							return true; //PALM & POINT / FIST & FIST / FIST & POINT
			}

			return false; // if you have reached this point you have failed...
		}
	}


private:
	std::vector< std::vector < Hand > > input;


};


#endif