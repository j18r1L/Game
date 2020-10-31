#ifndef TEXTURE2DCOMPONENT_H
#define TEXTURE2DCOMPONENT_H

#include "HartEng/Scene/Component.h"
#include "HartEng/Renderer/Texture.h"

namespace HE
{
    class Texture2DComponent: public Component
    {
    private:
        std::shared_ptr<Texture2D> m_Texture;

    public:
        Texture2DComponent();
        Texture2DComponent(Entity* entityHandle);
        Texture2DComponent(Entity* entityHandle, const std::string& filepath);
        Texture2DComponent(Entity* entityHandle, std::shared_ptr<Texture2D> texture);
        Texture2DComponent(Texture2DComponent&& other) = default;
        Texture2DComponent(Texture2DComponent& other) = default;
        virtual ~Texture2DComponent() = default;

        void SetImage(std::shared_ptr<Texture2D> texture);

        const Texture2D& GetTexture() const;
        void Bind(uint32_t id) const;
    };
}

#endif // TEXTURE2DCOMPONENT_H
