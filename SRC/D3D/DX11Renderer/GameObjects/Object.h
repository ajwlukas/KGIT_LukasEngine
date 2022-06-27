#pragma once

class Object
{
public:
	Object();
	virtual ~Object();

	virtual void Update();
	virtual void Render();

	Transform transform;

	string name;


	Mesh* mesh;
	Material* material;

	MeshAnimator* animator;
	SkinningAnimator* skinner;
};