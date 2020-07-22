#include "Image.hpp"

namespace vd
{
	namespace imgloader
	{
		// Implementations
		template class Pixel<uint8_t>;
		template class Pixel<uint16_t>;
		template class Pixel<float>;

		template class Image<uint8_t>;
		template class Image<uint16_t>;
		template class Image<float>;

		template Image<uint8_t> Image<float>::cast<uint8_t>() const;
		template Image<float> Image<uint8_t>::cast<float>() const;
		template Image<uint16_t> Image<uint8_t>::cast<uint16_t>() const;

		template<typename T>
		Pixel<T>::Pixel(T r, T g, T b, T a)
			: r(r)
			, g(g)
			, b(b)
			, a(a)
		{
		}

		template<typename T>
		Pixel<T>::Pixel()
		{
		}

		template<typename T>
		template<typename E> 
		Pixel<E> Pixel<T>::cast() const
		{
			Pixel<E> pixel;
			pixel.r = static_cast<E>(this->r);
			pixel.g = static_cast<E>(this->g);
			pixel.b = static_cast<E>(this->b);
			pixel.a = static_cast<E>(this->a);
			return pixel;
		}

		template<>
		template<>
		Pixel<uint8_t> Pixel<float>::cast() const
		{
			Pixel<uint8_t> pixel;
			pixel.r = (uint8_t)(this->r * 255.0f);
			pixel.g = (uint8_t)(this->g * 255.0f);
			pixel.b = (uint8_t)(this->b * 255.0f);
			pixel.a = (uint8_t)(this->a * 255.0f);
			return pixel;
		}
		
		template<>
		template<>
		Pixel<float> Pixel<uint8_t>::cast() const
		{
			Pixel<float> pixel;
			pixel.r = ((float)this->r / 255.0f);
			pixel.g = ((float)this->g / 255.0f);
			pixel.b = ((float)this->b / 255.0f);
			pixel.a = ((float)this->a / 255.0f);
			return pixel;
		}
		
		template<typename T>
		Image<T>::Image(size_t width, size_t height)
			: width(width)
			, height(height)
		{
			int len = width * height * 4;
			data.resize(len);
			iterator = 0;
		}

		template<typename T>
		Image<T>::Image(Image<T>& other)
			: width(other.width)
			, height(other.height)
		{
			int len = width * height * 4;
			data.resize(len);
			data.insert(data.begin(), other.data.begin(), other.data.end());
			iterator = 0;
		}

		template<typename T>
		Image<T>::~Image()
		{
			data.clear();
		}

		template<typename T>
		void Image<T>::expand(const Pixel<T>& pixel)
		{
			assert(this->width != 0 && this->height != 0);

			data[iterator] = pixel.r;
			data[iterator + 1] = pixel.g;
			data[iterator + 2] = pixel.b;
			data[iterator + 3] = pixel.a;

			iterator += 4;
		}

		template<typename T>
		void Image<T>::reverse()
		{
			int width_in_bytes = this->width * 4;

			T* top = NULL;
			T* bottom = NULL;
			T temp = 0;

			int half_height = this->height / 2;

			T* image_data = &data[0];

			for (int row = 0; row < half_height; row++) {
				top = image_data + row * width_in_bytes;
				bottom = image_data + (this->width - row - 1) * width_in_bytes;
				for (int col = 0; col < width_in_bytes; col++) {
					temp = *top;
					*top = *bottom;
					*bottom = temp;
					top++;
					bottom++;
				}
			}
		}

		template<typename T> 
		template<typename E>
		Image<E> Image<T>::cast() const
		{
			Image<E> _img(this->width, this->height);

			for (size_t i = 0; i < _img.height; ++i)
			{
				for (size_t j = 0; j < _img.width; ++j)
				{
					Pixel<E> _pixel = this->at(i, j, eRGBA).template cast<E>();
					_img.expand(_pixel);
				}
			}

			return _img;
		}

		template<typename T>
		Pixel<T> Image<T>::at(size_t i, size_t j, const PixelFormat& format) const
		{
			Pixel<T> p;

			size_t startIndex = (i * this->width + j) * 4;

			switch (format)
			{
				case eRGBA: 
				{
					p.r = this->data[startIndex];
					p.g = this->data[startIndex + 1];
					p.b = this->data[startIndex + 2];
					p.a = this->data[startIndex + 3];
					break;
				} 
				case eBGRA:
				default:
				{
					p.r = this->data[startIndex + 2];
					p.g = this->data[startIndex + 1];
					p.b = this->data[startIndex];
					p.a = this->data[startIndex + 3];
					break;
				}
			}

			return p;
		}
	}

}