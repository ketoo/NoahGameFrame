/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2020 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is open-source software and you can redistribute it and/or modify
   it under the terms of the License; besides, anyone who use this file/software must include this copyright announcement.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/



#ifndef NFI_ELO_MODULE_H
#define NFI_ELO_MODULE_H

#include <iostream>
#include "NFIModule.h"


class NFIELOModule
    : public NFIModule
{
public:
	// To calculate the Probability that B win A
	virtual float Probability(int ratingA, int ratingB) = 0;

	// To calculate Elo rating
	// aWin determines whether Player A wins or loses.
	//rA is the result for A, if A won B, rA > 0, else rA < 0
	//rB is the result for B, if B won A, rB > 0, else rB < 0
	//for example:
	/*
	 	int ratingA = 100;
		int ratingB = 160;

		float probability = Probability(ratingA, ratingB);

		int resultA = 0;
		int resultB = 0;
		EloRating(ratingA, ratingB, true, resultA, resultB);

	 	result:
	 	probability = 0.585;
	 	resultA = 29;
	 	resultB = -29;

		EloRating(ratingA, ratingB, false, resultA, resultB);
	 	resultA = -20;
	 	resultB = 20;
	 */
	virtual void EloRating(int ratingA, int ratingB, bool aWin, int& resultA, int& resultB) = 0;
};

#endif