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
		return std::abs(this->x - v.x) < 0.001f && std::abs(this->y - v.y) < 0.001f;
	}

	bool operator!= (const NFVector2& v) const
	{
		return std::abs(this->x - v.x) >= 0.001f || std::abs(this->y - v.y) >= 0.001f;
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

	bool FromString(const std::string& value)
	{
		std::vector<std::string> values;
		Split(value, values, ",");
		if (values.size() != 2)
		{
			return false;
		}
		x = lexical_cast<float>(values.at(0));
		y = lexical_cast<float>(values.at(1));

		return true;
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