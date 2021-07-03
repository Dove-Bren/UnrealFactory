#pragma once

// Convenience class for wrapping a dynamically-sized 0-point origin 2d array.
// Only one element (type T) is allowed per 'cell'.
// Cells are automatically grouped into 'chunks' for faster lookup of nearby cells.

#include <vector>

template<class T>
struct ElementChunk;

template<class T>
class Layout2D
{
private:
	// Array of element chunks.
	// New chunks added at end.
	std::vector<ElementChunk<T>> ElementChunks;

	ElementChunk<T> *AddChunk(int X, int Y);

	ElementChunk<T> *EnsureChunk(int X, int Y);

	ElementChunk<T> *GetChunk(int X, int Y);

public:

	Layout2D(int StartRadius);
	Layout2D() : Layout2D(10) {};
	virtual ~Layout2D() = default;

	virtual T *Get(int X, int Y);

	virtual T *Remove(int X, int Y);

	// Returns the element in the slot even if bReplace is false.
	virtual T *Insert(int X, int Y, T *Element, bool bReplace);

};



#define CHUNK_SIDE_LEN 16
#define CHUNK_SIZE CHUNK_SIDE_LEN * CHUNK_SIDE_LEN
template<class T>
struct ElementChunk
{
	int32_t WorldOffsetX;
	int32_t WorldOffsetY;

	T *Elements[CHUNK_SIZE];

	ElementChunk(int X, int Y) : WorldOffsetX(X), WorldOffsetY(Y), Elements() {};
	T *GetFromOffset(int OffsetX, int OffsetY);
	T *GetFromWorld(int WorldX, int WorldY);
	T *SetFromOffset(int OffsetX, int OffsetY, T *Element);
	T *SetFromWorld(int WorldX, int WorldY, T *Element);
};

#define TO_CHUNK_OFFSET(X) (X % CHUNK_SIDE_LEN)
#define TO_CHUNK_BOUNDARY(X) (X - TO_CHUNK_OFFSET(X))

static_assert(CHUNK_SIDE_LEN % 2 == 0, "Chunk side length MUST be a multiple of 2");
static_assert(CHUNK_SIDE_LEN > 0, "Chunk side length MUST be > 0");

// Make Chunk##X and Chunk##Y hold the offsets within the chunk being referred
#define CHUNK_POS(X, Y) int Chunk##X = TO_CHUNK_BOUNDARY(X), Chunk##Y = TO_CHUNK_BOUNDARY(Y)

// Make Chunk##X and Chunk##Y hold the offsets within the chunk being referred
#define OFFSET_POS(X, Y) int Offset##X = TO_CHUNK_OFFSET(X), Offset##Y = TO_CHUNK_OFFSET(Y)

template<class T>
T *ElementChunk<T>::GetFromOffset(int OffsetX, int OffsetY)
{
	int Idx = OffsetX + (OffsetY * CHUNK_SIDE_LEN);
	if (Idx < CHUNK_SIZE)
	{
		return Elements[Idx];
	}

	return nullptr;
}

template<class T>
T *ElementChunk<T>::GetFromWorld(int WorldX, int WorldY)
{
	OFFSET_POS(WorldX, WorldY);
	return GetFromOffset(OffsetWorldX, OffsetWorldY);
}

template<class T>
T *ElementChunk<T>::SetFromOffset(int OffsetX, int OffsetY, T *Element)
{
	int Idx = OffsetX + (OffsetY * CHUNK_SIDE_LEN);
	T *Existing = nullptr;
	if (Idx < CHUNK_SIZE)
	{
		Existing = Elements[Idx];
		Elements[Idx] = Element;
	}

	return Existing;
}

template<class T>
T *ElementChunk<T>::SetFromWorld(int WorldX, int WorldY, T *Element)
{
	OFFSET_POS(WorldX, WorldY);
	return SetFromOffset(OffsetWorldX, OffsetWorldY, Element);
}

template<class T>
Layout2D<T>::Layout2D(int StartRadius)
{
	for (int x = -StartRadius; x <= StartRadius; x++)
		for (int y = -StartRadius; y <= StartRadius; y++)
		{
			AddChunk(x * CHUNK_SIDE_LEN, y * CHUNK_SIDE_LEN);
		}
}

template<class T>
ElementChunk<T> *Layout2D<T>::AddChunk(int X, int Y)
{
	CHUNK_POS(X, Y);
	ElementChunks.push_back(ElementChunk<T>(ChunkX, ChunkY));
	return &ElementChunks.back();
}

template<class T>
ElementChunk<T> *Layout2D<T>::GetChunk(int X, int Y)
{
	CHUNK_POS(X, Y);
	ElementChunk<T> *Chunk = nullptr;
	for (auto Iter = ElementChunks.begin(); Iter != ElementChunks.end(); Iter++)
	{
		if ((*Iter).WorldOffsetX == ChunkX && (*Iter).WorldOffsetY == ChunkY)
		{
			Chunk = &(*Iter);
			break;
		}
	}
	return Chunk;
}

template<class T>
ElementChunk<T> *Layout2D<T>::EnsureChunk(int X, int Y)
{
	ElementChunk<T> Chunk = nullptr;
	if (GetChunk(X, Y) == nullptr)
	{
		Chunk = AddChunk(X, Y);
	}

	return Chunk;
}

template<class T>
T *Layout2D<T>::Get(int X, int Y)
{
	T *Element = nullptr;
	ElementChunk<T> *Chunk = GetChunk(X, Y);

	if (Chunk)
	{
		Element = Chunk->GetFromWorld(X, Y);
	}

	return Element;
}

template<class T>
T *Layout2D<T>::Remove(int X, int Y)
{
	T *Element = nullptr;
	ElementChunk<T> *Chunk = GetChunk(X, Y);

	if (Chunk)
	{
		Element = Chunk->SetFromWorld(X, Y, nullptr);
	}

	return Element;
}

template<class T>
T *Layout2D<T>::Insert(int X, int Y, T *Element, bool bReplace)
{
	T *Existing = nullptr;
	ElementChunk<T> *Chunk = GetChunk(X, Y);

	if (Chunk)
	{
		Existing = Chunk->GetFromWorld(X, Y);
		if (bReplace || Existing == nullptr)
		{
			Chunk->SetFromOffset(X, Y, Element);
		}
	}

	return Element;
}