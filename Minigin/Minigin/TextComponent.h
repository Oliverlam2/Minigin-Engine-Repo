#pragma once
#include <SDL_pixels.h>
#include "GraphicsComponent.h"
#include "Scene.h"
#include "Transform.h"

namespace dae {
	class Font;
	class Texture2D;
}

using namespace dae;
class TextComponent final : public GraphicsComponent
{
public:
	TextComponent(const std::string& font, unsigned int fontSize, SDL_Color color, Scene& scene, const std::string& text = " ");
	TextComponent() = default;
	virtual ~TextComponent() = default;
	TextComponent(const TextComponent & other) = delete;
	TextComponent(TextComponent && other) = delete;
	void operator=(const TextComponent & rhs) = delete;
	void operator=(const TextComponent && rhs) = delete;
	
	virtual void Update(float elapsedSec, GameObject& obj) override;
	void ObsUpdate();

	void SetText(std::string newText);
	void SetPos(const Transform& pos);

	void Render() const;
private:
	bool m_NeedsUpdate;
	std::string m_Text;
	std::shared_ptr<Font> m_pFont;
	SDL_Color m_Color;
	std::shared_ptr<Texture2D> m_pTexture;
	Transform m_Transform;
};