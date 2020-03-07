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

#include <string>
#include <vector>
#include "NFMeshLoader.h"

//#define TINYOBJLOADER_IMPLEMENTATION

NF_SHARE_PTR<NFMesh> NFMeshLoader::LoadObjAndConvert(float bmin[3], float bmax[3], const std::string& filename)
{
    std::string base_dir = GetBaseDir(filename);
    if (base_dir.empty()) {
        base_dir = ".";
    }
#ifdef _WIN32
    base_dir += "\\";
#else
    base_dir += "/";
#endif

    std::string warn;
    std::string err;

    NF_SHARE_PTR<NFMesh> mesh(NF_SHARE_PTR<NFMesh>(new NFMesh()));
    bool ret = tinyobj::LoadObj(&mesh->attrib, &mesh->shapes, &mesh->materials, &warn, &err, filename.c_str());

    if (!warn.empty())
    {
        std::cout << warn << std::endl;
    }

    if (!err.empty())
        std::vector<tinyobj::material_t> materials;
    {
        std::cerr << err << std::endl;
    }

    if (!ret)
    {
        return nullptr;
    }

    //printf("Parsing time: %d [ms]\n", (int)tm.msec());
    /*
    printf("# of vertices  = %d\n", (int)(mesh->attrib.vertices.size()) / 3);
    printf("# of normals   = %d\n", (int)(mesh->attrib.normals.size()) / 3);
    printf("# of texcoords = %d\n", (int)(mesh->attrib.texcoords.size()) / 2);
    printf("# of materials = %d\n", (int)mesh->materials.size());
    printf("# of shapes    = %d\n", (int)mesh->shapes.size());
    */

    // Append `default` material
    mesh->materials.push_back(tinyobj::material_t());

    /*
    for (size_t i = 0; i < mesh->materials.size(); i++)
    {
        printf("material[%d].diffuse_texname = %s\n", int(i), mesh->materials[i].diffuse_texname.c_str());
    }
    */

    // Load diffuse textures
    {
        for (size_t m = 0; m < mesh->materials.size(); m++)
        {
            tinyobj::material_t* mp = &mesh->materials[m];

            if (mp->diffuse_texname.length() > 0) 
            {
                // Only load the texture if it is not already loaded
                if (mesh->textures.find(mp->diffuse_texname) == mesh->textures.end())
                {
                    GLuint texture_id;
                    int w, h;
                    int comp;

                    std::string texture_filename = mp->diffuse_texname;
                    if (!FileExists(texture_filename))
                    {
                        // Append base dir.
                        texture_filename = base_dir + mp->diffuse_texname;
                        if (!FileExists(texture_filename))
                        {
                            std::cerr << "Unable to find file: " << mp->diffuse_texname << std::endl;
                            continue;
                            //exit(1);
                        }
                    }

                    unsigned char* image = stbi_load(texture_filename.c_str(), &w, &h, &comp, STBI_default);
                    if (!image)
                    {
                        std::cerr << "Unable to load texture: " << texture_filename << std::endl;
                        continue;
                        //exit(1);
                    }
                    std::cout << "Loaded texture: " << texture_filename << ", w = " << w
                        << ", h = " << h << ", comp = " << comp << std::endl;

                    glGenTextures(1, &texture_id);
                    glBindTexture(GL_TEXTURE_2D, texture_id);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    if (comp == 3)
                    {
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB,
                            GL_UNSIGNED_BYTE, image);
                    }
                    else if (comp == 4) 
                    {
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
                            GL_UNSIGNED_BYTE, image);
                    }
                    else 
                    {
                        //assert(0);  // TODO
                    }

                    glBindTexture(GL_TEXTURE_2D, 0);

                    stbi_image_free(image);

                    mesh->textures.insert(std::make_pair(mp->diffuse_texname, texture_id));
                }
            }
        }
    }

    bmin[0] = bmin[1] = bmin[2] = std::numeric_limits<float>::max();
    bmax[0] = bmax[1] = bmax[2] = -std::numeric_limits<float>::max();

    {
        for (size_t s = 0; s < mesh->shapes.size(); s++) {
            DrawObject o;
            std::vector<float> buffer;  // pos(3float), normal(3float), color(3float)

            // Check for smoothing group and compute smoothing normals
            std::map<int, vec3> smoothVertexNormals;
            if (hasSmoothingGroup(mesh->shapes[s]))
            {
                std::cout << "Compute smoothingNormal for shape [" << s << "]" << std::endl;
                computeSmoothingNormals(mesh->attrib, mesh->shapes[s], smoothVertexNormals);
            }

            for (size_t f = 0; f < mesh->shapes[s].mesh.indices.size() / 3; f++) 
            {
                tinyobj::index_t idx0 = mesh->shapes[s].mesh.indices[3 * f + 0];
                tinyobj::index_t idx1 = mesh->shapes[s].mesh.indices[3 * f + 1];
                tinyobj::index_t idx2 = mesh->shapes[s].mesh.indices[3 * f + 2];

                int current_material_id = mesh->shapes[s].mesh.material_ids[f];

                if ((current_material_id < 0) ||
                    (current_material_id >= static_cast<int>(mesh->materials.size()))) {
                    // Invaid material ID. Use default material.
                    current_material_id =  mesh->materials.size() - 1;
                    // Default material is added to the last item in `materials`.
                }
                // if (current_material_id >= materials.size()) {
                //    std::cerr << "Invalid material index: " << current_material_id <<
                //    std::endl;
                //}
                //
                float diffuse[3];
                for (size_t i = 0; i < 3; i++) {
                    diffuse[i] = mesh->materials[current_material_id].diffuse[i];
                }
                float tc[3][2];
                if (mesh->attrib.texcoords.size() > 0)
                {
                    if ((idx0.texcoord_index < 0) || (idx1.texcoord_index < 0) || (idx2.texcoord_index < 0))
                    {
                        // face does not contain valid uv index.
                        tc[0][0] = 0.0f;
                        tc[0][1] = 0.0f;
                        tc[1][0] = 0.0f;
                        tc[1][1] = 0.0f;
                        tc[2][0] = 0.0f;
                        tc[2][1] = 0.0f;
                    }
                    else
                    {
                        assert(mesh->attrib.texcoords.size() > size_t(2 * idx0.texcoord_index + 1));
                        assert(mesh->attrib.texcoords.size() > size_t(2 * idx1.texcoord_index + 1));
                        assert(mesh->attrib.texcoords.size() > size_t(2 * idx2.texcoord_index + 1));

                        // Flip Y coord.
                        tc[0][0] = mesh->attrib.texcoords[2 * idx0.texcoord_index];
                        tc[0][1] = 1.0f - mesh->attrib.texcoords[2 * idx0.texcoord_index + 1];
                        tc[1][0] = mesh->attrib.texcoords[2 * idx1.texcoord_index];
                        tc[1][1] = 1.0f - mesh->attrib.texcoords[2 * idx1.texcoord_index + 1];
                        tc[2][0] = mesh->attrib.texcoords[2 * idx2.texcoord_index];
                        tc[2][1] = 1.0f - mesh->attrib.texcoords[2 * idx2.texcoord_index + 1];
                    }
                }
                else 
                {
                    tc[0][0] = 0.0f;
                    tc[0][1] = 0.0f;
                    tc[1][0] = 0.0f;
                    tc[1][1] = 0.0f;
                    tc[2][0] = 0.0f;
                    tc[2][1] = 0.0f;
                }

                float v[3][3];
                for (int k = 0; k < 3; k++)
                {
                    int f0 = idx0.vertex_index;
                    int f1 = idx1.vertex_index;
                    int f2 = idx2.vertex_index;

                    assert(f0 >= 0);
                    assert(f1 >= 0);
                    assert(f2 >= 0);

                    v[0][k] = mesh->attrib.vertices[3 * f0 + k];
                    v[1][k] = mesh->attrib.vertices[3 * f1 + k];
                    v[2][k] = mesh->attrib.vertices[3 * f2 + k];
                    bmin[k] = std::min(v[0][k], bmin[k]);
                    bmin[k] = std::min(v[1][k], bmin[k]);
                    bmin[k] = std::min(v[2][k], bmin[k]);
                    bmax[k] = std::max(v[0][k], bmax[k]);
                    bmax[k] = std::max(v[1][k], bmax[k]);
                    bmax[k] = std::max(v[2][k], bmax[k]);
                }

                float n[3][3];
                {
                    bool invalid_normal_index = false;
                    if (mesh->attrib.normals.size() > 0) 
                    {
                        int nf0 = idx0.normal_index;
                        int nf1 = idx1.normal_index;
                        int nf2 = idx2.normal_index;

                        if ((nf0 < 0) || (nf1 < 0) || (nf2 < 0)) {
                            // normal index is missing from this face.
                            invalid_normal_index = true;
                        }
                        else
                        {
                            for (int k = 0; k < 3; k++) 
                            {
                                assert(size_t(3 * nf0 + k) < mesh->attrib.normals.size());
                                assert(size_t(3 * nf1 + k) < mesh->attrib.normals.size());
                                assert(size_t(3 * nf2 + k) < mesh->attrib.normals.size());
                                n[0][k] = mesh->attrib.normals[3 * nf0 + k];
                                n[1][k] = mesh->attrib.normals[3 * nf1 + k];
                                n[2][k] = mesh->attrib.normals[3 * nf2 + k];
                            }
                        }
                    }
                    else 
                    {
                        invalid_normal_index = true;
                    }

                    if (invalid_normal_index && !smoothVertexNormals.empty()) {
                        // Use smoothing normals
                        int f0 = idx0.vertex_index;
                        int f1 = idx1.vertex_index;
                        int f2 = idx2.vertex_index;

                        if (f0 >= 0 && f1 >= 0 && f2 >= 0)
                        {
                            n[0][0] = smoothVertexNormals[f0].v[0];
                            n[0][1] = smoothVertexNormals[f0].v[1];
                            n[0][2] = smoothVertexNormals[f0].v[2];

                            n[1][0] = smoothVertexNormals[f1].v[0];
                            n[1][1] = smoothVertexNormals[f1].v[1];
                            n[1][2] = smoothVertexNormals[f1].v[2];

                            n[2][0] = smoothVertexNormals[f2].v[0];
                            n[2][1] = smoothVertexNormals[f2].v[1];
                            n[2][2] = smoothVertexNormals[f2].v[2];

                            invalid_normal_index = false;
                        }
                    }

                    if (invalid_normal_index)
                    {
                        // compute geometric normal
                        CalcNormal(n[0], v[0], v[1], v[2]);
                        n[1][0] = n[0][0];
                        n[1][1] = n[0][1];
                        n[1][2] = n[0][2];
                        n[2][0] = n[0][0];
                        n[2][1] = n[0][1];
                        n[2][2] = n[0][2];
                    }
                }

                for (int k = 0; k < 3; k++)
                {
                    buffer.push_back(v[k][0]);
                    buffer.push_back(v[k][1]);
                    buffer.push_back(v[k][2]);
                    buffer.push_back(n[k][0]);
                    buffer.push_back(n[k][1]);
                    buffer.push_back(n[k][2]);
                    // Combine normal and diffuse to get color.
                    float normal_factor = 0.2;
                    float diffuse_factor = 1 - normal_factor;
                    float c[3] = { n[k][0] * normal_factor + diffuse[0] * diffuse_factor,
                                  n[k][1] * normal_factor + diffuse[1] * diffuse_factor,
                                  n[k][2] * normal_factor + diffuse[2] * diffuse_factor };
                    float len2 = c[0] * c[0] + c[1] * c[1] + c[2] * c[2];

                    if (len2 > 0.0f)
                    {
                        float len = sqrtf(len2);

                        c[0] /= len;
                        c[1] /= len;
                        c[2] /= len;
                    }
                    buffer.push_back(c[0] * 0.5 + 0.5);
                    buffer.push_back(c[1] * 0.5 + 0.5);
                    buffer.push_back(c[2] * 0.5 + 0.5);

                    buffer.push_back(tc[k][0]);
                    buffer.push_back(tc[k][1]);
                }
            }

            o.vb_id = 0;
            o.numTriangles = 0;

            // OpenGL viewer does not support texturing with per-face material.
            if (mesh->shapes[s].mesh.material_ids.size() > 0 && mesh->shapes[s].mesh.material_ids.size() > s)
            {
                o.material_id = mesh->shapes[s].mesh.material_ids[0];  // use the material ID
                                                                 // of the first face.
            }
            else 
            {
                o.material_id = mesh->materials.size() - 1;  // = ID for default material.
            }
            printf("shape[%d] material_id %d\n", int(s), int(o.material_id));

            if (buffer.size() > 0) 
            {
                glGenBuffers(1, &o.vb_id);
                glBindBuffer(GL_ARRAY_BUFFER, o.vb_id);
                glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(float), &buffer.at(0), GL_STATIC_DRAW);

                o.numTriangles = buffer.size() / (3 + 3 + 3 + 2) / 3;  // 3:vtx, 3:normal, 3:col, 2:texcoord

                printf("shape[%d] # of triangles = %d\n", static_cast<int>(s), o.numTriangles);
            }

            mesh->drawObjects.push_back(o);
        }
    }

    printf("bmin = %f, %f, %f\n", bmin[0], bmin[1], bmin[2]);
    printf("bmax = %f, %f, %f\n", bmax[0], bmax[1], bmax[2]);

    return mesh;
}

