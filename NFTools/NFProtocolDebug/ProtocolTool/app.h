////////////////////////////////////////////
//  Filename app.h
//  Copyright (C) 2017  Stonexin
//  CreateTime 2017/03/13
//
////////////////////////////////////////////
#ifndef APP_H__
#define APP_H__

#include "Dlg.h"

class MyApp : public wxApp
{
public:
	virtual bool OnInit();
	virtual int  OnExit();
};

DECLARE_APP(MyApp)

#endif // APP_H__