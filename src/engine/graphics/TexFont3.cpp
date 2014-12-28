/*----------------------------------------------------------------------------
    TEX FONT 3.CPP

    Copyright (c) 2011 eV Interactive, LLC
    contact: Matthew Harmon Matt@MatthewHarmon.com
*/

#include    <stdio.h>
//#include    "SysLib.h"              // Matt's operating system library
//#include    "I3d2.h"                // Matt's 3D rasterizer library
//#include    "I3dTexture2.h"
#include	"I3dIndxBuf.h"
#include	"I3dVertBuf.h"
//#include	"I3dRenderPrim.h"
//#include    "I3dShader1.h"
#include    "TexFont3.h"
#include    "TexFont3Bin.h"

#define MIN(a,b)    ((a) < (b) ? (a) : (b))

TEXFONT3*    TEXFONT3::gList          = NULL;
int          TEXFONT3::gRefCount      = 0;
SHADER1*     TEXFONT3::gShaderFont1   = NULL;
I3DVERTBUF*  TEXFONT3::gFontVb        = NULL;
I3DINDXBUF*  TEXFONT3::gFontIb        = NULL;
float        TEXFONT3::gLineLengths[TF3_MAX_LINES];
float        TEXFONT3::gSpaceAdjust[TF3_MAX_LINES];

TEXFONT3::~TEXFONT3()
    {
    gRefCount--;
    if (gRefCount == 0)
        {
        ShutdownClass();
        }

    SlSafeRelease(mTexture);
    if (mData != (char*)gConsoleFont)
        {
        SlSafeFree(mData);
        }
    }

void TEXFONT3::InitializeClass()
    {
    // Initialize shared font resources if they haven't been so far
    if (gShaderFont1 == NULL)
        gShaderFont1 = SHADER1::CreateFromFile("Font8Bit");

    if (gFontVb == NULL)
        gFontVb = new I3DVERTBUF(SL_TRUE, 
                                I3D_VC_XYZ | I3D_VC_DIFFUSE | I3D_VC_UV1,
                                TF3_MAX_CHARS * 4);

    if (gFontIb == NULL)
        {
        gFontIb = new I3DINDXBUF(SL_TRUE, TF3_MAX_CHARS * 6);
        gFontIb->Lock(TF3_MAX_CHARS*6);
        for (int i = 0; i < TF3_MAX_CHARS; i++)
            gFontIb->AddQuad(4*i, 4*i+1, 4*i+2, 4*i+3);
        gFontIb->Unlock();
        }
    }

I3DINDXBUF* TEXFONT3::GetFontIb()
    {
    return (gFontIb);
    }

void TEXFONT3::ShutdownClass()
    {
    SlSafeDelete(gFontVb);
    SlSafeDelete(gFontIb);
    SlSafeRelease(gShaderFont1);
    }

