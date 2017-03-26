#pragma once
extern "C" {
	namespace API
	{
		class Entity
		{
			static const char *ThisNamespace;

		public:
		public:
			DLL_PUBLIC struct PedComponent
			{
				int drawableid = -1;
				int textureid = -1;
				int paletteid = -1;
			};

			DLL_PUBLIC struct PedHeadBlend
			{
				int shapeFirst = 0;
				int shapeSecond = 0;
				int shapeThird = 0;
				int skinFirst = 0;
				int skinSecond = 0;
				int skinThird = 0;
				float shapeMix = 0;
				float skinMix = 0;
				float thirdMix = 0;
			};

			DLL_PUBLIC struct PedHeadOverlay
			{
				int index = 0;
				float opacity = 0.0f;
				int colorType = 0;
				int colorID = 0;
				int secondColorID = 0;
			};

			DLL_PUBLIC struct PedProp
			{
				int drawableid = 0;
				int textureid = 0;
			};

			DLL_PUBLIC struct PedFeature
			{
				float scale = 0.0f;
			};

			DLL_PUBLIC static void Destroy(int entity);
			DLL_PUBLIC static const CVector3 GetPosition(int entity);
			DLL_PUBLIC static void SetPosition(int entity, CVector3 position);
			DLL_PUBLIC static const CVector4 GetQuaternion(int entity);
			DLL_PUBLIC static void SetQuaternion(int entity, CVector4 quaternion);
			DLL_PUBLIC static const CVector3 GetRotation(int entity);
			DLL_PUBLIC static void SetRotation(int entity, CVector3 rotation);

			// NPC & Player
			DLL_PUBLIC static const PedComponent GetPedComponent(const int entity, const int componentid);
			DLL_PUBLIC static void SetPedComponent(const int entity, const int componentid, PedComponent component);
			DLL_PUBLIC static const PedHeadBlend GetPedHeadBlend(const int entity);
			DLL_PUBLIC static void SetPedHeadBlend(const int entity, PedHeadBlend headblend);
			DLL_PUBLIC static const PedHeadOverlay GetPedHeadOverlay(const int entity, const int index);
			DLL_PUBLIC static void SetPedHeadOverlay(const int entity, const int index, PedHeadOverlay overlay);
			DLL_PUBLIC static const PedProp GetPedProp(const int entity, const int index);
			DLL_PUBLIC static void SetPedProp(const int entity, const int index, PedProp prop);
			DLL_PUBLIC static const float GetPedFaceFeature(const int entity, const int index);
			DLL_PUBLIC static void SetPedFaceFeature(const int entity, const int index, const float scale);

			DLL_PUBLIC static const float GetViewDistance(const int entity);
			DLL_PUBLIC static void SetViewDistance(const int entity, const float distance);
		};
	}
}