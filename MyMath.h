#pragma once
#include "KamataEngine.h"

using namespace KamataEngine;

struct AABB 
{
	Vector3 min;
	Vector3 max;
};

/// <summary>
/// アフィン変換
/// </summary>


KamataEngine::Matrix4x4 MakeAffineMatrix(KamataEngine::Vector3& scale, KamataEngine::Vector3& rotation, KamataEngine::Vector3& translation);

class MyMath {};

float EaseInOut(float x1, float, float t);

bool IsCollision(const AABB& aabb1, const AABB& aabb2);