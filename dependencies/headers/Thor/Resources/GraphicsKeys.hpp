/////////////////////////////////////////////////////////////////////////////////
//
// Thor C++ Library
// Copyright (c) 2011-2012 Jan Haller
// 
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
// 
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////

/// @file
/// @brief Resource keys for SFML Graphics module

#ifndef THOR_GRAPHICSKEYS_HPP
#define THOR_GRAPHICSKEYS_HPP

#include <Thor/Resources/ResourceKeyTraits.hpp>
#include <Thor/SmartPtr/MovedPtr.hpp>
#include <Thor/Detail/Swap.hpp>
#include <Thor/Config.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Config.hpp>

#include THOR_TR1_HEADER(functional)
#include <string>


namespace thor
{

/// @addtogroup Resources
/// @{

/// @brief Namespace for resource-specific functionality
/// @details Contains resource-related functionality that is a bit more specific than ResourceManager and ResourcePtr.
namespace Resources
{

	/// @brief Predefined key class for sf::Image resources
	/// @details You can use this class to specifiy the access and loading of sf::Image resources in a ResourceManager.
	class THOR_API ImageKey
	{
		// ---------------------------------------------------------------------------------------------------------------------------
		// Public member functions
		public:
			/// @brief Load sf::Image from size and fill color
			/// @param width,height The size of the image.
			/// @param fillColor The color used to fill the image.
			/// @param tag If an existing key has the same loading parameters, you can use this ID to create a distinct key.
			/// @see sf::Image::Create()
			static ImageKey				FromSize(unsigned int width, unsigned int height, const sf::Color& fillColor = sf::Color(0, 0, 0),
											const std::string& tag = "");

			/// @brief Load sf::Image from file name
			/// @param fileName The name of the file from which you want to load the sf::Image.
			/// @param tag If an existing key has the same loading parameters, you can use this ID to create a distinct key.
			/// @see sf::Image::LoadFromFile()
			static ImageKey				FromFile(const std::string& fileName, const std::string& tag = "");

			/// @brief Load sf::Image from file in memory
			/// @param data Pointer to the file data in memory.
			/// @param size Size of the data to load, in bytes. 
			/// @param tag If an existing key has the same loading parameters, you can use this ID to create a distinct key.
			/// @see sf::Image::LoadFromMemory()
			static ImageKey				FromMemory(const void* data, std::size_t size, const std::string& tag = "");

			/// @brief Load sf::Image from array of pixels
			/// @param width,height The size of the image.
			/// @param pixels Pointer to the pixels in memory.
			/// @param tag If an existing key has the same loading parameters, you can use this ID to create a distinct key.
			/// @see sf::Image::Create()
			static ImageKey				FromPixels(unsigned int width, unsigned int height, const sf::Uint8* pixels,
											const std::string& tag = "");

			/// @brief Load sf::Image from input stream
			/// @param stream Source stream to read from.
			/// @param tag If an existing key has the same loading parameters, you can use this ID to create a distinct key.
			/// @see sf::Image::LoadFromStream()
			static ImageKey				FromStream(sf::InputStream& stream, const std::string& tag = "");

			/// @brief Loads the image as specified by the static functions.
			/// @return Movable smart pointer to the allocated sf::Image, or null pointer at loading failure.
			MovedPtr<sf::Image, NoCopy>	Load() const;

			/// @brief Exchanges the contents of @a *this with @a other.
			///
			void						Swap(ImageKey& other);


		// ---------------------------------------------------------------------------------------------------------------------------
		// Private member functions
		private:
			// Make default constructor private
										ImageKey();

		
		// ---------------------------------------------------------------------------------------------------------------------------
		// Private variables
		private:
			std::tr1::function<bool(sf::Image&)>	mLoader;
			std::string								mKey;

	
		// ---------------------------------------------------------------------------------------------------------------------------
		// Friends
		friend struct detail::KeyInfoAccessor;
	};


	/// @brief Predefined key class for sf::Texture resources
	/// @details You can use this class to specifiy the access and loading of sf::Texture resources in a ResourceManager.
	class THOR_API TextureKey
	{
		// ---------------------------------------------------------------------------------------------------------------------------
		// Public member functions
		public:
			/// @brief Load sf::Texture from size
			/// @param width,height The size of the texture.
			/// @param tag If an existing key has the same loading parameters, you can use this ID to create a distinct key.
			/// @see sf::Texture::Create()
			static TextureKey			FromSize(unsigned int width, unsigned int height, const std::string& tag = "");

			/// @brief Load sf::Texture from file name
			/// @param fileName The name of the file from which you want to load the sf::Texture.
			/// @param area Sub-rectangle of the image to load.
			/// @param tag If an existing key has the same loading parameters, you can use this ID to create a distinct key.
			/// @see sf::Texture::LoadFromFile()
			static TextureKey			FromFile(const std::string& fileName, const sf::IntRect& area = sf::IntRect(),
											const std::string& tag = "");

