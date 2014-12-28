#ifndef _TEXTURE_H
#define _TEXTURE_H

#include "../common/resource.h"
#include "graphics.h"
#include <string>

class CTexture
{
public:
	CTexture();
	CTexture( std::string file );
	~CTexture();

	void LoadAlpha( std::string file );
	void Load( std::string file );
	void CreateBlank( int width, int height );
	void Unload();

	int Width();
	int Height();

	void Bind();
	void Unbind();

	bool IsLoaded();

protected:
	int mWidth, mHeight;
	static bool gTexEnabled;
	bool mLoaded;
	GLuint mTextureId;
};

class CManagedTexture: public CManagedResource
{
public:
	CManagedTexture( std::string file, bool alpha);
	~CManagedTexture(); 

	static CTexture* Load( CResourceManager* m, std::string file, bool alpha );
	static CTexture* Load( CResourceManager* m, std::string file );
	static bool Unload( CResourceManager* m, CTexture* t );

	CTexture* GetTexture();

protected:
	bool LoadResource( std::string file, bool alpha );
	void UnloadResource();

	CTexture mData;
};



#endif
