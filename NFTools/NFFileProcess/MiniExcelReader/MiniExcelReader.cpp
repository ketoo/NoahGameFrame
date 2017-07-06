#include "MiniExcelReader.h"
#include <iostream>
#include <cmath>
#include "unzip.h"
#include <stdio.h>
#include <string>
#include <vector>
#include <map>

#ifndef PATH_MAX
#define PATH_MAX 260
#endif

namespace MiniExcelReader {


	struct ZipEntryInfo
	{
		unz_file_pos pos;
		uLong uncompressed_size;
	};

	class Zip
	{
	public:
		~Zip();

		bool open(const char* file);
		bool openXML(const char* filename, rapidxml::xml_document<>& doc);

	private:
		char* getFileData(const char* filename, unsigned long& size);
		std::map<std::string, ZipEntryInfo> _files;
		unzFile _zipFile;
	};

	Zip::~Zip()
	{
		unzClose(_zipFile);
	}

	bool Zip::open(const char* file)
	{
		_zipFile = unzOpen(file);

		if (!_zipFile)
			return false;

		char szCurrentFileName[PATH_MAX];
		unz_file_info64 fileInfo;

		int err = unzGoToNextFile2(_zipFile, &fileInfo,
			szCurrentFileName, sizeof(szCurrentFileName) - 1, nullptr, 0, nullptr, 0);
		while (err == UNZ_OK)
		{
			unz_file_pos posInfo;
			if (unzGetFilePos(_zipFile, &posInfo) == UNZ_OK)
			{
				std::string currentFileName = szCurrentFileName;

				ZipEntryInfo entry;
				entry.pos = posInfo;
				entry.uncompressed_size = (uLong)fileInfo.uncompressed_size;
				_files[currentFileName] = entry;
			}
			err = unzGoToNextFile2(_zipFile, &fileInfo,
				szCurrentFileName, sizeof(szCurrentFileName) - 1, nullptr, 0, nullptr, 0);
		}

		return true;
	}

	char* Zip::getFileData(const char* filename, unsigned long& size)
	{
		char * pBuffer = NULL;

		auto it = _files.find(filename);

		if (it == _files.end()) return NULL;

		ZipEntryInfo fileInfo = it->second;

		int nRet = unzGoToFilePos(_zipFile, &fileInfo.pos);
		if (UNZ_OK != nRet) return NULL;

		nRet = unzOpenCurrentFile(_zipFile);
		if (UNZ_OK != nRet) return NULL;

		pBuffer = new char[fileInfo.uncompressed_size];
		unzReadCurrentFile(_zipFile, pBuffer, fileInfo.uncompressed_size);

		size = fileInfo.uncompressed_size;
		unzCloseCurrentFile(_zipFile);

		return pBuffer;
	}

	bool Zip::openXML(const char* filename, rapidxml::xml_document<>& doc)
	{
		unsigned long size = 0;
		char* data = getFileData(filename, size);

		if (!data) return false;

		char* pData = new char[size + 1];
		strncpy(pData, data, size);
		pData[size] = 0;
		doc.parse<0>(pData);

		if (data)
			delete[] data;
		//if (pData)
		//	delete[] pData;

		return true;
	}


	Sheet::~Sheet()
	{
		for (unsigned i = 0; i < _cells.size(); i++)
		{
			if (_cells[i])
			{
				delete _cells[i];
			}
		}
		_cells.clear();
	}

	Cell* Sheet::getCell(int row, int col)
	{
		if (row < _dimension.firstRow || row > _dimension.lastRow)
			return nullptr;
		if (col < _dimension.firstCol || col > _dimension.lastCol)
			return nullptr;

		return _cells[toIndex(row, col)];
	}


	int Sheet::toIndex(int row, int col)
	{
		return (row - 1) * (_dimension.lastCol - _dimension.firstCol + 1) + (col - _dimension.firstCol);
	}

	void ExcelFile::readWorkBook(const char* filename)
	{
		rapidxml::xml_document<> doc;

		_zip->openXML(filename, doc);

		rapidxml::xml_node<>* e = doc.first_node("workbook");
		e = e->first_node("sheets");
		e = e->first_node("sheet");

		while (e)
		{
			Sheet s;

			s._name = e->first_attribute("name")->value();
			s._rid = e->first_attribute("r:id")->value();
			s._sheetId = lexical_cast<int>(e->first_attribute("sheetId")->value());
			s._visible = (e->first_attribute("state") && !strcmp(e->first_attribute("state")->value(), "hidden"));

			e = e->next_sibling("sheet");

			_sheets.push_back(s);
		}
	}

