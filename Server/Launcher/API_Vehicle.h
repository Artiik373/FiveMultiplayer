#pragma once
extern "C" {
	namespace API
	{
		class Vehicle
		{
			static const char *ThisNamespace;

		public:
			DLL_PUBLIC static const int Create(const std::wstring model, const CVector3 position, const float heading);
			DLL_PUBLIC static const int Create(const std::wstring model, const CVector3 position, const CVector3 rotation);

			DLL_PUBLIC static void SetColor(const int entity, const int layer, const int painttype, const int color);
			DLL_PUBLIC static void SetColor(const int entity, const int layer, const Color color);

			DLL_PUBLIC static void SetNumberPlate(const int entity, std::wstring plate);
			DLL_PUBLIC static const std::wstring GetNumberPlate(const int entity);

			DLL_PUBLIC static const int GetMod(const int entity, const int modType);
			DLL_PUBLIC static void SetMod(const int entity, const int modType, const int modIndex);

			DLL_PUBLIC static const bool GetEngineState(const int entity);
			DLL_PUBLIC static void SetEngineState(const int entity, const bool state);

			DLL_PUBLIC static const int GetDoorsLockState(const int entity);
			DLL_PUBLIC static void SetDoorsLockState(const int entity, const int state);
		};
	}
}