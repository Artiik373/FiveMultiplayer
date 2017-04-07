#ifndef __LUANPC_H__
#define __LUANPC_H__

struct NPC
{
	int entity;

	NPC()
	{
		entity = -1;
	}

	int Create(lua_State* L)
	{
		/*
		Arg 1: The pointer of the Object
		Arg 2+: The arguments entered
		*/
		const int args = lua_gettop(L);
		if (args == 4)
		{
			NPC* ent = reinterpret_cast<NPC*>(lua_touserdata(L, 1));

			std::wstring model;
			CVector3 poss;
			CVector3 rott;

			model = CString::utf8ToUtf16(lua_tostring(L, 2));

			if (lua_isuserdata(L, 3))
			{
				Vector3* pos = reinterpret_cast<Vector3*>(lua_touserdata(L, 3));
				poss = { pos->coord[0], pos->coord[1], pos->coord[2] };
				pos = nullptr;
			}
			else if (lua_istable(L, 3))
			{
				lua_getfield(L, 3, "x");
				lua_rawgeti(L, 3, 1);
				poss.fX = lua_tonumber(L, -2);
				lua_pop(L, 1);

				lua_getfield(L, 3, "y");
				lua_rawgeti(L, 3, 1);
				poss.fY = lua_tonumber(L, -2);
				lua_pop(L, 1);

				lua_getfield(L, 3, "z");
				lua_rawgeti(L, 3, 1);
				poss.fZ = lua_tonumber(L, -2);
				lua_pop(L, 1);
			}

			if (lua_isuserdata(L, 4))
			{
				Vector3* rot = reinterpret_cast<Vector3*>(lua_touserdata(L, 3));
				rott = { rot->coord[0], rot->coord[1], rot->coord[2] };
				rot = nullptr;
			}
			else if (lua_istable(L, 4))
			{
				lua_getfield(L, 4, "x");
				lua_rawgeti(L, 4, 1);
				rott.fX = lua_tonumber(L, -2);
				lua_pop(L, 1);

				lua_getfield(L, 4, "y");
				lua_rawgeti(L, 4, 1);
				rott.fY = lua_tonumber(L, -2);
				lua_pop(L, 1);

				lua_getfield(L, 4, "z");
				lua_rawgeti(L, 4, 1);
				rott.fZ = lua_tonumber(L, -2);
				lua_pop(L, 1);
			}
			
			ent->entity = API::NPC::Create(model, poss, rott);

			ent = nullptr;
		}
		return 0;
	}

	int Destroy(lua_State* L)
	{
		const int args = lua_gettop(L);
		if (args == 1)
		{
			NPC* ent = reinterpret_cast<NPC*>(lua_touserdata(L, 1));
			API::Entity::Destroy(ent->entity);
			ent->entity = -1;
			ent = nullptr;
		}
		return 0;
	}

	int GetPosition(lua_State* L)
	{
		const int args = lua_gettop(L);
		if (args == 1)
		{
			NPC* ent = reinterpret_cast<NPC*>(lua_touserdata(L, 1));

			CVector3 poss = API::Entity::GetPosition(ent->entity);
			/*Vec pos(poss.fX, poss.fY, poss.fZ);
			lua_pushlightuserdata(L, &pos);*/
			lua_newtable(L);

			lua_pushinteger(L, poss.fX);
			lua_setfield(L, -2, "x");

			lua_pushinteger(L, poss.fY);
			lua_setfield(L, -2, "y");

			lua_pushinteger(L, poss.fZ);
			lua_setfield(L, -2, "z");


			ent = nullptr;
		}
		else
		{
			lua_pushnil(L);
		}
		return 1;
	}

	int SetPosition(lua_State* L)
	{
		const int args = lua_gettop(L);
		if (args == 2 || args == 4)
		{
			NPC* ent = reinterpret_cast<NPC*>(lua_touserdata(L, 1));
			CVector3 poss;

			if (args == 2)
			{
				if (lua_isuserdata(L, 2))
				{
					Vector3* pos = reinterpret_cast<Vector3*>(lua_touserdata(L, 2));
					poss = { pos->coord[0], pos->coord[1], pos->coord[2] };
					pos = nullptr;
				}
				else if (lua_istable(L, 2))
				{
					lua_getfield(L, 2, "x");
					lua_rawgeti(L, 2, 1);
					poss.fX = lua_tonumber(L, -2);
					lua_pop(L, 1);

					lua_getfield(L, 2, "y");
					lua_rawgeti(L, 2, 1);
					poss.fY = lua_tonumber(L, -2);
					lua_pop(L, 1);

					lua_getfield(L, 2, "z");
					lua_rawgeti(L, 2, 1);
					poss.fZ = lua_tonumber(L, -2);
					lua_pop(L, 1);
				}
			}
			else
			{
				poss.fX = lua_tonumber(L, 2);
				poss.fX = lua_tonumber(L, 3);
				poss.fX = lua_tonumber(L, 4);
			}

			API::Entity::SetPosition(ent->entity, poss);
			ent = nullptr;
		}
		return 0;
	}