			/// @brief Load sf::Texture from file in memory
			/// @param data Pointer to the file data in memory.
			/// @param size Size of the data to load, in bytes. 
			/// @param area Sub-rectangle of the image to load.
			/// @param tag If an existing key has the same loading parameters, you can use this ID to create a distinct key.
			/// @see sf::Texture::LoadFromMemory()
			static TextureKey			FromMemory(const void* data, std::size_t size, const sf::IntRect& area = sf::IntRect(),
											const std::string& tag = "");

			/// @brief Load sf::Texture from an existing sf::Image
			/// @param image Image containing the pixels that are copied to the texture.
			/// @param area Sub-rectangle of the image to load.
			/// @param tag If an existing key has the same loading parameters, you can use this ID to create a distinct key.
			/// @see sf::Texture::LoadFromImage()
			static TextureKey			FromImage(const sf::Image& image, const sf::IntRect& area = sf::IntRect(),
											const std::string& tag = "");

			/// @brief Load sf::Texture from input stream
			/// @param stream Source stream to read from.
			/// @param area Sub-rectangle of the image to load.
			/// @param tag If an existing key has the same loading parameters, you can use this ID to create a distinct key.
			/// @see sf::Texture::LoadFromStream()
			static TextureKey			FromStream(sf::InputStream& stream, const sf::IntRect& area = sf::IntRect(), 
											const std::string& tag = "");

			/// @brief Loads the image as specified by the static functions.
			/// @return Movable smart pointer to the allocated sf::Texture, or null pointer at loading failure.
			MovedPtr<sf::Texture, NoCopy>	Load() const;

			/// @brief Exchanges the contents of @a *this with @a other.
			///
			void						Swap(TextureKey& other);


		// ---------------------------------------------------------------------------------------------------------------------------
		// Private member functions
		private:
			// Make default constructor private
										TextureKey();

		
		// ---------------------------------------------------------------------------------------------------------------------------
		// Private variables
		private:
			std::tr1::function<bool(sf::Texture&)>	mLoader;
			std::string								mKey;

	
		// ---------------------------------------------------------------------------------------------------------------------------
		// Friends
		friend struct detail::KeyInfoAccessor;
	};


	/// @brief Predefined key class for sf::Font resources
	/// @details You can use this class to specifiy the access and loading of sf::Font resources in a ResourceManager.
	class THOR_API FontKey
	{
		// ---------------------------------------------------------------------------------------------------------------------------
		// Public member functions
		public:
			/// @brief Load sf::Font from file name
			/// @param fileName The name of the file from which you want to load the sf::Font.
			/// @param tag If an existing key has the same loading parameters, you can use this ID to create a distinct key.
			/// @see sf::Font::LoadFromFile()
			static FontKey				FromFile(const std::string& fileName, const std::string& tag = "");

			/// @brief Load sf::Font from file in memory
			/// @param data Pointer to the file data in memory.
			/// @param size Size of the data to load, in bytes. 
			/// @param tag If an existing key has the same loading parameters, you can use this ID to create a distinct key.
			/// @see sf::Font::LoadFromMemory()
			static FontKey				FromMemory(const void* data, std::size_t size, const std::string& tag = "");

			/// @brief Load sf::Font from input stream
			/// @param stream Source stream to read from.
			/// @param tag If an existing key has the same loading parameters, you can use this ID to create a distinct key.
			/// @see sf::Font::LoadFromStream()
			static FontKey				FromStream(sf::InputStream& stream, const std::string& tag = "");

			/// @brief Loads the font as specified by the static functions.
			/// @details This function is mainly provided for ResourceManager, you don't need to call it directly.
			/// @return Movable smart pointer to the allocated sf::Font, or null pointer at loading failure.
			MovedPtr<sf::Font, NoCopy>	Load() const;

			/// @brief Exchanges the contents of @a *this with @a other.
			///
			void						Swap(FontKey& other);


		// ---------------------------------------------------------------------------------------------------------------------------
		// Private member functions
		private:
			// Make default constructor private
										FontKey();

		
		// ---------------------------------------------------------------------------------------------------------------------------
		// Private variables
		private:
			std::tr1::function<bool(sf::Font&)>		mLoader;
			std::string								mKey;

	
		// ---------------------------------------------------------------------------------------------------------------------------
		// Friends
		friend struct detail::KeyInfoAccessor;
	};


	/// @brief Predefined key class for sf::Shader resources
	/// @details You can use this class to specifiy the access and loading of sf::Shader resources in a ResourceManager.
	class THOR_API ShaderKey
	{
		// ---------------------------------------------------------------------------------------------------------------------------
		// Public member functions
		public:
			/// @brief Load sf::Shader from file name
			/// @param fileName The name of the file from which you want to load the sf::Shader.
			/// @param shaderType Type of shader (vertex or fragment)
			/// @param tag If an existing key has the same loading parameters, you can use this ID to create a distinct key.
			/// @see sf::Shader::LoadFromFile()
			static ShaderKey			FromFile(const std::string& fileName, THOR_FAKE_DOC(unsigned int, sf::Shader::Type) shaderType, const std::string& tag = "");

