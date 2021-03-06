#pragma once

#ifdef __cplusplus
extern "C" {
#endif
	namespace API
	{
		class Visual
		{
		public:
			/// <summary>
			/// Sends a notification message that displays above the game minimap to all clients.
			/// </summary>
			/// <param name="message">The message of the notification</param>
			/// <param name="pic">Picture to display (pastebin.com/XdpJVbHz)</param>
			/// <param name="icontype">The icon type to use (iconTypes: 1 : Chat Box, 2 : Email, 3 : Add Friend Request, 4 : Nothing, 5 : Nothing, 6 : Nothing, 7 : Right Jumping Arrow, 8 : RP Icon, 9 : $ Icon)</param>
			/// <param name="sender">The sender is the very top header. This can be any string.</param>
			/// <param name="subject">The subject is the header under the sender.</param>
			/// <returns></returns>
			DLL_PUBLIC_I static void ShowMessageAboveMap(const std::wstring message, const std::wstring pic, const int icontype, const std::wstring sender, const std::wstring subject);

			/// <summary>
			/// Sends a notification message that displays above the game minimap to a specific client.
			/// </summary>
			/// <param name="entity">The entity id of the player you wish to send it too</param>
			/// <param name="message">The message of the notification</param>
			/// <param name="pic">Picture to display (pastebin.com/XdpJVbHz)</param>
			/// <param name="icontype">The icon type to use (iconTypes: 1 : Chat Box, 2 : Email, 3 : Add Friend Request, 4 : Nothing, 5 : Nothing, 6 : Nothing, 7 : Right Jumping Arrow, 8 : RP Icon, 9 : $ Icon)</param>
			/// <param name="sender">The sender is the very top header. This can be any string.</param>
			/// <param name="subject">The subject is the header under the sender.</param>
			/// <returns></returns>
			DLL_PUBLIC_I static void ShowMessageAboveMapToPlayer(const int entity, const std::wstring message, const std::wstring pic, const int icontype, const std::wstring sender, const std::wstring subject);

			/// <summary>
			/// Sends a chat message to all clients.
			/// </summary>
			/// <param name="message">The string of the message</param>
			DLL_PUBLIC_I static void SendChatMessage(const std::string message);

			/// <summary>
			/// Sends a chat message to a client.
			/// </summary>
			/// <param name="entity">The entityid of the player you wish to send the message to.</param>
			/// <param name="message">The string of the message</param>
			DLL_PUBLIC_I static void SendChatMessageToPlayer(const int entity, const std::string message);

			/// <summary>
			/// Enables/disables the cursor on-screen. Works with CEF and ImGui
			/// </summary>
			/// <param name="entity">The entityid of the player you wish to enable/disable the cursor of.</param>
			/// <param name="show">Whether to show the cursor or not (true/false)</param>
			DLL_PUBLIC_I static void ShowCursor(const int entity, bool show);

			/// <summary>
			/// Sends a subtitle message that displays on the bottom of the game to all clients
			/// </summary>
			/// <param name="message">The message of the subtitle</param>
			/// <param name="duration">The amount of time the subtitle should be shown (in milliseconds)</param>
			/// <param name="shownow">Whether the subtitle must be showed immediately or not.</param>
			/// <returns></returns>
			DLL_PUBLIC_I static void ShowSubtitle(const std::wstring message, const int duration, const bool shownow);

			/// <summary>
			/// Sends a subtitle message that displays on the bottom of the game to a specific client.
			/// </summary>
			/// <param name="entity">The entity id of the player you wish to send it too</param>
			/// <param name="message">The message of the subtitle</param>
			/// <param name="duration">The amount of time the subtitle should be shown (in milliseconds)</param>
			/// <param name="shownow">Whether the subtitle must be showed immediately or not.</param>
			/// <returns></returns>
			DLL_PUBLIC_I static void ShowSubtitleToPlayer(const int entity, const std::wstring message, const int duration, const bool shownow);
		};
	}
#ifdef __cplusplus
}
#endif