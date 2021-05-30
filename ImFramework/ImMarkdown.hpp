#pragma once

#include <sstream>
#include <string>
#include <algorithm>
#include <filesystem>
#include <unordered_map>
#include <gl/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <imgui.h>



class ImMarkdown
{

private:
    // rel image path and opengl texture id map
    static std::unordered_map<std::string, GLuint> image_id_map;

public:
    
    static void Render(std::string markdown_str, std::string work_dir = "") {
        std::istringstream str_stream(markdown_str.c_str());

        // TODO:
        // Trim begin and end
        // Bold text
        // Italic
        // Basic html -> sub and sup
        // Blockquotes -> containers
        // Nested blockquotes
        // Lists
        // Tables

        std::string str = "";
        while (std::getline(str_stream, str)) {
            
            if (str.find("###") != std::string::npos) {
                std::replace(str.begin(), str.end(), '#', ' ');

                ImMarkdown::RenderHeading1(str);

                continue;
            }
            if (str.find("##") != std::string::npos) {
                std::replace(str.begin(), str.end(), '#', ' ');

                ImMarkdown::RenderHeading1(str);

                continue;
            }
            if (str.find("#") != std::string::npos) {
                std::replace(str.begin(), str.end(), '#', ' ');

                ImMarkdown::RenderHeading1(str);

                continue;
            }
            if (str.find("---") != std::string::npos)
            {
                ImGui::Separator();
                continue;
            }

            if (str.find("![") != std::string::npos && 
                str.find("]") != std::string::npos &&
                str.find("(") != std::string::npos && 
                str.find(")") != std::string::npos)
            {

                auto begin_img_path = str.find("(");
                auto end_img_path = str.find(")");

                if (begin_img_path + 1 < end_img_path)
                {
                    // Get absolute path
                    std::filesystem::path rel_path(
                        // +1 to remove '(' and ')'
                        str.substr(begin_img_path + 1, end_img_path - begin_img_path - 1)
                    );

                    std::string abs_path = "";
                    if (work_dir.compare("") == 0) { 
                        abs_path = std::filesystem::absolute(rel_path).string(); 
                    }
                    else
                    {
                        //remove dot slash ./ at beginning
                        if (abs_path.substr(0, 2).compare("./") == 0) { 
                            abs_path = abs_path.substr(1, abs_path.size() - 1);
                        }

                        abs_path = 
                            work_dir + 
                            rel_path.string(); 
                    }

                    
                    std::replace(abs_path.begin(), abs_path.end(), '\\', '/');

                    ImMarkdown::RenderImage(abs_path.c_str());
                }
                continue;
            }

            // Default text
            ImGui::TextWrapped(str.c_str());


        }
    }

    static void RenderHeading1(std::string heading1_str) { 
        ImGui::TextWrapped(heading1_str.c_str());
    }

    static bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
    {
       
        // Load from file
        int            image_width  = 0;
        int            image_height = 0;
        unsigned char* image_data   = stbi_load(filename, &image_width, &image_height, NULL, 4);
        if (image_data == NULL) return false;

        // Create a OpenGL texture identifier
        GLuint image_texture;
        glGenTextures(1, &image_texture);
        glBindTexture(GL_TEXTURE_2D, image_texture);

        // Setup filtering parameters for display
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(
            GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
            GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

        // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, image_data);
        stbi_image_free(image_data);

        *out_texture = image_texture;
        *out_width   = image_width;
        *out_height  = image_height;

        return true;
    }

    static void RenderImage(std::string abs_path)
    { 
        static GLuint img_id = -1;
        static std::string last_path = "";
        static int width  = 0;
        static int height = 0;

        if (abs_path.compare(last_path) != 0) {

            // cleanup last image
            if (img_id != -1)
            {
                glDeleteTextures(1, (GLuint*)img_id);
                img_id = -1;
            }

            if (!std::filesystem::exists(abs_path)) {
                 GLuint texture = 0;
                 
                 // TODO: Convert relative path to absolute path
                 if (LoadTextureFromFile(abs_path.c_str(), &texture, &width, &height)) { 
                     img_id = texture;
                     last_path = abs_path;
                 }
            }
        }

        ImGui::Image((void*)img_id, ImVec2(width, height));

        // In your application you would load an image based on data_ input. Here we just use the
        // imgui font texture.
        // ImTextureID image = ImGui::GetIO().Fonts->TexID;
        // > C++14 can use ImGui::MarkdownImageData imageData{ true, false, image,
        // ImVec2( 40.0f, 20.0f ) };

        //std::string link = data_.link;

        //// 2 -> asume that the relative path begins with ./
        //link                                 = link.substr(2, data_.linkLength - 2);
        //static std::filesystem::path imgPath = "";

        //static ImGui::MarkdownImageData imageData;

        //auto tmpImgPath =
        //    std::filesystem::path(currCardSetPath).parent_path() / std::filesystem::path(link);
        //;
        //if (tmpImgPath.compare(imgPath) == 0) { return imageData; }
        //else
        //{
        //    imgPath = tmpImgPath;
        //}

        //if (!std::filesystem::exists(imgPath)) { return imageData; }

        //GLuint texture = 0;
        //int    width   = 0;
        //int    height  = 0;
        //if (!LoadTextureFromFile(imgPath.string().c_str(), &texture, &width, &height))
        //{ return imageData; }

        //if (imageData.user_texture_id != 0)
        //{ glDeleteTextures(0, (GLuint*)imageData.user_texture_id); }

        //imageData.isValid         = true;
        //imageData.useLinkCallback = false;
        //imageData.user_texture_id = (ImTextureID)texture;
        //imageData.size            = ImVec2(width, height);

        //// For image resize when available size.x > image width, add
        //ImVec2 const contentSize = ImGui::GetContentRegionAvail();
        //if (imageData.size.x > contentSize.x)
        //{
        //    float const ratio = imageData.size.y / imageData.size.x;
        //    imageData.size.x  = contentSize.x;
        //    imageData.size.y  = contentSize.x * ratio * ImFramework::GetScaleFactor().x;
        //}

        //imageData.size.x *= ImFramework::GetScaleFactor().x;
        //imageData.size.y *= ImFramework::GetScaleFactor().x;

        //return imageData;

    }

};