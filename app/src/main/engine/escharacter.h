#ifndef ESCHARACTER_H
#define ESCHARACTER_H

#include <map>
using std::map;
using std::pair;

// FreeType
#include "ft2build.h"
#include FT_FREETYPE_H

#include "glm/glm.hpp"
using glm::ivec2;

#include "esutil.h"
#include "esfile.h"
#include "eslogger.h"

/// Holds all state information relevant to a character as loaded using FreeType
struct ESCharacter {
    GLuint TextureID;    // ID handle of the glyph texture
    glm::ivec2 Size;     // Size of glyph
    glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
    GLuint Advance;      // Horizontal offset to advance to next glyph
};

class ESCharactersHolder {
    FT_Library ft;
    FT_Face face;        // Load font as face

    //map from codepoint to character
    map<uint32_t, ESCharacter> Characters;

    //GL buffer for render
    GLuint vboHandle;
    GLuint vaoHandle;

    //px for load per glyph
    static const FT_UInt pixel_height = 48;


    // Copyright (c) 2008-2009 Bjoern Hoehrmann <bjoern@hoehrmann.de>
    // See http://bjoern.hoehrmann.de/utf-8/decoder/dfa/ for details.

    static const uint32_t UTF8_ACCEPT = 0;
    static const uint32_t UTF8_REJECT = 1;

    static const uint8_t utf8d[];

    uint32_t inline decodeUTF8(uint32_t* state, uint32_t* codep, uint32_t byte) {
      uint32_t type = utf8d[byte];

      *codep = (*state != UTF8_ACCEPT) ?
        (byte & 0x3fu) | (*codep << 6) :
        (0xff >> type) & (byte);

      *state = utf8d[256 + *state*16 + type];
      return *state;
    }

public:
    ESCharactersHolder() {
        // Configure VAO/VBO for texture quads
        glGenVertexArrays(1, &vaoHandle);
        glGenBuffers(1, &vboHandle);
        glBindVertexArray(vaoHandle);
        glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    ~ESCharactersHolder() {
        clearCharacters();
        doneFreeTypeLoad();
        glDeleteVertexArrays(1, &vaoHandle);
        glDeleteBuffers(1, &vboHandle);
    }

    void loadFont(const char *fontPath) {
        FT_Error retCode = 0;
        // All functions return a value different than 0 whenever an error occurred
        retCode = FT_Init_FreeType(&ft);
        if (retCode) {
            ESLogger::checkFreeTypeError(retCode, __FILE__, __LINE__);
        }

        esFile *fontFile = ESFileWrapper::esFileOpen(fontPath);
        long size = ESFileWrapper::esFileGetLength(fontFile);
        unsigned char* buffer = (unsigned char*) malloc (sizeof(unsigned char) *size);
        ESFileWrapper::esFileRead( fontFile, size, buffer);

        // Load font as face
        retCode = FT_New_Memory_Face(ft, buffer, size, 0, &face);
        if (retCode) {
            ESLogger::checkFreeTypeError(retCode, __FILE__, __LINE__);
        }

        // Set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, pixel_height);
    }

    void clearCharacters() {
        Characters.clear();
    }

    void doneFreeTypeLoad() {
        // Destroy FreeType once we're finished current font type render
        if (face != NULL) {
            FT_Done_Face(face);
            face = NULL;
        }
        if (ft != NULL) {
            FT_Done_FreeType(ft);
            ft = NULL;
        }
    }

    void displayText(const char *text, GLfloat x, GLfloat y, GLfloat scale) {
        addCharactersUtf8(text);
        renderTextUtf8(text, x, y, scale);
    }

    void addCharactersUtf8(const char *str) {

        // Disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        FT_Error retCode = 0;
        uint32_t codepoint = 0;
        uint32_t state = 0;   //state maintain for decodeUTF8 funcs

        for (; *str; ++str) {
            if (decodeUTF8(&state, &codepoint, *(unsigned char*)str))
                continue;
            // Load character glyph by codepoint
            retCode = FT_Load_Char(face, codepoint, FT_LOAD_RENDER);
            if (retCode) {
                ESLogger::checkFreeTypeError(retCode, __FILE__, __LINE__);
                continue;
            }
            // Escape keycode which already in the map
            if (Characters.count(codepoint) > 0){
                continue;
            }
            // Generate texture
            GLuint texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            // Set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // Now store character for later use
            ESCharacter character = {
                texture,
                ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x
            };
            Characters.insert(pair<uint32_t, ESCharacter>(codepoint, character));

        }
        glBindTexture(GL_TEXTURE_2D, 0);

        // Disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    }

    void renderTextUtf8(const char* text, GLfloat x, GLfloat y, GLfloat scale) {
        // Activate corresponding render state
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(vaoHandle);

        // Iterate through all characters
        uint32_t codepoint = 0;
        uint32_t state = 0;   //state maintain for decodeUTF8 funcs

        for (; *text; ++text) {
            if (decodeUTF8(&state, &codepoint, *(unsigned char*)text))
                continue;
            ESCharacter ch = Characters[codepoint];

            GLfloat xpos = x + ch.Bearing.x * scale;
            GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

            GLfloat w = ch.Size.x * scale;
            GLfloat h = ch.Size.y * scale;
            // Update VBO for each character
            GLfloat vertices[6][4] = {
                { xpos,     ypos + h,   0.0, 0.0 },
                { xpos,     ypos,       0.0, 1.0 },
                { xpos + w, ypos,       1.0, 1.0 },

                { xpos,     ypos + h,   0.0, 0.0 },
                { xpos + w, ypos,       1.0, 1.0 },
                { xpos + w, ypos + h,   1.0, 0.0 }
            };
            // Render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            // Update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // Render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

};

const uint8_t ESCharactersHolder::utf8d[] = {
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 00..1f
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 20..3f
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 40..5f
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 60..7f
      1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, // 80..9f
      7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7, // a0..bf
      8,8,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, // c0..df
      0xa,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x4,0x3,0x3, // e0..ef
      0xb,0x6,0x6,0x6,0x5,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8, // f0..ff
      0x0,0x1,0x2,0x3,0x5,0x8,0x7,0x1,0x1,0x1,0x4,0x6,0x1,0x1,0x1,0x1, // s0..s0
      1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,1, // s1..s2
      1,2,1,1,1,1,1,2,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1, // s3..s4
      1,2,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,3,1,3,1,1,1,1,1,1, // s5..s6
      1,3,1,1,1,1,1,3,1,3,1,1,1,1,1,1,1,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1, // s7..s8
    };

#endif //ESCHARACTER_H

