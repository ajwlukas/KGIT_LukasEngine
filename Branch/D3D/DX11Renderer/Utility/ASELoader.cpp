#include "pch_dx_11.h"
#include "ASEFile.h"
#include "ASELoader.h"

#define NEXT aseLexer->GetToken(tokenString)

#define rInt move(atoi(tokenString))
#define rFloat move(atof(tokenString))//r = Read

//바꿔줄 필요 없음 실제 데이터는 이 뎁스에서 안 받음
void ASELoader::LoadASE(string fileName)//depth 0
{
	InitVariables();

	this->fileName = fileName;
	aseLexer->Open((LPSTR)fileName.c_str());
	LONG token = TOKEN_NONE;

	while (token = aseLexer->GetToken(tokenString), token != TOKEND_END)
	{
		if (token == TOKENR_MATERIAL_LIST)
			ReadMaterials();
		else if (token == TOKENR_GEOMOBJECT)
			ReadGeoObject();
		else if (token == TOKENR_HELPEROBJECT)
			ReadHelperObject();
		else if (token == TOKENR_SHAPEOBJECT)
			ReadHelperObject();
	}
}

void ASELoader::InitVariables()
{
	aseMats.clear();
	matCount = 0;
	for (ASEObject* o : aseObjects)
		delete o;
	aseObjects.clear();
	fileName = "";
	ZeroMemory(tokenString, 256);
	aseLexer->Close();
}


//바꿔줄 필요 없음 실제 데이터는 이 뎁스에서 안 받음
void ASELoader::ReadHelperObject()
{
	aseObjects.push_back(new ASEObject());
	aseObjects.back()->type = ObjectType::HELPER;

	UINT depth = 0;
	LONG token = TOKEN_NONE;
	while (token = aseLexer->GetToken(tokenString), token != TOKEND_END)
	{
		if (token == TOKEND_BLOCK_START)
			depth++;
		else if (token == TOKEND_BLOCK_END)
		{
			--depth; if (depth <= 0) return;
		}
		else if (token == TOKENR_NODE_PARENT)
		{
			NEXT;
			aseObjects.back()->parentName = tokenString;
		}
		else if (token == TOKENR_NODE_NAME)
		{
			NEXT;//node Name DATA
			aseObjects.back()->name = tokenString;
		}
		else if (token == TOKENR_NODE_TM)
			ReadTransform(aseObjects.back()->transform);
		else if (token == TOKENR_TM_ANIMATION)
			ReadAnimClip(aseObjects.back()->animClip);
	}
}

//바꿔줄 필요 없음
void ASELoader::ReadMaterials()// depth 1
{
	UINT depth = 0;
	LONG token = TOKEN_NONE;
	while (token = aseLexer->GetToken(tokenString), token != TOKEND_END)
	{
		if (token == TOKEND_BLOCK_START)
			depth++;
		else if (token == TOKEND_BLOCK_END)
		{
			--depth; if (depth <= 0) return;
		}
		else if (token == TOKENR_MATERIAL_COUNT)
		{
			NEXT;//count Data
			aseMats.resize(rInt);
		}
		else if (token == TOKENR_MATERIAL)
			ReadMaterial();
	}

}

//바꿔줄 필요 없음
void ASELoader::ReadMaterial()
{
	UINT depth = 0;
	NEXT; // matIndex data
	UINT curIndex = rInt;
	ASEMat& aseMat = aseMats[curIndex];
	LONG token = TOKEN_NONE;
	while (token = aseLexer->GetToken(tokenString), token != TOKEND_END)
	{
		if (token == TOKEND_BLOCK_START)
			depth++;
		else if (token == TOKEND_BLOCK_END)
		{
			--depth; if (depth <= 0) return;
		}
		else if (token == TOKENR_MATERIAL_NAME)
		{
			NEXT;//name Data
			aseMat.name = tokenString;
		}
		else if (token == TOKENR_MATERIAL_AMBIENT)
		{
			NEXT;
			aseMat.ambient.x = rFloat;
			NEXT;
			aseMat.ambient.y = rFloat;
			NEXT;
			aseMat.ambient.z = rFloat;
		}
		else if (token == TOKENR_MATERIAL_DIFFUSE)
		{
			NEXT;
			aseMat.diffuse.x = rFloat;
			NEXT;
			aseMat.diffuse.y = rFloat;
			NEXT;
			aseMat.diffuse.z = rFloat;
		}
		else if (token == TOKENR_MATERIAL_SPECULAR)
		{
			NEXT;
			aseMat.specular.x = rFloat;
			NEXT;
			aseMat.specular.y = rFloat;
			NEXT;
			aseMat.specular.z = rFloat;
		}
		else if (token == TOKENR_MAP_DIFFUSE)
			ReadDiffuseMap(curIndex);
		else if (token == TOKENR_MAP_BUMP)
			ReadNormalMap(curIndex);
	}

}

