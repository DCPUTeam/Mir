///
/// @brief The class declaration for the texture manager.
/// @author James Rhodes
///

#ifndef __MIR_TEXTURE_MANAGER_H
#define __MIR_TEXTURE_MANAGER_H

#include <GL/glew.h>
#include <GL/glfw3.h>
#include <string>
#include <map>

///
/// @brief Loads and manages textures.
///
class TextureManager
{
private:
    TextureManager();
    GLuint _LoadTexture(std::string path);
    void _FreeTexture(GLuint id);

    std::map<std::string, GLuint> m_Textures;
    static TextureManager* m_Instance;
public:
    static void LoadTexture(std::string name, std::string path);
    static GLuint GetTexture(std::string name);
};

#endif
