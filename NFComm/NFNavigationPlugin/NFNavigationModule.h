/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2020 NoahFrame(NoahGameFrame)

   File creator: Bluesky
   
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

#ifndef NF_NAVIGATION_MODULE_H
#define NF_NAVIGATION_MODULE_H

#include <iostream>
#include <unordered_map>
#include "navigation/DetourNavMeshBuilder.h"
#include "navigation/DetourNavMeshQuery.h"
#include "navigation/DetourCommon.h"
#include "navigation/DetourNavMesh.h"
#include "NFComm/NFCore/NFIObject.h"
#include "NFComm/NFPluginModule/NFGUID.h"
#include "NFComm/NFPluginModule/NFINavigationModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFCore/NFVector3.hpp"

/** 安全的释放一个指针内存 */
#define SAFE_RELEASE(i)										\
	if (i)													\
		{													\
			delete i;										\
			i = NULL;										\
		}

/** 安全的释放一个指针数组内存 */
#define SAFE_RELEASE_ARRAY(i)								\
	if (i)													\
		{													\
			delete[] i;										\
			i = NULL;										\
		}

// Returns a random number [0..1)
static float frand()
{
	//	return ((float)(rand() & 0xffff)/(float)0xffff);
	return (float)rand() / (float)RAND_MAX;
}

struct NavMeshSetHeader
{
	int version;
	int tileCount;
	dtNavMeshParams params;
};

struct NavMeshTileHeader
{
	dtTileRef tileRef;
	int dataSize;
};

class NFNavigationHandle
{
public:
	static const int NAV_ERROR = -1;

	static const int MAX_POLYS = 256;
	static const int NAV_ERROR_NEARESTPOLY = -2;

	static const long RCN_NAVMESH_VERSION = 1;
	static const int INVALID_NAVMESH_POLYREF = 0;

	struct NavmeshLayer
	{
		dtNavMesh* pNavmesh;
		dtNavMeshQuery* pNavmeshQuery;
	};

public:
	NFNavigationHandle(){};

	virtual ~NFNavigationHandle() 
	{
		dtFreeNavMesh(navmeshLayer.pNavmesh);
		dtFreeNavMeshQuery(navmeshLayer.pNavmeshQuery);
	};

	int FindStraightPath(const NFVector3& start, const NFVector3& end, std::vector<NFVector3>& paths)
	{
		dtNavMeshQuery* navmeshQuery = navmeshLayer.pNavmeshQuery;

		float spos[3];
		spos[0] = start.X();
		spos[1] = start.Y();
		spos[2] = start.Z();

		float epos[3];
		epos[0] = end.X();
		epos[1] = end.Y();
		epos[2] = end.Z();

		dtQueryFilter filter;
		filter.setIncludeFlags(0xffff);
		filter.setExcludeFlags(0);

		const float extents[3] = { 2.f, 4.f, 2.f };

		dtPolyRef startRef = INVALID_NAVMESH_POLYREF;
		dtPolyRef endRef = INVALID_NAVMESH_POLYREF;

		float startNearestPt[3];
		float endNearestPt[3];
		navmeshQuery->findNearestPoly(spos, extents, &filter, &startRef, startNearestPt);
		navmeshQuery->findNearestPoly(epos, extents, &filter, &endRef, endNearestPt);

		if (!startRef || !endRef)
		{
			//debuf_msg("NavMeshHandle::findStraightPath({%s}): Could not find any nearby poly's ({%d}, {%d})\n", resPath.c_str(), startRef, endRef);
			return NAV_ERROR_NEARESTPOLY;
		}

		dtPolyRef polys[MAX_POLYS];
		int npolys;
		float straightPath[MAX_POLYS * 3];
		unsigned char straightPathFlags[MAX_POLYS];
		dtPolyRef straightPathPolys[MAX_POLYS];
		int nstraightPath;
		int pos = 0;

		navmeshQuery->findPath(startRef, endRef, startNearestPt, endNearestPt, &filter, polys, &npolys, MAX_POLYS);
		nstraightPath = 0;

		if (npolys)
		{
			float epos1[3];
			dtVcopy(epos1, endNearestPt);

			if (polys[npolys - 1] != endRef)
				navmeshQuery->closestPointOnPoly(polys[npolys - 1], endNearestPt, epos1, 0);

			navmeshQuery->findStraightPath(startNearestPt, endNearestPt, polys, npolys, straightPath, straightPathFlags, straightPathPolys, &nstraightPath, MAX_POLYS);

			NFVector3 currpos;
			for (int i = 0; i < nstraightPath * 3; )
			{
				currpos.SetX(straightPath[i++]);
				currpos.SetY(straightPath[i++]);
				currpos.SetZ(straightPath[i++]);
				paths.push_back(currpos);
				pos++;
			}
		}

		return pos;
	}