//바꿔줄 필요 없음
void ASELoader::ReadDiffuseMap(UINT curMatIndex)
{
	UINT depth = 0;
	LONG token = TOKEN_NONE;
	while (token = aseLexer->GetToken(tokenString), token != TOKEND_END)
	{
		if (token == TOKEND_BLOCK_START)
			depth++;
		else if (token == TOKEND_BLOCK_END)
		{
			--depth; if (depth <= 0) return;
		}
		else if (token == TOKENR_BITMAP)
		{
			NEXT;//diffuseMap FileName Data
			aseMats[curMatIndex].diffuseMap.fileName = tokenString;
		}
	}
}

//바꿔줄 필요 없음
void ASELoader::ReadNormalMap(UINT curMatIndex)
{
	UINT depth = 0;
	LONG token = TOKEN_NONE;
	while (token = aseLexer->GetToken(tokenString), token != TOKEND_END)
	{
		if (token == TOKEND_BLOCK_START)
			depth++;
		else if (token == TOKEND_BLOCK_END)
		{
			--depth; if (depth <= 0) return;
		}
		else if (token == TOKENR_BITMAP)
		{
			NEXT;//bumpMap FileName Data
			aseMats[curMatIndex].normalMap.fileName = tokenString;
		}
	}
}

//바꿔줄 필요 없음 실제 데이터는 이 뎁스에서 안 받음
void ASELoader::ReadGeoObject()
{
#define Geo aseObjects.back()
	aseObjects.push_back(new ASEObject());
	aseObjects.back()->type = ObjectType::GEO;
	UINT depth = 0;
	LONG token = TOKEN_NONE;
	while (token = aseLexer->GetToken(tokenString), token != TOKEND_END)
	{
		if (token == TOKEND_BLOCK_START)
			depth++;
		else if (token == TOKEND_BLOCK_END)
		{
			--depth; if (depth <= 0) return;
		}
		else if (token == TOKENR_NODE_NAME)
		{
			NEXT;//node Name DATA
			Geo->name = tokenString;
		}
		else if (token == TOKENR_NODE_PARENT)
		{
			NEXT;
			Geo->parentName = tokenString;
		}
		else if (token == TOKENR_NODE_TM)
			ReadTransform(Geo->transform);
		else if (token == TOKENR_MESH)
			ReadMesh(Geo->mesh);
		else if (token == TOKENR_TM_ANIMATION)
			ReadAnimClip(Geo->animClip);
		else if (token == TOKENR_MATERIAL_REF)
		{
			NEXT;
			Geo->matRef = rInt;
		}
		else if (token == TOKENR_MESH_NUMBONE)
		{
			NEXT;
			Geo->boneList.resize(rInt);
		}
		else if (token == TOKENR_BONE_LIST)
			ReadBones(Geo->boneList);
		else if (token == TOKENR_MESH_WVERTEXS)
		{
			Geo->weights.resize(Geo->mesh.numVertex);
			ReadWeights(Geo->weights);
		}
	}
}

//혜성이 기준으로 바꿔줌
void ASELoader::ReadTransform(AseTransform& trans)
{
	UINT depth = 0;
	LONG token = TOKEN_NONE;
	while (token = aseLexer->GetToken(tokenString), token != TOKEND_END)
	{
		if (token == TOKEND_BLOCK_START)
			depth++;
		else if (token == TOKEND_BLOCK_END)
		{
			--depth;
			if (depth <= 0) return;
		}
		else if (token == TOKENR_TM_ROW0)
		{
			NEXT;//row0.x
			trans.row0.x = rFloat;
			NEXT;//row0.y
			trans.row0.z = rFloat;
			NEXT;//row0.z
			trans.row0.y = rFloat;
		}
		else if (token == TOKENR_TM_ROW1)
		{
			NEXT;//row1.x
			trans.row2.x = rFloat;
			NEXT;//row1.y
			trans.row2.z = rFloat;
			NEXT;//row1.z
			trans.row2.y = rFloat;
		}
		else if (token == TOKENR_TM_ROW2)
		{
			NEXT;//row2.x
			trans.row1.x = rFloat;
			NEXT;//row2.y
			trans.row1.z = rFloat;
			NEXT;//row2.z
			trans.row1.y = rFloat;
		}
		else if (token == TOKENR_TM_ROW3)
		{
			NEXT;//row3.x
			trans.row3.x = rFloat;
			NEXT;//row3.y
			trans.row3.z = rFloat;
			NEXT;//row3.z
			trans.row3.y = rFloat;
		}
		else if (token == TOKENR_TM_POS)
		{
			/*NEXT;
			trans.pos.x = -rFloat;
			NEXT;
			trans.pos.z = rFloat;
			NEXT;
			trans.pos.y = rFloat;*/
		}
		else if (token == TOKENR_TM_ROTAXIS)
		{
			/*NEXT;
			trans.rotAxis.x = -rFloat;
			NEXT;
			trans.rotAxis.z = rFloat;
			NEXT;
			trans.rotAxis.y = rFloat;*/
		}
		else if (token == TOKENR_TM_ROTANGLE)
		{
			/*NEXT;
			trans.rotAngle = rFloat;*/
		}
		else if (token == TOKENR_TM_SCALE)
		{
			/*NEXT;
			trans.scale.x = rFloat;
			NEXT;
			trans.scale.z = rFloat;
			NEXT;
			trans.scale.y = rFloat;*/
		}
		else if (token == TOKENR_TM_SCALEAXIS)
		{
			/*NEXT;
			trans.scaleAxis.x = rFloat;
			NEXT;
			trans.scaleAxis.z = rFloat;
			NEXT;
			trans.scaleAxis.y = rFloat;*/
		}
		else if (token == TOKENR_TM_SCALEAXISANG)
		{
			/*NEXT;
			trans.scaleAxisAngle = rFloat;*/
		}
	}
}

