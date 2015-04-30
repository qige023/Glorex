#ifndef ESPROGRAM_H
#define ESPROGRAM_H

#include "esutil.h"

#include <string>
using std::string;
#include <map>

#include <glm/glm.hpp>
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::mat3;

#include <stdexcept>

class ESProgramException : public std::runtime_error{
  public:
    ESProgramException( const string & msg ) :
      std::runtime_error(msg) { }
};

namespace ESShader {
  enum ESShaderType {
    VERTEX = GL_VERTEX_SHADER, 
    FRAGMENT = GL_FRAGMENT_SHADER
  };
};

class ESProgram
{
  private:
    int  handle;
    bool linked;
    std::map<string, int> uniformLocations;

    GLint  getUniformLocation(const char * name );
    bool fileExists( const string & fileName );
    string getExtension( const char * fileName );

    // Make these private in order to make the object non-copyable
    ESProgram( const ESProgram & other ) { }
    ESProgram & operator=( const ESProgram &other ) { return *this; }

  public:
    ESProgram();
    ~ESProgram();

    void   compileShader( const char * fileName ) throw (ESProgramException);
    void   compileShader( const char * fileName, ESShader::ESShaderType type ) throw (ESProgramException);
    void   compileShader( const char * source, ESShader::ESShaderType type,
        const char *fileName) throw (ESProgramException);

    void   link() throw (ESProgramException);
    void   validate() throw(ESProgramException);
    void   use() throw (ESProgramException);

    int    getHandle();
    bool   isLinked();

    void   bindAttribLocation( GLuint location, const char * name);
//    void   bindFragDataLocation( GLuint location, const char * name );

    void   setUniform( const char *name, float x, float y, float z);
    void   setUniform( const char *name, const vec2 & v);
    void   setUniform( const char *name, const vec3 & v);
    void   setUniform( const char *name, const vec4 & v);
    void   setUniform( const char *name, const mat4 & m);
    void   setUniform( const char *name, const mat3 & m);
    void   setUniform( const char *name, float val );
    void   setUniform( const char *name, int val );
    void   setUniform( const char *name, bool val );
    void   setUniform( const char *name, GLuint val );

//    void   printActiveUniforms();
//    void   printActiveUniformBlocks();
//    void   printActiveAttribs();

    const char * getTypeString( GLenum type );

};

#endif // ESPROGRAM_H
