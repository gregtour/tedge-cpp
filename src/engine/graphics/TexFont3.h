/*----------------------------------------------------------------------------
    TEX FONT 3
    Rev 3 of our texture font drawing package.
    Uses Bitmap Font Generator from AngelCode which can be found here:
    http://www.angelcode.com/products/bmfont/

    His code for loading / rendering the fonts can be found here:
    http://www.angelcode.com/toolbox/

    Notes:

    * Should everything be done with ints or floats.  That is, we want to support
      pixel-perfect drawing when needed, but may also like sub-pixel positioning.
    * Not sure if his binary format is load-and-go.
    * Need a byte-swap feature for Wii.

*/

/*--------------------------------------------------------------------------*/
/*                           Compilation Control                            */
/*--------------------------------------------------------------------------*/

#ifndef _TEXFONT3_H
#define _TEXFONT3_H


/*--------------------------------------------------------------------------*/
/*                              Include Files                               */
/*--------------------------------------------------------------------------*/

//#include    "ManagedResource.h"

typedef unsigned char UBYTE;
typedef signed short SWORD;
// .. L"STRING"
typedef char WCHAR;

/*--------------------------------------------------------------------------*/
/*                            Defined Constants                             */
/*--------------------------------------------------------------------------*/

#define TF3_MAX_CHARS           512
#define TF3_MAX_LINES           128
#define TF3_TAB_WIDTH           8

#define TF3_LATIN_START         0x20
#define TF3_LATIN_END           0x7F
#define TF3_LATIN_SCRIPT_SIZE   (TF3_LATIN_END-TF3_LATIN_START)

#define GLYPH_WII_A             L"\500"         // Wiimote control glyphs
#define GLYPH_WII_B             L"\501" 
#define GLYPH_WII_1             L"\502"
#define GLYPH_WII_2             L"\503" 
#define GLYPH_WII_M             L"\504"
#define GLYPH_WII_P             L"\505"
#define GLYPH_WII_DPAD          L"\506"
#define GLYPH_WII_DPADLR        L"\507"
#define GLYPH_INFINITY          L"\508"

#define GLYPH_A_BUTTON          L"\400"         // xbox control glyphs
#define GLYPH_B_BUTTON          L"\401" 
#define GLYPH_X_BUTTON          L"\402" 
#define GLYPH_Y_BUTTON          L"\403" 
#define GLYPH_WHITE_BUTTON      L"\404" 
#define GLYPH_BLACK_BUTTON      L"\405" 
#define GLYPH_LEFT_BUTTON       L"\406" 
#define GLYPH_RIGHT_BUTTON      L"\407" 
#define GLYPH_START1_BUTTON     L"\410" 
#define GLYPH_START2_BUTTON     L"\411" 
#define GLYPH_BACK1_BUTTON      L"\412" 
#define GLYPH_BACK2_BUTTON      L"\413"

#define GLYPH_HUNTER            L"\600"         // custom glyphs
#define GLYPH_RHINO             L"\601"

// these glyphs get mapped from above based on platform
#if defined(RVL)
#define GLYPH_A               GLYPH_WII_A
#define GLYPH_B               GLYPH_WII_B
#define GLYPH_1               GLYPH_WII_1
#define GLYPH_2               GLYPH_WII_2
#define GLYPH_M               GLYPH_WII_M
#define GLYPH_P               GLYPH_WII_P
#define GLYPH_DPAD            GLYPH_WII_DPAD
#define GLYPH_DPADLR          GLYPH_WII_DPADLR
#else // Pc/Xbox
#define GLYPH_A               GLYPH_WII_A//GLYPH_A_BUTTON
#define GLYPH_B               GLYPH_WII_B//GLYPH_B_BUTTON
#define GLYPH_1               GLYPH_WII_1//GLYPH_X_BUTTON
#define GLYPH_2               GLYPH_WII_2//GLYPH_Y_BUTTON
#define GLYPH_M               GLYPH_WII_M
#define GLYPH_P               GLYPH_WII_P
#define GLYPH_DPAD            GLYPH_WII_DPAD
#define GLYPH_DPADLR          GLYPH_WII_DPADLR
#endif


/*--------------------------------------------------------------------------*/
/*                              Data Structures                             */
/*--------------------------------------------------------------------------*/

struct TF3COMMON
    {
    UWORD   lineHeight;
    UWORD   base;
    UWORD   scaleW;
    UWORD   scaleH;
    UWORD   pages;
    UBYTE   packed;
    UBYTE   alphaChnl;
    UBYTE   redChnl;
    UBYTE   greenChnl;
    UBYTE   blueChnl;
    };


struct TF3GLYPH
    {
    UDWORD  id;
    UWORD   x;
    UWORD   y;
    UWORD   width;
    UWORD   height;
    SWORD   xoffs;
    SWORD   yoffs;
    SWORD   xadvance;
    union 
        {
        struct { UBYTE   page, channel; } loc;
        UWORD kern;
        } footer;
    };

struct TF3KERNING
    {
    UDWORD  first;
    UDWORD  second;
    SWORD   amount;
    };