//혜성이 기준으로 바꿔줌
void ASELoader::ReadAnimClip(ASEAnimClip& anim)
{
	UINT depth = 0;
	LONG token = TOKEN_NONE;
	while (token = aseLexer->GetToken(tokenString), token != TOKEND_END)
	{
		if (token == TOKEND_BLOCK_START)
			depth++;
		else if (token == TOKEND_BLOCK_END)
		{
			--depth; if (depth <= 0) return;
		}
		else if (token == TOKENR_NODE_NAME)
		{
			NEXT;
			anim.name = tokenString;
		}
		else if (token == TOKENR_CONTROL_POS_SAMPLE)
		{
			NEXT;
			anim.posClip.time.push_back(rFloat);
			NEXT;
			float x = rFloat;
			NEXT;
			float z = rFloat;
			NEXT;
			float y = rFloat;
			anim.posClip.controlPos.push_back({ x,y,z });
		}
		else if (token == TOKENR_CONTROL_ROT_SAMPLE)
		{
			NEXT;
			anim.rotClip.time.push_back(rFloat);
			NEXT;
			float x = rFloat;
			NEXT;
			float z = rFloat;
			NEXT;
			float y = rFloat;
			NEXT;
			float w = rFloat;
			anim.rotClip.controlRot.push_back({ x,y,z,w });
		}
	}
}

//바꿔줄 필요 없음 실제 데이터는 이 뎁스에서 안 받음
void ASELoader::ReadMesh(ASEMesh& mesh)
{
	UINT depth = 0;
	LONG token = TOKEN_NONE;
	while (token = aseLexer->GetToken(tokenString), token != TOKEND_END)
	{
		if (token == TOKEND_BLOCK_START)
			depth++;
		else if (token == TOKEND_BLOCK_END)
		{
			--depth; if (depth <= 0) return;
		}
		else if (token == TOKENR_TIMEVALUE)
		{
			NEXT;
			mesh.timeValue = rFloat;
		}
		else if (token == TOKENR_MESH_NUMVERTEX)
		{
			NEXT;
			mesh.numVertex = rInt;
			mesh.vertexList.resize(mesh.numVertex);
		}
		else if (token == TOKENR_MESH_NUMFACES)
		{
			NEXT;
			mesh.numFaces = rInt;
			mesh.faceList.resize(mesh.numFaces);
		}
		else if (token == TOKENR_MESH_VERTEX_LIST)
			ReadVertexList(mesh.vertexList);
		else if (token == TOKENR_MESH_FACE_LIST)
			ReadFaceList(mesh.faceList);
		else if (token == TOKENR_MESH_NUMTVERTEX)
		{
			NEXT;
			mesh.numTVertex = rInt;
			mesh.tVertList.resize(mesh.numTVertex);
		}
		else if (token == TOKENR_MESH_TVERTLIST)
			ReadTVertList(mesh.tVertList);
		else if (token == TOKENR_MESH_NUMTVFACES)
		{
			NEXT;
			mesh.numTFaces = rInt;
			mesh.TFaceList.resize(mesh.numTFaces);
		}
		else if (token == TOKENR_MESH_TFACELIST)
			ReadTFaceList(mesh.TFaceList);
		else if (token == TOKENR_MESH_NORMALS)
		{
			mesh.normals.resize(mesh.numFaces);
			ReadMeshNormals(mesh.normals);
		}
	}
}

//혜성이 기준으로 바꿔줌
void ASELoader::ReadVertexList(vector<float3>& v)
{
	UINT depth = 0;
	LONG token = TOKEN_NONE;
	while (token = aseLexer->GetToken(tokenString), token != TOKEND_END)
	{
		if (token == TOKEND_BLOCK_START)
			depth++;
		else if (token == TOKEND_BLOCK_END)
		{
			--depth; if (depth <= 0) return;
		}
		else if (token == TOKENR_MESH_VERTEX)
		{
			NEXT;
			UINT index = rInt;
			NEXT;
			v[index].x = rFloat;
			NEXT;
			v[index].z = rFloat;
			NEXT;
			v[index].y = rFloat;
		}
	}
}

//혜성이 기준으로 바꿔줌
void ASELoader::ReadFaceList(vector<vector<UINT>>& f)
{
	UINT index = 0;
	UINT depth = 0;
	LONG token = TOKEN_NONE;
	while (token = aseLexer->GetToken(tokenString), token != TOKEND_END)
	{
		if (token == TOKEND_BLOCK_START)
			depth++;
		else if (token == TOKEND_BLOCK_END)
		{
			--depth; if (depth <= 0) return;
		}
		else if (token == TOKENR_MESH_FACE)
		{
			NEXT;//faceIndex + :
			NEXT;// A:
			NEXT;// A's Index
			f[index].resize(3);
			f[index][0] = rInt;
			NEXT;// B:
			NEXT;// B's Index
			f[index][2] = rInt;
			NEXT;// C:
			NEXT;// C's Index
			f[index][1] = rInt;
			index++;
		}
	}
}

