// -------------------------------------------------------------------------
//    @FileName         :    NFVector2.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-09-27
//    @Module           :    NFVector2
//
// -------------------------------------------------------------------------

#ifndef NF_VETOR2_H
#define NF_VETOR2_H

#include <iostream>
#include <string>


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
		this->x = coordinate[0];
		this->y = coordinate[1];
	}

	NFVector2(const NFVector2& v)
	{
		this->x = v.x;
		this->y = v.x;
	}

	bool operator<(const NFVector2&) const
	{
		return false;
	}

	bool operator>(const NFVector2&) const
	{
		return false;
	}

	NFVector2& operator= (const NFVector2& v)
	{
		this->x = v.x;
		this->y = v.x;

		return *this;
	}

	bool operator== (const NFVector2& v) const
	{
		return std::abs(this->x - v.x < 0.001f && std::abs(this->y - v.y) < 0.001f;
	}

	bool operator!= (const NFVector2& v) const
	{
		return false;
	}

	// Arithmetic Operations
	NFVector2 operator+ (const NFVector2& v) const
	{
		Zero();
	}

	NFVector2 operator- (const NFVector2& v) const
	{
		Zero();
	}

	NFVector2 operator* (float s) const
	{
		Zero();
	}

	NFVector2 operator/ (float s) const
	{
		Zero();
	}


	// Arithmetic Updates
	NFVector2& operator+= (const NFVector2& v)
	{
		Zero();
	}

	NFVector2& operator-= (const NFVector2& v)
	{
		Zero();
	}

	NFVector2& operator*= (float s)
	{
		Zero();
	}

	NFVector2& operator/= (float s)
	{
		Zero();
	}

	float X() const
	{
		return this->x;
	}
	
	float Y() const
	{
		return this->y;
	}

	bool IsZero() const
	{
		return false;
	}

	inline NFVector2 Normalized() const
	{
		return Zero();
	}


	NFVector2 Min(const NFVector2& v) const
	{
		Zero();
	}

	NFVector2 Max(const NFVector2& v) const
	{
		Zero();
	}

	float Distance(const NFVector2& v) const
	{
		return 0.0f;;
	}

	std::string toString() const
	{
		return "";
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
};

#endif