	int FindRandomPointAroundCircle(const NFVector3& centerPos, std::vector<NFVector3>& points, NFINT32 max_points, float maxRadius)
	{
		dtNavMeshQuery* navmeshQuery = navmeshLayer.pNavmeshQuery;

		dtQueryFilter filter;
		filter.setIncludeFlags(0xffff);
		filter.setExcludeFlags(0);

		if (maxRadius <= 0.0001f)
		{
			NFVector3 currpos;

			for (int i = 0; i < max_points; i++)
			{
				float pt[3];
				dtPolyRef ref;
				dtStatus status = navmeshQuery->findRandomPoint(&filter, frand, &ref, pt);
				if (dtStatusSucceed(status))
				{
					currpos.SetX(pt[0]);
					currpos.SetY(pt[1]);
					currpos.SetZ(pt[2]);

					points.push_back(currpos);
				}
			}

			return (int)points.size();
		}

		const float extents[3] = { 2.f, 4.f, 2.f };

		dtPolyRef startRef = INVALID_NAVMESH_POLYREF;

		float spos[3];
		spos[0] = centerPos.X();
		spos[1] = centerPos.Y();
		spos[2] = centerPos.Z();

		float startNearestPt[3];
		navmeshQuery->findNearestPoly(spos, extents, &filter, &startRef, startNearestPt);

		if (!startRef)
		{
			//debuf_msg("NavMeshHandle::findRandomPointAroundCircle({%s}): Could not find any nearby poly's ({%d})\n", resPath, startRef);
			return NAV_ERROR_NEARESTPOLY;
		}

		NFVector3 currpos;
		bool done = false;
		int itry = 0;

		while (itry++ < 3 && points.size() == 0)
		{
			max_points -= (int)points.size();

			for (int i = 0; i < max_points; i++)
			{
				float pt[3];
				dtPolyRef ref;
				dtStatus status = navmeshQuery->findRandomPointAroundCircle(startRef, spos, maxRadius, &filter, frand, &ref, pt);

				if (dtStatusSucceed(status))
				{
					done = true;
					currpos.SetX(pt[0]);
					currpos.SetY(pt[1]);
					currpos.SetZ(pt[2]);

					NFVector3 v = centerPos - currpos;
					float dist_len = v.Length();
					if (dist_len > maxRadius)
						continue;

					points.push_back(currpos);
				}
			}

			if (!done)
				break;
		}

		return (int)points.size();
	}

	int Raycast(const NFVector3& start, const NFVector3& end, std::vector<NFVector3>& hitPointVec)
	{
		dtNavMeshQuery* navmeshQuery = navmeshLayer.pNavmeshQuery;

		float hitPoint[3];

		float spos[3];
		spos[0] = start.X();
		spos[1] = start.Y();
		spos[2] = start.Z();

		float epos[3];
		epos[0] = end.X();
		epos[1] = end.Y();
		epos[2] = end.Z();

		dtQueryFilter filter;
		filter.setIncludeFlags(0xffff);
		filter.setExcludeFlags(0);

		const float extents[3] = { 2.f, 4.f, 2.f };

		dtPolyRef startRef = INVALID_NAVMESH_POLYREF;

		float nearestPt[3];
		navmeshQuery->findNearestPoly(spos, extents, &filter, &startRef, nearestPt);

		if (!startRef)
		{
			return NAV_ERROR_NEARESTPOLY;
		}

		float t = 0;
		float hitNormal[3];
		memset(hitNormal, 0, sizeof(hitNormal));

		dtPolyRef polys[MAX_POLYS];
		int npolys;

		navmeshQuery->raycast(startRef, spos, epos, &filter, &t, hitNormal, polys, &npolys, MAX_POLYS);

		if (t > 1)
		{
			// no hit
			return NAV_ERROR;
		}
		else
		{
			// Hit
			hitPoint[0] = spos[0] + (epos[0] - spos[0]) * t;
			hitPoint[1] = spos[1] + (epos[1] - spos[1]) * t;
			hitPoint[2] = spos[2] + (epos[2] - spos[2]) * t;
			if (npolys)
			{
				float h = 0;
				navmeshQuery->getPolyHeight(polys[npolys - 1], hitPoint, &h);
				hitPoint[1] = h;
			}
		}

		hitPointVec.push_back(NFVector3(hitPoint[0], hitPoint[1], hitPoint[2]));
		return 1;
	}