//-----------------------------------------------------------------------------
// Name: Create(filename)
// Desc: Loads a .fnt file and the corresponding texture
//-----------------------------------------------------------------------------
int TEXFONT3::Create(
    char*       fileName)
    {
    int         len;
    int         size;
    int         i;
    WCHAR       c;
    char        fileNameBuffer[SL_MAX_PATH];
    UBYTE*      pData;
    UDWORD      blockSize;

    // Initialize values
    mSpaceSize = 0.0f;
    mLineHeight = 0.0f;
    mKernBias = 0;
    mLeadingBias = 0.0f;
    mHorizScaleFactor = 1.0f;
    mVertScaleFactor = 1.0f;
    mSlantFactor = 0.0f;
    mHorizAlign = TF3_LEFT;
    mVertAlign = TF3_BASELINE;
    mColor = 0xFFFFFFFF;

    InitializeClass();

    // assure extension is ".fnt" in case no extension was supplied
    if (fileName)
        {
        SlStrCpy(fileNameBuffer, fileName);
        SlPathSetExtension(fileNameBuffer, "fnt");
        size = SlFileAllocLoad((void**)&mData, fileNameBuffer, SL_BINARY);
        if (size <= 0)
            return(SL_FAIL);
        }
    else
        {
        mData = (char*)gConsoleFont;
        size = sizeof(gConsoleFont);
        }

    // header
    if (mData[0] != 'B' || mData[1] != 'M' || mData[2] != 'F' || mData[3] != 3)
        return(SL_FAIL);
    pData = (UBYTE*)mData + 4;

    // block 1: info
    if (pData[0] != 1)
        return(SL_FAIL);
    pData++;
    blockSize = *(UDWORD*)(pData);
    SlSwapFromLittle32(&blockSize);
    pData += blockSize + 4;
    
    // block 2: common
    if (pData[0] != 2)
        return(SL_FAIL);
    pData++;
    blockSize = *(UDWORD*)(pData);
    SlSwapFromLittle32(&blockSize);
    pData += 4;
    if (blockSize != 15)
        return(SL_FAIL);
    mCommon = (TF3COMMON*)pData;
    SlSwapFromLittle16(&mCommon->lineHeight);
    SlSwapFromLittle16(&mCommon->base);
    SlSwapFromLittle16(&mCommon->scaleW);
    SlSwapFromLittle16(&mCommon->scaleH);
    SlSwapFromLittle16(&mCommon->pages);
    mLineHeight = mCommon->lineHeight;

    SlAssertEx(mCommon->pages == 1, "TexFont3 does not support fonts that span multiple textures at this time.");
    SlAssertEx(mCommon->packed == 0, "TexFont3 does not support packing by channel at this time.");

    pData = pData + blockSize;

    // block 3: pages
    if (pData[0] != 3)
        return(SL_FAIL);
    pData++;
    blockSize = *(UDWORD*)(pData);
    SlSwapFromLittle32(&blockSize);
    pData += 4;
    // load the font texture image - our texture loader doesn't need a file extension, so hack it
    if (fileName)
        {
        SlExtractPath(fileName, fileNameBuffer);
        len = SlStrLen(fileNameBuffer);
        SlStrCpy(fileNameBuffer + len, (char*)pData);
        SlPathSetExtension(fileNameBuffer, NULL);
        mTexture = I3DTEXTURE2::CreateFromFile(fileNameBuffer, NULL, TEX_MANAGED);
        }
    else
        {
        mTexture = LoadConsoleTexture();
        }

    if (!mTexture)
        return(SL_FAIL);

    pData = pData + blockSize;

    // block 4: chars
    if (pData[0] != 4)
        return(SL_FAIL);
    pData++;
    blockSize = *(UDWORD*)(pData);
    SlSwapFromLittle32(&blockSize);
    pData += 4;
    mNumGlyphs = blockSize/20;
    mGlyphs = (TF3GLYPH*)pData;
    mInvalidGlyph = NULL;

    for (i = 0; i < mNumGlyphs; i++)
        {
        SlSwapFromLittle32(&mGlyphs[i].id);
        SlSwapFromLittle16(&mGlyphs[i].x);
        SlSwapFromLittle16(&mGlyphs[i].y);
        SlSwapFromLittle16(&mGlyphs[i].width);
        SlSwapFromLittle16(&mGlyphs[i].height);
        SlSwapFromLittle16(&mGlyphs[i].xoffs);
        SlSwapFromLittle16(&mGlyphs[i].yoffs);
        SlSwapFromLittle16(&mGlyphs[i].xadvance);
        }

    // check if the invalid glyph was exported
    if (mNumGlyphs)
        {
        if (mGlyphs[0].id == -1)
            {
            mInvalidGlyph = &mGlyphs[0];
            mNumGlyphs--;
            if (mNumGlyphs)
                mGlyphs++;
            else
                mGlyphs = NULL;
            }
        }
    else
        mGlyphs = NULL;

    // figure out the width of a space; use the width of a period otherwise
    if (mGlyphs)
        {
        TF3GLYPH* g = GetGlyph(L' ');
        if (!g)
            g = GetGlyph(L'.');
        if (g)
            mSpaceSize = g->xadvance;
        }

    // make a lookup table for the ASCII portion of the table
    for (c = TF3_LATIN_START; c < TF3_LATIN_END; c++)
        {
        TF3GLYPH* g = GetGlyph(c);
        mLatinScript[c-TF3_LATIN_START] = g;
        }

    mLatinTable = true;

    pData += blockSize;
    
    // block 5: kerning pairs
    mNumKerningPairs = 0;
    mKerningPairs = NULL;

    if (pData - (UBYTE*)mData < size && pData[0] == 5)
        {
        pData++;
        blockSize = *(UDWORD*)(pData);
        SlSwapFromLittle32(&blockSize);
        mNumKerningPairs = blockSize/10;
        if (mNumKerningPairs)
            {
            pData += 4;
            mKerningPairs = (TF3KERNING*)pData;
            }
        }

    // TODO: kerning pairs need to be sorted somewhere

    if (fileName == NULL)
        mData = NULL;

    return (SL_SUCCESS);
}