	int GetRotation(lua_State* L)
	{
		const int args = lua_gettop(L);
		if (args == 1)
		{
			NPC* ent = reinterpret_cast<NPC*>(lua_touserdata(L, 1));

			CVector3 rot = API::Entity::GetRotation(ent->entity);
			/*Vec pos(poss.fX, poss.fY, poss.fZ);
			lua_pushlightuserdata(L, &pos);*/
			lua_newtable(L);

			lua_pushinteger(L, rot.fX);
			lua_setfield(L, -2, "x");

			lua_pushinteger(L, rot.fY);
			lua_setfield(L, -2, "y");

			lua_pushinteger(L, rot.fZ);
			lua_setfield(L, -2, "z");


			ent = nullptr;
		}
		else
		{
			lua_pushnil(L);
		}
		return 1;
	}

	int SetRotation(lua_State* L)
	{
		const int args = lua_gettop(L);
		if (args == 2 || args == 4)
		{
			NPC* ent = reinterpret_cast<NPC*>(lua_touserdata(L, 1));
			CVector3 rott;

			if (args == 2)
			{
				if (lua_isuserdata(L, 2))
				{
					Vector3* rot = reinterpret_cast<Vector3*>(lua_touserdata(L, 2));
					rott = { rot->coord[0], rot->coord[1], rot->coord[2] };
					rot = nullptr;
				}
				else if (lua_istable(L, 2))
				{
					lua_getfield(L, 2, "x");
					lua_rawgeti(L, 2, 1);
					rott.fX = lua_tonumber(L, -2);
					lua_pop(L, 1);

					lua_getfield(L, 2, "y");
					lua_rawgeti(L, 2, 1);
					rott.fY = lua_tonumber(L, -2);
					lua_pop(L, 1);

					lua_getfield(L, 2, "z");
					lua_rawgeti(L, 2, 1);
					rott.fZ = lua_tonumber(L, -2);
					lua_pop(L, 1);
				}
			}
			else
			{
				rott.fX = lua_tonumber(L, 2);
				rott.fX = lua_tonumber(L, 3);
				rott.fX = lua_tonumber(L, 4);
			}

			API::Entity::SetRotation(ent->entity, rott);
			ent = nullptr;
		}
		return 0;
	}

	int GetViewDistance(lua_State* L)
	{
		const int args = lua_gettop(L);
		if (args == 1)
		{
			NPC* ent = reinterpret_cast<NPC*>(lua_touserdata(L, 1));

			const float distance = API::Entity::GetViewDistance(ent->entity);

			lua_pushnumber(L, distance);

			ent = nullptr;
		}
		else
		{
			lua_pushnil(L);
		}
		return 1;
	}

	int SetViewDistance(lua_State* L)
	{
		const int args = lua_gettop(L);
		if (args == 2)
		{
			NPC* ent = reinterpret_cast<NPC*>(lua_touserdata(L, 1));

			const float distance = lua_tonumber(L, 2);

			API::Entity::SetViewDistance(ent->entity, distance);
			ent = nullptr;
		}
		return 0;
	}

	int GetPedComponent(lua_State* L)
	{
		const int args = lua_gettop(L);
		if (args == 1)
		{
			NPC* ent = reinterpret_cast<NPC*>(lua_touserdata(L, 1));
			const int index = lua_tointeger(L, 2);

			PedComponent comp = API::Entity::GetPedComponent(ent->entity, index);

			lua_newtable(L);

			lua_pushinteger(L, comp.drawableid);
			lua_setfield(L, -2, "drawableid");

			lua_pushinteger(L, comp.paletteid);
			lua_setfield(L, -2, "paletteid");

			lua_pushinteger(L, comp.textureid);
			lua_setfield(L, -2, "textureid");

			ent = nullptr;
		}
		else
		{
			lua_pushnil(L);
		}
		return 1;
	}

	int SetPedComponent(lua_State* L)
	{
		const int args = lua_gettop(L);
		if (args >= 3)
		{
			NPC* ent = reinterpret_cast<NPC*>(lua_touserdata(L, 1));
			const int index = lua_tointeger(L, 2);

			PedComponent comp;

			if (lua_istable(L, 3))
			{
				lua_getfield(L, 3, "drawableid");
				lua_rawgeti(L, 3, 1);
				comp.drawableid = lua_tonumber(L, -2);
				lua_pop(L, 1);

				lua_getfield(L, 3, "paletteid");
				lua_rawgeti(L, 3, 1);
				comp.paletteid = lua_tonumber(L, -2);
				lua_pop(L, 1);

				lua_getfield(L, 3, "textureid");
				lua_rawgeti(L, 3, 1);
				comp.textureid = lua_tonumber(L, -2);
				lua_pop(L, 1);
			}

			API::Entity::SetPedComponent(ent->entity, index, comp);

			ent = nullptr;
		}
		return 0;
	}