void ASELoader::ReadTVertList(vector<TVert>& tV)
{
	UINT depth = 0;
	LONG token = TOKEN_NONE;
	while (token = aseLexer->GetToken(tokenString), token != TOKEND_END)
	{
		if (token == TOKEND_BLOCK_START)
			depth++;
		else if (token == TOKEND_BLOCK_END)
		{
			--depth; if (depth <= 0) return;
		}
		else if (token == TOKENR_MESH_TVERT)
		{
			NEXT;// index
			UINT index = rInt;
			NEXT;//u data
			tV[index].u = rFloat;
			NEXT;//v data
			tV[index].v = 1 - rFloat;
			NEXT;//w data
			tV[index].w = rFloat;
		}
	}
}

//혜성이 기준으로 바꿈
void ASELoader::ReadTFaceList(vector<vector<UINT>>& tF)
{
	UINT depth = 0;
	LONG token = TOKEN_NONE;
	while (token = aseLexer->GetToken(tokenString), token != TOKEND_END)
	{
		if (token == TOKEND_BLOCK_START)
			depth++;
		else if (token == TOKEND_BLOCK_END)
		{
			--depth; if (depth <= 0) return;
		}
		else if (token == TOKENR_MESH_TFACE)
		{
			NEXT;//index
			UINT index = rInt;

			tF[index].resize(3);
			NEXT; //A
			tF[index][0] = rInt;
			NEXT; //C
			tF[index][2] = rInt;
			NEXT; //B
			tF[index][1] = rInt;
		}
	}
}

//혜성이 기준으로 바꿈
void ASELoader::ReadMeshNormals(vector<vector<float3>>& fN)
{
	UINT depth = 0;
	LONG token = TOKEN_NONE;
	while (token = aseLexer->GetToken(tokenString), token != TOKEND_END)
	{
		if (token == TOKEND_BLOCK_START)
			depth++;
		else if (token == TOKEND_BLOCK_END)
		{
			--depth; if (depth <= 0) return;
		}
		else if (token == TOKENR_MESH_FACENORMAL)
		{
			NEXT;//face Index
			UINT index = rInt;
			fN[index].resize(3);
			NEXT, NEXT, NEXT;//faceNormal xyz

			NEXT; // *MESH_VERTEXNORMAL
			NEXT;//vertexList Index
			NEXT;//x
			fN[index][0].x = rFloat;
			NEXT;//z
			fN[index][0].z = rFloat;
			NEXT;//y
			fN[index][0].y = rFloat;

			NEXT; // *MESH_VERTEXNORMAL
			NEXT;//vertexList Index
			NEXT;//x
			fN[index][2].x = rFloat;
			NEXT;//zZ
			fN[index][2].z = rFloat;
			NEXT;//y
			fN[index][2].y = rFloat;

			NEXT; // *MESH_VERTEXNORMAL
			NEXT;//vertexList Index
			NEXT;//x
			fN[index][1].x = rFloat;
			NEXT;//z
			fN[index][1].z = rFloat;
			NEXT;//y
			fN[index][1].y = rFloat;
		}
	}
}

void ASELoader::ReadBones(vector<string>& bones)
{
	UINT depth = 0;
	LONG token = TOKEN_NONE;
	while (token = aseLexer->GetToken(tokenString), token != TOKEND_END)
	{
		if (token == TOKEND_BLOCK_START)
			depth++;
		else if (token == TOKEND_BLOCK_END)
		{
			--depth; if (depth <= 0) return;
		}
		else if (token == TOKENR_BONE)
		{
			NEXT;
			int index = rInt;
			ReadBone(bones[index]);
		}
	}
}

void ASELoader::ReadBone(string& bone)
{
	UINT depth = 0;
	LONG token = TOKEN_NONE;
	while (token = aseLexer->GetToken(tokenString), token != TOKEND_END)
	{
		if (token == TOKEND_BLOCK_START)
			depth++;
		else if (token == TOKEND_BLOCK_END)
		{
			--depth; if (depth <= 0) return;
		}
		else if (token == TOKENR_BONE_NAME)
		{
			NEXT;
			bone = tokenString;
		}
	}
}

void ASELoader::ReadWeights(vector<vector<ASEMeshWeight>>& weights)
{
	UINT depth = 0;
	LONG token = TOKEN_NONE;
	while (token = aseLexer->GetToken(tokenString), token != TOKEND_END)
	{
		if (token == TOKEND_BLOCK_START)
			depth++;
		else if (token == TOKEND_BLOCK_END)
		{
			--depth; if (depth <= 0) return;
		}
		else if (token == TOKENR_MESH_WEIGHT)
		{
			NEXT;
			ReadWeight(weights[rInt]);
		}
	}
}