//-----------------------------------------------------------------------------
// Name: LoadConsoleTexture()
// Desc: Copies the bitmap data for the built in font into a texture
//-----------------------------------------------------------------------------
I3DTEXTURE2* TEXFONT3::LoadConsoleTexture()
    {
    I3DTEXTURE2* texture;
    unsigned char* data;
    int width, height, pitch;
    int x, y;

    texture = I3DTEXTURE2::Create(
        TF3_CONSOLE_FONT_WIDTH, TF3_CONSOLE_FONT_HEIGHT, 
        FMT_A_8, 1, "TF3ConsoleFont", TEX_WRITABLE);

    if (!texture)
        return (NULL);

    texture->Lock(0, (void**)&data, &width, &height, &pitch);

    if (!data)
        {
        SlSafeRelease(texture);
        return (NULL);
        }

    // write from the monochrome bmp data into a texture
    for (y = TF3_CONSOLE_FONT_HEIGHT - 1; y >= 0; y--)
        for (x = 0; x < (TF3_CONSOLE_FONT_WIDTH/8); x++)
            {
            unsigned char b = gConsoleBitmap[y * (TF3_CONSOLE_FONT_WIDTH/8) + x];
            for (int j = 0; j < 8; j++)
                {
                unsigned char c = (0x80 & (b << j)) ? 255 : 0;
                *data = c; data++;
                }
            }

    texture->Unlock(0);

    return (texture);
    }


//-----------------------------------------------------------------------------
// Name: Create(filename)
// Desc: Loads a .fnt file and the corresponding texture
//-----------------------------------------------------------------------------
int TEXFONT3::Create()
    {
    return Create(NULL);
    }



//-----------------------------------------------------------------------------
// Name: DrawString()
// Desc: Draws text as textured polygons
//-----------------------------------------------------------------------------
int TEXFONT3::DrawString(
    float           fOriginX, 
    float           fOriginY, 
    const WCHAR*    strText)
    {
    return DrawTextbox(fOriginX, fOriginY, 0.0f, strText);
    }

//-----------------------------------------------------------------------------
// Name: DrawTextbox()
// Desc: Draws text as textured polygons
//-----------------------------------------------------------------------------
int TEXFONT3::DrawTextbox(
    float           x,
    float           y,
    float           w,
    const WCHAR*    strText)
    {
    MATRIX4     matPos;

    // Build it
    int count = BufferTextbox(w, strText, &gFontVb);

    // Position it
    //I3dSetViewTransform(Mat4GetIdentity());
    Mat4Identity(&matPos);
    Mat4Translate (&matPos, x, y, 0);
    I3dSetWorldTransform(&matPos);

    // Draw it
	I3dSelectBuffers(gFontVb, gFontIb);
    gShaderFont1->Begin();
    gShaderFont1->SetTexture(0, mTexture);
    gShaderFont1->SetupPass(0);
    I3dRenderPrimIdxNew(I3D_TRILIST, count * 2, 0, 0, 0, count * 4);
    gShaderFont1->End();
    
    // Reset matrices
    I3dSetViewTransform(Mat4GetIdentity());
    I3dSetWorldTransform(Mat4GetIdentity());

    return(0);
    }


//-----------------------------------------------------------------------------
// Name: GetStringWidth()
// Desc: Finds the line length of a string
//-----------------------------------------------------------------------------
float TEXFONT3::GetStringWidth(
    const WCHAR*    strText)
    {
    GetStringSize(strText, 0.0f, gLineLengths, NULL);
    return gLineLengths[0];
    }

