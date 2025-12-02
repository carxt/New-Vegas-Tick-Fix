#pragma once

struct PluginInfo;

//Massively stripped down version of the original, as I didn't care about 90% of its contents.


typedef uint32_t	PluginHandle;	// treat this as an opaque type

enum {
	kPluginHandle_Invalid = 0xFFFFFFFF,
};



struct NVSEInterface {

	enum {
		kInterface_Serialization = 0,
		kInterface_Console,

		// Added v0002
		kInterface_Messaging,
		kInterface_CommandTable,

		// Added v0004
		kInterface_StringVar,
		kInterface_ArrayVar,
		kInterface_Script,

		// Added v0005 - version bumped to 3
		kInterface_Data,

		kInterface_Max
	};

	uint32_t	nvseVersion;
	uint32_t	runtimeVersion;
	uint32_t	editorVersion;
	uint32_t	isEditor;
	bool	(*RegisterCommand)(void* info);	// returns true for success, false for failure
	void	(*SetOpcodeBase)(uint32_t opcode);
	void*	(*QueryInterface)(uint32_t id);

	// call during your Query or Load functions to get a PluginHandle uniquely identifying your plugin
	// invalid if called at any other time, so call it once and save the result
	PluginHandle(*GetPluginHandle)(void);

	// CommandReturnType enum defined in CommandTable.h
	// does the same as RegisterCommand but includes return type; *required* for commands returning arrays
	bool	(*RegisterTypedCommand)(void* info, uint32_t retnType);
	// returns a full path the the game directory
	const char* (*GetRuntimeDirectory)();

	// Allows checking for nogore edition
	uint32_t	isNogore;
};


struct PluginInfo {
	enum {
		kInfoVersion = 1
	};

	uint32_t			infoVersion;
	const char* name;
	uint32_t			version;
};

typedef bool (*_NVSEPlugin_Query)(const NVSEInterface* nvse, PluginInfo* info);
typedef bool (*_NVSEPlugin_Load)(const NVSEInterface* nvse);


struct NVSEMessagingInterface {
	struct Message {
		const char* sender;
		uint32_t		type;
		uint32_t		dataLen;
		void* data;
	};

	typedef void (*EventCallback)(Message* msg);

	enum {
		kVersion = 3
	};

	// NVSE messages
	enum {
		kMessage_PostLoad,				// sent to registered plugins once all plugins have been loaded (no data)

		kMessage_ExitGame,				// exit to windows from main menu or in-game menu

		kMessage_ExitToMainMenu,		// exit to main menu from in-game menu

		kMessage_LoadGame,				// Dispatched immediately before plugin serialization callbacks invoked, after savegame has been read by Fallout
		// dataLen: length of file path, data: char* file path of .fos savegame file
		// Receipt of this message does not *guarantee* the serialization callback will be invoked
		// as there may be no .nvse file associated with the savegame

		kMessage_SaveGame,				// as above

		kMessage_Precompile,			// EDITOR: Dispatched when the user attempts to save a script in the script editor.
		// NVSE first does its pre-compile checks; if these pass the message is dispatched before
		// the vanilla compiler does its own checks.
		// data: ScriptBuffer* to the buffer representing the script under compilation

		kMessage_PreLoadGame,			// dispatched immediately before savegame is read by Fallout
		// dataLen: length of file path, data: char* file path of .fos savegame file

		kMessage_ExitGame_Console,		// exit game using 'qqq' console command

		kMessage_PostLoadGame,			//dispatched after an attempt to load a saved game has finished (the game's LoadGame() routine
		//has returned). You will probably want to handle this event if your plugin uses a Preload callback
		//as there is a chance that after that callback is invoked the game will encounter an error
		//while loading the saved game (eg. corrupted save) which may require you to reset some of your
		//plugin state.
		//data: bool, true if game successfully loaded, false otherwise */

		kMessage_PostPostLoad,			// sent right after kMessage_PostLoad to facilitate the correct dispatching/registering of messages/listeners
		// plugins may register as listeners during the first callback while deferring dispatches until the next
		kMessage_RuntimeScriptError,	// dispatched when an NVSE script error is encountered during runtime/
		// data: char* errorMessageText
		// added for kVersion = 2
		kMessage_DeleteGame,			// sent right before deleting the .nvse cosave and the .fos save.
		// dataLen: length of file path, data: char* file path of .fos savegame file
		kMessage_RenameGame,			// sent right before renaming the .nvse cosave and the .fos save.
		// dataLen: length of old file path, data: char* old file path of .fos savegame file
		// you are expected to save the data and wait for kMessage_RenameNewGame
		kMessage_RenameNewGame,			// sent right after kMessage_RenameGame.
		// dataLen: length of new file path, data: char* new file path of .fos savegame file
		kMessage_NewGame,				// sent right before iterating through plugins newGame.
		// dataLen: 0, data: NULL
		// added for kVersion == 3
		kMessage_DeleteGameName,		// version of the messages sent with a save file name instead of a save file path.
		kMessage_RenameGameName,
		kMessage_RenameNewGameName,
		// added for kVersion == 4 (xNVSE)
		kMessage_DeferredInit,
		kMessage_ClearScriptDataCache,
		kMessage_MainGameLoop,			// called each game loop
		kMessage_ScriptCompile   // EDITOR: called after successful script compilation in GECK. data: pointer to Script
	};

	uint32_t	version;
	bool	(*RegisterListener)(PluginHandle listener, const char* sender, EventCallback handler);
	bool	(*Dispatch)(PluginHandle sender, uint32_t messageType, void* data, uint32_t dataLen, const char* receiver);
};