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

#ifndef NF_VETOR3_H
#define NF_VETOR3_H

#include <iostream>
#include <string>
#include "NFVector2.hpp"

class NFLine;
class NFBox;
class NFPlane;

class NFVector3
{
private:
	float x, y, z;
	void InitData()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

public:
	// construction
	NFVector3()
	{
		InitData();
	}

	NFVector3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	NFVector3(const NFVector2& v, float z)
	{
		this->x = v.X();
		this->y = v.Y();
		this->z = z;
	}

	NFVector3(float coordinate[3])
	{
		this->x = coordinate[0];
		this->y = coordinate[1];
		this->z = coordinate[2];
	}

	NFVector3(double coordinate[3])
	{
		this->x = (float) coordinate[0];
		this->y = (float) coordinate[1];
		this->z = (float) coordinate[2];
	}

	NFVector3(const NFVector3& v)
	{
		this->x = v.x;
		this->y = v.y;
		this->z = v.z;
	}
	NFVector3(const char* v, const int len)
	{
		FromString(std::string(v,  len));
	}
	NFVector3(const std::string& v)
	{
		FromString(v);
	}
	//----------------------------------------------------------------------------
	bool operator<(const NFVector3& v) const
	{
		return this->Length() < v.Length() ;
	}

	bool operator>(const NFVector3& v) const
	{
		return this->Length() > v.Length();
	}

	NFVector3& operator= (const NFVector3& v)
	{
		this->x = v.x;
		this->y = v.y;
		this->z = v.z;

		return *this;
	}

	bool operator== (const NFVector3& v) const
	{
		return std::abs(this->x - v.x) < 0.001f && std::abs(this->y - v.y) < 0.001f && std::abs(this->z - v.z) < 0.001f;
	}

	bool operator!= (const NFVector3& v) const
	{
		return std::abs(this->x - v.x) >= 0.001f || std::abs(this->y - v.y) >= 0.001f || std::abs(this->z - v.z) >= 0.001f;
	}

	//----------------------------------------------------------------------------
    // Arithmetic Operations
	NFVector3 operator+ (const NFVector3& v) const
	{
		NFVector3 xV;

		xV.x = this->x + v.x;
		xV.y = this->y + v.y;
		xV.z = this->z + v.z;
		return xV;
	}

	NFVector3 operator- (const NFVector3& v) const
	{
		NFVector3 xV;

		xV.x = this->x - v.x;
		xV.y = this->y - v.y;
		xV.z = this->z - v.z;
		return xV;
	}

	NFVector3 operator- () const 
	{
		return NFVector3(-x, -y, -z);
	}

	NFVector3 operator* (float s) const
	{
		return NFVector3(x * s, y * s, z * s);
	}

	NFVector3 operator/ (float s) const
	{
		if (std::abs(s) > 0.001f)
		{
			return NFVector3(x / s, y / s, z / s);
		}

		return Zero();
	}


	//----------------------------------------------------------------------------
    // Arithmetic Updates
	NFVector3& operator+= (const NFVector3& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	NFVector3& operator-= (const NFVector3& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	NFVector3& operator*= (float s)
	{
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}

	NFVector3 operator/= (float s)
	{
		//if (std::abs(s) > 0.001f)
		{
			return NFVector3(x / s, y / s, z / s);
		}

		//return Zero();
	}

	//----------------------------------------------------------------------------
	float X() const
	{
		return this->x;
	}

	float Y() const
	{
		return this->y;
	}

	float Z() const
	{
		return this->z;
	}

	void SetX(float x)
	{
		this->x = x;
	}

	void SetY(float y)
	{
		this->y = y;
	}

	void SetZ(float z)
	{
		this->z = z;
	}

	//----------------------------------------------------------------------------
	bool IsZero() const
	{
		return std::abs(x) < 0.001f && std::abs(y) < 0.001f && std::abs(z) < 0.001f;
	}
	//----------------------------------------------------------------------------
	inline float SquaredMagnitude() const
	{
		return x*x + y*y + z*z;
	}

	//----------------------------------------------------------------------------
	inline float SquaredLength() const 
	{
		return SquaredMagnitude();
	}

	//----------------------------------------------------------------------------
	inline float Magnitude() const 
	{
		return sqrtf(x*x + y*y + z*z);
	}

	//----------------------------------------------------------------------------
	inline float Length() const 
	{
		return Magnitude();
	}

	//----------------------------------------------------------------------------
	inline NFVector3 Direction() const 
	{
		if (this->IsZero())
		{
			return Zero();
		}

		float lenSquared = SquaredMagnitude();
		float invSqrt = 1.0f / sqrtf(lenSquared);
		return NFVector3(x * invSqrt, y * invSqrt, z * invSqrt);
	}

	//----------------------------------------------------------------------------
    inline NFVector3 Normalized() const
    {
        return Direction();
    }

	//----------------------------------------------------------------------------
	float Distance(const NFVector3& v) const
	{
		NFVector3 vX = *this - v;
		return vX.Length();
	}

	//----------------------------------------------------------------------------
	bool FromString(const std::string& value)
	{
		std::vector<std::string> values;
		Split(value, values, ",");
		if (values.size() != 3)
		{
			return false;
		}
		try
		{
			x = lexical_cast<float>(values.at(0));
			y = lexical_cast<float>(values.at(1));
			z = lexical_cast<float>(values.at(2));

			return true;
		}
		catch (const std::exception&)
		{

		}

		return false;
	}

	//----------------------------------------------------------------------------
	std::string ToString() const
	{
		return lexical_cast<std::string>(x) + "," + lexical_cast<std::string>(y) + "," + lexical_cast<std::string>(z);
	}

    // Special values.
    inline static const NFVector3& Zero()
    {
        static NFVector3 v(0, 0, 0);
        return v;
    }
    inline static const NFVector3& One()
    {
        static NFVector3 v(1, 1, 1);
        return v;
    }
    inline static const NFVector3& UnitX()
    {
        static NFVector3 v(1, 0, 0);
        return v;
    }
    inline static const NFVector3& UnitY()
    {
        static NFVector3 v(0, 1, 0);
        return v;
    }
	inline static const NFVector3& UnitZ()
	{
		static NFVector3 v(0, 0, 1);
		return v;
	}

	NFVector2 xx() const
	{
		return NFVector2(this->x, this->x);
	}

	NFVector2 yx() const
	{
		return NFVector2(this->y, this->x);
	}

	NFVector2 zx() const
	{
		return NFVector2(this->z, this->x);
	}

	NFVector2 xy() const
	{
		return NFVector2(this->x, this->y);
	}

	NFVector2 yy() const
	{
		return NFVector2(this->y, this->y);
	}

	NFVector2 zy() const
	{
		return NFVector2(this->z, this->y);
	}

	NFVector2 xz() const
	{
		return NFVector2(this->x, this->z);
	}

	NFVector2 yz() const
	{
		return NFVector2(this->y, this->z);
	}

	NFVector2 zz() const
	{
		return NFVector2(this->z, this->z);
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