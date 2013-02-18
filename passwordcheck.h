#ifndef PASSCHECK_H
#define PASSCHECK_H

#include "hand.h"
#include "gesturedetector.h"
#include <iostream>


class PasswordCheck
{

public:
    PasswordCheck() { }

	void addHandSet(std::vector<Hand> set)
	{
        for(unsigned int i = 0; i < set.size() ; i++)
			std::cout << "adding: " << set[i].type << "\t";
        input.push_back(set);
	}

	void reset()
	{
		input.clear();
		std::cout << "clearing input.\n";
	}

	// NEEDS IMPROVEMENT!!!
	// Implement with decision tree instead of conditionals
	bool checkPassword()
    {
		
        int i = -1;
		Hand curHand;
        std::cout << "beginning yo mama check...\n";

        std::cout << "array size:" << input.size();

		while (true) //loop to allow reset sequence
		{
            if(input.size() - ++i < 3) //check for too few items
                return false;

			if(input[0].size() == 1)
			{
				if(input[i][0].type == FIST)
				{
					std::cout << "level 1\n";
					if(input[++i][0].type == FIST)
						if(input[++i][0].type == FIST)
							return true; // FIST / FIST / FIST
				}
				else if(input[i][0].type == POINT)
				{
					std::cout << "level 2\n";
					if(input[++i][0].type == FIST)
						if(input[++i][0].type == PALM)
							return true; // POINT / FIST / PALM
				}
				else if(input[i][0].type == PALM)
				{
					std::cout << "level 3\n";
					if(input[++i][0].type == NONE ||
						input[i][0].type == UNK)
						if(input[++i][0].type == PALM)
							return true; //PALM / (NONE|UNKOWN) / PALM
				}
				else
				{
					std::cout << "level 4\n";
                    if(input[++i][0].type == NONE ||
						input[i][0].type == UNK)
						if(input[++i][0].type == NONE ||
								input[i][0].type == UNK)
							continue; //begin the loop again =  RESET, continue
				}
                for(unsigned int j = 0; j < input.size() ; j++)
                    std::cout << "element: " << input[j][0].type << "\t";
			}
			else if(input[0].size() == 2)
			{
				if((input[i][0].type == POINT && input[i][1].type == PALM) ||
					(input[i][1].type == POINT && input[i][0].type == PALM))
					if((input[++i][0].type == FIST && input[i][1].type == FIST))
						if((input[++i][0].type == POINT && input[i][1].type == FIST) ||
							(input[i][1].type == POINT && input[i][0].type == FIST))
							return true; //PALM & POINT / FIST & FIST / FIST & POINT
			}

			return false; // if you have reached this point you have failed...
		}
	}


private:
	std::vector< std::vector < Hand > > input;


};


#endif