//-----------------------------------------------------------------------------
// Name: GetStringSize()
// Desc: Finds the line extents of a string in a textbox
//-----------------------------------------------------------------------------
int TEXFONT3::GetStringSize(
    const WCHAR*    strText,
    float           margin,
    float*          lineLengths,
    int*            charCount)
    {
    TF3GLYPH* prev    = NULL;
    float   cursor    = 0.0f;
    float   start     = 0.0f;
    float   end       = 0.0f;

    int     numLines  = 0;
    int     numChars  = 0;
    int     lineSpaces = 0;
    bool    wrap;

    wrap = (margin > 0.0f);

    // starting from the upper left you don't need to count lines or line lengths
    if (!wrap && mHorizAlign == TF3_LEFT && 
        (mVertAlign == TF3_BASELINE || mVertAlign == TF3_TOP))
        return (0);

    // check parameters and font state
    if (lineLengths == NULL)
        {
        if (charCount)
            *charCount = 0;
        return (0);
        }
    else if (strText == NULL || mGlyphs == NULL)
        {
        lineLengths[0] = 0.0f;
        if (charCount)
            *charCount = 0;
        return (0);
        }

    // loop until end of string or running out of lines
    for (; *strText && numLines < TF3_MAX_LINES - 1; strText++)
        {
        WCHAR c = *strText;
        TF3GLYPH* g = NULL;

        if (c == L' ')
            {
            if (prev)
                {
                end = cursor;
                prev = NULL;
                }
            cursor += (mSpaceSize + mKernBias) * mHorizScaleFactor;
            lineSpaces++;
            start = cursor;
            }
        else if (c == L'\t')
            {
            if (prev)
                {
                end = cursor;
                prev = NULL;
                }
            float tabSize = (mSpaceSize + mKernBias) * TF3_TAB_WIDTH * mHorizScaleFactor;
            int tabs = (int)(cursor/tabSize);
            cursor = tabSize * (tabs+1);
            start = cursor;
            }
        else if (c == L'\n' && wrap)
            {
            if (mHorizAlign == TF3_JUSTIFIED)
                {
                lineLengths[numLines] = cursor;
                gSpaceAdjust[numLines] = 0.0f;
                lineSpaces = 0;
                }
            else
                {
                lineLengths[numLines] = cursor;
                }

            numLines++;
            cursor = 0.0f;
            start = 0.0f;
            end = 0.0f;
            prev = NULL;
            }
        else
            {
            g = GetGlyph(c);

            // count the width of visible characters
            if (g)
                {
#ifdef TF3_KERNING
                cursor += GetKerningPair(prev, c) * mHorizScaleFactor;
#endif
                cursor += (g->xadvance + mKernBias) * mHorizScaleFactor;

                if (wrap && cursor > margin && end > 0.0f)
                    {
                    if (mHorizAlign == TF3_JUSTIFIED)
                        {
                        lineLengths[numLines] = margin;
                        gSpaceAdjust[numLines] = (margin - end) / lineSpaces;
                        lineSpaces = 0;
                        }
                    else
                        {
                        lineLengths[numLines] = end;
                        }

                    numLines++;
                    cursor -= start;
                    start = 0.0f;
                    end = 0.0f;
                    }

                // only count visible characters
                numChars++;
                prev = g;
                }
            }
        }

    if (charCount)
        *charCount = numChars;

    if (cursor > 0.0f || numLines == 0)
        {
        lineLengths[numLines] = cursor;
        if (mHorizAlign == TF3_JUSTIFIED)
            gSpaceAdjust[numLines] = 0.0f;
        numLines++;
        }

    return (numLines);
    }

