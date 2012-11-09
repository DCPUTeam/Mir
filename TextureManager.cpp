///
/// @brief The implementation of the texture manager.
/// @author James Rhodes
///

#include <iostream>
#include <stdlib.h>
#include <stdint.h>
#include <png.h>
#include "TextureManager.h"

///
/// @source http://blog.nobel-joergensen.com/2010/11/07/loading-a-png-as-texture-in-opengl-using-libpng/
///
bool LoadPNG(const char *name, int &outWidth, int &outHeight, bool &outHasAlpha, GLubyte **outData)
{
    png_structp png_ptr;
    png_infop info_ptr;
    unsigned int sig_read = 0;
    int color_type, interlace_type;
    FILE *fp;

    if ((fp = fopen(name, "rb")) == NULL)
        return false;

    /* Create and initialize the png_struct
     * with the desired error handler
     * functions.  If you want to use the
     * default stderr and longjump method,
     * you can supply NULL for the last
     * three parameters.  We also supply the
     * the compiler header file version, so
     * that we know if the application
     * was compiled with a compatible version
     * of the library.  REQUIRED
     */
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
            NULL, NULL, NULL);

    if (png_ptr == NULL) {
        fclose(fp);
        return false;
    }

    /* Allocate/initialize the memory
     * for image information.  REQUIRED. */
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        fclose(fp);
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return false;
    }

    /* Set error handling if you are
     * using the setjmp/longjmp method
     * (this is the normal method of
     * doing things with libpng).
     * REQUIRED unless you  set up
     * your own error handlers in
     * the png_create_read_struct()
     * earlier.
     */
    if (setjmp(png_jmpbuf(png_ptr))) {
        /* Free all of the memory associated
         * with the png_ptr and info_ptr */
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        /* If we get here, we had a
         * problem reading the file */
        return false;
    }

    /* Set up the output control if
     * you are using standard C streams */
    png_init_io(png_ptr, fp);

    /* If we have already
     * read some of the signature */
    png_set_sig_bytes(png_ptr, sig_read);

    /*
     * If you have enough memory to read
     * in the entire image at once, and
     * you need to specify only
     * transforms that can be controlled
     * with one of the PNG_TRANSFORM_*
     * bits (this presently excludes
     * dithering, filling, setting
     * background, and doing gamma
     * adjustment), then you can read the
     * entire image (including pixels)
     * into the info structure with this
     * call
     *
     * PNG_TRANSFORM_STRIP_16 |
     * PNG_TRANSFORM_PACKING  forces 8 bit
     * PNG_TRANSFORM_EXPAND forces to
     *  expand a palette into RGB
     */
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);

    outWidth = png_get_image_width(png_ptr, info_ptr);
    outHeight = png_get_image_width(png_ptr, info_ptr);
    switch (png_get_color_type(png_ptr, info_ptr)) {
        case PNG_COLOR_TYPE_RGBA:
            outHasAlpha = true;
            break;
        case PNG_COLOR_TYPE_RGB:
            outHasAlpha = false;
            break;
        default:
            std::cout << "Color type " << png_get_color_type(png_ptr, info_ptr) << " not supported" << std::endl;
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            fclose(fp);
            return false;
    }
    unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    *outData = (unsigned char*) malloc(row_bytes * outHeight);

    png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);

    for (int i = 0; i < outHeight; i++) {
        // note that png is ordered top to
        // bottom, but OpenGL expect it bottom to top
        // so the order or swapped
        memcpy(*outData+(row_bytes * (outHeight-1-i)), row_pointers[i], row_bytes);
    }

    /* Clean up after the read,
     * and free any memory allocated */
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    /* Close the file */
    fclose(fp);

    /* That's it */
    return true;
    /*
    unsigned int sig_read = 0;

    FILE* file = fopen(name, "rb");
    if (file == NULL)
        return NULL;

    // Create structures.
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL)
    {
        fclose(file);
        return NULL;
    }
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL)
    {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(file);
        return NULL;
    }

    // Error handling.
    if (setjmp(png_jmpbuf(png_ptr)) != 0)
    {
        // FIXME: Report back the actual error?
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(file);
        return NULL;
    }

    // Read initial PNG data.
    png_init_io(png_ptr, file);
    png_set_sig_bytes(png_ptr, sig_read);
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);
    *width = png_get_image_width(png_ptr, info_ptr);
    *height = png_get_image_height(png_ptr, info_ptr);
    png_uint_32 bit_depth, color_type;
    bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    color_type = png_get_color_type(png_ptr, info_ptr);
    //if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    //    png_set_gray_1_2_4_to_8(png_ptr);
    if (bit_depth == 16)
        png_set_strip_16(png_ptr);
    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png_ptr);
    else if (color_type == PNG_COLOR_TYPE_GRAY ||
             color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png_ptr);
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png_ptr);
    else
        png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER);
    png_read_update_info(png_ptr, info_ptr);

    // Read in the color bytes and convert.
    png_uint_32 rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    png_uint_32 numbytes = rowbytes*(*height);
    png_byte* pixels = (png_byte*)malloc(numbytes);
    png_byte** row_ptrs = (png_byte**)malloc((*height) * sizeof(png_byte*));
    for (int i = 0; i < *height; i++)
        row_ptrs[i] = pixels + ((*height) - 1 - i) * rowbytes;
    png_read_image(png_ptr, row_ptrs);
    free(row_ptrs);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(file);

    // Return data.
    return (unsigned char*)pixels;*/
}