void ASELoader::ReadWeight(vector<ASEMeshWeight>& weight)
{
	UINT depth = 0;
	LONG token = TOKEN_NONE;
	while (token = aseLexer->GetToken(tokenString), token != TOKEND_END)
	{
		if (token == TOKEND_BLOCK_START)
			depth++;
		else if (token == TOKEND_BLOCK_END)
		{
			--depth; if (depth <= 0) return;
		}
		else if (token == TOKENR_BONE_BLENGING_WEIGHT)
		{
			NEXT;
			ASEMeshWeight w;
			w.index = rInt;
			NEXT;
			w.weight = rFloat;
			weight.emplace_back(w);
		}
	}
}

ASELoader::ASELoader()
	:aseMats{}, matCount(0), aseObjects{}
{
	aseLexer = new ASELexer();
}

ASELoader::~ASELoader()
{
}

void ASELoader::CreateObjects(vector<Object*>& objects)
{
	objects.resize(aseObjects.size());
	//지오오브젝트 transform 데이터 넣어줌
	for (UINT i = 0; i < aseObjects.size(); i++)
	{
		objects[i] = new Object();
		ASEObject* src = aseObjects[i];
		Object* dest = objects[i];

		ParseName(dest, src);

		MatrixCorrection(src);

		///From now only object which has Mesh
		if (src->type != ObjectType::GEO) continue;

		//머티리얼
		CreateMaterial(dest, src);

		//탄젠트 정보 생성
		CreateTangents(src);

		VertexSet vertexSet;
		//버텍스 정보에 따라 input elementDesc 생성해주는 곳
		CreateInputElementDesc(src, vertexSet);
		//버텍스 정보 채워주는 부분
		CreateMesh(dest, src, vertexSet);
	}

	for (UINT i = 0; i < aseObjects.size(); i++)
	{
		ASEObject* src = aseObjects[i];
		Object* dest = objects[i];
		
		ConnectParentInfo(dest, src);
	}

	for (UINT i = 0; i < aseObjects.size(); i++)
	{
		ASEObject* src = aseObjects[i];
		Object* dest = objects[i];

		CreateTransform(dest, src);
	}
	CreatAnimClip(objects);
	ConnectTransformsForSkinningAnimation(objects, objPtrByName);
}

void ASELoader::CreateObject(Object* dest, ASEObject* src)
{
	ParseName(dest, src);
}

void ASELoader::ParseName(Object* dest, ASEObject* src)
{
	dest->name = src->name;

	objPtrByName.insert(make_pair(dest->name, dest));
}

void ASELoader::GeneratePreInfo(ASEObject* src)
{
}

void ASELoader::MatrixCorrection(ASEObject* src)
{
	Matrix world = XMMatrixIdentity();

	world = {
		{src->transform.row0.x,src->transform.row0.y,src->transform.row0.z,0},
		{src->transform.row1.x,src->transform.row1.y,src->transform.row1.z,0},
		{src->transform.row2.x,src->transform.row2.y,src->transform.row2.z,0},
		{src->transform.row3.x,src->transform.row3.y,src->transform.row3.z,1}
	};

	XMVECTOR determinant = XMMatrixDeterminant(world);

	if (determinant.m128_f32[0] < 0)
	{
		XMVECTOR _scale;

		XMVECTOR _rotationQuat;

		XMVECTOR _translation;

		XMMatrixDecompose(&_scale, &_rotationQuat, &_translation, world);

		_scale = -1 * _scale;

		// 스케일을 똑같은 기준으로 바꿔주고 다시 NodeTM을 조립하겠습니다 ..!
		world = XMMatrixScalingFromVector(_scale) * XMMatrixRotationQuaternion(_rotationQuat)
			* XMMatrixTranslation(_translation.m128_f32[0], _translation.m128_f32[1], _translation.m128_f32[2]);
	}


	allMatrix.insert(make_pair(src->name, world));
	determinants.insert(make_pair(src->name, determinant.m128_f32[0] < 0));
}

void ASELoader::CreateGeometryData(Object* dest, ASEObject* src)
{
	//CreateMesh(dest, src);
	//CreateMaterial(dest, src);
}

