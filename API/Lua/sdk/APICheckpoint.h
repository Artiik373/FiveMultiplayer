#pragma once

#ifdef __cplusplus
extern "C" {
#endif
	namespace API
	{
		class Checkpoint
		{
		public:
			/// <summary>
			/// Creates a Checkpoint
			/// </summary>
			/// <param name="position">The you wish the checkpoint to be at.</param>
			/// <param name="pointto">The position the checkpoint points too.</param>
			/// <param name="type">The type of checkpoint, pastebin.com/raw/iG0NkEhF</param>
			/// <param name="radius">The radius of the checkpoint</param>
			/// <param name="color">The color of the checkpoint</param>
			/// <param name="reserved">The reserved type of checkpoint, pastebin.com/raw/iG0NkEhF</param>
			/// <returns name="entity">The checkpoint server entity id</returns>
			DLL_PUBLIC_I static const int Create(const CVector3 position, const CVector3 pointto, const int type, const float radius, const Color color, const int reserved);

			/// <summary>
			/// Displays the checkpoint for a player
			/// </summary>
			/// <param name="checkpointentity">The entity of the checkpoint you wish to display.</param>
			/// <param name="playerentity">The entity of the player you wish to display the checkpoint too.  [Tip: -1 will display it too all connected players]</param>
			DLL_PUBLIC_I static void Show(const int checkpointentity, const int playerentity);

			/// <summary>
			/// Hides the checkpoint for a player
			/// </summary>
			/// <param name="checkpointentity">The entity of the checkpoint you wish to hide.</param>
			/// <param name="playerentity">The entity of the player you wish to hide the checkpoint from.  [Tip: -1 will hide it from all connected players]</param>
			DLL_PUBLIC_I static void Hide(const int checkpointentity, const int playerentity);

			/// <summary>
			/// Gets the checkpoints near height
			/// </summary>
			/// <param name="checkpointentity">The entity of the checkpoint.</param>
			/// <returns name="height">The checkpoint near height</returns>
			DLL_PUBLIC_I static const float GetNearHeight(const int checkpointentity);

			/// <summary>
			/// Sets the checkpoints near distance height
			/// </summary>
			/// <param name="checkpointentity">The entity of the checkpoint.</param>
			/// <param name="height">The height</param>
			DLL_PUBLIC_I static void SetNearHeight(const int checkpointentity, const float height);

			/// <summary>
			/// Gets the checkpoints far height
			/// </summary>
			/// <param name="checkpointentity">The entity of the checkpoint.</param>
			/// <returns name="height">The checkpoint far height</returns>
			DLL_PUBLIC_I static const float GetFarHeight(const int checkpointentity);

			/// <summary>
			/// Sets the checkpoints far distance height
			/// </summary>
			/// <param name="checkpointentity">The entity of the checkpoint.</param>
			/// <param name="height">The height</param>
			DLL_PUBLIC_I static void SetFarHeight(const int checkpointentity, const float height);
		};
	}
#ifdef __cplusplus
}
#endif