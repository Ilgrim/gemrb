/* GemRB - Infinity Engine Emulator
 * Copyright (C) 2003 The GemRB Project
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * $Header: /data/gemrb/cvs2svn/gemrb/gemrb/gemrb/plugins/Core/Map.h,v 1.65 2005/03/15 17:53:15 avenger_teambg Exp $
 *
 */

class Map;

#ifndef MAP_H
#define MAP_H

#include "TileMap.h"
#include "ImageMgr.h"
#include "Actor.h"
#include "ScriptedAnimation.h"
#include "GameControl.h"
#include "PathFinder.h"
#include <queue>

class Ambient;

#ifdef WIN32

#ifdef GEM_BUILD_DLL
#define GEM_EXPORT __declspec(dllexport)
#else
#define GEM_EXPORT __declspec(dllimport)
#endif

#else
#define GEM_EXPORT
#endif

typedef struct SongHeaderType {
	ieDword SongList[5];
} SongHeaderType;

typedef struct RestHeaderType {
	ieDword Strref[10];
	ieResRef CreResRef[10];
	ieWord CreatureNum;
	ieWord DayChance;
	ieWord NightChance;
} RestHeaderType;

typedef struct WallGroup {
	Gem_Polygon** polys;
	int polygons;
} WallGroup;

typedef struct Entrance {
	char Name[33];
	Point Pos;
	ieDword Face;
} Entrance;

typedef class MapNote {
public:
	Point Pos;
	ieWord color;
	char *text;
	MapNote() { text=NULL; };
	~MapNote() { if(text) free(text); };
} MapNote;

class GEM_EXPORT Map : public Scriptable {
public:
	TileMap* TMap;
	ImageMgr* LightMap;
	ImageMgr* SearchMap;
	ImageMgr* SmallMap;
	ieDword AreaFlags;
	ieWord AreaType;
	bool ChangeArea; //set true if movement is allowed between areas
	Variables *vars;
	ieByte* ExploredBitmap;
	ieByte* VisibleBitmap;
private:
	unsigned short* MapSet;
	std::queue< unsigned int> InternalStack;
	unsigned int Width, Height;
	std::vector< Animation*> animations;
	std::vector< Actor*> actors;
	std::vector< WallGroup*> wallGroups;
	std::vector< ScriptedAnimation*> vvcCells;
	std::vector< Entrance*> entrances;
	std::vector< Ambient*> ambients;
	std::vector< MapNote*> mapnotes;
	Actor** queue[3];
	int Qcount[3];
	unsigned int lastActorCount[3];
	void GenerateQueue(int priority);
	Actor* GetRoot(int priority);
	void DeleteActor(Actor* actor);
public:
	Map(void);
	~Map(void);
	/** prints useful information on console */
	void DebugDump();
	void AddTileMap(TileMap* tm, ImageMgr* lm, ImageMgr* sr, ImageMgr* sm);
	void CreateMovement(char *command, const char *area, const char *entrance);
	void DrawMap(Region viewport, GameControl* gc);
	void PlayAreaSong(int);
	void AddAnimation(Animation* anim);
	Animation* GetAnimation(const char* Name);
	void Shout(Scriptable* Sender, int shoutID, unsigned int radius);
	void AddActor(Actor* actor);
	void AddWallGroup(WallGroup* wg);
	int GetBlocked(Point &p);
	Actor* GetActor(Point &p, int flags);
	Actor* GetActor(const char* Name);
	Actor* GetActorByDialog(const char* resref);
	bool HasActor(Actor *actor);
	void RemoveActor(Actor* actor);
	//returns actors in rect (onlyparty could be more sophisticated)
	int GetActorInRect(Actor**& actors, Region& rgn, bool onlyparty);
	SongHeaderType SongHeader;
	RestHeaderType RestHeader;
	void AddVVCCell(ScriptedAnimation* vvc);
	void AddEntrance(char* Name, int XPos, int YPos, short Face);
	Entrance* GetEntrance(const char* Name);
	bool CanFree();
	Actor* GetActor(int i) { return actors[i]; }
	int GetActorCount() const { return (int) actors.size(); }
	int GetWidth() const { return Width; }
	int GetHeight() const { return Height; }
	int GetExploredMapSize() const;
	/*fills the explored bitmap with setreset */
	void Explore(int setreset);
	/*fills the visible bitmap with setreset */
	void SetMapVisibility(int setreset = 0);
	/* set one fog tile as visible. x, y are tile coordinates */
	void ExploreTile(Point &Tile);
	/* explore map from given point in map coordinates */
	void ExploreMapChunk(Point &Pos, int range, bool los);
	/* update VisibleBitmap by resolving vision of all explore actors */
	void UpdateFog();
	//PathFinder
	/* Finds the nearest passable point */
	void AdjustPosition(Point &goal, unsigned int radius=0);
	/* Jumps actors out of impassable areas, call this after a searchmap change */
	void FixAllPositions();
	/* Finds the path which leads the farthest from d */
	PathNode* RunAway(Point &s, Point &d, unsigned int PathLen, bool Backing);
	/* Returns true if there is no path to d */
	bool TargetUnreachable(Point &s, Point &d);
	/* Finds the path which leads to d */
	PathNode* FindPath(Point &s, Point &d);
	bool IsVisible(Point &s, Point &d);
	/* returns edge direction of map boundary, only worldmap regions */
	int WhichEdge(Point &s);

	//ambients
	void AddAmbient(Ambient *ambient) { ambients.push_back(ambient); }
	void SetupAmbients();

	//mapnotes
	void AddMapNote(Point point, int color, char *text);
	void RemoveMapNote(Point point);
	MapNote *GetMapNote(int i) { return mapnotes[i]; }
	MapNote *GetMapNote(Point point);
	unsigned int GetMapNoteCount() { return (unsigned int) mapnotes.size(); }

	/* May spawn creature(s), returns true in case of an interrupted rest */
	bool Rest(Point pos, int hours);
	/* Spawns creature(s) in radius of position */
	void SpawnCreature(Point pos, char *CreName, int radius = 0);
private:
	void Leveldown(unsigned int px, unsigned int py, unsigned int& level,
		Point &p, unsigned int& diff);
	void SetupNode(unsigned int x, unsigned int y, unsigned int Cost);
	//maybe this is unneeded and orientation could be calculated on the fly
	void UseExit(Actor *pc, InfoPoint *ip);
};

#endif