	int GetPedHeadBlend(lua_State* L)
	{
		const int args = lua_gettop(L);
		if (args == 1)
		{
			NPC* ent = reinterpret_cast<NPC*>(lua_touserdata(L, 1));
			const int index = lua_tointeger(L, 2);

			PedHeadBlend blend = API::Entity::GetPedHeadBlend(ent->entity);

			lua_newtable(L);
			//table{shapefirst = int, shapesecond = int, shapethird = int, skinfirst = int, skinsecond = int, skinthird = int, shapemix = float, skinmix = float, thirdmix = float}
			lua_pushinteger(L, blend.shapeFirst);
			lua_setfield(L, -2, "shapefirst");

			lua_pushinteger(L, blend.shapeSecond);
			lua_setfield(L, -2, "shapesecond");

			lua_pushinteger(L, blend.shapeThird);
			lua_setfield(L, -2, "shapethird");

			lua_pushinteger(L, blend.skinFirst);
			lua_setfield(L, -2, "skinfirst");

			lua_pushinteger(L, blend.skinSecond);
			lua_setfield(L, -2, "skinsecond");

			lua_pushinteger(L, blend.skinThird);
			lua_setfield(L, -2, "skinthird");

			lua_pushnumber(L, blend.shapeMix);
			lua_setfield(L, -2, "shapemix");

			lua_pushnumber(L, blend.skinMix);
			lua_setfield(L, -2, "skinmix");

			lua_pushnumber(L, blend.thirdMix);
			lua_setfield(L, -2, "thirdmix");

			ent = nullptr;
		}
		else
		{
			lua_pushnil(L);
		}
		return 1;
	}

	int SetPedHeadBlend(lua_State* L)
	{
		const int args = lua_gettop(L);
		if (args >= 2)
		{
			NPC* ent = reinterpret_cast<NPC*>(lua_touserdata(L, 1));

			PedHeadBlend headblend;
			
			if (lua_istable(L, 2))
			{
				lua_getfield(L, 2, "shapefirst");
				lua_rawgeti(L, 2, 1);
				headblend.shapeFirst = lua_tonumber(L, -2);
				lua_pop(L, 1);

				lua_getfield(L, 2, "shapesecond");
				lua_rawgeti(L, 2, 1);
				headblend.shapeSecond = lua_tonumber(L, -2);
				lua_pop(L, 1);

				lua_getfield(L, 2, "shapethird");
				lua_rawgeti(L, 2, 1);
				headblend.shapeThird = lua_tonumber(L, -2);
				lua_pop(L, 1);

				lua_getfield(L, 2, "skinfirst");
				lua_rawgeti(L, 2, 1);
				headblend.skinFirst = lua_tonumber(L, -2);
				lua_pop(L, 1);

				lua_getfield(L, 2, "skinsecond");
				lua_rawgeti(L, 2, 1);
				headblend.skinSecond = lua_tonumber(L, -2);
				lua_pop(L, 1);

				lua_getfield(L, 2, "skinthird");
				lua_rawgeti(L, 2, 1);
				headblend.skinThird = lua_tonumber(L, -2);
				lua_pop(L, 1);
				
				lua_getfield(L, 2, "shapemix");
				lua_rawgeti(L, 2, 1);
				headblend.shapeMix = lua_tonumber(L, -2);
				lua_pop(L, 1);

				lua_getfield(L, 2, "skinmix");
				lua_rawgeti(L, 2, 1);
				headblend.skinMix = lua_tonumber(L, -2);
				lua_pop(L, 1);

				lua_getfield(L, 2, "thirdmix");
				lua_rawgeti(L, 2, 1);
				headblend.thirdMix = lua_tonumber(L, -2);
				lua_pop(L, 1);
			}

			API::Entity::SetPedHeadBlend(ent->entity, headblend);

			ent = nullptr;
		}
		return 0;
	}

