#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"

class FollowCamera {
public:

	
	void Initialize();

	void Update();

	
	const ViewProjection& GetViewProjection() { return viewProjection_; }


private:
	ViewProjection viewProjection_;
	const WorldTransform* target_ = nullptr;
};