enum TF3_HALIGN
    {
    TF3_LEFT = 0,
    TF3_CENTER,
    TF3_RIGHT,
    TF3_JUSTIFIED
    };

enum TF3_VALIGN
    {
    TF3_TOP = 0,
    TF3_BASELINE,
    TF3_VCENTER,
    TF3_BOTTOM,
    };
class MANAGEDRES {} ;
class TEXFONT3 : public MANAGEDRES
    {
    public:

    static TEXFONT3*    gList;
    static TEXFONT3*    CreateFromFile (char*           filePath);
    static TEXFONT3*    Find           (char*           name);

                        TEXFONT3       ();
                        ~TEXFONT3      ();

    int                 Create         (char*           fileName);
    int                 Create         (void);
    
    float               GetHeight      (void) const;
    float               GetGlyphWidth  (const WCHAR     c) const;
    float               GetStringWidth (const WCHAR*    strText);

    int                 DrawString     (float           x, 
                                        float           y, 
                                        const WCHAR*    strText);

    int                 DrawTextbox    (float           x,
                                        float           y,
                                        float           w,
                                        const WCHAR*    strText);

    I3DVERTBUF*         BufferString   (const WCHAR*    strText);

    int                 BufferTextbox  (float           w,
                                        const WCHAR*    strText,
                                        I3DVERTBUF**    vbPtr);

    I3DINDXBUF*         GetFontIb      ();

/*
    SETGETMEMBER       (float,      LeadingBias);
    SETGETMEMBER       (int,        KernBias);
    SETGETMEMBER       (float,      HorizScaleFactor);
    SETGETMEMBER       (float,      VertScaleFactor);
    SETGETMEMBER       (float,      SlantFactor);
    SETGETMEMBER       (TF3_HALIGN, HorizAlign);
    SETGETMEMBER       (TF3_VALIGN, VertAlign);
    SETGETMEMBER       (COLOR4B,    Color);
    */

    float       mLeadingBias;
    int         mKernBias;
    float       mHorizScaleFactor;
    float       mVertScaleFactor;
    float       mSlantFactor;
    TF3_HALIGN  mHorizAlign;
    TF3_VALIGN  mVertAlign;
    COLOR4B     mColor;

    private:

    static unsigned char gConsoleFont[];
    static unsigned char gConsoleBitmap[];
    static int          gRefCount;
    static SHADER1*     gShaderFont1;
    static I3DVERTBUF*  gFontVb;
    static I3DINDXBUF*  gFontIb;
    static float        gLineLengths[TF3_MAX_LINES];
    static float        gSpaceAdjust[TF3_MAX_LINES];

    static void         InitializeClass();
    static void         ShutdownClass();

    char*               mData;      // .fnt file binary data 
    TF3COMMON*          mCommon;
    I3DTEXTURE2*        mTexture;
    TF3GLYPH*           mGlyphs;
    bool                mLatinTable;
    TF3GLYPH*           mLatinScript[TF3_LATIN_SCRIPT_SIZE];
    TF3GLYPH*           mInvalidGlyph;
    int                 mNumGlyphs;
    TF3KERNING*         mKerningPairs;
    int                 mNumKerningPairs;
    float               mSpaceSize;
    float               mLineHeight;

    float               XOffset            (float           width);
    float               YOffset            (int             lines);

    int                 GetStringSize      (const WCHAR*    strText,
                                            float           margin,
                                            float*          lines,
                                            int*            count);

    TF3GLYPH*           GetGlyph           (const WCHAR     c) const;

    int                 BuildLatinTable    (void);

    float               GetKerningPair     (TF3GLYPH*       first,
                                            WCHAR           second) const;

    I3DTEXTURE2*        LoadConsoleTexture (void);
    };  

/*----------------------------------------------------------------------------
    TEXFONT3 - Constructor
*/
inline TEXFONT3::TEXFONT3()
    {
    mGlyphs = NULL;
    mKerningPairs = NULL;
    mLatinTable = false;
    mInvalidGlyph = NULL;
    mCommon = NULL;
    mNumGlyphs = 0;
    mTexture = NULL;
    if (gRefCount == 0)
        InitializeClass();
    gRefCount++;
    }


/*----------------------------------------------------------------------------
    GetGlyph - Look up the character from the table or use binary search
*/
inline TF3GLYPH* TEXFONT3::GetGlyph(
    const WCHAR     c) const
    {
    SlAssertEx(mGlyphs != NULL, "Glyphs not loaded.");

    // ASCII table
    if (mLatinTable && c < TF3_LATIN_END && c >= TF3_LATIN_START)
        return (mLatinScript[c - TF3_LATIN_START]);

    // binary search for extended characters
    int i, min, max;
    min = 0;
    max = mNumGlyphs - 1;
    while (min <= max)
        {
        i = min + (max - min)/2;
        WCHAR id = (WCHAR)mGlyphs[i].id;
        if (id < c)
            min = i + 1;
        else if (id > c)
            max = i - 1;
        else
            return &mGlyphs[i];
        }
    return mInvalidGlyph;
    }