void ASELoader::CreateMesh(Object* dest, ASEObject* src, VertexSet& vertexSet)
{
	vector<UINT> indices;

	//CreateInputElementDesc(src, vertexSet);

	Matrix worldInverse = XMMatrixInverse(nullptr, allMatrix[src->name]);
	//버텍스 정보 채워주는 부분
	for (UINT j = 0; j < src->mesh.numFaces; j++)
	{
		for (UINT k = 0; k < 3; k++)
		{
			indices.push_back(3 * j + k);

			float pos[3];
			Vector3 vertexPos;

			//포지션 넣어주는 곳
			{
				vertexPos.x = src->mesh.vertexList[src->mesh.faceList[j][k]].x;
				vertexPos.y = src->mesh.vertexList[src->mesh.faceList[j][k]].y;
				vertexPos.z = src->mesh.vertexList[src->mesh.faceList[j][k]].z;
				vertexPos = vertexPos * worldInverse;

				pos[0] = vertexPos.x;
				pos[1] = vertexPos.y;
				pos[2] = vertexPos.z;
				vertexSet.AddData(pos, sizeof(pos));
			}

			//uv넣어주는 곳
			if (src->mesh.tVertList.size() > 0)
			{
				float uv[2];
				uv[0] = src->mesh.tVertList[src->mesh.TFaceList[j][k]].u;
				uv[1] = src->mesh.tVertList[src->mesh.TFaceList[j][k]].v;

				vertexSet.AddData(uv, sizeof(uv));
			}

			//normal 넣어주는 곳
			if (src->mesh.normals.size() > 0)
			{
				float normal[3];
				normal[0] = determinants[src->name] ? -src->mesh.normals[j][k].x : src->mesh.normals[j][k].x;
				normal[1] = determinants[src->name]  ? -src->mesh.normals[j][k].y : src->mesh.normals[j][k].y;
				normal[2] = determinants[src->name] ? -src->mesh.normals[j][k].z : src->mesh.normals[j][k].z;

				vertexSet.AddData(normal, sizeof(normal));
			}

			//tangent 넣어주는 곳
			if (src->mesh.tangents.size() > 0)
			{
				float tangent[3];
				tangent[0] = src->mesh.tangents[src->mesh.TFaceList[j][k]].x;
				tangent[1] = src->mesh.tangents[src->mesh.TFaceList[j][k]].y;
				tangent[2] = src->mesh.tangents[src->mesh.TFaceList[j][k]].z;

				vertexSet.AddData(tangent, sizeof(tangent));
			}

			//skinning 정보 넣어주는 곳
			if (src->weights.size() > 0)
			{
				UINT refIndex[4] = {};
				float weight[4] = {};
				for (UINT m = 0; m < src->weights[src->mesh.faceList[j][k]].size(); m++)
				{
					refIndex[m] = src->weights[src->mesh.faceList[j][k]][m].index;
					weight[m] = src->weights[src->mesh.faceList[j][k]][m].weight;
				}

				vertexSet.AddData(refIndex, sizeof(refIndex));
				vertexSet.AddData(weight, sizeof(weight));
			}
		}
	}


	dest->mesh = new Mesh(vertexSet, indices.data(), indices.size(), vertexSet.GetCorrespondingShaderFileName());

	if (src->mesh.tangents.size() > 0)
	{
		dest->mesh->SetShader(L"UVLightNormalMapVertex.hlsl");
		dest->material->SetShader(L"UVLightNormalMapPixel.hlsl");
	}
}

void ASELoader::CreateMaterial(Object* dest, ASEObject* src)
{
	//머티리얼
	dest->material = new Material();
	if (src->matRef > -1)
	{
		if (aseMats.size() > 0)
		{
			if (aseMats[src->matRef].diffuseMap.fileName.length() > 0)
			{
				string str = aseMats[src->matRef].diffuseMap.fileName;
				wstring wstr(str.begin(), str.end());
				dest->material->SetDiffuseMap(wstr);
				dest->material->SetShader(L"UVLightPixel.hlsl");
			}

			if (aseMats[src->matRef].normalMap.fileName.length() > 0)
			{
				string str1 = aseMats[src->matRef].normalMap.fileName;
				wstring wstr1(str1.begin(), str1.end());
				dest->material->SetNormalMap(wstr1);
			}
		}
	}
}

void ASELoader::CreateTangents(ASEObject* src)
{
	if (src->matRef > -1)
	{
		if (aseMats[src->matRef].normalMap.fileName.length() > 0)
		{
			src->mesh.tangents.resize(src->mesh.tVertList.size());
			for (UINT j = 0; j < src->mesh.numFaces; j++)
			{
				struct tempVertex
				{
					float3 position;
					float2 uv;
				};
				tempVertex A, B, C;

				//포지션 정보를 갖고 있는 컨테이너 vertexList 의 인덱스정보
				int aIndex = src->mesh.faceList[j][0];
				int bIndex = src->mesh.faceList[j][1];
				int cIndex = src->mesh.faceList[j][2];

				A.position = src->mesh.vertexList[aIndex];
				B.position = src->mesh.vertexList[bIndex];
				C.position = src->mesh.vertexList[cIndex];

				//텍스쳐 uv 좌표의 정보를 갖고 있는 tVertList 컨테이너의 인덱스정보
				int aTIndex = src->mesh.TFaceList[j][0];
				int bTIndex = src->mesh.TFaceList[j][1];
				int cTIndex = src->mesh.TFaceList[j][2];

				A.uv.x = src->mesh.tVertList[aTIndex].u;
				A.uv.y = src->mesh.tVertList[aTIndex].v;

				B.uv.x = src->mesh.tVertList[bTIndex].u;
				B.uv.y = src->mesh.tVertList[bTIndex].v;

				C.uv.x = src->mesh.tVertList[cTIndex].u;
				C.uv.y = src->mesh.tVertList[cTIndex].v;

				Vector3 e0;
				e0.w = 0.0f;
				e0.x = B.position.x - A.position.x;
				e0.y = B.position.y - A.position.y;
				e0.z = B.position.z - A.position.z;

				Vector3 e1;
				e1.w = 0.0f;
				e1.x = C.position.x - A.position.x;
				e1.y = C.position.y - A.position.y;
				e1.z = C.position.z - A.position.z;

				float u0Delta = B.uv.x - A.uv.x;
				float u1Delta = C.uv.x - A.uv.x;
				float v0Delta = B.uv.y - A.uv.y;
				float v1Delta = C.uv.y - A.uv.y;

				Vector3 T;
				T.x = (1 / (u0Delta * v1Delta - v0Delta * u1Delta)) * (v1Delta * e0.x - v0Delta * e1.x);
				T.y = (1 / (u0Delta * v1Delta - v0Delta * u1Delta)) * (v1Delta * e0.y - v0Delta * e1.y);
				T.z = (1 / (u0Delta * v1Delta - v0Delta * u1Delta)) * (v1Delta * e0.z - v0Delta * e1.z);

				T.Normalize();

				src->mesh.tangents.at(src->mesh.TFaceList[j][0]) += T;
				src->mesh.tangents.at(src->mesh.TFaceList[j][1]) += T;
				src->mesh.tangents.at(src->mesh.TFaceList[j][2]) += T;
			}

			//탄젠트 정규화 (평균 느낌으로다가)
			for (Vector3& Tangent : src->mesh.tangents)
				Tangent.Normalize();
		}
	}
}