			/// @brief Load sf::Shader from two file names
			/// @param vertexShaderFilename The name of the file from which you want to load the vertex shader.
			/// @param fragmentShaderFilename The name of the file from which you want to load the fragment shader.
			/// @param tag If an existing key has the same loading parameters, you can use this ID to create a distinct key.
			/// @see sf::Shader::LoadFromFile()
			static ShaderKey			FromFile(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename, const std::string& tag = "");

			/// @brief Load sf::Shader from source code in memory
			/// @param shaderCode String containing the source code of the shader.
			/// @param shaderType Type of shader (vertex or fragment)
			/// @param tag If an existing key has the same loading parameters, you can use this ID to create a distinct key.
			/// @see sf::Shader::LoadFromMemory()
			static ShaderKey			FromMemory(const std::string& shaderCode, THOR_FAKE_DOC(unsigned int, sf::Shader::Type) shaderType, const std::string& tag = "");

			/// @brief Load sf::Shader from two source codes in memory
			/// @param vertexShaderCode String containing the source code of the vertex shader.
			/// @param fragmentShaderCode String containing the source code of the fragment shader.
			/// @param tag If an existing key has the same loading parameters, you can use this ID to create a distinct key.
			/// @see sf::Shader::LoadFromMemory()
			static ShaderKey			FromMemory(const std::string& vertexShaderCode, const std::string& fragmentShaderCode, const std::string& tag = "");
			
			/// @brief Load sf::Shader from input stream
			/// @param stream Source stream to read from.
			/// @param shaderType Type of shader (vertex or fragment)
			/// @param tag If an existing key has the same loading parameters, you can use this ID to create a distinct key.
			/// @see sf::Shader::LoadFromStream()
			static ShaderKey			FromStream(sf::InputStream& stream, THOR_FAKE_DOC(unsigned int, sf::Shader::Type) shaderType, const std::string& tag = "");

			/// @brief Load sf::Shader from two input streams
			/// @param vertexShaderStream Source stream to read the vertex shader from.
			/// @param fragmentShaderStream Source stream to read the fragment shader from.
			/// @param tag If an existing key has the same loading parameters, you can use this ID to create a distinct key.
			/// @see sf::Shader::LoadFromStream()
			static ShaderKey			FromStream(sf::InputStream& vertexShaderStream, sf::InputStream& fragmentShaderStream, const std::string& tag = "");

			/// @brief Loads the shader as specified by the static functions.
			/// @return Movable smart pointer to the allocated sf::Shader, or null pointer at loading failure.
			MovedPtr<sf::Shader, NoCopy> Load() const;

			/// @brief Exchanges the contents of @a *this with @a other.
			///
			void						Swap(ShaderKey& other);


		// ---------------------------------------------------------------------------------------------------------------------------
		// Private member functions
		private:
			// Make default constructor private
										ShaderKey();

		
		// ---------------------------------------------------------------------------------------------------------------------------
		// Private variables
		private:
			std::tr1::function<bool(sf::Shader&)>	mLoader;
			std::string								mKey;

	
		// ---------------------------------------------------------------------------------------------------------------------------
		// Friends
		friend struct detail::KeyInfoAccessor;
	};


	/// @relates ImageKey 
	/// @brief Comparison operator for strict weak ordering
	bool THOR_API				operator< (const ImageKey& lhs, const ImageKey& rhs);

	/// @relates TextureKey 
	/// @brief Comparison operator for strict weak ordering
	bool THOR_API				operator< (const TextureKey& lhs, const TextureKey& rhs);

	/// @relates FontKey 
	/// @brief Comparison operator for strict weak ordering
	bool THOR_API				operator< (const FontKey& lhs, const FontKey& rhs);

	/// @relates ShaderKey 
	/// @brief Comparison operator for strict weak ordering
	bool THOR_API				operator< (const ShaderKey& lhs, const ShaderKey& rhs);


	/// @relates ImageKey 
	/// @brief Swaps two sf::Image resource keys.
	THOR_GLOBAL_SWAP(ImageKey)

	/// @relates TextureKey 
	/// @brief Swaps two sf::Texture resource keys.
	THOR_GLOBAL_SWAP(TextureKey)

	/// @relates FontKey
	/// @brief Swaps two sf::Font resource keys.
	THOR_GLOBAL_SWAP(FontKey)

	/// @relates ShaderKey 
	/// @brief Swaps two sf::Shader resource keys.
	THOR_GLOBAL_SWAP(ShaderKey)

} // namespace Resources

/// @}

} // namespace thor

#endif // THOR_GRAPHICSKEYS_HPP
