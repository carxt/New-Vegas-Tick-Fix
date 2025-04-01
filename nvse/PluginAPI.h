#pragma once

struct PluginInfo;

//Massively stripped down version of the original, as I didn't care about 90% of its contents.


typedef uint32_t	PluginHandle;	// treat this as an opaque type

enum {
	kPluginHandle_Invalid = 0xFFFFFFFF,
};



struct NVSEInterface {
	uint32_t	nvseVersion;
	uint32_t	runtimeVersion;
	uint32_t	editorVersion;
	uint32_t	isEditor;
	bool	(*RegisterCommand)(void* info);	// returns true for success, false for failure
	void	(*SetOpcodeBase)(uint32_t opcode);
	void* (*QueryInterface)(uint32_t id);

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
