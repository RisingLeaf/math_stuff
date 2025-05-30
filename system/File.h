#ifndef FILE_H
#define FILE_H

#include <string>
#include <vector>


namespace File {
  class PixelHandle
  {
    void *pixels;
  public:
    explicit PixelHandle(void *_pixels) : pixels(_pixels) {}

    PixelHandle(const PixelHandle &other) = delete;
    PixelHandle(PixelHandle &&other) noexcept
    :pixels(other.pixels)
    {
      other.pixels = nullptr;
    }
    PixelHandle &operator=(const PixelHandle &other) = delete;
    PixelHandle &operator=(PixelHandle &&other) noexcept
    {
      this->pixels = other.pixels;
      other.pixels = nullptr;
      return *this;
    }

    ~PixelHandle();

    [[nodiscard]] const void * Get() const { return pixels; }
  };
  std::vector<char> Read(const std::string &path);
  std::string       ReadString(const std::string &path);
  PixelHandle       ReadImage(const std::string &path, int &width, int &height, int &channels, bool red_only = false);
}



#endif //FILE_H
