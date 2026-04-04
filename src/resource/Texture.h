#pragma once
#include <string>

class Texture {
public:
    Texture(const std::string& path);
    ~Texture();
    void Bind(unsigned int slot = 0) const;
    void Unbind() const;

    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }

private:
    unsigned int m_id;
    int m_width, m_height, m_channels;
};