// -------------------------------------------------------------------------
//    @FileName			:    NFINavigationModule.h
//    @Author           :    Bluesky
//    @Date             :    2016-06-22
//    @Module           :    NFINavigationModule
//    @Desc             :
// -------------------------------------------------------------------------


#ifndef NFI_NAVIGATION_MODULE_H
#define NFI_NAVIGATION_MODULE_H

#include "NFIModule.h"

class NFINavigationModule : public NFIModule
{
public:
	virtual bool ExistNavigation(NFINT64 scendId) = 0;

	virtual bool RemoveNavigation(NFINT64 scendId) = 0;

	virtual int FindPath(NFINT64 scendId, const NFVector3& start, const NFVector3& end, std::vector<NFVector3>& paths) = 0;

	virtual int FindRandomPointAroundCircle(NFINT64 scendId, const NFVector3& centerPos, std::vector<NFVector3>& points, NFINT32 max_points, float maxRadius) = 0;

	virtual int Raycast(NFINT64 scendId, const NFVector3& start, const NFVector3& end, std::vector<NFVector3>& hitPointVec) = 0;
};

#endif