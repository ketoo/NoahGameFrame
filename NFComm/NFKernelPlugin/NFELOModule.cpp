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

#include "NFELOModule.h"

bool NFELOModule::Init()
{

    return true;
}


bool NFELOModule::Shut()
{
    return true;
}

bool NFELOModule::Execute()
{
    return true;
}

bool NFELOModule::AfterInit()
{
	int ratingA = 100;
	int ratingB = 160;

	float probability = Probability(ratingA, ratingB);

	int resultA = 0;
	int resultB = 0;

	EloRating(ratingA, ratingB, true, resultA, resultB);

	EloRating(ratingA, ratingB, false, resultA, resultB);

    return true;
}

// Function to calculate the Probability
float NFELOModule::Probability(int ratingA, int ratingB)
{
	return 1.0 * 1.0 / (1 + 1.0 * pow(10, 1.0 * (ratingA - ratingB) / 400));
}

// Function to calculate Elo rating
// K is a constant.
// d determines whether Player A wins or Player B.
void NFELOModule::EloRating(int ratingA, int ratingB, bool aWin, int& resultA, int& resultB)
{
	resultA = 0;
	resultB = 0;

	// To calculate the Winning
	// Probability of Player B
	float Pb = Probability(ratingA, ratingB);

	// To calculate the Winning
	// Probability of Player A
	float Pa = Probability(ratingB, ratingA);

	// Case -1 When Player A wins
	// Updating the Elo Ratings
	if (aWin == 1)
	{
		resultA = EloK() * (1 - Pa);
		resultB = EloK() * (0 - Pb);
	}
	else
	{
		// Case -2 When Player B wins
		// Updating the Elo Ratings

		resultA = EloK() * (0 - Pa);
		resultB = EloK() * (1 - Pb);
	}
}

int NFELOModule::EloK()
{
	return K;
}