void ASELoader::ConnectParentInfo(Object* dest, ASEObject* src)
{
	if (src->parentName.length() > 0)
		dest->transform.parent = &(objPtrByName.at(src->parentName)->transform);
}

void ASELoader::CreateInputElementDesc(ASEObject* src, VertexSet& vertexSet)
{
	wstring shaderFileName = L"LightVertex.hlsl";
	vertexSet.AddElementToDesc(sizeof(float) * 3, DataType::FLOAT, "POSITION");

	if (src->mesh.tVertList.size() > 0)
	{
		vertexSet.AddElementToDesc(sizeof(float) * 2, DataType::FLOAT, "UV");
		shaderFileName = L"UVLightVertex.hlsl";
	}
	if (src->mesh.normals.size() > 0)
		vertexSet.AddElementToDesc(sizeof(float) * 3, DataType::FLOAT, "NORMAL");
	if (src->mesh.tangents.size() > 0)
		vertexSet.AddElementToDesc(sizeof(float) * 3, DataType::FLOAT, "TANGENT");
	if (src->weights.size() > 0)
	{
		shaderFileName = L"SkinningAnimationVertex.hlsl";//todo : 임시 코드
		vertexSet.AddElementToDesc(sizeof(UINT) * 4, DataType::UINT, "REFINDEX");
		vertexSet.AddElementToDesc(sizeof(float) * 4, DataType::FLOAT, "WEIGHT");
	}

}

void ASELoader::CreateTransform(Object* dest, ASEObject* src)
{
	Matrix local = allMatrix.at(src->name);

	if (src->parentName.length() > 0)
	{
		Matrix parentWorld = allMatrix.at(src->parentName);

		Matrix parentWorldInv = XMMatrixInverse(nullptr, parentWorld);

		local *= parentWorldInv;
	}

	XMVECTOR pos, scale, rot;

	XMMatrixDecompose(&scale, &rot, &pos, local);

	dest->transform.scale.x = scale.m128_f32[0];
	dest->transform.scale.y = scale.m128_f32[1];
	dest->transform.scale.z = scale.m128_f32[2];

	dest->transform.pos.x = pos.m128_f32[0];
	dest->transform.pos.y = pos.m128_f32[1];
	dest->transform.pos.z = pos.m128_f32[2];


	JWMath::QuatToEuler(dest->transform.rot.x, dest->transform.rot.y, dest->transform.rot.z,
		move(rot.m128_f32[0]), move(rot.m128_f32[1]), move(rot.m128_f32[2]), move(rot.m128_f32[3]));

}

void ASELoader::CreateAnimClip(Object* dest, ASEObject* src)
{
	//transform 데이터 넣어줌

		//animation정보가 있다면
	if (src->animClip.name.length() > 0)
	{
		//애니메이터를 생성해주고
		dest->animator = new MeshAnimator();

		dest->animator->SetTransform(&(dest->transform));

		//정보를 담을 구조체
		MeshAnimClip* input = new MeshAnimClip();

		//이름을 입력
		input->name = src->animClip.name;


		//pos만 있는지 rot만 있는지 둘다 있는지

		//pos Frames
		for (UINT j = 0; j < src->animClip.posClip.time.size(); j++)
		{
			input->posFrames.push_back(new MeshPosFrame());
			input->posFrames.back()->time = src->animClip.posClip.time[j] / 1000.0f;
			input->posFrames.back()->pos = src->animClip.posClip.controlPos[j];
		}


		XMVECTOR lastQuaternion;
		//pos Frames
		for (UINT j = 0; j < src->animClip.rotClip.time.size(); j++)
		{
			input->rotFrames.push_back(new MeshRotFrame());
			input->rotFrames.back()->time = src->animClip.rotClip.time[j] / 1000.0f;

			XMVECTOR _axis = XMVectorSet(
				src->animClip.rotClip.controlRot[j].x,
				src->animClip.rotClip.controlRot[j].y,
				src->animClip.rotClip.controlRot[j].z, 0.f);

			XMQuaternionRotationAxis(_axis, src->animClip.rotClip.controlRot[j].w);
			XMVECTOR _quarternion = XMQuaternionRotationAxis(_axis, src->animClip.rotClip.controlRot[j].w);

			if (j > 0)
			{
				_quarternion = XMQuaternionMultiply(lastQuaternion, _quarternion);
			}

			lastQuaternion = _quarternion;

			JWMath::QuatToEuler(input->rotFrames.back()->rot.x, input->rotFrames.back()->rot.y, input->rotFrames.back()->rot.z,
				move(_quarternion.m128_f32[0]),
				move(_quarternion.m128_f32[1]),
				move(_quarternion.m128_f32[2]),
				move(_quarternion.m128_f32[3]));
		}

		dest->animator->AddAnimClip(input);
	}
}

