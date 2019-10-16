#include "MiddleStart.h"
#include <Input.h>
#include <Path.h>
#include <ResourceManager.h>
#include <SoundResource.h>

// 참조추가 - 해당 솔루션
// 속성 - 디렉토리에서 포함디렉토리도 설정해줘야
// 그 솔루션의 헤더를 참조할 수 있다.


MiddleStart::MiddleStart()
{
}


MiddleStart::~MiddleStart()
{
}

void MiddleStart::Init()
{
	// 경로 생성 
	Path::RootToCreatePath(L"Sound");
	Path::RootToCreatePath(L"Texture");
	Path::RootToCreatePath(L"Character", L"Texture\\Character");
	Path::RootToCreatePath(L"MapTile", L"Texture\\Map\\Tile");
	Path::RootToCreatePath(L"SaveImage", L"Texture\\SaveImage");

	// 맵타일
	Path::RootToCreatePath(L"arcana1.img", L"Texture\\Map\\Tile\\arcana1.img");
	Path::RootToCreatePath(L"arcana2.img", L"Texture\\Map\\Tile\\arcana2.img");
	Path::RootToCreatePath(L"blueToyCastle.img", L"Texture\\Map\\Tile\\blueToyCastle.img");
	Path::RootToCreatePath(L"BM2-1.img", L"Texture\\Map\\Tile\\BM2-1.img");
	Path::RootToCreatePath(L"BM2-2.img", L"Texture\\Map\\Tile\\BM2-2.img");
	Path::RootToCreatePath(L"critias.img", L"Texture\\Map\\Tile\\critias.img");
	Path::RootToCreatePath(L"darkGrassySoil.img", L"Texture\\Map\\Tile\\darkGrassySoil.img");
	Path::RootToCreatePath(L"destructionField.img", L"Texture\\Map\\Tile\\destructionField.img");
	Path::RootToCreatePath(L"flagDay.img", L"Texture\\Map\\Tile\\flagDay.img");
	Path::RootToCreatePath(L"grassySoil.img", L"Texture\\Map\\Tile\\grassySoil.img");
	Path::RootToCreatePath(L"greenToyCastle.img", L"Texture\\Map\\Tile\\greenToyCastle.img");
	Path::RootToCreatePath(L"logMarble.img", L"Texture\\Map\\Tile\\logMarble.img");
	Path::RootToCreatePath(L"zerotempleFiled.img", L"Texture\\Map\\Tile\\zerotempleFiled.img");

	// 인게임 맵
	Path::RootToCreatePath(L"IG_AltairCamp", L"Texture\\Map\\InGame\\AltairCamp");
	Path::RootToCreatePath(L"IG_BrilliantLake", L"Texture\\Map\\InGame\\BrilliantLake");
	Path::RootToCreatePath(L"IG_CristalGarden", L"Texture\\Map\\InGame\\CristalGarden");
	Path::RootToCreatePath(L"IG_RudibriumPlasticSurgery", L"Texture\\Map\\InGame\\RudibriumPlasticSurgery");
	Path::RootToCreatePath(L"IG_RudibriumTown", L"Texture\\Map\\InGame\\RudibriumTown");
	Path::RootToCreatePath(L"IG_TwilightOfTheGodsn", L"Texture\\Map\\InGame\\TwilightOfTheGods");

	Path::RootToCreatePath(L"IG_Arcana", L"Texture\\Map\\InGame\\Arcana");
	Path::RootToCreatePath(L"IG_ArcanaWeather", L"Texture\\Map\\InGame\\Arcana\\Weather");

	Path::RootToCreatePath(L"IG_ChewChewIsland", L"Texture\\Map\\InGame\\ChewChewIsland");
	Path::RootToCreatePath(L"IG_Esfera", L"Texture\\Map\\InGame\\Esfera");

	// 스킬
	Path::RootToCreatePath(L"SKILL_BlazingExtinction", L"Texture\\Skill\\BlazingExtinction");
	Path::RootToCreatePath(L"SKILL_Blizzard", L"Texture\\Skill\\Blizzard");
	Path::RootToCreatePath(L"SKILL_PoisonMist", L"Texture\\Skill\\PoisonMist");
	Path::RootToCreatePath(L"SKILL_SoulPenetration", L"Texture\\Skill\\SoulPenetration");
	Path::RootToCreatePath(L"SKILL_Cataclysm", L"Texture\\Skill\\Cataclysm");
	Path::RootToCreatePath(L"SKILL_CataclysmScene", L"Texture\\Skill\\Cataclysm\\Scene");

	// 데미지 스킨
	Path::RootToCreatePath(L"DS_Chick", L"Texture\\DamageSkin\\Chick");
	Path::RootToCreatePath(L"DS_Hangeul", L"Texture\\DamageSkin\\Hangeul");
	Path::RootToCreatePath(L"DS_Monster", L"Texture\\DamageSkin\\Monster");
	Path::RootToCreatePath(L"DS_Star", L"Texture\\DamageSkin\\Star");

	// UI
	Path::RootToCreatePath(L"UI_Base", L"Texture\\UI\\Base");
	Path::RootToCreatePath(L"UI_Button", L"Texture\\UI\\Button");
	Path::RootToCreatePath(L"UI_Gauge", L"Texture\\UI\\Gauge");
	Path::RootToCreatePath(L"UI_MonsterGauge", L"Texture\\UI\\Gauge\\HPBar");
	Path::RootToCreatePath(L"UI_Key", L"Texture\\UI\\Key");
	Path::RootToCreatePath(L"UI_Portal", L"Texture\\UI\\Portal");
	Path::RootToCreatePath(L"UI_Inventory", L"Texture\\UI\\Inventory");
	Path::RootToCreatePath(L"UI_Cursor", L"Texture\\UI\\Cursor");
	Path::RootToCreatePath(L"UI_Number", L"Texture\\UI\\Number");
	Path::RootToCreatePath(L"UI_QuickSlot", L"Texture\\UI\\QuickSlot");
	Path::RootToCreatePath(L"UI_NameTag", L"Texture\\UI\\NameTag");
	Path::RootToCreatePath(L"UI_DamageSkin", L"Texture\\UI\\DamageSkin");
	
	// 상점
	Path::RootToCreatePath(L"Shop", L"Texture\\Shop");

	// 퀘스트
	Path::RootToCreatePath(L"UI_Quest", L"Texture\\Quest");

	// 퀘스트 버튼
	Path::RootToCreatePath(L"UI_QuestBT", L"Texture\\Quest\\Button");

	// 퀘스트 알리미
	Path::RootToCreatePath(L"UI_QuestAlarm", L"Texture\\QuestAlarm");

	// 아이템
	Path::RootToCreatePath(L"Item_Potion", L"Texture\\Item\\Potion_General");
	Path::RootToCreatePath(L"Item_Food", L"Texture\\Item\\Potion_Food");

	// 몬스터
	Path::RootToCreatePath(L"Monster", L"Texture\\Monster");

	Path::RootToCreatePath(L"GM_ChildMannequin", L"Texture\\Monster\\General\\ChildMannequin");
	Path::RootToCreatePath(L"GM_ContaminatedSap", L"Texture\\Monster\\General\\ContaminatedSap");
	Path::RootToCreatePath(L"GM_PredatorOfFear", L"Texture\\Monster\\General\\PredatorOfFear");
	Path::RootToCreatePath(L"GM_Tianlu", L"Texture\\Monster\\General\\Tianlu");
	Path::RootToCreatePath(L"GM_TransformSlime", L"Texture\\Monster\\General\\TransformSlime");
	Path::RootToCreatePath(L"GM_Velaris", L"Texture\\Monster\\General\\Velaris");
	Path::RootToCreatePath(L"GM_WhiteSandRabbit", L"Texture\\Monster\\General\\WhiteSandRabbit");

	Path::RootToCreatePath(L"MBM_Swoo", L"Texture\\Monster\\MidBoss\\Swoo");

	Path::RootToCreatePath(L"Boss_Pinkbin", L"Texture\\Monster\\Boss\\Pinkbin");


	// NPC
	Path::RootToCreatePath(L"NPC", L"Texture\\NPC");
	Path::RootToCreatePath(L"NPC_RockSpirit", L"Texture\\NPC\\RockSpirit");
	Path::RootToCreatePath(L"NPC_SmallSpirit", L"Texture\\NPC\\SmallSpirit");
	Path::RootToCreatePath(L"NPC_TreeSpirit", L"Texture\\NPC\\TreeSpirit");
	Path::RootToCreatePath(L"NPC_WindSpirit", L"Texture\\NPC\\WindSpirit");
	Path::RootToCreatePath(L"NPC_NONE", L"Texture\\NPC\\NONE_NPC");

	// 아이템
	Path::RootToCreatePath(L"Item_Money", L"Texture\\Item\\Money");
	
	// 타이틀
	Path::RootToCreatePath(L"Title", L"Texture\\Title");

	// 사운드
	Path::RootToCreatePath(L"Sound_BGM", L"Texture\\Sound\\BGM");
	Path::RootToCreatePath(L"Sound_Effect", L"Texture\\Sound\\Effect");
	Path::RootToCreatePath(L"Sound_UI", L"Texture\\Sound\\UI");
	Path::RootToCreatePath(L"Sound_Pinkbin", L"Texture\\Sound\\Pinkbin");

	Path::RootToCreatePath(L"Sound_Voice", L"Texture\\Sound\\Voice");
	Path::RootToCreatePath(L"Sound_Character", L"Texture\\Sound\\Character_Sound");
	Path::RootToCreatePath(L"Sound_BGMUI", L"Texture\\Sound\\BGMUI");
	Path::RootToCreatePath(L"Sound_Pet", L"Texture\\Sound\\Pet");

	// 사운드 로드
	/*std::vector<std::wstring> SoundNameVector = Path::PathToFullFileList(Path::FindPath(L"Sound_BGM"), L"*.mp3");
	for (size_t i = 0; i < SoundNameVector.size(); ++i)
	{
		ResourceManager<SoundResource>::Load(Path::FindPath(L"Sound_BGM") + SoundNameVector[i]);
	}

	SoundNameVector = Path::PathToFullFileList(Path::FindPath(L"Sound_Effect"), L"*.mp3");
	for (size_t i = 0; i < SoundNameVector.size(); ++i)
	{
		ResourceManager<SoundResource>::Load(Path::FindPath(L"Sound_Effect") + SoundNameVector[i]);
	}

	SoundNameVector = Path::PathToFullFileList(Path::FindPath(L"Sound_Pet"), L"*.mp3");
	for (size_t i = 0; i < SoundNameVector.size(); ++i)
	{
		ResourceManager<SoundResource>::Load(Path::FindPath(L"Sound_Pet") + SoundNameVector[i]);
	}

	SoundNameVector = Path::PathToFullFileList(Path::FindPath(L"Sound_UI"), L"*.mp3");
	for (size_t i = 0; i < SoundNameVector.size(); ++i)
	{
		ResourceManager<SoundResource>::Load(Path::FindPath(L"Sound_UI") + SoundNameVector[i]);
	}

	SoundNameVector = Path::PathToFullFileList(Path::FindPath(L"Sound_Pinkbin"), L"*.mp3");
	for (size_t i = 0; i < SoundNameVector.size(); ++i)
	{
		ResourceManager<SoundResource>::Load(Path::FindPath(L"Sound_Pinkbin") + SoundNameVector[i]);
	}

	SoundNameVector = Path::PathToFullFileList(Path::FindPath(L"Sound_Voice"), L"*.mp3");
	for (size_t i = 0; i < SoundNameVector.size(); ++i)
	{
		ResourceManager<SoundResource>::Load(Path::FindPath(L"Sound_Voice") + SoundNameVector[i]);
	}

	SoundNameVector = Path::PathToFullFileList(Path::FindPath(L"Sound_Character"), L"*.mp3");
	for (size_t i = 0; i < SoundNameVector.size(); ++i)
	{
		ResourceManager<SoundResource>::Load(Path::FindPath(L"Sound_Character") + SoundNameVector[i]);
	}

	SoundNameVector = Path::PathToFullFileList(Path::FindPath(L"Sound_BGMUI"), L"*.mp3");
	for (size_t i = 0; i < SoundNameVector.size(); ++i)
	{
		ResourceManager<SoundResource>::Load(Path::FindPath(L"Sound_BGMUI") + SoundNameVector[i]);
	}*/


	// 입력키 생성
	Input::Create_Key(L"Left", VK_LEFT);
	Input::Create_Key(L"Right", VK_RIGHT);
	Input::Create_Key(L"Up", VK_UP);
	Input::Create_Key(L"Down", VK_DOWN);
	//Input::Create_Key(L"Jump", VK_MENU);				// Alt키
	Input::Create_Key(L"Jump", 'C');					// 점프로..

	// 타일 나머지 생성할때 사용할 키
	Input::Create_Key(L"TileFinish", VK_F9);

	//Input::Create_Key(L"BaseAttack", VK_LCONTROL);		// Ctrl키
	Input::Create_Key(L"BaseAttack", 'X');		// Ctrl키
	Input::Create_Key(L"ItemPickUp", 'Z');

	Input::Create_Key(L"S", 'T');

	Input::Create_Key(L"Q", 'Q');
	Input::Create_Key(L"T", 'T');

	Input::Create_Key(L"Rotation", 'V');
	Input::Create_Key(L"Revolution", 'B');

	// 인벤토리
	Input::Create_Key(L"Inventory", 'I');

	// 캐릭터 스킬
	//Input::Create_Key(L"BlazingExtinction", VK_INSERT);
	//Input::Create_Key(L"Blizzard", VK_HOME);
	//Input::Create_Key(L"PoisonMist", VK_PRIOR);
	//Input::Create_Key(L"SoulPenetration", VK_NEXT);
	//Input::Create_Key(L"Cataclysm", VK_END);

	Input::Create_Key(L"Key_Insert", VK_INSERT);
	Input::Create_Key(L"Key_Home", VK_HOME);
	Input::Create_Key(L"Key_PageUp", VK_PRIOR);
	Input::Create_Key(L"Key_PageDown", VK_NEXT);
	Input::Create_Key(L"Key_End", VK_END);
	Input::Create_Key(L"Key_Delete", VK_DELETE);
	Input::Create_Key(L"Key_Space", VK_SPACE);

	Input::Create_Key(L"Key_1", '1');			// 1
	Input::Create_Key(L"Key_2", '2');			// 2
	Input::Create_Key(L"Key_3", '3');			// 3
	Input::Create_Key(L"Key_4", '4');			// 4
}
