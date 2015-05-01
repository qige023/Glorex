#include "esprogram.h"

#include "esutil.h"
#include "esfile.h"
#include <iostream>

using std::cout;
using std::endl;

namespace ESShaderInfo {
struct shader_file_extension {
    const char *ext;
    ESShader::ESShaderType type;
};

struct shader_file_extension extensions[] = {
        { ".vs",  ESShader::VERTEX },
        {".vert", ESShader::VERTEX },
        { ".fs",  ESShader::FRAGMENT },
        { ".frag",ESShader::FRAGMENT }
    };
}

ESProgram::ESProgram() :
        handle(0), linked(false) {
}

ESProgram::~ESProgram() {
    if (handle == 0)
        return;

    // Query the number of attached shaders
    GLint numShaders = 0;
    glGetProgramiv(handle, GL_ATTACHED_SHADERS, &numShaders);

    // Get the shader names
    GLuint * shaderNames = new GLuint[numShaders];
    glGetAttachedShaders(handle, numShaders, NULL, shaderNames);

    // Delete the shaders
    for (int i = 0; i < numShaders; i++)
        glDeleteShader(shaderNames[i]);

    // Delete the program
    glDeleteProgram(handle);

    delete[] shaderNames;
}

void ESProgram::compileShader(const char * fileName) throw (ESProgramException) {
    cout << "ESProgram::compileShader fileName:" << fileName << "..." << endl;

    int numExts = sizeof(ESShaderInfo::extensions)
            / sizeof(ESShaderInfo::shader_file_extension);

    // Check the file name's extension to determine the shader type
    string ext = getExtension(fileName);
    ESShader::ESShaderType type = ESShader::VERTEX;
    bool matchFound = false;
    for (int i = 0; i < numExts; i++) {
        if (ext == ESShaderInfo::extensions[i].ext) {
            matchFound = true;
            type = ESShaderInfo::extensions[i].type;
            break;
        }
    }

    // If we didn't find a match, throw an exception
    if (!matchFound) {
        string msg = "Unrecognized extension: " + ext;
        throw ESProgramException(msg);
    }

    // Pass the discovered shader type along
    compileShader(fileName, type);
}

string ESProgram::getExtension(const char * name) {
    string nameStr(name);

    size_t loc = nameStr.find_last_of('.');
    if (loc != string::npos) {
        return nameStr.substr(loc, string::npos);
    }
    return "";
}

void ESProgram::compileShader(const char * fileName,
        ESShader::ESShaderType type) throw (ESProgramException) {

    if (handle <= 0) {
        handle = glCreateProgram();
        if (handle == 0) {
            throw ESProgramException("Unable to create shader program.");
        }
    }

    char * buffer;
    FILE *file = ESFileWrapper::esFopen(fileName, "r");

    if (!file) {
        string message = string("Unable to open: ") + fileName;
        throw ESProgramException(message);
    }
    fseek( file , 0L , SEEK_END);
    size_t fileSize  =  ftell( file );
    rewind( file );
    /* allocate memory for entire content */
    buffer = (char *) calloc( 1, fileSize + 1 );

    fread(buffer, fileSize, 1, file);

    compileShader(buffer, type, fileName);
}

void ESProgram::compileShader(const char * source,
        ESShader::ESShaderType type, const char * fileName)
                throw (ESProgramException) {
    if (handle <= 0) {
        handle = glCreateProgram();
        if (handle == 0) {
            throw ESProgramException("Unable to create shader program.");
        }
    }

    GLuint shaderHandle = glCreateShader(type);

    const char * c_code = source;
    glShaderSource(shaderHandle, 1, &c_code, NULL);

    // Compile the shader
    glCompileShader(shaderHandle);

    // Check for errors
    int result;
    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &result);
    if ( GL_FALSE == result) {
        // Compile failed, get log
        int length = 0;
        string logString;
        glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &length);
        if (length > 0) {
            char * c_log = new char[length];
            int written = 0;
            glGetShaderInfoLog(shaderHandle, length, &written, c_log);
            logString = c_log;
            delete[] c_log;
        }
        string msg = string(fileName) + ": shader compilation failed\n";
        msg += logString;

        throw ESProgramException(msg);

    } else {
        // Compile succeeded, attach shader
        glAttachShader(handle, shaderHandle);
    }
}

