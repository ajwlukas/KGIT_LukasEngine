#pragma once

class ASELoader
{
/// general Code
private:
	/// datas starts here
	struct DiffuseMap
	{
		string fileName;
	};
	struct NormalMap
	{
		string fileName;
	};

	struct ASEMat
	{
		string name;
		float3 ambient;
		float3 diffuse;
		float3 specular;
		DiffuseMap diffuseMap;

		NormalMap normalMap;
	};
	vector<ASEMat> aseMats;
	UINT matCount;

	struct AseTransform
	{
		float3 row0, row1, row2, row3;
		float3 pos, rotAxis, scale, scaleAxis;
		float rotAngle, scaleAxisAngle;
	};

	struct TVert
	{
		float u, v, w;
	};


	struct ASEMeshWeight
	{
		UINT index;
		float weight;
	};

	struct ASEMesh
	{
		float timeValue;

		UINT numVertex;
		vector<float3> vertexList;

		UINT numFaces;
		vector<vector<UINT>> faceList;

		UINT numTVertex;
		vector<TVert> tVertList;

		UINT numTFaces;
		vector<vector<UINT>> TFaceList;

		vector<vector<float3>> normals;

		vector<Vector3> tangents;//ASE에 들어있는 정보는 아니다.

	};

	struct AnimClipPos
	{
		vector<float> time;
		vector<float3> controlPos;
	};
	
	struct AnimClipRot
	{
		vector<float> time;
		vector<float4> controlRot;//axis float3 + rot flaot
	};

	struct ASEAnimClip
	{
		string name;
		AnimClipPos posClip;
		AnimClipRot rotClip;
	};

	enum class ObjectType
	{
		NONE,
		HELPER,
		GEO,
	};


	struct ASEObject
	{
		ObjectType type;
		string name;
		string parentName;
		AseTransform transform;
		ASEAnimClip animClip;
		ASEMesh mesh;

		int matRef = -1;
		vector<string> boneList;
		vector<vector<ASEMeshWeight>> weights;//weights[vertexIndex][weightIndex]
	};
	vector<ASEObject*> aseObjects;
	/// data ends here

	
public:
	void LoadASE(string fileName);
private:
	void InitVariables();

private:
	string fileName;

	void ReadHelperObject();

	void ReadMaterials();
	void ReadMaterial();
	void ReadDiffuseMap(UINT curMatIndex);
	void ReadNormalMap(UINT curMatIndex);


	void ReadGeoObject();
	void ReadTransform(AseTransform& trans);

	void ReadAnimClip(ASEAnimClip& anim);

	//mesh
	void ReadMesh(ASEMesh& mesh);
	void ReadVertexList(vector<float3>& v);
	void ReadFaceList(vector<vector<UINT>>& f);

	//texture
	void ReadTVertList(vector<TVert>& tV);
	void ReadTFaceList(vector<vector<UINT>>& tF);

	//normal
	void ReadMeshNormals(vector<vector<float3>>& fN);

	//weight
	void ReadBones(vector<string>& bones);
	void ReadBone(string& bone);
	void ReadWeights(vector<vector<ASEMeshWeight>>& weights);
	void ReadWeight(vector<ASEMeshWeight>& weight);
public:
	ASELoader();
	~ASELoader();

	private:
	char tokenString[256] = {};
	class ASELexer* aseLexer;

public:
	///custom Code

	//joe Mesh Animation
	void CreateObjects(vector<Object*>& objects);


private:
	void CreateObject(Object* dest, ASEObject* src);

	//사전에 만들어 놓을 정보들, 매트릭스 제대로 & 탄젠트 만들기;
	void GeneratePreInfo(ASEObject* src);

	void MatrixCorrection(ASEObject* src);
	void CreateTangents(ASEObject* src);
	
	void ParseName(Object* dest, ASEObject* src);
	void CreateGeometryData(Object* dest, ASEObject* src);
	void CreateMesh(Object* dest, ASEObject* src,VertexSet& vertexSet);
	void CreateMaterial(Object* dest, ASEObject* src);
	void CreateInputElementDesc(ASEObject* src, VertexSet& vertexSet);
	void ConnectParentInfo(Object* dest, ASEObject* src);
	void CreateTransform(Object* dest, ASEObject* src);
	void CreateAnimClip(Object* dest, ASEObject* src);
	void ConnectTransformsForSkinningAnimation(Object* dest, ASEObject* src);


	void CreatAnimClip(vector<Object*>& objects);
	//skinning animation
	void ConnectTransformsForSkinningAnimation(vector<Object*>& objects, unordered_map<string, Object*>& objPtrByName);

	//임시 작업 공간
	unordered_map<string, Object*> objPtrByName;//transform parent purpose
	unordered_map<string, Matrix> allMatrix;
	unordered_map<string, bool> determinants;
};