//-----------------------------------------------------------------------------
// Name: BufferTextbox()
// Desc: Renders text to a vertex buffer
//-----------------------------------------------------------------------------
int TEXFONT3::BufferTextbox(
    float           width,
    const WCHAR*    strText,
    I3DVERTBUF**    vbPtr)
    {
    I3DVERTBUF* vb;

    TF3GLYPH*   prev      = NULL;
    float       cursor    = 0.0f;
    float       start     = 0.0f;

    int         count;
    int         numLines  = 0;
    int         numChars  = 0;
    bool        wrap;

    float       texWidth;
    float       texHeight;

    SlAssert(vbPtr);
    SlAssert(mGlyphs);
    SlAssert(mTexture);

    if (mGlyphs == NULL || mTexture == NULL || strText == NULL)
        return(0);

    // find the texture dimensions
    texWidth = (float)(mTexture->GetWidth());
    texHeight = (float)(mTexture->GetHeight());

    // find text dimensions
    wrap = (width > 0.0f);
    int lines = GetStringSize(strText, width, gLineLengths, &count);
    float cursorX = XOffset(gLineLengths[0]);
    float cursorY = YOffset(lines);

    // get vertex buffer
    if (*vbPtr == NULL)
        {
        *vbPtr = gFontVb;
        }
    vb = *vbPtr;

    // fill vertex bufer, prevent overflow of font buffer
    vb->Lock();
    for (; *strText && numChars < TF3_MAX_CHARS; strText++)
        {
        WCHAR c = *strText;
        TF3GLYPH* g = NULL;

        if (c == L' ')
            {
            prev = NULL;
            cursor += (mSpaceSize + mKernBias) * mHorizScaleFactor;
            if (mHorizAlign == TF3_JUSTIFIED)
                cursor += gSpaceAdjust[numLines];
            start = cursor;
            }
        else if (c == L'\t')
            {
            prev = NULL;
            float tabSize = (mSpaceSize + mKernBias) * TF3_TAB_WIDTH * mHorizScaleFactor;
            int tabs = (int)(cursor/tabSize);
            cursor = tabSize * (tabs+1);
            start = cursor;
            }
        else if (wrap && c == L'\n')
            {
            numLines++;
            cursorY += (mLineHeight + mLeadingBias) * mVertScaleFactor;
            cursorX = XOffset(gLineLengths[numLines]);
            cursor = 0.0f;
            start = 0.0f;
            prev = NULL;
            }
        else
            {
            g = GetGlyph(c);

            // count the width of visible characters
            if (g)
                {
#ifdef TF3_KERNING
                cursor += GetKerningPair(prev, c) * mHorizScaleFactor;
#endif
                float newCursor = cursor;
                newCursor += (g->xadvance + mKernBias) * mHorizScaleFactor;

                if (wrap && newCursor > gLineLengths[numLines])
                    {
                    numLines++;
                    if (numLines >= TF3_MAX_LINES)
                        break;
                    cursorY += (mLineHeight + mLeadingBias) * mVertScaleFactor;
                    cursorX = XOffset(gLineLengths[numLines]);
                    cursor -= start;
                    newCursor -= start;
                    start = 0.0f;
                    }

                float x1 = (int)(cursorX + cursor) + (g->xoffs) * mHorizScaleFactor;
                float x2 = (int)(cursorX + cursor) + (g->xoffs + g->width) * mHorizScaleFactor;
                float y1 = cursorY + (g->yoffs) * mVertScaleFactor;
                float y2 = cursorY + (g->yoffs + g->height) * mVertScaleFactor;

                float uOffs = 0.5f / mHorizScaleFactor;
                float vOffs = 0.5f / mVertScaleFactor;
                float u1 = (g->x + uOffs)/texWidth;
                float u2 = (g->x + g->width + uOffs)/texWidth;
                float v1 = (g->y + vOffs)/texHeight;
                float v2 = (g->y + g->height + vOffs)/texHeight;

                float slantTop = (mCommon->base - g->yoffs) * mVertScaleFactor * mSlantFactor * mHorizScaleFactor;
                float slantBottom = (mCommon->base - g->yoffs - g->height) * mVertScaleFactor * mSlantFactor * mHorizScaleFactor;

                vb->SetPos(x1 + slantBottom, y2,  0.0f);
                vb->SetUV0(u1, v2);
                vb->SetColor(mColor);
                vb->NextVert();

                vb->SetPos(x1 + slantTop, y1,  0.0f);
                vb->SetUV0(u1, v1);
                vb->SetColor(mColor);
                vb->NextVert();

                vb->SetPos(x2 + slantTop, y1,  0.0f);
                vb->SetUV0(u2, v1);
                vb->SetColor(mColor);
                vb->NextVert();

                vb->SetPos(x2 + slantBottom, y2,  0.0f);
                vb->SetUV0(u2, v2);
                vb->SetColor(mColor);
                vb->NextVert();

                // only count visible characters
                cursor = newCursor;
                numChars++;
                prev = g;
                }
            }
        }
    vb->Unlock();

    return(numChars);
    }