void ESProgram::link() throw (ESProgramException) {
    if (linked)
        return;
    if (handle <= 0)
        throw ESProgramException("Program has not been compiled.");

    glLinkProgram(handle);

    int status = 0;
    glGetProgramiv(handle, GL_LINK_STATUS, &status);
    if ( GL_FALSE == status) {
        // Store log and return false
        int length = 0;
        string logString;

        glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &length);

        if (length > 0) {
            char * c_log = new char[length];
            int written = 0;
            glGetProgramInfoLog(handle, length, &written, c_log);
            logString = c_log;
            delete[] c_log;
        }

        throw ESProgramException(string("Program link failed:\n") + logString);
    } else {
        uniformLocations.clear();
        linked = true;
    }
}

void ESProgram::use() throw (ESProgramException) {
    if (handle <= 0 || (!linked))
        throw ESProgramException("Shader has not been linked");
    glUseProgram(handle);
}

int ESProgram::getHandle() {
    return handle;
}

bool ESProgram::isLinked() {
    return linked;
}

void ESProgram::bindAttribLocation(GLuint location, const char * name) {
    glBindAttribLocation(handle, location, name);
}

//void ESProgram::bindFragDataLocation(GLuint location, const char * name) {
//    glBindFragDataLocation(handle, location, name);
//}

void ESProgram::setUniform(const char *name, float x, float y, float z) {
    GLint loc = getUniformLocation(name);
    glUniform3f(loc, x, y, z);
}

void ESProgram::setUniform(const char *name, const vec3 & v) {
    this->setUniform(name, v.x, v.y, v.z);
}

void ESProgram::setUniform(const char *name, const vec4 & v) {
    GLint loc = getUniformLocation(name);
    glUniform4f(loc, v.x, v.y, v.z, v.w);
}

void ESProgram::setUniform(const char *name, const vec2 & v) {
    GLint loc = getUniformLocation(name);
    glUniform2f(loc, v.x, v.y);
}

