#pragma once

struct PluginInfo;

//Massively stripped down version of the original, as I didn't care about 90% of its contents.


typedef UInt32	PluginHandle;	// treat this as an opaque type

enum {
	kPluginHandle_Invalid = 0xFFFFFFFF,
};



struct NVSEInterface {
	UInt32	nvseVersion;
	UInt32	runtimeVersion;
	UInt32	editorVersion;
	UInt32	isEditor;
	bool	(*RegisterCommand)(void* info);	// returns true for success, false for failure
	void	(*SetOpcodeBase)(UInt32 opcode);
	void* (*QueryInterface)(UInt32 id);

	// call during your Query or Load functions to get a PluginHandle uniquely identifying your plugin
	// invalid if called at any other time, so call it once and save the result
	PluginHandle(*GetPluginHandle)(void);

	// CommandReturnType enum defined in CommandTable.h
	// does the same as RegisterCommand but includes return type; *required* for commands returning arrays
	bool	(*RegisterTypedCommand)(void* info, UInt32 retnType);
	// returns a full path the the game directory
	const char* (*GetRuntimeDirectory)();

	// Allows checking for nogore edition
	UInt32	isNogore;
};


struct PluginInfo {
	enum {
		kInfoVersion = 1
	};

	UInt32			infoVersion;
	const char* name;
	UInt32			version;
};

typedef bool (*_NVSEPlugin_Query)(const NVSEInterface* nvse, PluginInfo* info);
typedef bool (*_NVSEPlugin_Load)(const NVSEInterface* nvse);
