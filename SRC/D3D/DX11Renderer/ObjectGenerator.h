#pragma once

struct MaterialDesc;
struct VertexDesc;
struct ObjectDesc
{
	MaterialDesc matDesc;

	
};

class ObjectGenerator
{
public:
	ObjectGenerator();
	~ObjectGenerator();

	Object* GenerateObject(const ObjectDesc& objectDesc);
};