void ESProgram::setUniform(const char *name, const mat4 & m) {
    GLint loc = getUniformLocation(name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
}

void ESProgram::setUniform(const char *name, const mat3 & m) {
    GLint loc = getUniformLocation(name);
    glUniformMatrix3fv(loc, 1, GL_FALSE, &m[0][0]);
}

void ESProgram::setUniform(const char *name, float val) {
    GLint loc = getUniformLocation(name);
    glUniform1f(loc, val);
}

void ESProgram::setUniform(const char *name, int val) {
    GLint loc = getUniformLocation(name);
    glUniform1i(loc, val);
}

void ESProgram::setUniform(const char *name, GLuint val) {
    GLint loc = getUniformLocation(name);
    glUniform1ui(loc, val);
}

void ESProgram::setUniform(const char *name, bool val) {
    int loc = getUniformLocation(name);
    glUniform1i(loc, val);
}

//void ESProgram::printActiveUniforms() {
//    GLint numUniforms = 0;
//    glGetProgramInterfaceiv(handle, GL_UNIFORM, GL_ACTIVE_RESOURCES,
//            &numUniforms);
//
//    GLenum properties[] =
//            { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION, GL_BLOCK_INDEX };
//
//    printf("Active uniforms:\n");
//    for (int i = 0; i < numUniforms; ++i) {
//        GLint results[4];
//        glGetProgramResourceiv(handle, GL_UNIFORM, i, 4, properties, 4, NULL,
//                results);
//
//        if (results[3] != -1)
//            continue;  // Skip uniforms in blocks
//        GLint nameBufSize = results[0] + 1;
//        char * name = new char[nameBufSize];
//        glGetProgramResourceName(handle, GL_UNIFORM, i, nameBufSize, NULL,
//                name);
//        printf("%-5d %s (%s)\n", results[2], name, getTypeString(results[1]));
//        delete[] name;
//    }
//}
//
//void ESProgram::printActiveUniformBlocks() {
//    GLint numBlocks = 0;
//
//    glGetProgramInterfaceiv(handle, GL_UNIFORM_BLOCK, GL_ACTIVE_RESOURCES,
//            &numBlocks);
//    GLenum blockProps[] = { GL_NUM_ACTIVE_VARIABLES, GL_NAME_LENGTH };
//    GLenum blockIndex[] = { GL_ACTIVE_VARIABLES };
//    GLenum props[] = { GL_NAME_LENGTH, GL_TYPE, GL_BLOCK_INDEX };
//
//    for (int block = 0; block < numBlocks; ++block) {
//        GLint blockInfo[2];
//        glGetProgramResourceiv(handle, GL_UNIFORM_BLOCK, block, 2, blockProps,
//                2, NULL, blockInfo);
//        GLint numUnis = blockInfo[0];
//
//        char * blockName = new char[blockInfo[1] + 1];
//        glGetProgramResourceName(handle, GL_UNIFORM_BLOCK, block,
//                blockInfo[1] + 1, NULL, blockName);
//        printf("Uniform block \"%s\":\n", blockName);
//        delete[] blockName;
//
//        GLint * unifIndexes = new GLint[numUnis];
//        glGetProgramResourceiv(handle, GL_UNIFORM_BLOCK, block, 1, blockIndex,
//                numUnis, NULL, unifIndexes);
//
//        for (int unif = 0; unif < numUnis; ++unif) {
//            GLint uniIndex = unifIndexes[unif];
//            GLint results[3];
//            glGetProgramResourceiv(handle, GL_UNIFORM, uniIndex, 3, props, 3,
//            NULL, results);
//
//            GLint nameBufSize = results[0] + 1;
//            char * name = new char[nameBufSize];
//            glGetProgramResourceName(handle, GL_UNIFORM, uniIndex, nameBufSize,
//            NULL, name);
//            printf("    %s (%s)\n", name, getTypeString(results[1]));
//            delete[] name;
//        }
//
//        delete[] unifIndexes;
//    }
//}
//
//void ESProgram::printActiveAttribs() {
//    GLint numAttribs;
//    glGetProgramInterfaceiv(handle, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES,
//            &numAttribs);
//
//    GLenum properties[] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION };
//
//    printf("Active attributes:\n");
//    for (int i = 0; i < numAttribs; ++i) {
//        GLint results[3];
//        glGetProgramResourceiv(handle, GL_PROGRAM_INPUT, i, 3, properties, 3,
//        NULL, results);
//
//        GLint nameBufSize = results[0] + 1;
//        char * name = new char[nameBufSize];
//        glGetProgramResourceName(handle, GL_PROGRAM_INPUT, i, nameBufSize, NULL,
//                name);
//        printf("%-5d %s (%s)\n", results[2], name, getTypeString(results[1]));
//        delete[] name;
//    }
//}

const char * ESProgram::getTypeString(GLenum type) {
    // There are many more types than are covered here, but
    // these are the most common in these examples.
    switch (type) {
    case GL_FLOAT:
        return "float";
    case GL_FLOAT_VEC2:
        return "vec2";
    case GL_FLOAT_VEC3:
        return "vec3";
    case GL_FLOAT_VEC4:
        return "vec4";
    case GL_INT:
        return "int";
    case GL_UNSIGNED_INT:
        return "unsigned int";
    case GL_BOOL:
        return "bool";
    case GL_FLOAT_MAT2:
        return "mat2";
    case GL_FLOAT_MAT3:
        return "mat3";
    case GL_FLOAT_MAT4:
        return "mat4";
    default:
        return "?";
    }
}

void ESProgram::validate() throw (ESProgramException) {
    if (!isLinked())
        throw ESProgramException("Program is not linked");

    GLint status;
    glValidateProgram(handle);
    glGetProgramiv(handle, GL_VALIDATE_STATUS, &status);

    if ( GL_FALSE == status) {
        // Store log and return false
        int length = 0;
        string logString;

        glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &length);

        if (length > 0) {
            char * c_log = new char[length];
            int written = 0;
            glGetProgramInfoLog(handle, length, &written, c_log);
            logString = c_log;
            delete[] c_log;
        }

        throw ESProgramException(
                string("Program failed to validate\n") + logString);

    }
}

int ESProgram::getUniformLocation(const char * name) {
    std::map<string, int>::iterator pos;
    pos = uniformLocations.find(name);

    if (pos == uniformLocations.end()) {
        uniformLocations[name] = glGetUniformLocation(handle, name);
    }

    return uniformLocations[name];
}
