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

#ifndef NF_VETOR2_H
#define NF_VETOR2_H

#include <iostream>
#include <string>
#include <vector>
#include "NFComm/NFPluginModule/NFPlatform.h"

class NFLine;
class NFBox;
class NFPlane;

class NFVector2
{
private:
	float x, y;
	void InitData()
	{
		x = 0.0f;
		y = 0.0f;
	}

public:
	// construction
	NFVector2()
	{
		InitData();
	}

	NFVector2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	NFVector2(float coordinate[2])
	{
		this->x = coordinate[0];
		this->y = coordinate[1];
	}

	NFVector2(double coordinate[2])
	{
		this->x = (float) coordinate[0];
		this->y = (float) coordinate[1];
	}

	NFVector2(const NFVector2& v)
	{
		this->x = v.x;
		this->y = v.y;
	}
	
	NFVector2(const std::string& v)
	{
		FromString(v);
	}

	bool operator<(const NFVector2& v) const
	{
		return this->Length() < v.Length();
	}

	bool operator>(const NFVector2& v) const
	{
		return this->Length() > v.Length();
	}

	NFVector2& operator= (const NFVector2& v)
	{
		this->x = v.x;
		this->y = v.y;

		return *this;
	}

	bool operator== (const NFVector2& v) const
	{
		return std::abs(this->x - v.x) < 0.001f && std::abs(this->y - v.y) < 0.001f;
	}

	bool operator!= (const NFVector2& v) const
	{
		return std::abs(this->x - v.x) >= 0.001f || std::abs(this->y - v.y) >= 0.001f;
	}
	//----------------------------------------------------------------------------
	// Arithmetic Operations
	NFVector2 operator+ (const NFVector2& v) const
	{
		NFVector2 xV;

		xV.x = this->x + v.x;
		xV.y = this->y + v.y;
		return xV;
	}

	NFVector2 operator- (const NFVector2& v) const
	{
		NFVector2 xV;

		xV.x = this->x - v.x;
		xV.y = this->y - v.y;
		return xV;
	}

	NFVector2 operator- () const
	{
		return NFVector2(-x, -y);
	}

	NFVector2 operator* (float s) const
	{
		return NFVector2(x * s, y * s);
	}

	NFVector2 operator/ (float s) const
	{
		if (std::abs(s) > 0.001f)
		{
			return NFVector2(x / s, y / s);
		}

		return Zero();
	}

	//----------------------------------------------------------------------------
	// Arithmetic Updates
	NFVector2& operator+= (const NFVector2& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	NFVector2& operator-= (const NFVector2 v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	NFVector2& operator*= (float s)
	{
		x *= s;
		y *= s;
		return *this;
	}

	NFVector2 operator/= (float s)
	{
		//if (std::abs(s) > 0.001f)
		{
			return NFVector2(x / s, y / s);
		}

		//return Zero();
	}

	float X() const
	{
		return this->x;
	}
	
	float Y() const
	{
		return this->y;
	}

	void SetX(float x)
	{
		this->x = x;
	}

	void SetY(float y)
	{
		this->y = y;
	}

	bool IsZero() const
	{
		return x < 0.001f && y < 0.001f;
	}

	//----------------------------------------------------------------------------
	inline float SquaredMagnitude() const
	{
		return x*x + y*y;
	}

	//----------------------------------------------------------------------------
	inline float SquaredLength() const
	{
		return SquaredMagnitude();
	}

	//----------------------------------------------------------------------------
	inline float Magnitude() const
	{
		return sqrtf(x*x + y*y);
	}

	//----------------------------------------------------------------------------
	inline float Length() const
	{
		return Magnitude();
	}

	//----------------------------------------------------------------------------
	inline NFVector2 Direction() const
	{
		if (this->IsZero())
		{
			return Zero();
		}

		float lenSquared = SquaredMagnitude();
		float invSqrt = 1.0f / sqrtf(lenSquared);
		return NFVector2(x * invSqrt, y * invSqrt);
	}

	//----------------------------------------------------------------------------
	inline NFVector2 Normalized() const
	{
		return Direction();
	}

	//----------------------------------------------------------------------------
	float Distance(const NFVector2& v) const
	{
		NFVector2 vX = *this - v;
		return vX.Length();
	}


	bool FromString(const std::string& value)
	{
		std::vector<std::string> values;
		Split(value, values, ",");
		if (values.size() != 2)
		{
			return false;
		}

		try
		{
			x = lexical_cast<float>(values.at(0));
			y = lexical_cast<float>(values.at(1));

			return true;
		}
		catch (const std::exception&)
		{

		}

		return false;
	}

	std::string ToString() const
	{
		return lexical_cast<std::string>(x) + "," + lexical_cast<std::string>(this->y);
	}

	// Special values.
	inline static const NFVector2& Zero()
	{
		static NFVector2 v(0, 0);
		return v;
	}

	inline static const NFVector2& One()
	{
		static NFVector2 v(1, 1);
		return v;
	}

	inline static const NFVector2& UnitX()
	{
		static NFVector2 v(1, 0);
		return v;
	}

	inline static const NFVector2& UnitY()
	{
		static NFVector2 v(0, 1);
		return v;
	}

	bool Split(const std::string& str, std::vector<std::string>& result, std::string delim)
	{
		if (str.empty())
		{
			return false;
		}

		std::string tmp;
		size_t pos_begin = str.find_first_not_of(delim);
		size_t pos = 0;
		while (pos_begin != std::string::npos)
		{
			pos = str.find(delim, pos_begin);
			if (pos != std::string::npos)
			{
				tmp = str.substr(pos_begin, pos - pos_begin);
				pos_begin = pos + delim.length();
			}
			else
			{
				tmp = str.substr(pos_begin);
				pos_begin = pos;
			}

			if (!tmp.empty())
			{
				result.push_back(tmp);
				tmp.clear();
			}
		}
		return true;
	}
};

#endif