	static NF_SHARE_PTR<NFNavigationHandle> Create(std::string resPath)
	{
		FILE* fp = fopen(resPath.c_str(), "rb");
		if (!fp)
		{
			printf("NFNavigationHandle::create: open({%s}) is error!\n", resPath.c_str());
			return NULL;
		}

		printf("NFNavigationHandle::create: ({%s}), layer={%d}\n", resPath.c_str(), 0);

		bool safeStorage = true;
		int pos = 0;
		int size = sizeof(NavMeshSetHeader);

		fseek(fp, 0, SEEK_END);
		size_t flen = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		uint8_t* data = new uint8_t[flen];
		if (data == NULL)
		{
			printf("NFNavigationHandle::create: open({%s}), memory(size={%d}) error!\n", resPath.c_str(), (int)flen);

			fclose(fp);
			SAFE_RELEASE_ARRAY(data);
			return NULL;
		}

		size_t readsize = fread(data, 1, flen, fp);
		if (readsize != flen)
		{
			printf("NFNavigationHandle::create: open({%s}), read(size={%d} != {%d}) error!\n", resPath.c_str(), (int)readsize, (int)flen);

			fclose(fp);
			SAFE_RELEASE_ARRAY(data);
			return NULL;
		}

		if (readsize < sizeof(NavMeshSetHeader))
		{
			printf("NFNavigationHandle::create: open({%s}), NavMeshSetHeader is error!\n", resPath.c_str());

			fclose(fp);
			SAFE_RELEASE_ARRAY(data);
			return NULL;
		}

		NavMeshSetHeader header;
		memcpy(&header, data, size);

		pos += size;

		if (header.version != NFNavigationHandle::RCN_NAVMESH_VERSION)
		{
			printf("NFNavigationHandle::create: navmesh version({%d}) is not match({%d})!\n",header.version, ((int)NFNavigationHandle::RCN_NAVMESH_VERSION));

			fclose(fp);
			SAFE_RELEASE_ARRAY(data);
			return NULL;
		}

		dtNavMesh* mesh = dtAllocNavMesh();
		if (!mesh)
		{
			printf("NavMeshHandle::create: dtAllocNavMesh is failed!\n");
			fclose(fp);
			SAFE_RELEASE_ARRAY(data);
			return NULL;
		}

		dtStatus status = mesh->init(&header.params);
		if (dtStatusFailed(status))
		{
			printf("NFNavigationHandle::create: mesh init is error({%d})!\n", status);
			fclose(fp);
			SAFE_RELEASE_ARRAY(data);
			return NULL;
		}

		// Read tiles.
		bool success = true;
		for (int i = 0; i < header.tileCount; ++i)
		{
			NavMeshTileHeader tileHeader;
			size = sizeof(NavMeshTileHeader);
			memcpy(&tileHeader, &data[pos], size);
			pos += size;

			size = tileHeader.dataSize;
			if (!tileHeader.tileRef || !tileHeader.dataSize)
			{
				success = false;
				status = DT_FAILURE + DT_INVALID_PARAM;
				break;
			}

			unsigned char* tileData =
				(unsigned char*)dtAlloc(size, DT_ALLOC_PERM);
			if (!tileData)
			{
				success = false;
				status = DT_FAILURE + DT_OUT_OF_MEMORY;
				break;
			}
			memcpy(tileData, &data[pos], size);
			pos += size;

			status = mesh->addTile(tileData
				, size
				, (safeStorage ? DT_TILE_FREE_DATA : 0)
				, tileHeader.tileRef
				, 0);

			if (dtStatusFailed(status))
			{
				success = false;
				break;
			}
		}

		fclose(fp);
		SAFE_RELEASE_ARRAY(data);

		if (!success)
		{
			printf("NavMeshHandle::create:  error({%d})!\n", status);
			dtFreeNavMesh(mesh);
			return NULL;
		}

		NF_SHARE_PTR<NFNavigationHandle> pNavMeshHandle = NF_SHARE_PTR<NFNavigationHandle>(NF_NEW NFNavigationHandle());
		dtNavMeshQuery* pNavmeshQuery = new dtNavMeshQuery();

		pNavmeshQuery->init(mesh, 1024);
		pNavMeshHandle->resPath = resPath;
		pNavMeshHandle->navmeshLayer.pNavmeshQuery = pNavmeshQuery;
		pNavMeshHandle->navmeshLayer.pNavmesh = mesh;

		uint32_t tileCount = 0;
		uint32_t nodeCount = 0;
		uint32_t polyCount = 0;
		uint32_t vertCount = 0;
		uint32_t triCount = 0;
		uint32_t triVertCount = 0;
		uint32_t dataSize = 0;

		const dtNavMesh* navmesh = mesh;
		for (int i = 0; i < navmesh->getMaxTiles(); ++i)
		{
			const dtMeshTile* tile = navmesh->getTile(i);
			if (!tile || !tile->header)
				continue;

			tileCount++;
			nodeCount += tile->header->bvNodeCount;
			polyCount += tile->header->polyCount;
			vertCount += tile->header->vertCount;
			triCount += tile->header->detailTriCount;
			triVertCount += tile->header->detailVertCount;
			dataSize += tile->dataSize;
		}

		printf("\t==> resPath: {%s}\n", resPath.c_str());
		printf("\t==> tiles loaded: {%d}\n", tileCount);
		printf("\t==> BVTree nodes: {%d}\n", nodeCount);
		printf("\t==> {%d} polygons ({%d} vertices)\n", polyCount, vertCount);
		printf("\t==> {%d} triangles ({%d} vertices)\n", triCount, triVertCount);
		printf("\t==> {%f:.2f} MB of data (not including pointers)\n", (((float)dataSize / sizeof(unsigned char)) / 1048576));
		printf("\t==> ----------------------------------------\n");

		return pNavMeshHandle;
	}

