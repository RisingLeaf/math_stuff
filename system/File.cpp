#include "File.h"

#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include "../../external/stb_image.h"

#include "Log.h"


std::vector<char> File::Read(const std::string &path)
{
  std::ifstream file(path, std::ios::ate | std::ios::binary);

  if (!file.is_open())
  {
    Log::Msg("Unable to read file: " + path);
    return {};
  }

  size_t file_size = file.tellg();
  std::vector<char> buffer(file_size);
  file.seekg(0);
  file.read(buffer.data(), file_size);
  file.close();

  return buffer;
}



std::string File::ReadString(const std::string &path)
{
  std::ifstream ifs(path);
  return std::string(std::istreambuf_iterator(ifs), std::istreambuf_iterator<char>());
}




File::PixelHandle File::ReadImage(const std::string &path, int &width, int &height, int &channels, const bool red_only)
{
  stbi_uc* pixels = stbi_load(path.c_str(), &width, &height, &channels, red_only ? STBI_grey : STBI_rgb_alpha);

  if (!pixels)
    Log::Msg("Image: " + path + " could not be loaded.");

  if(red_only)
    channels = 1;
  else
    channels = 4;

  return PixelHandle(pixels);
}



File::PixelHandle::~PixelHandle()
{
  if(pixels) stbi_image_free(pixels);
}