void NFMesh::Draw()
{

    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);

    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0, 1.0);
    GLsizei stride = (3 + 3 + 3 + 2) * sizeof(float);
    for (size_t i = 0; i < drawObjects.size(); i++)
    {
        const DrawObject& o = drawObjects[i];
        if (o.vb_id < 1)
        {
            continue;
        }

        glBindBuffer(GL_ARRAY_BUFFER, o.vb_id);
        //glEnableClientState(GL_VERTEX_ARRAY);
        //glEnableClientState(GL_NORMAL_ARRAY);
        //glEnableClientState(GL_COLOR_ARRAY);
        //glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glBindTexture(GL_TEXTURE_2D, 0);
        if ((o.material_id < materials.size()))
        {
            std::string diffuse_texname = materials[o.material_id].diffuse_texname;
            if (textures.find(diffuse_texname) != textures.end()) 
            {
                glBindTexture(GL_TEXTURE_2D, textures[diffuse_texname]);
            }
        }

        //glVertexPointer(3, GL_FLOAT, stride, (const void*)0);
        //glNormalPointer(GL_FLOAT, stride, (const void*)(sizeof(float) * 3));
        //glColorPointer(3, GL_FLOAT, stride, (const void*)(sizeof(float) * 6));
        //glTexCoordPointer(2, GL_FLOAT, stride, (const void*)(sizeof(float) * 9));

        glDrawArrays(GL_TRIANGLES, 0, 3 * o.numTriangles);
        //CheckErrors("drawarrays");
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    // draw wireframe
    glDisable(GL_POLYGON_OFFSET_FILL);
    glPolygonMode(GL_FRONT, GL_LINE);
    glPolygonMode(GL_BACK, GL_LINE);

    //glColor3f(0.0f, 0.0f, 0.4f);

    for (size_t i = 0; i < drawObjects.size(); i++)
    {
        DrawObject o = drawObjects[i];
        if (o.vb_id < 1)
        {
            continue;
        }

        glBindBuffer(GL_ARRAY_BUFFER, o.vb_id);
        //glEnableClientState(GL_VERTEX_ARRAY);
        //glEnableClientState(GL_NORMAL_ARRAY);
        //glDisableClientState(GL_COLOR_ARRAY);
        //glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        //glVertexPointer(3, GL_FLOAT, stride, (const void*)0);
        //glNormalPointer(GL_FLOAT, stride, (const void*)(sizeof(float) * 3));
        //glColorPointer(3, GL_FLOAT, stride, (const void*)(sizeof(float) * 6));
        //glTexCoordPointer(2, GL_FLOAT, stride, (const void*)(sizeof(float) * 9));

        glDrawArrays(GL_TRIANGLES, 0, 3 * o.numTriangles);
        //CheckErrors("drawarrays");
    }
    /*
    
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);

    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0, 1.0);
    GLsizei stride = (3 + 3 + 3 + 2) * sizeof(float);
    for (size_t i = 0; i < drawObjects.size(); i++)
    {
        const DrawObject& o = drawObjects[i];
        if (o.vb_id < 1)
        {
            continue;
        }

        glBindBuffer(GL_ARRAY_BUFFER, o.vb_id);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glBindTexture(GL_TEXTURE_2D, 0);
        if ((o.material_id < materials.size()))
        {
            std::string diffuse_texname = materials[o.material_id].diffuse_texname;
            if (textures.find(diffuse_texname) != textures.end()) {
                glBindTexture(GL_TEXTURE_2D, textures[diffuse_texname]);
            }
        }
        glVertexPointer(3, GL_FLOAT, stride, (const void*)0);
        glNormalPointer(GL_FLOAT, stride, (const void*)(sizeof(float) * 3));
        glColorPointer(3, GL_FLOAT, stride, (const void*)(sizeof(float) * 6));
        glTexCoordPointer(2, GL_FLOAT, stride, (const void*)(sizeof(float) * 9));

        glDrawArrays(GL_TRIANGLES, 0, 3 * o.numTriangles);
        CheckErrors("drawarrays");
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    // draw wireframe
    glDisable(GL_POLYGON_OFFSET_FILL);
    glPolygonMode(GL_FRONT, GL_LINE);
    glPolygonMode(GL_BACK, GL_LINE);

    glColor3f(0.0f, 0.0f, 0.4f);

    for (size_t i = 0; i < drawObjects.size(); i++) 
    {
        DrawObject o = drawObjects[i];
        if (o.vb_id < 1)
        {
            continue;
        }

        glBindBuffer(GL_ARRAY_BUFFER, o.vb_id);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glVertexPointer(3, GL_FLOAT, stride, (const void*)0);
        glNormalPointer(GL_FLOAT, stride, (const void*)(sizeof(float) * 3));
        glColorPointer(3, GL_FLOAT, stride, (const void*)(sizeof(float) * 6));
        glTexCoordPointer(2, GL_FLOAT, stride, (const void*)(sizeof(float) * 9));

        glDrawArrays(GL_TRIANGLES, 0, 3 * o.numTriangles);
        CheckErrors("drawarrays");
    }
    */
}