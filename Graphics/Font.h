#pragma once

#include "Graphics/Graphics.h"
#include "Engine/Resource.h"

#include "Engine/Serializer.h"
#include "Rendering/RRenderResource.h"

namespace Lunar
{
    L_DECLARE_RPTR( RTexture2d );

    /// Font resource.
    class LUNAR_GRAPHICS_API Font : public Resource
    {
        L_DECLARE_OBJECT( Font, Resource );

    public:
        /// Font texture sheet compression options.
        L_ENUM(
            ECompression,
            COMPRESSION,
            ( GRAYSCALE_UNCOMPRESSED )  // Grayscale only, no compression (8 bits per pixel).
            ( COLOR_COMPRESSED ) );     // Block compression (DXT1).

        /// Default texture point size.
        static const uint32_t DEFAULT_POINT_SIZE = 12;
        /// Default display resolution, in DPI.
        static const uint32_t DEFAULT_DPI = 72;

        /// Default texture sheet width, in pixels.
        static const uint16_t DEFAULT_TEXTURE_SHEET_WIDTH = 256;
        /// Default texture sheet height, in pixels.
        static const uint16_t DEFAULT_TEXTURE_SHEET_HEIGHT = 256;

        /// Default texture compression scheme.
        static const ECompression_Value DEFAULT_TEXTURE_COMPRESSION = COMPRESSION_COLOR_COMPRESSED;

        /// Character information.
        struct LUNAR_GRAPHICS_API Character
        {
            /// Unicode code point value.
            uint32_t codePoint;

            /// Horizontal pixel coordinate of the top-left corner of the character in the texture sheet.
            uint16_t imageX;
            /// Vertical pixel coordinate of the top-left corner of the character in the texture sheet.
            uint16_t imageY;
            /// Pixel width of the character in the texture sheet.
            uint16_t imageWidth;
            /// Pixel height of the character in the texture sheet.
            uint16_t imageHeight;

            /// Character bounding box width (26.6 fixed-point value, in pixels).
            int32_t width;
            /// Character bounding box height (26.6 fixed-point value, in pixels).
            int32_t height;
            /// Horizontal distance from the cursor position to the left-most border of the character bounding box (26.6
            /// fixed-point value, in pixels).
            int32_t bearingX;
            /// Vertical distance from the cursor position (on the baseline) to the top-most border of the character
            /// bounding box (26.6 fixed-point value, in pixels).
            int32_t bearingY;
            /// Horizontal distance used to increment the cursor position when drawing the character (26.6 fixed-point
            /// value, in pixels).
            int32_t advance;

            /// Texture sheet index.
            uint8_t texture;

            /// @name Serialization
            //@{
            void Serialize( Serializer& s );
            //@}
        };

        /// @name Construction/Destruction
        //@{
        Font();
        virtual ~Font();
        //@}

        /// @name Serialization
        //@{
        virtual void Serialize( Serializer& s );

        virtual bool NeedsPrecacheResourceData() const;
        virtual bool BeginPrecacheResourceData();
        virtual bool TryFinishPrecacheResourceData();
        //@}

        /// @name Resource Serialization
        //@{
        virtual void SerializePersistentResourceData( Serializer& s );
        //@}

        /// @name Resource Caching Support
        //@{
        virtual Name GetCacheName() const;
        //@}

        /// @name Data Access
        //@{
        inline float32_t GetPointSize() const;
        inline uint32_t GetDpi() const;

        inline uint16_t GetTextureSheetWidth() const;
        inline uint16_t GetTextureSheetHeight() const;

        inline ECompression GetTextureCompression() const;

        inline int32_t GetAscenderFixed() const;
        inline int32_t GetDescenderFixed() const;
        inline int32_t GetHeightFixed() const;
        inline int32_t GetMaxAdvanceFixed() const;

        inline float32_t GetAscenderFloat() const;
        inline float32_t GetDescenderFloat() const;
        inline float32_t GetHeightFloat() const;
        inline float32_t GetMaxAdvanceFloat() const;
        //@}

        /// @name Character Information
        /// All character codes must be provided as Unicode code points (this may require conversion of string data if
        /// the application is not built in Unicode mode, or for other special cases such as surrogate pairs in UTF-16
        /// strings).
        //@{
        inline uint32_t GetCharacterCount() const;
        inline const Character& GetCharacter( uint32_t index ) const;

        inline const Character* FindCharacter( uint32_t codePoint ) const;
        //@}

        /// @name Static Utility Functions
        //@{
        inline static float32_t Fixed26x6ToFloat32( int32_t value );
        inline static int32_t Float32ToFixed26x6( float32_t value );
        //@}

    private:
        /// Font size in points.
        float32_t m_pointSize;
        /// Font resolution, in DPI.
        uint32_t m_dpi;

        /// Width of each texture sheet.
        uint16_t m_textureSheetWidth;
        /// Height of each texture sheet.
        uint16_t m_textureSheetHeight;

        /// Texture compression scheme.
        ECompression m_textureCompression;

        /// Cached ascender height, in pixels (26.6 fixed-point value).
        int32_t m_ascender;
        /// Cached descender depth, in pixels (26.6 fixed-point value).
        int32_t m_descender;
        /// Cached font height, in pixels (26.6 fixed-point value).
        int32_t m_height;
        /// Maximum advance width when rendering text, in pixels (26.6 fixed-point value).
        int32_t m_maxAdvance;

        /// Array of characters (ordered by code point value to allow for binary searching).
        Character* m_pCharacters;
        /// Character count.
        uint32_t m_characterCount;

        /// Array of texture sheets.
        RTexture2dPtr* m_pspTextures;
        /// Texture sheet load IDs.
        size_t* m_pTextureLoadIds;
        /// Number of texture sheets.
        uint8_t m_textureCount;
    };
}

#include "Graphics/Font.inl"