/*----------------------------------------------------------------------------
    BuildLatinTable - Build a lookup table for ASCII characters
*/
inline int TEXFONT3::BuildLatinTable()
    {
    // clear the lookup table
    int i, num;
    for (i = 0; i < TF3_LATIN_SCRIPT_SIZE; i++)
        mLatinScript[i] = NULL;

    // fill it in
    num = 0;
    for (i = 0; i < mNumGlyphs; i++)
        {
        if (mGlyphs[i].id < TF3_LATIN_END && mGlyphs[i].id >= TF3_LATIN_START)
            {
            mLatinScript[i - TF3_LATIN_START] = &mGlyphs[i];
            num++;
            }
        }

    mLatinTable = true;

    return num;
    }

/*----------------------------------------------------------------------------
    GetGlyphWidth - Return the width of a character with current state
*/
inline float TEXFONT3::GetGlyphWidth(
    const WCHAR     c) const
    {
    TF3GLYPH* g = GetGlyph(c);
    if (g)
        return((g->xadvance + mKernBias) * mHorizScaleFactor);
    else if (c == L' ')
        return((mSpaceSize + mKernBias) * mHorizScaleFactor);
    else if (c == L'\t')
        return((mSpaceSize + mKernBias) * TF3_TAB_WIDTH * mHorizScaleFactor);
    else if (mInvalidGlyph && c != L'\n')
        return ((mInvalidGlyph->xadvance + mKernBias) * mHorizScaleFactor);
    else
        return(0.0f);
    }

//-----------------------------------------------------------------------------
// Name: GetHeight()
// Desc: Returns the line height with given state
//-----------------------------------------------------------------------------
inline float TEXFONT3::GetHeight(void) const
    {
    return (mLineHeight + mLeadingBias) * mVertScaleFactor;
    }

/*----------------------------------------------------------------------------
    GetKerningPair - Scan the kerning pair table for an entry
    TODO: kerning pair table must be sorted to work
*/
inline float TEXFONT3::GetKerningPair(
    TF3GLYPH*       first,
    WCHAR           second) const
    {
#ifdef Tf3_KERNING
    if (first == NULL || mKerningPairs == NULL || first->footer.kern < 0)
        return 0.0f;

    int i = first->footer.kern;
    while (i < mNumKerningPairs && mKerningPairs[i].first == first->id)
        {
        if (mKerningPairs[i].second == second)
            return (float)mKerningPairs[i].amount;
        i++;
        }
#endif
    return 0.0f;
    }


/*----------------------------------------------------------------------------
    XOffset - Adjust for horizontal alignment
*/
inline float TEXFONT3::XOffset(
    float       width)
    {
    if (mHorizAlign == TF3_LEFT || mHorizAlign == TF3_JUSTIFIED)
        return 0.0f;
    else if (mHorizAlign == TF3_CENTER)
        return -width/2.0f;
    else if (mHorizAlign == TF3_RIGHT)
        return -width;
    
    SlAssertEx(false, "Invalid horizontal alignment.");
    return(0.0f);
    }

/*----------------------------------------------------------------------------
    YOffset - Adjust for vertical alignment
*/
inline float TEXFONT3::YOffset(
    int         lines)
    {
    float height = lines * (mLineHeight + mLeadingBias) * mVertScaleFactor;

    if (mVertAlign == TF3_TOP)
        return(0.0f);
    else if (mVertAlign == TF3_BASELINE)
        {
        if (mCommon)
            return(float)(-mCommon->base * mVertScaleFactor);
        else
            return(-mLineHeight * mVertScaleFactor);
        }
    else if (mVertAlign == TF3_VCENTER)
        return(-height/2.0f);
    else if (mVertAlign == TF3_BOTTOM)
        return(-height);

    SlAssertEx(false, "Invalid vertical alignment.");
    return(0.0f);
    }

/*----------------------------------------------------------------------------
    CREATE FROM FILE
*/

inline TEXFONT3* TEXFONT3::CreateFromFile(
    char*           filePath)
    {
    char            fileName[DS2_NAME_LEN];
    TEXFONT3*       f;

    // see if it already exists and return a reference if it does
    SlExtractFileName(filePath, fileName);
    f = (TEXFONT3*)MANAGEDRES::GetReference(fileName, gList);
    if (f)
        return(f);

    // create a font from the given file
    f = new TEXFONT3();
    f->Create(filePath);

    // if the file didn't exist, or creation failed, free and abort
    if (f->mTexture == NULL)
        {
        delete f;
        return(NULL);
        }

    f->SetName(fileName);
    f->Hook((MANAGEDRES**)&gList);

    return(f);
    }


/*----------------------------------------------------------------------------
    FIND
*/

inline TEXFONT3* TEXFONT3::Find(
    char*   name)
    {
    return( (TEXFONT3*)FindByName(name, (MANAGEDRES*)gList) );
    }


/*--------------------------------------------------------------------------*/
/*                           Compilation Control                            */
/*--------------------------------------------------------------------------*/

#endif  // TEXFONT3_H
