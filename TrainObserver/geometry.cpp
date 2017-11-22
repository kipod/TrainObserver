#include "geometry.h"
#include "render_dx9.h"
#include <algorithm>
#include <fstream>
#include "fileformats\tiny_obj_loader.h"
#include "math\Vector3.h"
#include "vertex_formats.h"
#include "geometry_utils.h"

namespace
{
	struct ObjMeshData
	{
		std::vector<XYZNUVTB>		vertices;
		std::vector<PrimitiveGroup> primitiveGroups;
	};

	void generateNormal(XYZNUVTB& v0, XYZNUVTB& v1, XYZNUVTB& v2)
	{
		graph::Vector3 l0(v0.pos - v1.pos);
		graph::Vector3 l1(v0.pos - v2.pos);

		graph::Vector3 normal(l0 * l1);
		normal.Normalize();

		v0.normal = v1.normal = v2.normal = normal;
	}


	bool loadObj(LPDIRECT3DDEVICE9 pDevice, const std::string& path, ObjMeshData& outData)
	{
		std::string err;
		
		std::ifstream ifs(path);

		if (ifs.fail()) 
		{
			LOG(MSG_ERROR, "file not found.");
			return false;
		}

		

		std::string dir;
		size_t pos = path.find_last_of("/\\");
		if (pos != std::string::npos)
		{
			dir = path.substr(0, pos+1);
		}

		tinyobj::MaterialFileReader mtlReader(dir);

		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		if (tinyobj::LoadObj(&attrib, &shapes, &materials, &err, &ifs, &mtlReader))
		{
			outData.vertices.reserve(attrib.vertices.size() / 3);

			bool hasUV = !attrib.texcoords.empty();
			bool hasNormal = !attrib.normals.empty();

			for (const auto shape : shapes)
			{
				PrimitiveGroup primitiveGroup;
				primitiveGroup.nTriangles = shape.mesh.indices.size() / 3;
				primitiveGroup.vertexOffset = outData.vertices.size();
				int matId = shape.mesh.material_ids[0];

				if (matId >= 0 && materials.size() > 0)
				{
					auto diffTex = materials[matId].diffuse_texname;
					if (!diffTex.empty())
					{
						bool exists = fileExists(diffTex);
						if (!exists)
						{
							diffTex = dir + diffTex;
							exists = fileExists(diffTex);
						}

						if (exists)
						{
							primitiveGroup.properties.setTexture("diffuseTex", diffTex.c_str());
						}
					}
				}

				for (uint f = 0; f < primitiveGroup.nTriangles; ++f)
				{
					for (uint i = 0; i < 3; ++i)
					{
						XYZNUVTB v;
						tinyobj::index_t idx = shape.mesh.indices[3 * f + i];

						v.pos.x = attrib.vertices[idx.vertex_index * 3];
						v.pos.y = attrib.vertices[idx.vertex_index * 3 + 1];
						v.pos.z = attrib.vertices[idx.vertex_index * 3 + 2];

						if (hasUV)
						{
							v.u = attrib.texcoords[2 * idx.texcoord_index];
							v.v = 1.0f - attrib.texcoords[2 * idx.texcoord_index + 1];
						}
						else
						{
							v.u = v.v = 0.0f;
						}

						if (hasNormal)
						{
							v.normal.x = attrib.normals[idx.normal_index * 3];
							v.normal.y = attrib.normals[idx.normal_index * 3 + 1];
							v.normal.z = attrib.normals[idx.normal_index * 3 + 2];
						}
						outData.vertices.emplace_back(v);
					}
				}

				outData.primitiveGroups.emplace_back(primitiveGroup);
			}

			if (!hasNormal)
			{
				for (uint i = 0; i < outData.vertices.size() / 3; ++i)
				{
					generateNormal(outData.vertices[i * 3], outData.vertices[i * 3 + 1], outData.vertices[i * 3 + 2]);
				}
			}

			for (auto& v : outData.vertices)
			{
				generateTangentAndBinormal(v);
			}
		}
		else
		{
			if (!err.empty())
			{
				LOG(MSG_ERROR, err.c_str());
			}

			LOG(MSG_ERROR, "Failed to parse .obj");
			return false;
		}


		return true;
	}

