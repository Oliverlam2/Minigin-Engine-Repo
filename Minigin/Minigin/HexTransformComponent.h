#pragma once
#include "GameObject.h"
#include "TransformComponent.h"
#include "utils.h"

using namespace dae;
class HexTransformComponent : public TransformComponent
{
public:	
	enum class Direction
	{
		topLeft,
		topRight,
		downLeft,
		downRight
	};

	HexTransformComponent(std::vector<utils::Tile>& grid);
	virtual ~HexTransformComponent() = default;
	HexTransformComponent(const HexTransformComponent& other) = delete;
	HexTransformComponent(HexTransformComponent&& other) = delete;
	void operator=(const HexTransformComponent& rhs) = delete;
	void operator=(const HexTransformComponent&& rhs) = delete;

	virtual void Update(float elapsedSec, GameObject& obj) override;
	void Move(Direction direction);

	std::pair<int, int> GetRowCol();
protected:
	std::map<std::pair<int, int>, utils::Tile*> m_Grid;
	const float m_OffsetX{ 12.5f }, m_OffsetY{ 45.f };
	int m_Row, m_Col;
	bool m_NeedsUpdate;
	float m_Timer;
};