///
/// @source http://en.wikibooks.org/wiki/OpenGL_Programming/Intermediate/Textures
/// @source http://blog.nobel-joergensen.com/2010/11/07/loading-a-png-as-texture-in-opengl-using-libpng/
///
GLuint TextureManager::_LoadTexture(std::string path)
{
    int width;
    int height;
    bool hasAlpha;
    GLubyte *data;
    if (!LoadPNG(path.c_str(), width, height, hasAlpha, &data))
    {
        std::cerr << "UNABLE TO LOAD TEXTURE FROM " << path << "!" << std::endl;
        return -1;
    }

    // Allocate texture.
    GLuint texture;
    glGenTextures(1, &texture);

    // Set current texture as active.
    glBindTexture(GL_TEXTURE_2D, texture);

    // Select modulate to mix texture with color for shading.
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_DECAL);

    // When texture area is small, bilinear filter the closest mipmap.
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    // When texture area is large, bilinear filter the first mipmap.
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Texture should tile.
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Build texture mipmaps.
    gluBuild2DMipmaps(GL_TEXTURE_2D, hasAlpha ? 4 : 3, width, height, hasAlpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);

    // Return texture ID.
    return texture;
}

void TextureManager::_FreeTexture(GLuint id)
{
    // FIXME: Implement this.
}

TextureManager* TextureManager::m_Instance = NULL;

TextureManager::TextureManager()
{
}

///
/// @brief Loads a texture from file if not previously loaded.
///
/// @param name The name to assign this texture.
/// @param path The path to load the PNG texture from.
///
void TextureManager::LoadTexture(std::string name, std::string path)
{
    if (TextureManager::m_Instance == NULL)
        TextureManager::m_Instance = new TextureManager();
    GLuint id = TextureManager::m_Instance->_LoadTexture(path);
    if (id != -1)
        TextureManager::m_Instance->m_Textures.insert(std::pair<std::string, GLuint>(name, id));
}

///
/// @brief Returns the ID of the texture with the specified name.
///
GLuint TextureManager::GetTexture(std::string name)
{
    if (TextureManager::m_Instance == NULL)
        TextureManager::m_Instance = new TextureManager();
    std::map<std::string, GLuint>::iterator i = TextureManager::m_Instance->m_Textures.find(name);
    if (i == TextureManager::m_Instance->m_Textures.end())
        return -1;
    else
        return i->second;
}