	enum EFileType
	{
		X,
		OBJ,
		UNSUPPORTED
	};

	EFileType getType(const char* path)
	{
		std::string str(path);
		size_t pos = str.find_last_of('.');

		if (pos == std::string::npos)
			return UNSUPPORTED;

		str = str.substr(pos + 1);
		std::transform(str.begin(), str.end(), str.begin(), tolower);

		if (str == "x")
			return X;
		if (str == "obj")
			return OBJ;

		return UNSUPPORTED;
	}

	LPD3DXMESH loadMesh(LPDIRECT3DDEVICE9 pDevice, const char* path)
	{
		LPD3DXMESH mesh = nullptr;
		auto hr = D3DXLoadMeshFromX(path, D3DXMESH_SYSTEMMEM, pDevice, NULL, NULL, NULL, NULL, &mesh);

		if (FAILED(hr))
			return nullptr;


		DWORD fvf = mesh->GetFVF();
		// Флаг D3DFVF_NORMAL указан в формате вершин сетки?
		if (!(fvf & D3DFVF_NORMAL))
		{
			// Нет, клонируем сетку и добавляем флаг D3DFVF_NORMAL
			// к ее формату вершин:
			ID3DXMesh* pTempMesh = 0;
			mesh->CloneMeshFVF(
				D3DXMESH_MANAGED,
				fvf | D3DFVF_NORMAL, // добавляем флаг
				pDevice,
				&pTempMesh);

			// Вычисляем нормали:
			D3DXComputeNormals(pTempMesh, 0);

			mesh->Release();  // удаляем старую сетку
			mesh = pTempMesh; // сохраняем новую сетку с нормалями
		}

		return mesh;
	}

}


Geometry::Geometry():
	m_mesh(nullptr),
	m_vb(nullptr),
	m_ib(nullptr),
	m_nTriangles(0),
	m_nVertices(0)
{
}


Geometry::~Geometry()
{
}

Geometry* Geometry::create(const std::string& path, bool normalizeSize)
{
	auto pDevice = RenderSystemDX9::instance().renderer().device();
	Geometry* pGeom = nullptr;

	switch (getType(path.c_str()))
	{
	case X:
	{
		auto mesh = loadMesh(pDevice, path.c_str());
		if (mesh != nullptr)
		{
			pGeom = new Geometry();
			pGeom->m_mesh = mesh;
		}
		break;
	}
	case OBJ:
	{
		ObjMeshData meshData;
		if (loadObj(pDevice, path, meshData))
		{
			pGeom = new Geometry();
			if (!pGeom->create<XYZNUVTB, uint>(pDevice, meshData.vertices, normalizeSize, &meshData.primitiveGroups))
			{
				delete pGeom;
				pGeom = nullptr;
			}
		}
		break;
	}
	case UNSUPPORTED:
	default:
		break;
	}

	return pGeom;
}

void Geometry::draw(LPDIRECT3DDEVICE9 pDevice, Effect& effect)
{
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	if (m_mesh)
	{
		if (effect.begin())
		{
			for (UINT i = 0; i < effect.numPasses(); i++)
			{
				if (effect.beginPass(i))
				{
					m_mesh->DrawSubset(0);
					effect.endPass();
				}
			}

			effect.end();
		}
	}
	else
	{
		pDevice->SetFVF(m_fvf);
		pDevice->SetStreamSource(0, m_vb, 0, m_vertexSize);

		if (m_ib)
		{
			pDevice->SetIndices(m_ib);
		}

		if (effect.begin())
		{
			for (UINT i = 0; i < effect.numPasses(); i++)
			{
				if (effect.beginPass(i))
				{
					for (const auto primGroup : m_primitiveGroups)
					{
						primGroup.properties.applyProperties(&effect);
						effect.flush();
						if (m_ib)
						{
							pDevice->DrawIndexedPrimitive(
								D3DPT_TRIANGLELIST,
								0,
								primGroup.vertexOffset,
								m_nVertices,
								primGroup.indexOffset,
								primGroup.nTriangles);
						}
						else
						{
							pDevice->DrawPrimitive(
								D3DPT_TRIANGLELIST,
								primGroup.vertexOffset,
								primGroup.nTriangles);
						}
					}

					effect.endPass();
				}
			}

			effect.end();
		}
	}

}