	void ExcelFile::readWorkBookRels(const char* filename)
	{
		rapidxml::xml_document<> doc;

		_zip->openXML(filename, doc);
		rapidxml::xml_node<>* e = doc.first_node("Relationships");
		e = e->first_node("Relationship");

		while (e)
		{
			const char* rid = e->first_attribute("Id")->value();

			for (Sheet& sheet : _sheets)
			{
				if (sheet._rid == rid)
				{
					sheet._path = "xl/" + std::string(e->first_attribute("Target")->value());

					break;
				}
			}

			e = e->next_sibling("Relationship");
		}
	}

	void ExcelFile::readSharedStrings(const char* filename)
	{
		rapidxml::xml_document<> doc;

		if (!_zip->openXML(filename, doc)) return;

		rapidxml::xml_node<>* e = doc.first_node("sst");
		e = e->first_node("si");

		rapidxml::xml_node<> *t, *r;
		int i = 0;

		while (e)
		{
			t = e->first_node("t");
			i++;
			if (t)
			{
				const char* text = t->value();
				_sharedString.push_back(text ? text : "");
			}
			else
			{
				r = e->first_node("r");
				std::string value;
				while (r)
				{
					t = r->first_node("t");
					value += t->value();
					r = r->next_sibling("r");
				}
				_sharedString.push_back(value);
			}
			e = e->next_sibling("si");
		}
	}

	void ExcelFile::readStyles(const char* filename)
	{
		rapidxml::xml_document<> doc;

		_zip->openXML(filename, doc);
	}

	void ExcelFile::parseCell(const std::string& value, int& row, int& col)
	{
		int index = 0;
		col = 0;

		int arr[10];

		while (index < (int)value.length())
		{
			if (isdigit(value[index])) break;
			arr[index] = value[index] - 'A' + 1;
			index++;
		}

		for (int i = 0; i < index; i++)
		{
			col += (int)(arr[i] * pow(26, index - i - 1));
		}

		row = atoi(value.c_str() + index);
	}

	void ExcelFile::parseRange(const std::string& value, Range& range)
	{
		size_t index = value.find_first_of(':');

		if (index != std::string::npos)
		{
			parseCell(value.substr(0, index), range.firstRow, range.firstCol);
			parseCell(value.substr(index + 1), range.lastRow, range.lastCol);
		}
		else
		{
			parseCell(value, range.firstRow, range.firstCol);
			range.lastCol = range.firstCol;
			range.lastRow = range.firstRow;
		}
	}

	void ExcelFile::readSheet(Sheet& sh)
	{
		rapidxml::xml_document<> doc;
		rapidxml::xml_node<> *root, *row, *c, *v, *d;

		_zip->openXML(sh._path.c_str(), doc);

		root = doc.first_node("worksheet");

		d = root->first_node("dimension");
		if (d)
			parseRange(d->first_attribute("ref")->value(), sh._dimension);

		row = root->first_node("sheetData");
		row = row->first_node("row");

		int vecsize = (sh._dimension.lastCol - sh._dimension.firstCol + 1) * (sh._dimension.lastRow - sh._dimension.firstRow + 1);

		sh._cells.resize(vecsize);


		while (row)
		{
			int rowIdx = lexical_cast<int>(row->first_attribute("r")->value());
			c = row->first_node("c");

			while (c)
			{
				int colIdx = 0;
				parseCell(c->first_attribute("r")->value(), rowIdx, colIdx);
				int index = sh.toIndex(rowIdx, colIdx);

				const char *s, *t;

				v = c->first_node("v");

				Cell* cell = new Cell;

				if (v)
				{
					s = v->value();
					if (c->first_attribute("t"))
					{
						t = c->first_attribute("t")->value();
						if (!strcmp(t, "s"))
						{
							cell->value = (char*)_sharedString[atoi(s)].c_str();
							cell->type = "string";
						}
						else if (!strcmp(t, "b"))
						{
							if (!strcmp(s, "0"))
							{
								cell->value = "FALSE";
							}
							else
							{
								cell->value = "TRUE";
							}
							cell->type = "bool";
						}
					}
					else
					{
						cell->type = "unknow";
						cell->value = (char*)s;
					}
				}
				sh._cells[index] = cell;
				c = c->next_sibling("c");
			}

			row = row->next_sibling("row");
		}
	}

	ExcelFile::~ExcelFile()
	{
		if (_zip) delete _zip;
	}

	bool ExcelFile::open(const char* filename)
	{
		_zip = new Zip();

		if (!_zip->open(filename))
			return false;

		readWorkBook("xl/workbook.xml");
		readWorkBookRels("xl/_rels/workbook.xml.rels");
		readSharedStrings("xl/sharedStrings.xml");
		readStyles("styles.xml");

		for (auto& s : _sheets)
		{
			readSheet(s);
		}

		return true;
	}


	Sheet* ExcelFile::getSheet(const char* name)
	{
		for (Sheet& sh : _sheets)
		{
			if (sh._name == name)
				return &sh;
		}

		return nullptr;
	}

}