void ASELoader::ConnectTransformsForSkinningAnimation(Object* dest, ASEObject* src)
{
		if (src->weights.size() > 0)
		{
			dest->mesh->SetShader(L"SkinningAnimationVertex.hlsl");

			dest->skinner = new SkinningAnimator(dest);

			UINT boneNum = src->boneList.size();

			dest->skinner->refTransforms.resize(boneNum);

			for (UINT j = 0; j < boneNum; j++)
			{
				dest->skinner->refTransforms[j] = &(objPtrByName.at(src->boneList[j])->transform);
				objPtrByName.at(src->boneList[j])->transform.UpdateWorld();

				dest->skinner->skinWorldTMInv.push_back(XMMatrixInverse(nullptr, *(objPtrByName.at(src->boneList[j])->transform.World())));
			}
		}
	

}

void ASELoader::CreatAnimClip(vector<Object*>& objects)
{
	//transform 데이터 넣어줌
	for (UINT i = 0; i < aseObjects.size(); i++)
	{
		ASEObject* from = aseObjects[i];
		Object* o = objects[i];

		//animation정보가 있다면
		if (from->animClip.name.length() > 0)
		{
			//애니메이터를 생성해주고
			o->animator = new MeshAnimator();

			o->animator->SetTransform(&(o->transform));

			//정보를 담을 구조체
			MeshAnimClip* input = new MeshAnimClip();

			//이름을 입력
			input->name = from->animClip.name;


			//pos만 있는지 rot만 있는지 둘다 있는지

			//pos Frames
			for (UINT j = 0; j < from->animClip.posClip.time.size(); j++)
			{
				input->posFrames.push_back(new MeshPosFrame());
				input->posFrames.back()->time = from->animClip.posClip.time[j] / 1000.0f;
				input->posFrames.back()->pos = from->animClip.posClip.controlPos[j];
			}


			XMVECTOR lastQuaternion;
			//pos Frames
			for (UINT j = 0; j < from->animClip.rotClip.time.size(); j++)
			{
				input->rotFrames.push_back(new MeshRotFrame());
				input->rotFrames.back()->time = from->animClip.rotClip.time[j] / 1000.0f;

				XMVECTOR _axis = XMVectorSet(
					from->animClip.rotClip.controlRot[j].x,
					from->animClip.rotClip.controlRot[j].y,
					from->animClip.rotClip.controlRot[j].z, 0.f);

				XMQuaternionRotationAxis(_axis, from->animClip.rotClip.controlRot[j].w);
				XMVECTOR _quarternion = XMQuaternionRotationAxis(_axis, from->animClip.rotClip.controlRot[j].w);

				if (j > 0)
				{
					_quarternion = XMQuaternionMultiply(lastQuaternion, _quarternion);
				}

				lastQuaternion = _quarternion;

				JWMath::QuatToEuler(input->rotFrames.back()->rot.x, input->rotFrames.back()->rot.y, input->rotFrames.back()->rot.z,
					move(_quarternion.m128_f32[0]),
					move(_quarternion.m128_f32[1]),
					move(_quarternion.m128_f32[2]),
					move(_quarternion.m128_f32[3]));
			}

			o->animator->AddAnimClip(input);
		}
	}
}

void ASELoader::ConnectTransformsForSkinningAnimation(vector<Object*>& objects, unordered_map<string, Object*>& objPtrByName)
{
	for (UINT i = 0; i < aseObjects.size(); i++)
	{
		ASEObject* from = aseObjects[i];
		Object* o = objects[i];

		if (from->type != ObjectType::GEO) continue;

		if (from->weights.size() > 0)
		{

			o->skinner = new SkinningAnimator(o);

			UINT boneNum = from->boneList.size();

			o->skinner->refTransforms.resize(boneNum);

			for (UINT j = 0; j < boneNum; j++)
			{
				o->skinner->refTransforms[j] = &(objPtrByName.at(from->boneList[j])->transform);
				objPtrByName.at(from->boneList[j])->transform.UpdateWorld();

				o->skinner->skinWorldTMInv.push_back(XMMatrixInverse(nullptr, *(objPtrByName.at(from->boneList[j])->transform.World())));
			}
		}
	}

}