	int GetPedHeadOverlay(lua_State* L)
	{
		const int args = lua_gettop(L);
		if (args == 1)
		{
			NPC* ent = reinterpret_cast<NPC*>(lua_touserdata(L, 1));
			const int overlayid = lua_tointeger(L, 2);

			PedHeadOverlay overlay = API::Entity::GetPedHeadOverlay(ent->entity, overlayid);

			lua_newtable(L);
			//table{index = int, opacity = float, colortype = int, colorid = int, secondcolorid = int}
			lua_pushinteger(L, overlay.index);
			lua_setfield(L, -2, "index");

			lua_pushnumber(L, overlay.opacity);
			lua_setfield(L, -2, "shapesecond");

			lua_pushinteger(L, overlay.colorType);
			lua_setfield(L, -2, "colortype");

			lua_pushinteger(L, overlay.colorID);
			lua_setfield(L, -2, "colorid");

			lua_pushinteger(L, overlay.secondColorID);
			lua_setfield(L, -2, "secondcolorid");

			ent = nullptr;
		}
		else
		{
			lua_pushnil(L);
		}
		return 1;
	}

	int SetPedHeadOverlay(lua_State* L)
	{
		const int args = lua_gettop(L);
		if (args >= 2)
		{
			NPC* ent = reinterpret_cast<NPC*>(lua_touserdata(L, 1));

			const int overlayid = lua_tointeger(L, 2);

			PedHeadOverlay overlay;

			if (lua_istable(L, 3))
			{
				//table{index = int, opacity = float, colortype = int, colorid = int, secondcolorid = int}
				lua_getfield(L, 3, "index");
				lua_rawgeti(L, 3, 1);
				overlay.index = lua_tointeger(L, -2);
				lua_pop(L, 1);

				lua_getfield(L, 3, "opacity");
				lua_rawgeti(L, 3, 1);
				overlay.opacity = lua_tonumber(L, -2);
				lua_pop(L, 1);

				lua_getfield(L, 3, "colortype");
				lua_rawgeti(L, 3, 1);
				overlay.colorType = lua_tointeger(L, -2);
				lua_pop(L, 1);

				lua_getfield(L, 3, "colorid");
				lua_rawgeti(L, 3, 1);
				overlay.colorID = lua_tointeger(L, -2);
				lua_pop(L, 1);

				lua_getfield(L, 3, "secondcolorid");
				lua_rawgeti(L, 3, 1);
				overlay.secondColorID = lua_tointeger(L, -2);
				lua_pop(L, 1);

				
			}

			API::Entity::SetPedHeadOverlay(ent->entity, overlayid, overlay);

			ent = nullptr;
		}
		return 0;
	}

	int GetPedProp(lua_State* L)
	{
		const int args = lua_gettop(L);
		if (args == 1)
		{
			NPC* ent = reinterpret_cast<NPC*>(lua_touserdata(L, 1));
			const int componentid = lua_tointeger(L, 2);

			PedProp prop = API::Entity::GetPedProp(ent->entity, componentid);

			lua_newtable(L);
			//table{drawableid = int, textureid = int}
			lua_pushinteger(L, prop.drawableid);
			lua_setfield(L, -2, "drawableid");

			lua_pushnumber(L, prop.textureid);
			lua_setfield(L, -2, "textureid");

			ent = nullptr;
		}
		else
		{
			lua_pushnil(L);
		}
		return 1;
	}

	int SetPedProp(lua_State* L)
	{
		const int args = lua_gettop(L);
		if (args >= 2)
		{
			NPC* ent = reinterpret_cast<NPC*>(lua_touserdata(L, 1));

			const int componentid = lua_tointeger(L, 2);

			PedProp prop;

			if (lua_istable(L, 3))
			{
				//table{drawableid = int, textureid = int}
				lua_getfield(L, 3, "drawableid");
				lua_rawgeti(L, 3, 1);
				prop.drawableid = lua_tointeger(L, -2);
				lua_pop(L, 1);

				lua_getfield(L, 3, "textureid");
				lua_rawgeti(L, 3, 1);
				prop.textureid = lua_tointeger(L, -2);
				lua_pop(L, 1);
			}

			API::Entity::SetPedProp(ent->entity, componentid, prop);

			ent = nullptr;
		}
		return 0;
	}

	int GetPedFaceFeature(lua_State* L)
	{
		const int args = lua_gettop(L);
		if (args == 1)
		{
			NPC* ent = reinterpret_cast<NPC*>(lua_touserdata(L, 1));
			const int feature = lua_tointeger(L, 2);

			float scale = API::Entity::GetPedFaceFeature(ent->entity, feature);

			lua_pushnumber(L, scale);

			ent = nullptr;
		}
		else
		{
			lua_pushnil(L);
		}
		return 1;
	}

	int SetPedFaceFeature(lua_State* L)
	{
		const int args = lua_gettop(L);
		if (args >= 2)
		{
			NPC* ent = reinterpret_cast<NPC*>(lua_touserdata(L, 1));

			const int feature = lua_tointeger(L, 2);
			const float scale = lua_tonumber(L, 3);

			API::Entity::SetPedFaceFeature(ent->entity, feature, scale);

			ent = nullptr;
		}
		return 0;
	}
};
#endif