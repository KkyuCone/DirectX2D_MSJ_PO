#pragma once

class ItemKind
{
public:
	enum ITEM_KIND
	{
		EQUIP = 0,
		CONSUME,
		ETC,
		INSTALL,
		CASH,
		NONE,
	};
public:
	ItemKind() {}
	~ItemKind() {}
};
