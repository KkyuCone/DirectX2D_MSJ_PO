#include "TextureLoadingScene.h"

#include <GameObject.h>
#include <DXHeader.h>
#include <Camera.h>
#include <Animation2D.h>
#include <SpriteRenderer2D.h>

// 내꺼
#include <PlayerMove.h>
#include <PlayerAnimation.h>
#include <SkillManager.h>
#include <UIMove.h>

//
#include <Texture.h>
#include <Sprite.h>
#include <RectCol2D.h>
#include <GameDebug.h>
#include <InGameCamera.h>

#include <SoundResource.h>



TextureLoadingScene::TextureLoadingScene()
{
}


TextureLoadingScene::~TextureLoadingScene()
{
}

//
bool TextureLoadingScene::Loading()
{
	TextureLoading();

	return true;
}

void TextureLoadingScene::LateUpdate()
{
	//ArcanaTownScene
	//Scene::Scene_Change(L"TestScene");
	// PinkbinScene
	// ArcanaTown6Scene
	// ChewChewIslandScene
	// EsferaScene
	Scene::Scene_Change(L"TitleScene");
}


//

void TextureLoadingScene::TextureLoading()
{
	// 이미지 파일(PNG)들 가져오기
	CharacterImages = Path::PathToFullFileList(Path::FindPath(L"Character"), L"*.png");

	for (size_t i = 0; i < CharacterImages.size(); i++)
	{
		ResourceManager<Texture>::Load(Path::FindPath(L"Character") + CharacterImages[i]);
	}
	// 이미지 잘라내기
	ResourceManager<Sprite>::Create(L"AllCharacter.png", L"AllCharacter.png", 10, 5);			// 올캐릭터
	ResourceManager<Sprite>::Create(L"Arm.png", L"Arm.png", 10, 5);
	ResourceManager<Sprite>::Create(L"Body.png", L"Body.png", 10, 5);
	ResourceManager<Sprite>::Create(L"Coat.png", L"Coat.png", 10, 5);
	ResourceManager<Sprite>::Create(L"CoatArm.png", L"CoatArm.png", 10, 5);
	ResourceManager<Sprite>::Create(L"Eye.png", L"Eye.png", 10, 5);
	ResourceManager<Sprite>::Create(L"Hair.png", L"Hair.png", 10, 5);
	ResourceManager<Sprite>::Create(L"Head.png", L"Head.png", 10, 5);
	ResourceManager<Sprite>::Create(L"Weapon_Rabbit.png", L"Weapon_Rabbit.png", 10, 5);


	// 배경 이미지 및 충돌타일
	BKImages = Path::PathToFullFileList(Path::FindPath(L"IG_AltairCamp"), L"*.png");

	for (size_t i = 0; i < BKImages.size(); ++i)
	{
		ResourceManager<Texture>::Load(Path::FindPath(L"IG_AltairCamp") + BKImages[i]);
	}

	ResourceManager<Sprite>::Create(L"AC_Background.png", L"AC_Background.png", 1, 1);
	ResourceManager<Sprite>::Create(L"AC_MoveTile.png", L"AC_MoveTile.png", 1, 1);

	// 마을 맵
	BKImages = Path::PathToFullFileList(Path::FindPath(L"IG_Arcana"), L"*.png");

	for (size_t i = 0; i < BKImages.size(); ++i)
	{
		ResourceManager<Texture>::Load(Path::FindPath(L"IG_Arcana") + BKImages[i]);
	}

	ResourceManager<Sprite>::Create(L"ArcanaBGAni_1.png", L"ArcanaBGAni_1.png", 3, 3);
	ResourceManager<Sprite>::Create(L"ArcanaBGAni_2.png", L"ArcanaBGAni_2.png", 3, 3);
	ResourceManager<Sprite>::Create(L"ArcanaMinimap.png", L"ArcanaMinimap.png", 1, 1);
	ResourceManager<Sprite>::Create(L"ArcanaTile.png", L"ArcanaTile.png", 1, 1);
	ResourceManager<Sprite>::Create(L"Weather_Ani_1.png", L"Weather_Ani_1.png", 5, 3);
	ResourceManager<Sprite>::Create(L"Weather_Ani_2.png", L"Weather_Ani_2.png", 5, 3);


	// IG_ArcanaWeather
	BKImages = Path::PathToFullFileList(Path::FindPath(L"IG_ArcanaWeather"), L"*.png");

	for (size_t i = 0; i < BKImages.size(); ++i)
	{
		ResourceManager<Texture>::Load(Path::FindPath(L"IG_ArcanaWeather") + BKImages[i]);
	}

	ResourceManager<Sprite>::Create(L"Weather_Ani_1.png", L"Weather_Ani_1.png", 3, 2);
	ResourceManager<Sprite>::Create(L"Weather_Ani_2.png", L"Weather_Ani_2.png", 3, 2);
	ResourceManager<Sprite>::Create(L"Weather_Ani_3.png", L"Weather_Ani_3.png", 3, 2);
	ResourceManager<Sprite>::Create(L"Weather_Ani_4.png", L"Weather_Ani_4.png", 3, 2);
	ResourceManager<Sprite>::Create(L"Weather_Ani_5.png", L"Weather_Ani_5.png", 3, 2);
	ResourceManager<Sprite>::Create(L"Weather_Ani_6.png", L"Weather_Ani_6.png", 2, 1);

	// 핑크빈 신전 IG_TwilightOfTheGodsn
	BKImages = Path::PathToFullFileList(Path::FindPath(L"IG_TwilightOfTheGodsn"), L"*.png");

	for (size_t i = 0; i < BKImages.size(); ++i)
	{
		ResourceManager<Texture>::Load(Path::FindPath(L"IG_TwilightOfTheGodsn") + BKImages[i]);
		ResourceManager<Sprite>::Create(BKImages[i], BKImages[i].c_str(), 1, 1);
	}

	// 츄츄 아일랜드
	BKImages = Path::PathToFullFileList(Path::FindPath(L"IG_ChewChewIsland"), L"*.png");

	for (size_t i = 0; i < BKImages.size(); ++i)
	{
		ResourceManager<Texture>::Load(Path::FindPath(L"IG_ChewChewIsland") + BKImages[i]);
		ResourceManager<Sprite>::Create(BKImages[i], BKImages[i].c_str(), 1, 1);
	}

	// 에스페라
	BKImages = Path::PathToFullFileList(Path::FindPath(L"IG_Esfera"), L"*.png");

	for (size_t i = 0; i < BKImages.size(); ++i)
	{
		ResourceManager<Texture>::Load(Path::FindPath(L"IG_Esfera") + BKImages[i]);
		ResourceManager<Sprite>::Create(BKImages[i], BKImages[i].c_str(), 1, 1);
	}

	// UI
	std::wstring UIFileName = L"";
	UIImages = Path::PathToFullFileList(Path::FindPath(L"UI_Base"), L"*.png");

	for (size_t i = 0; i < UIImages.size(); ++i)
	{
		ResourceManager<Texture>::Load(Path::FindPath(L"UI_Base") + UIImages[i]);
		UIFileName = UIImages[i];
		ResourceManager<Sprite>::Create(UIFileName, UIFileName.c_str(), 1, 1);
	}

	UIImages = Path::PathToFullFileList(Path::FindPath(L"UI_Button"), L"*.png");
	UIFileName = L"";
	for (size_t i = 0; i < UIImages.size(); ++i)
	{
		ResourceManager<Texture>::Load(Path::FindPath(L"UI_Button") + UIImages[i]);
		UIFileName = UIImages[i];
		ResourceManager<Sprite>::Create(UIFileName, UIFileName.c_str(), 4, 1);
	}

	UIImages = Path::PathToFullFileList(Path::FindPath(L"UI_Gauge"), L"*.png");
	UIFileName = L"";
	for (size_t i = 0; i < UIImages.size(); ++i)
	{
		ResourceManager<Texture>::Load(Path::FindPath(L"UI_Gauge") + UIImages[i]);
		UIFileName = UIImages[i];
		ResourceManager<Sprite>::Create(UIFileName, UIFileName.c_str(), 1, 1);
	}

	UIImages = Path::PathToFullFileList(Path::FindPath(L"UI_Key"), L"*.png");
	UIFileName = L"";
	for (size_t i = 0; i < UIImages.size(); ++i)
	{
		ResourceManager<Texture>::Load(Path::FindPath(L"UI_Key") + UIImages[i]);
		UIFileName = UIImages[i];
		ResourceManager<Sprite>::Create(UIFileName, UIFileName.c_str(), 1, 1);
	}
	
	// UI - 이름태그 UI_NameTag
	UIImages = Path::PathToFullFileList(Path::FindPath(L"UI_NameTag"), L"*.png");
	UIFileName = L"";
	for (size_t i = 0; i < UIImages.size(); ++i)
	{
		ResourceManager<Texture>::Load(Path::FindPath(L"UI_NameTag") + UIImages[i]);
		UIFileName = UIImages[i];
		ResourceManager<Sprite>::Create(UIFileName, UIFileName.c_str(), 1, 1);
	}

	// UI - 퀵슬롯
	UIImages = Path::PathToFullFileList(Path::FindPath(L"UI_QuickSlot"), L"*.png");
	for (size_t i = 0; i < UIImages.size(); ++i)
	{
		ResourceManager<Texture>::Load(Path::FindPath(L"UI_QuickSlot") + UIImages[i]);
	}
	ResourceManager<Sprite>::Create(L"quickSlot.backgrnd.png", L"quickSlot.backgrnd.png", 1, 1);
	ResourceManager<Sprite>::Create(L"quickSlot.layer_cover.png", L"quickSlot.layer_cover.png", 1, 1);
	ResourceManager<Sprite>::Create(L"QuickSlot_CoolTime.png", L"QuickSlot_CoolTime.png", 10, 2);
	ResourceManager<Sprite>::Create(L"StatusBarBaseQuickSlot.png", L"StatusBarBaseQuickSlot.png", 1, 1);

	// t숫자
	UIImages = Path::PathToFullFileList(Path::FindPath(L"UI_Number"), L"*.png");
	for (size_t i = 0; i < UIImages.size(); ++i)
	{
		ResourceManager<Texture>::Load(Path::FindPath(L"UI_Number") + UIImages[i]);
	}
	ResourceManager<Sprite>::Create(L"UI_ItemNumber.png", L"UI_ItemNumber.png", 10, 1);
	ResourceManager<Sprite>::Create(L"UI_MoneyNumber.png", L"UI_MoneyNumber.png", 10, 1);
	ResourceManager<Sprite>::Create(L"UI_StateNumber.png", L"UI_StateNumber.png", 10, 1);
	ResourceManager<Sprite>::Create(L"UI_MoneyNumberWhite.png", L"UI_MoneyNumberWhite.png", 10, 1);
	ResourceManager<Sprite>::Create(L"UI_MoneyNumberUnit.png", L"UI_MoneyNumberUnit.png", 1, 1);

	// 포탈
	UIImages = Path::PathToFullFileList(Path::FindPath(L"UI_Portal"), L"*.png");
	for (size_t i = 0; i < UIImages.size(); ++i)
	{
		ResourceManager<Texture>::Load(Path::FindPath(L"UI_Portal") + UIImages[i]);
	}
	ResourceManager<Sprite>::Create(L"Portal.png", L"Portal.png", 8, 1);

	// 데미지 스킨 - UI_DamageSkin
	UIImages = Path::PathToFullFileList(Path::FindPath(L"UI_DamageSkin"), L"*.png");
	for (size_t i = 0; i < UIImages.size(); ++i)
	{
		ResourceManager<Texture>::Load(Path::FindPath(L"UI_DamageSkin") + UIImages[i]);
	}
	ResourceManager<Sprite>::Create(L"DamageSkin_Chick.png", L"DamageSkin_Chick.png", 10, 1);


	// 상점 - Shop
	UIImages = Path::PathToFullFileList(Path::FindPath(L"Shop"), L"*.png");
	for (size_t i = 0; i < UIImages.size(); ++i)
	{
		ResourceManager<Texture>::Load(Path::FindPath(L"Shop") + UIImages[i]);
	}
	ResourceManager<Sprite>::Create(L"ShopBt_All.png", L"ShopBt_All.png", 2, 1);
	ResourceManager<Sprite>::Create(L"ShopBt_Consume.png", L"ShopBt_Consume.png", 2, 1);
	ResourceManager<Sprite>::Create(L"ShpoBt_Equip.png", L"ShpoBt_Equip.png", 2, 1);
	ResourceManager<Sprite>::Create(L"ShopBt_Etc.png", L"ShopBt_Etc.png", 2, 1);
	ResourceManager<Sprite>::Create(L"ShopBt_Cash.png", L"ShopBt_Cash.png", 2, 1);
	ResourceManager<Sprite>::Create(L"ShopBt_Install.png", L"ShopBt_Install.png", 2, 1);
	ResourceManager<Sprite>::Create(L"ShopBt_ReBuy.png", L"ShopBt_ReBuy.png", 2, 1);
	ResourceManager<Sprite>::Create(L"Bt_ShopBuy.png", L"Bt_ShopBuy.png", 4, 1);
	ResourceManager<Sprite>::Create(L"Bt_ShopExit.png", L"Bt_ShopExit.png", 4, 1);
	ResourceManager<Sprite>::Create(L"Bt_ShopSell.png", L"Bt_ShopSell.png", 4, 1);
	ResourceManager<Sprite>::Create(L"PotionShop_Background.png", L"PotionShop_Background.png", 1, 1);
	ResourceManager<Sprite>::Create(L"Shop_BluePotion.png", L"Shop_BluePotion.png", 2, 1);
	ResourceManager<Sprite>::Create(L"Shop_ManaElixir.png", L"Shop_ManaElixir.png", 2, 1);
	ResourceManager<Sprite>::Create(L"Shop_OrangePotion.png", L"Shop_OrangePotion.png", 2, 1);
	ResourceManager<Sprite>::Create(L"Shop_RedPoiton.png", L"Shop_RedPoiton.png", 2, 1);
	ResourceManager<Sprite>::Create(L"Shop_WhitePotion.png", L"Shop_WhitePotion.png", 2, 1);

	// HPBar - 몬스터 UI_MonsterGauge
	UIImages = Path::PathToFullFileList(Path::FindPath(L"UI_MonsterGauge"), L"*.png");
	for (size_t i = 0; i < UIImages.size(); ++i)
	{
		ResourceManager<Texture>::Load(Path::FindPath(L"UI_MonsterGauge") + UIImages[i]);
		ResourceManager<Sprite>::Create(UIImages[i], UIImages[i].c_str(), 1, 1);
	}

	// 아이템 -  포션
	UIImages = Path::PathToFullFileList(Path::FindPath(L"Item_Potion"), L"*.png");
	for (size_t i = 0; i < UIImages.size(); ++i)
	{
		ResourceManager<Texture>::Load(Path::FindPath(L"Item_Potion") + UIImages[i]);
		ResourceManager<Sprite>::Create(UIImages[i], UIImages[i].c_str(), 1, 1);
	}

	// 아이템 - 음식
	UIImages = Path::PathToFullFileList(Path::FindPath(L"Item_Food"), L"*.png");
	for (size_t i = 0; i < UIImages.size(); ++i)
	{
		ResourceManager<Texture>::Load(Path::FindPath(L"Item_Food") + UIImages[i]);
		ResourceManager<Sprite>::Create(UIImages[i], UIImages[i].c_str(), 1, 1);
	}

	// 아이템 - 돈 Item_Money
	UIImages = Path::PathToFullFileList(Path::FindPath(L"Item_Money"), L"*.png");
	for (size_t i = 0; i < UIImages.size(); ++i)
	{
		ResourceManager<Texture>::Load(Path::FindPath(L"Item_Money") + UIImages[i]);
		ResourceManager<Sprite>::Create(UIImages[i], UIImages[i].c_str(), 4, 1);
	}

	// 타이틀 - Title
	UIImages = Path::PathToFullFileList(Path::FindPath(L"Title"), L"*.png");
	for (size_t i = 0; i < UIImages.size(); ++i)
	{
		ResourceManager<Texture>::Load(Path::FindPath(L"Title") + UIImages[i]);
	}
	ResourceManager<Sprite>::Create(L"FadeInOutBG.png", L"FadeInOutBG.png", 1, 1);
	ResourceManager<Sprite>::Create(L"LoadingImage.png", L"LoadingImage.png", 10, 2);
	ResourceManager<Sprite>::Create(L"TitleBG.png", L"TitleBG.png", 1, 1);

	// 인벤토리  - UI_Inventory
	UIImages = Path::PathToFullFileList(Path::FindPath(L"UI_Inventory"), L"*.png");
	for (size_t i = 0; i < UIImages.size(); ++i)
	{
		ResourceManager<Texture>::Load(Path::FindPath(L"UI_Inventory") + UIImages[i]);
	}
	ResourceManager<Sprite>::Create(L"Inven_AppraiseBt.png", L"Inven_AppraiseBt.png", 4, 1);
	ResourceManager<Sprite>::Create(L"Inven_Base.png", L"Inven_Base.png", 1, 1);
	ResourceManager<Sprite>::Create(L"Inven_CloseBt.png", L"Inven_CloseBt.png", 4, 1);
	ResourceManager<Sprite>::Create(L"Inven_DisassembleBt.png", L"Inven_DisassembleBt.png", 4, 1);
	ResourceManager<Sprite>::Create(L"Inven_ExtractBt.png", L"Inven_ExtractBt.png", 4, 1);
	ResourceManager<Sprite>::Create(L"Inven_Kind.png", L"Inven_Kind.png", 5, 2);
	ResourceManager<Sprite>::Create(L"Inven_PotBt.png", L"Inven_PotBt.png", 1, 1);
	ResourceManager<Sprite>::Create(L"Inven_ToadBt.png", L"Inven_ToadBt.png", 4, 1);
	ResourceManager<Sprite>::Create(L"Inven_UpgradeBt.png", L"Inven_UpgradeBt.png", 4, 1);

	ResourceManager<Sprite>::Create(L"Inven_Kind_Cash.png", L"Inven_Kind_Cash.png", 2, 1);
	ResourceManager<Sprite>::Create(L"Inven_Kind_Comsum.png", L"Inven_Kind_Comsum.png", 2, 1);
	ResourceManager<Sprite>::Create(L"Inven_Kind_Equip.png", L"Inven_Kind_Equip.png", 2, 1);
	ResourceManager<Sprite>::Create(L"Inven_Kind_Etc.png", L"Inven_Kind_Etc.png", 2, 1);
	ResourceManager<Sprite>::Create(L"Inven_Kind_Install.png", L"Inven_Kind_Install.png", 2, 1);

	ResourceManager<Sprite>::Create(L"Inven_ExampleTex01.png", L"Inven_ExampleTex01.png", 1, 1);
	ResourceManager<Sprite>::Create(L"Inven_ExampleTex02.png", L"Inven_ExampleTex02.png", 1, 1);
	ResourceManager<Sprite>::Create(L"RedPoiton.png", L"RedPoiton.png", 1, 1);


	// 마우스 커서 - UI_Cursor
	UIImages = Path::PathToFullFileList(Path::FindPath(L"UI_Cursor"), L"*.png");
	for (size_t i = 0; i < UIImages.size(); ++i)
	{
		ResourceManager<Texture>::Load(Path::FindPath(L"UI_Cursor") + UIImages[i]);
	}
	ResourceManager<Sprite>::Create(L"MouseCursor.png", L"MouseCursor.png", 7, 1);

	// NPC
	NPCImage = Path::PathToFullFileList(Path::FindPath(L"NPC_RockSpirit"), L"*.png");
	for (size_t i = 0; i < NPCImage.size(); ++i)
	{
		ResourceManager<Texture>::Load(Path::FindPath(L"NPC_RockSpirit") + NPCImage[i]);
	}
	ResourceManager<Sprite>::Create(L"RockSpiritBaseMotion.png", L"RockSpiritBaseMotion.png", 10, 3);
	ResourceManager<Sprite>::Create(L"RockSpiritFace.png", L"RockSpiritFace.png", 2, 1);

	NPCImage = Path::PathToFullFileList(Path::FindPath(L"NPC_SmallSpirit"), L"*.png");
	for (size_t i = 0; i < NPCImage.size(); ++i)
	{
		ResourceManager<Texture>::Load(Path::FindPath(L"NPC_SmallSpirit") + NPCImage[i]);
	}
	ResourceManager<Sprite>::Create(L"SmallSpiritBaseMotion.png", L"SmallSpiritBaseMotion.png", 10, 5);
	ResourceManager<Sprite>::Create(L"SmallSpiritFace.png", L"SmallSpiritFace.png", 8, 1);

	NPCImage = Path::PathToFullFileList(Path::FindPath(L"NPC_TreeSpirit"), L"*.png");
	for (size_t i = 0; i < NPCImage.size(); ++i)
	{
		ResourceManager<Texture>::Load(Path::FindPath(L"NPC_TreeSpirit") + NPCImage[i]);
	}
	ResourceManager<Sprite>::Create(L"TreeSpiritFace.png", L"TreeSpiritFace.png", 1, 1);
	ResourceManager<Sprite>::Create(L"TreeSpiritMotion.png", L"TreeSpiritMotion.png", 8, 1);

	NPCImage = Path::PathToFullFileList(Path::FindPath(L"NPC_WindSpirit"), L"*.png");
	for (size_t i = 0; i < NPCImage.size(); ++i)
	{
		ResourceManager<Texture>::Load(Path::FindPath(L"NPC_WindSpirit") + NPCImage[i]);
	}
	ResourceManager<Sprite>::Create(L"WindSpiritBaseMotion.png", L"WindSpiritBaseMotion.png", 10, 5);
	ResourceManager<Sprite>::Create(L"WindSpiritFace.png", L"WindSpiritFace.png", 3, 1);

	NPCImage = Path::PathToFullFileList(Path::FindPath(L"NPC"), L"*.png");
	for (size_t i = 0; i < NPCImage.size(); ++i)
	{
		ResourceManager<Texture>::Load(Path::FindPath(L"NPC") + NPCImage[i]);
	}
	ResourceManager<Sprite>::Create(L"PotionMerchant.png", L"PotionMerchant.png", 6, 1);
	ResourceManager<Sprite>::Create(L"WarehouseKeeper.png", L"WarehouseKeeper.png", 10, 2);

	NPCImage = Path::PathToFullFileList(Path::FindPath(L"NPC_NONE"), L"*.png");
	for (size_t i = 0; i < NPCImage.size(); ++i)
	{
		ResourceManager<Texture>::Load(Path::FindPath(L"NPC_NONE") + NPCImage[i]);
	}
	ResourceManager<Sprite>::Create(L"Rock_01.png", L"Rock_01.png", 10, 3);
	ResourceManager<Sprite>::Create(L"Rock_02.png", L"Rock_02.png", 10, 3);
	ResourceManager<Sprite>::Create(L"Tree_01.png", L"Tree_01.png", 10, 2);
	ResourceManager<Sprite>::Create(L"Tree_02.png", L"Tree_02.png", 10, 2);
	ResourceManager<Sprite>::Create(L"Wind_01.png", L"Wind_01.png", 10, 3);
	ResourceManager<Sprite>::Create(L"Wind_02.png", L"Wind_02.png", 10, 2);

	// 퀘스트 관련 - UI_Quest
	NPCImage = Path::PathToFullFileList(Path::FindPath(L"UI_Quest"), L"*.png");
	for (size_t i = 0; i < NPCImage.size(); ++i)
	{
		ResourceManager<Texture>::Load(Path::FindPath(L"UI_Quest") + NPCImage[i]);
	}
	ResourceManager<Sprite>::Create(L"Quest_Alarm.png", L"Quest_Alarm.png", 8, 1);
	ResourceManager<Sprite>::Create(L"Quest_Complication.png", L"Quest_Complication.png", 8, 1);
	ResourceManager<Sprite>::Create(L"Quest_Ing.png", L"Quest_Ing.png", 4, 1);

	ResourceManager<Sprite>::Create(L"Dialog_NPCchat.png", L"Dialog_NPCchat.png", 4, 2);
	ResourceManager<Sprite>::Create(L"Dialog_RockQuest_01.png", L"Dialog_RockQuest_01.png", 1, 1);
	ResourceManager<Sprite>::Create(L"Dialog_RockQuest_02.png", L"Dialog_RockQuest_02.png", 1, 1);
	ResourceManager<Sprite>::Create(L"Dialog_RockQuest_03.png", L"Dialog_RockQuest_03.png", 1, 1);
	ResourceManager<Sprite>::Create(L"Dialog_RockQuest_04.png", L"Dialog_RockQuest_04.png", 1, 1);
	ResourceManager<Sprite>::Create(L"Dialog_Tree01.png", L"Dialog_Tree01.png", 1, 1);
	ResourceManager<Sprite>::Create(L"Dialog_Wind01.png", L"Dialog_Wind01.png", 1, 1);

	ResourceManager<Sprite>::Create(L"DialogBaseBackgorund.png", L"DialogBaseBackgorund.png", 1, 1);
	ResourceManager<Sprite>::Create(L"DialogNameTagBackground.png", L"DialogNameTagBackground.png", 1, 1);
	ResourceManager<Sprite>::Create(L"DialogNPC_NameTags.png", L"DialogNPC_NameTags.png", 4, 1);
	ResourceManager<Sprite>::Create(L"NPCNameTag_01.png", L"NPCNameTag_01.png", 1, 1);
	ResourceManager<Sprite>::Create(L"NPCNameTag_02.png", L"NPCNameTag_02.png", 1, 1);
	ResourceManager<Sprite>::Create(L"NPCNameTag_03.png", L"NPCNameTag_03.png", 1, 1);
	ResourceManager<Sprite>::Create(L"NPCNameTag_04.png", L"NPCNameTag_04.png", 1, 1);

	// 퀘스트 버튼 - UI_QuestBT
	NPCImage = Path::PathToFullFileList(Path::FindPath(L"UI_QuestBT"), L"*.png");
	for (size_t i = 0; i < NPCImage.size(); ++i)
	{
		ResourceManager<Texture>::Load(Path::FindPath(L"UI_QuestBT") + NPCImage[i]);
		ResourceManager<Sprite>::Create(NPCImage[i].c_str(), NPCImage[i].c_str(), 4, 1);
	}


	// 퀘스트 관련 - UI_QuestAlarm
	NPCImage = Path::PathToFullFileList(Path::FindPath(L"UI_QuestAlarm"), L"*.png");
	for (size_t i = 0; i < NPCImage.size(); ++i)
	{
		ResourceManager<Texture>::Load(Path::FindPath(L"UI_QuestAlarm") + NPCImage[i]);
	}
	ResourceManager<Sprite>::Create(L"QuestAlarmBackground.png", L"QuestAlarmBackground.png", 1, 1);
	ResourceManager<Sprite>::Create(L"QuestAlarmBackground_02.png", L"QuestAlarmBackground_02.png", 1, 1);
	ResourceManager<Sprite>::Create(L"QuestAlarmBackground_03.png", L"QuestAlarmBackground_03.png", 1, 1);

	// 스킬
	// 블레이징 익스팅션
	SkillImage = Path::PathToFullFileList(Path::FindPath(L"SKILL_BlazingExtinction"), L"*.png");

	for (size_t i = 0; i < SkillImage.size(); ++i)
	{
		ResourceManager<Texture>::Load(Path::FindPath(L"SKILL_BlazingExtinction") + SkillImage[i]);
	}

	ResourceManager<Sprite>::Create(L"BlazingExtinction_Effect.png", L"BlazingExtinction_Effect.png", 10, 2);
	ResourceManager<Sprite>::Create(L"BlazingExtinction_Hit.png", L"BlazingExtinction_Hit.png", 7, 1);
	ResourceManager<Sprite>::Create(L"BlazingExtinction_Ball.png", L"BlazingExtinction_Ball.png", 6, 1);
	ResourceManager<Sprite>::Create(L"BlazingExtinction_Icon.png", L"BlazingExtinction_Icon.png", 1, 1);


	// 블리자드
	SkillImage = Path::PathToFullFileList(Path::FindPath(L"SKILL_Blizzard"), L"*.png");
	for (size_t i = 0; i < SkillImage.size(); ++i)
	{
		ResourceManager<Texture>::Load(Path::FindPath(L"SKILL_Blizzard") + SkillImage[i]);
	}

	ResourceManager<Sprite>::Create(L"Blizzard_Effect.png", L"Blizzard_Effect.png", 8, 1);
	ResourceManager<Sprite>::Create(L"Blizzard_Effect0.png", L"Blizzard_Effect0.png", 10, 3);
	ResourceManager<Sprite>::Create(L"Blizzard_Hit.png", L"Blizzard_Hit.png", 6, 1);
	ResourceManager<Sprite>::Create(L"Blizzard_ObjectHit.png", L"Blizzard_ObjectHit.png", 10, 3);
	ResourceManager<Sprite>::Create(L"Blizzard_ObjectShortHit.png", L"Blizzard_ObjectShortHit.png", 10, 3);
	ResourceManager<Sprite>::Create(L"Blizzard_Icon.png", L"Blizzard_Icon.png", 1, 1);

	// 포이즌 미스트
	SkillImage = Path::PathToFullFileList(Path::FindPath(L"SKILL_PoisonMist"), L"*.png");
	for (size_t i = 0; i < SkillImage.size(); ++i)
	{
		ResourceManager<Texture>::Load(Path::FindPath(L"SKILL_PoisonMist") + SkillImage[i]);
	}

	ResourceManager<Sprite>::Create(L"PoisonMist_Effect.png", L"PoisonMist_Effect.png", 10, 2);
	ResourceManager<Sprite>::Create(L"PoisonMist_Tile0.png", L"PoisonMist_Tile0.png", 5, 1);
	ResourceManager<Sprite>::Create(L"PoisonMist_Tile1.png", L"PoisonMist_Tile1.png", 6, 1);
	ResourceManager<Sprite>::Create(L"PoisonMist_Tile2.png", L"PoisonMist_Tile2.png", 5, 1);
	ResourceManager<Sprite>::Create(L"PoisonMist_Tile3.png", L"PoisonMist_Tile3.png", 5, 1);
	ResourceManager<Sprite>::Create(L"PoisonMist_Tile4.png", L"PoisonMist_Tile4.png", 6, 1);
	ResourceManager<Sprite>::Create(L"PoisonMist_Tile5.png", L"PoisonMist_Tile5.png", 7, 1);
	ResourceManager<Sprite>::Create(L"PoisonMist_Tile6.png", L"PoisonMist_Tile6.png", 6, 1);
	ResourceManager<Sprite>::Create(L"PoisonMist_Tile7.png", L"PoisonMist_Tile7.png", 6, 1);
	ResourceManager<Sprite>::Create(L"PoisonMist_Tile8.png", L"PoisonMist_Tile8.png", 7, 1);
	ResourceManager<Sprite>::Create(L"PoisonMist_Icon.png", L"PoisonMist_Icon.png", 1, 1);

	// 소울 퍼네트레이션
	SkillImage = Path::PathToFullFileList(Path::FindPath(L"SKILL_SoulPenetration"), L"*.png");
	for (size_t i = 0; i < SkillImage.size(); ++i)
	{
		ResourceManager<Texture>::Load(Path::FindPath(L"SKILL_SoulPenetration") + SkillImage[i]);
	}

	ResourceManager<Sprite>::Create(L"SoulPenetration_Effect.png", L"SoulPenetration_Effect.png", 10, 3);
	ResourceManager<Sprite>::Create(L"SoulPenetration_Hit.png", L"SoulPenetration_Hit.png", 8, 1);
	ResourceManager<Sprite>::Create(L"SoulPenetration_Mob.png", L"SoulPenetration_Mob.png", 5, 1);
	ResourceManager<Sprite>::Create(L"SoulPenetration_MobHit.png", L"SoulPenetration_MobHit.png", 7, 1);
	ResourceManager<Sprite>::Create(L"SoulPenetration_Icon.png", L"SoulPenetration_Icon.png", 1, 1);


	// 카타클리즘
	SkillImage = Path::PathToFullFileList(Path::FindPath(L"SKILL_Cataclysm"), L"*.png");
	for (size_t i = 0; i < SkillImage.size(); ++i)
	{
		ResourceManager<Texture>::Load(Path::FindPath(L"SKILL_Cataclysm") + SkillImage[i]);
	}
	ResourceManager<Sprite>::Create(L"Cataclysm_Effect.png", L"Cataclysm_Effect.png", 16, 1);
	ResourceManager<Sprite>::Create(L"Cataclysm_Effect0.png", L"Cataclysm_Effect0.png", 15, 1);
	ResourceManager<Sprite>::Create(L"Cataclysm_Hit.png", L"Cataclysm_Hit.png", 6, 1);
	ResourceManager<Sprite>::Create(L"Cataclysm_Scene00.png", L"Cataclysm_Scene00.png", 1, 1);
	ResourceManager<Sprite>::Create(L"Cataclysm_Icon.png", L"Cataclysm_Icon.png", 1, 1);

	SkillImage = Path::PathToFullFileList(Path::FindPath(L"SKILL_CataclysmScene"), L"*.png");
	for (size_t i = 0; i < SkillImage.size(); ++i)
	{
		ResourceManager<Texture>::Load(Path::FindPath(L"SKILL_CataclysmScene") + SkillImage[i]);
	}
	ResourceManager<Sprite>::Create(L"Cataclysm_Scene01.png", L"Cataclysm_Scene01.png", 7, 1);
	ResourceManager<Sprite>::Create(L"Cataclysm_Scene02.png", L"Cataclysm_Scene02.png", 7, 1);
	ResourceManager<Sprite>::Create(L"Cataclysm_Scene03.png", L"Cataclysm_Scene03.png", 7, 1);
	ResourceManager<Sprite>::Create(L"Cataclysm_Scene04.png", L"Cataclysm_Scene04.png", 7, 1);
	ResourceManager<Sprite>::Create(L"Cataclysm_Scene05.png", L"Cataclysm_Scene05.png", 7, 1);
	ResourceManager<Sprite>::Create(L"Cataclysm_Scene06.png", L"Cataclysm_Scene06.png", 7, 1);
	ResourceManager<Sprite>::Create(L"Cataclysm_Scene07.png", L"Cataclysm_Scene07.png", 7, 1);
	ResourceManager<Sprite>::Create(L"Cataclysm_Scene08.png", L"Cataclysm_Scene08.png", 7, 1);


	// 몬스터
	// 아이 마네킹
	MonsterImages = Path::PathToFullFileList(Path::FindPath(L"GM_ChildMannequin"), L"*.png");
	for (size_t i = 0; i < MonsterImages.size(); ++i)
	{
		ResourceManager<Texture>::Load(Path::FindPath(L"GM_ChildMannequin") + MonsterImages[i]);
	}
	ResourceManager<Sprite>::Create(L"ChildMannequin.png", L"ChildMannequin.png", 10, 2);
	
	// 오염된 수액
	MonsterImages = Path::PathToFullFileList(Path::FindPath(L"GM_ContaminatedSap"), L"*.png");
	for (size_t i = 0; i < MonsterImages.size(); ++i)
	{
		ResourceManager<Texture>::Load(Path::FindPath(L"GM_ContaminatedSap") + MonsterImages[i]);
	}
	ResourceManager<Sprite>::Create(L"ContaminatedSap.png", L"ContaminatedSap.png", 10, 5);
	ResourceManager<Sprite>::Create(L"ContaminatedSap_AttackEffect.png", L"ContaminatedSap_AttackEffect.png", 5, 1);

	// 공포 감시자
	MonsterImages = Path::PathToFullFileList(Path::FindPath(L"GM_PredatorOfFear"), L"*.png");
	for (size_t i = 0; i < MonsterImages.size(); ++i)
	{
		ResourceManager<Texture>::Load(Path::FindPath(L"GM_PredatorOfFear") + MonsterImages[i]);
	}
	ResourceManager<Sprite>::Create(L"PredatorOfFear.png", L"PredatorOfFear.png", 10, 7);
	ResourceManager<Sprite>::Create(L"PredatorOfFear_AttackEffect.png", L"PredatorOfFear_AttackEffect.png", 7, 1);

	// 천록
	MonsterImages = Path::PathToFullFileList(Path::FindPath(L"GM_Tianlu"), L"*.png");
	for (size_t i = 0; i < MonsterImages.size(); ++i)
	{
		ResourceManager<Texture>::Load(Path::FindPath(L"GM_Tianlu") + MonsterImages[i]);
	}
	ResourceManager<Sprite>::Create(L"Tianlu.png", L"Tianlu.png", 10, 6);
	ResourceManager<Sprite>::Create(L"Tianlu_AttackEffect.png", L"Tianlu_AttackEffect.png", 10, 3);

	// 변형된 슬라임
	MonsterImages = Path::PathToFullFileList(Path::FindPath(L"GM_TransformSlime"), L"*.png");
	for (size_t i = 0; i < MonsterImages.size(); ++i)
	{
		ResourceManager<Texture>::Load(Path::FindPath(L"GM_TransformSlime") + MonsterImages[i]);
	}
	ResourceManager<Sprite>::Create(L"TransformSlime.png", L"TransformSlime.png", 10, 7);
	ResourceManager<Sprite>::Create(L"TransformSlimeAttackEffect.png", L"TransformSlimeAttackEffect.png", 4, 1);

	// 벨라리스
	MonsterImages = Path::PathToFullFileList(Path::FindPath(L"GM_Velaris"), L"*.png");
	for (size_t i = 0; i < MonsterImages.size(); ++i)
	{
		ResourceManager<Texture>::Load(Path::FindPath(L"GM_Velaris") + MonsterImages[i]);
	}
	ResourceManager<Sprite>::Create(L"Velaris.png", L"Velaris.png", 10, 5);
	ResourceManager<Sprite>::Create(L"Velaris_AttackEffect.png", L"Velaris_AttackEffect.png", 5, 1);

	// 흰 모래토끼
	MonsterImages = Path::PathToFullFileList(Path::FindPath(L"GM_WhiteSandRabbit"), L"*.png");
	for (size_t i = 0; i < MonsterImages.size(); ++i)
	{
		ResourceManager<Texture>::Load(Path::FindPath(L"GM_WhiteSandRabbit") + MonsterImages[i]);
	}
	ResourceManager<Sprite>::Create(L"WhiteSandRabbit.png", L"WhiteSandRabbit.png", 10, 4);
	ResourceManager<Sprite>::Create(L"WhiteSandRabbit_AttackEffect.png", L"WhiteSandRabbit_AttackEffect.png", 5, 1);

	// 핑크빈
	MonsterImages = Path::PathToFullFileList(Path::FindPath(L"Boss_Pinkbin"), L"*.png");
	for (size_t i = 0; i < MonsterImages.size(); ++i)
	{
		ResourceManager<Texture>::Load(Path::FindPath(L"Boss_Pinkbin") + MonsterImages[i]);
	}
	ResourceManager<Sprite>::Create(L"Pinkbin_Attack.png", L"Pinkbin_Attack.png", 10, 10);
	ResourceManager<Sprite>::Create(L"Pinkbin_Base.png", L"Pinkbin_Base.png", 10, 19);
	ResourceManager<Sprite>::Create(L"Pinkbin_Regen.png", L"Pinkbin_Regen.png", 10, 2);
	ResourceManager<Sprite>::Create(L"Pinkbin_SkillEtc.png", L"Pinkbin_SkillEtc.png", 10, 11);

	ResourceManager<Sprite>::Create(L"Attack1_Effect.png", L"Attack1_Effect.png", 10, 3);
	ResourceManager<Sprite>::Create(L"Attack3_Effect.png", L"Attack3_Effect.png", 10, 3);
	ResourceManager<Sprite>::Create(L"Attack4_Effect.png", L"Attack4_Effect.png", 7, 1);

	ResourceManager<Sprite>::Create(L"Attack1_Hit.png", L"Attack1_Hit.png", 7, 1);
	ResourceManager<Sprite>::Create(L"Attack2_Hit.png", L"Attack2_Hit.png", 5, 1);
	ResourceManager<Sprite>::Create(L"Attack3_Hit.png", L"Attack1_Hit.png", 6, 1);
	ResourceManager<Sprite>::Create(L"Attack4_Hit.png", L"Attack4_Hit.png", 4, 1);


}