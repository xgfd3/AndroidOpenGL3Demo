//
// Created by xucz on 2020/8/30.
//

#include "Grid.h"

extern "C" {

typedef struct __UserData {
    UserDataBase parent;

    GLuint s_TexSamplerLoc;
    GLuint u_offsetLoc;
    GLuint texSizeLoc;

    glm::vec2 texSize;
} UserData;

const char f2DShaderStr[] =
        "#version 300 es\n"
        "precision mediump float;\n"
        "in vec2 v_texCoord;\n"
        "layout(location = 0) out vec4 outColor;\n"
        "uniform sampler2D s_TexSampler;\n"
        "uniform float u_offset;\n"
        "uniform vec2 texSize;\n"
        "void main(){ \n"
        "    vec2 imgTexCoord = v_texCoord * texSize;\n" // 将纹理坐标转成图片坐标
        //"    vec2 imgTexCoord = v_texCoord;\n"
        "    float sideLength = texSize.y / 6.0;\n"// 风格边长
        "    float maxOffset = 0.15 * sideLength;\n"
        "    float x = mod(imgTexCoord.x, floor(sideLength));\n"
        "    float y = mod(imgTexCoord.y, floor(sideLength));\n"
        "    float offset = u_offset * maxOffset;\n"
        "    if(offset<=x && x<=sideLength-offset && offset <=y && y <= sideLength - offset){\n"
        "        outColor = texture(s_TexSampler, v_texCoord);\n"
        "    } else {\n"
        "        outColor = vec4(1.0, 1.0, 1.0, 0.0);\n"
        "    }\n"
        "}\n";

const char fOESShaderStr[] =
        "#version 300 es\n"
        "#extension GL_OES_EGL_image_external_essl3 : require\n"
        "precision mediump float;\n"
        "in vec2 v_texCoord;\n"
        "layout(location = 0) out vec4 outColor;\n"
        "uniform samplerExternalOES s_TexSampler;\n"
        "uniform float u_offset;\n"
        "uniform vec2 texSize;\n"
        "void main(){ \n"
        "    vec2 imgTexCoord = v_texCoord * texSize;\n" // 将纹理坐标转成图片坐标
        //"    vec2 imgTexCoord = v_texCoord;\n"
        "    float sideLength = texSize.y / 6.0;\n"// 风格边长
        "    float maxOffset = 0.15 * sideLength;\n"
        "    float x = mod(imgTexCoord.x, floor(sideLength));\n"
        "    float y = mod(imgTexCoord.y, floor(sideLength));\n"
        "    float offset = u_offset * maxOffset;\n"
        "    if(offset<=x && x<=sideLength-offset && offset <=y && y <= sideLength - offset){\n"
        "        outColor = texture(s_TexSampler, v_texCoord);\n"
        "    } else {\n"
        "        outColor = vec4(1.0, 1.0, 1.0, 0.0);\n"
        "    }\n"
        "}\n";


void GridInit(ESContext *esContext) {
    auto *userData = static_cast<UserData *>(esContext->userData);
    if (!userData) {
        userData = new UserData();
        esContext->userData = userData;
    }

    // 编译程序
    UserDataBase *pUserDataBase = (UserDataBase *) userData;
    char *fShaderStr;
    if (pUserDataBase->cameraTexType == TEX_TYPE_OES) {
        fShaderStr = (char *) fOESShaderStr;
    } else {
        fShaderStr = (char *) f2DShaderStr;
    }

    GLuint program = CameraBaseLoadProgram(pUserDataBase, fShaderStr);
    if (!program) {
        return;
    }
    userData->s_TexSamplerLoc = glGetUniformLocation(program, "s_TexSampler");
    userData->u_offsetLoc = glGetUniformLocation(program, "u_offset");
    userData->texSizeLoc = glGetUniformLocation(program, "texSize");
}

void GridUnInit(ESContext *esContext) {
    auto *userData = static_cast<UserData *>(esContext->userData);
    if (!userData) {
        return;
    }
    CameraBaseRelease((UserDataBase *) userData);

    delete userData;
    esContext->userData = nullptr;
}

void GridSetCameraTexId(ESContext *esContext, GLuint texId, int texType, int width, int height,
                        int facing) {
    auto *userData = static_cast<UserData *>(esContext->userData);
    if (!userData) {
        return;
    }
    int shouldUpdate = CameraBaseUpdateParam((UserDataBase *) userData, texId, texType, width,
                                             height, facing);
    if (shouldUpdate) {
        CameraBaseUpdateMatrix((UserDataBase *) userData, esContext->width, esContext->height);
        userData->texSize = glm::vec2(1.0f * width, 1.0f * height);
        GridInit(esContext);
    }
}

void GridDraw(ESContext *esContext) {
    auto *userData = static_cast<UserData *>(esContext->userData);
    if (!userData) {
        return;
    }

    UserDataBase *pUserDataBase = (UserDataBase *) userData;
    CameraBaseBeforeDraw(pUserDataBase, esContext->width, esContext->height);
    if (!pUserDataBase->cameraTexId) {
        return;
    }

    if (pUserDataBase->cameraTexType == TEX_TYPE_OES) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_EXTERNAL_OES, pUserDataBase->cameraTexId);
        glUniform1i(userData->s_TexSamplerLoc, 0);
    } else {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, pUserDataBase->cameraTexId);
        glUniform1i(userData->s_TexSamplerLoc, 0);
    }

    // 图片坐标
    glUniform2fv(userData->texSizeLoc, 1, &userData->texSize[0]);

    // 网格偏移
    float offset = 0.5f * CameraBaseGetProgress(pUserDataBase, 30, 1, 0, 1);
    glUniform1f(userData->u_offsetLoc, offset);


    CameraBaseAfterDraw(pUserDataBase);

    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, GL_NONE);
}


}