	NavmeshLayer navmeshLayer;
	std::string resPath;
};

class NFNavigationModule : public NFINavigationModule
{
public:
	NFNavigationModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }


    virtual ~NFNavigationModule()
    {
    }

	virtual bool Init();
	virtual bool AfterInit();
	virtual bool BeforeShut();
	virtual bool Shut();
	virtual bool Execute();

	NF_SHARE_PTR<NFNavigationHandle> LoadNavigation(NFINT64 scendId, std::string resPath);

	NF_SHARE_PTR<NFNavigationHandle> FindNavigation(NFINT64 scendId);

	virtual bool ExistNavigation(NFINT64 scendId);

	virtual bool RemoveNavigation(NFINT64 scendId);

	virtual int FindPath(NFINT64 scendId, const NFVector3& start, const NFVector3& end, std::vector<NFVector3>& paths);

	virtual int FindRandomPointAroundCircle(NFINT64 scendId, const NFVector3& centerPos, std::vector<NFVector3>& points, NFINT32 max_points, float maxRadius);

	virtual int Raycast(NFINT64 scendId, const NFVector3& start, const NFVector3& end, std::vector<NFVector3>& hitPointVec);

private:
	NFILogModule* m_pLogModule;
	NFIClassModule* m_pClassModule;
	NFIElementModule* m_pElementModule;

	std::unordered_map<NFINT64, NF_SHARE_PTR<NFNavigationHandle>> m_Navhandles;
};

#endif
