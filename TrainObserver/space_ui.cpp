#include "space_ui.h"
#include "space.h"
#include "camera.h"
#include "render_dx9.h"
#include "ui_manager.h"
#include "..\ui.h"
#include <d3d9types.h>




namespace SpaceUI
{
enum class UIObjectType : uint
{
	POST = 1,
	TRAIN = 2,
};

const uint COLOR_COUNT = 4;

const DWORD colors[COLOR_COUNT] = {D3DCOLOR_ARGB(255, 210, 145, 20),
								   D3DCOLOR_ARGB(255, 210, 250, 180),
								   D3DCOLOR_ARGB(255, 180, 210, 250),
								   D3DCOLOR_ARGB(255, 245, 180, 250)};


uint generateUIIndex(UIObjectType objType, uint objIdx, uint uiElementIdx)
{
	return ((uint)objType << 24) | (objIdx << 16) | uiElementIdx;
}

void createPostUI(const Vector3& pos, const Post& post)
{
	auto& rs = RenderSystemDX9::instance();
	auto& view = rs.uiManager().view();

	// get screen pos from world pos
	ScreenPos screenPos;
	bool	  isInScreen = rs.renderer().camera().worldPosToScreenPos(pos, screenPos);

	uint uiControlIdx = 0;

	{
		uint uiIdx = generateUIIndex(UIObjectType::POST, post.idx, uiControlIdx++);

		view.RemoveControl(uiIdx); // remove previous frame control

		if (isInScreen)
		{
			char	  buf[512];
			ScreenPos controlSize = {100, 40};
			switch (post.type)
			{
			case EPostType::CITY:
				sprintf_s(
					buf,
					"%s\npopulation: %d / %d\nproduct: %d / %d\narmor: %d / %d",
					post.name.c_str(),
					post.population,
					post.population_capacity,
					post.product,
					post.product_capacity,
					post.armor,
					post.armor_capacity);
				controlSize.x = 100;
				controlSize.y = 70;
				break;
			case EPostType::MARKET:
				sprintf_s(buf, "%s\nproduct: %d / %d", post.name.c_str(), post.product, post.product_capacity);
				controlSize.x = 90;
				controlSize.y = 40;
				break;
			case EPostType::MILITARY_STORAGE:
				sprintf_s(buf, "%s\narmor: %d / %d", post.name.c_str(), post.armor, post.armor_capacity);
				controlSize.x = 80;
				controlSize.y = 40;
				break;
			}

			view.AddStatic(
				uiIdx,
				buf,
				screenPos.x - controlSize.x / 2,
				screenPos.y - controlSize.y * 2,
				controlSize.x,
				controlSize.y);
			view.GetStatic(uiIdx)->SetTextColor(colors[(unsigned)post.type]);
		}
	}
}

void createTrainUI(const Vector3& pos, const Train& train)
{
	auto& rs = RenderSystemDX9::instance();
	auto& view = rs.uiManager().view();

	// get screen pos from world pos
	ScreenPos screenPos;
	bool	  isInScreen = rs.renderer().camera().worldPosToScreenPos(pos, screenPos);


	uint uiIdx = generateUIIndex(UIObjectType::TRAIN, train.idx, 0);

	view.RemoveControl(uiIdx); // remove previous frame control

	if (isInScreen)
	{
		static std::unordered_map<std::string, DWORD> player_color;
		static int									  color_num = 0;

		DWORD color = 0;
		auto  it = player_color.find(train.player_id);
		if (it == player_color.end())
		{
			color = colors[color_num++ % COLOR_COUNT];
			player_color[train.player_id] = color;
		}
		else
		{
			color = it->second;
		}

		char	  buf[512];
		ScreenPos controlSize = {100, 40};
		sprintf_s(
			buf,
			"player: %s\ngoods: %d/%d\nlvl: %d",
			train.player_id.c_str(),
			train.goods,
			train.goods_capacity,
			train.level);
		controlSize.x = 80;
		controlSize.y = 60;

		view.AddStatic(
			uiIdx, buf, screenPos.x - controlSize.x / 2, screenPos.y - controlSize.y * 2, controlSize.x, controlSize.y);
		view.GetStatic(uiIdx)->SetTextColor(color);
	}
}

} // namespace SpaceUI
