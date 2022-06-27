#pragma once

class Object;
class Camera;

class Grid : public Object
{
public:
	Grid(Camera* camera);

	virtual void Update() override;
	virtual void Render() override;

private:
	void CreateGrid();

	Camera* cam;
};