#pragma once
#include "vector.h"
#include "Valve\dt_common.h"
#include "Valve\dt_recv.h"
#include "color.h"

#define FLOW_OUTGOING	0
#define FLOW_INCOMING	1
#define MAX_FLOWS		2		// in & out

struct matrix3x4 {
	float data[ 3 ][ 4 ];
};

enum class e_player_states : int {
	FL_ONGROUND = 1 << 0,
	FL_DUCKING = 1 << 1,
	FL_WATERJUMP = 1 << 2,
	FL_ONTRAIN = 1 << 3,
	FL_INRAIN = 1 << 4,
	FL_FROZEN = 1 << 5,
	FL_ATCONTROLS = 1 << 6,
	FL_CLIENT = 1 << 7,
	FL_FAKECLIENT = 1 << 8,
	FL_INWATER = 1 << 9,
};

enum class e_move_type : int {
	MOVETYPE_NONE = 0, // never moves
	MOVETYPE_ISOMETRIC, // For players -- in TF2 commander view, etc.
	MOVETYPE_WALK, // Player only - moving on the ground
	MOVETYPE_STEP, // gravity, special edge handling -- monsters use this
	MOVETYPE_FLY, // No gravity, but still collides with stuff
	MOVETYPE_FLYGRAVITY, // flies through the air + is affected by gravity
	MOVETYPE_VPHYSICS, // uses VPHYSICS for simulation
	MOVETYPE_PUSH, // no clip to world, push and crush
	MOVETYPE_NOCLIP, // No gravity, no collisions, still do velocity/avelocity
	MOVETYPE_LADDER, // Used by players only when going onto a ladder
	MOVETYPE_OBSERVER, // Observer movement, depends on player's observer mode
	MOVETYPE_CUSTOM, // Allows the entity to describe its own physics

					 // should always be defined as the last item in the list
	MOVETYPE_LAST = MOVETYPE_CUSTOM,
	MOVETYPE_MAX_BITS = 4,
};

enum e_net_channels {
	GENERIC = 0, // must be first and is default group
	LOCALPLAYER, // bytes for local player entity update
	OTHERPLAYERS, // bytes for other players update
	ENTITIES, // all other entity bytes
	SOUNDS, // game sounds
	EVENTS, // event messages
	TEMPENTS, // temp entities
	USERMESSAGES, // user messages
	ENTMESSAGES, // entity messages
	VOICE, // voice data
	STRINGTABLE, // a stringtable update
	MOVE, // client move cmds
	STRINGCMD, // string command
	SIGNON, // various signondata
	TOTAL, // must be last and is not a real group
};

class client_state_t {
public:
	char pad_0000[ 156 ]; //0x0000
	class INetChannel* m_netch; //0x0094
	char pad_0098[ 16 ]; //0x0098
	unsigned int m_retrynum; //0x00A8
	char pad_00AC[ 84 ]; //0x00AC
	int m_signon_state; //0x0100
	char pad_0104[ 8 ]; //0x0104
	float m_flNextCmdTime; //0x010C
	int m_nServerCount; //0x0110
	int m_nCurrentSequence; //0x0114
	char pad_0118[ 75 ]; //0x0118
	int m_server_tick; //0x0163
	int m_client_tick; //0x0167
	int m_delta_tick; //0x016B
	char pad_016F[ 4 ]; //0x016F
	int m_view_entity; //0x0173
	char pad_0177[ 8 ]; //0x0177
	char m_level_name[ 260 ]; //0x017F
	char m_level_name_short[ 40 ]; //0x0283
	char pad_02AB[ 18940 ]; //0x02AB
	char pad_0x2DA0; //0x4CA7
	int m_lastoutgoingcommand; //0x4CA8
	int m_chokedcommands; //0x4CAC
	int m_last_acknowledged_cmd; //0x4CB0
	int m_command_ack; //0x4CB4
	int m_sound_seq; //0x4CB8
	char pad_4CBC[ 8 ]; //0x4CBC
	bool m_ishltv; //0x4CC4
};

struct netframe_t {
	// Data received from server
	float			time;			// net_time received/send
	int				size;			// total size in bytes
	float			latency;		// raw ping for this packet, not cleaned. set when acknowledged otherwise -1.
	float			avg_latency;	// averaged ping for this packet
	bool			valid;			// false if dropped, lost, flushed
	int				choked;			// number of previously chocked packets
	int				dropped;
	float			m_flInterpolationAmount;
	unsigned short	msggroups[ e_net_channels::TOTAL ];	// received bytes for each message group
};

struct netflow_t {
	float		nextcompute;	// Time when we should recompute k/sec data
	float		avgbytespersec;	// average bytes/sec
	float		avgpacketspersec;// average packets/sec
	float		avgloss;		// average packet loss [0..1]
	float		avgchoke;		// average packet choke [0..1]
	float		avglatency;		// average ping, not cleaned
	float		latency;		// current ping, more accurate also more jittering
	int			totalpackets;	// total processed packets
	int			totalbytes;		// total processed bytes
	int			currentindex;		// current frame index
	netframe_t	frames[ 64 ]; // frame history
	netframe_t	*currentframe;	// current frame
};

class user_cmd_t {
public:
	virtual ~user_cmd_t( ) { };

	int command_number;
	int tick_count;
	vec3_t viewangles;
	vec3_t aimdirection;
	float forwardmove;
	float sidemove;
	float upmove;
	int buttons;
	uint8_t impulse;
	int weaponselect;
	int weaponsubtype;
	int random_seed;
	short mousedx;
	short mousedy;
	bool hasbeenpredicted; 
	char pad_0x4C[ 0x18 ];
};

class globalvars_t {
public:
	float m_realtime;
	int m_framecount;
	float m_absframetime;
	float m_absoluteframestarttimestddev;
	float m_curtime;
	float m_frametime;
	int m_maxclients;
	int m_tickcount;
	float m_interval_per_tick;
	float m_interpolation_amount;
	int m_sim_ticks;
	int network_protocol;
	char pad00[ 14 ];
};

struct verified_user_cmd_t {
	user_cmd_t	m_cmd;
	uint32_t	m_crc;
};

class input_t {
private:
	void* pvftable;
public:
	bool m_pad_something;
	bool m_mouse_initialized;
	bool m_mouse_active;
	bool pad_something01;
	char pad_0x08[ 0x2C ];
	void* m_keys;
	char pad_0x38[ 0x64 ];
	__int32 pad_0x41;
	__int32 pad_0x42;
	bool m_camera_intercepting_mouse;
	bool m_cam_in_3rd_person;
	bool m_cam_moving_with_mouse;
	vec3_t m_camera_offset;
	bool m_cam_distance_move;
	int m_cam_old_x;
	int m_cam_old_y;
	int m_cam_x;
	int m_cam_y;
	bool m_camera_is_orthographic;
	vec3_t m_previous_viewangles;
	vec3_t m_previous_viewangles_tilt;
	float m_last_forward_move;
	int m_clear_input_state;
	char pad_0xE4[ 0x8 ];
	user_cmd_t* m_commands;
	void* m_verified_commands;
};

class c_client_class {
public:
	void* m_create_fn;
	void* m_create_event_fn;
	const char* m_network_name;
	RecvTable* m_recv;
	c_client_class* m_next;
	int m_index;
};

struct glow_object_t {
	uintptr_t			m_ent;				//0000
	fclr_t				m_color;
	uint8_t				pad_01[ 8 ];
	float				m_bloom_amt;
	uint8_t				pad_02[ 4 ];
	bool				m_rwo;	//0024
	bool				m_rwuo;	//0025
	bool				m_full_bloom;
	uint8_t				pad_03[ 14 ];
};

struct glow_obj_manager_t {
	glow_object_t*	m_objects;
	uint32_t		m_max;
	uint32_t		unk02;
	uint32_t		m_count;
	uintptr_t		m_data_ptr_back;
	int				m_first_free_slot;
	uint32_t		unk1;
	uint32_t		unk2;
	uint32_t		unk3;
	uint32_t		unk4;
	uint32_t		unk5;
};

enum e_class_id {
	CAI_BaseNPC,
	CAK47,
	CBaseAnimating,
	CBaseAnimatingOverlay,
	CBaseAttributableItem,
	CBaseButton,
	CBaseCombatCharacter,
	CBaseCombatWeapon,
	CBaseCSGrenade,
	CBaseCSGrenadeProjectile,
	CBaseDoor,
	CBaseEntity_,
	CBaseFlex,
	CBaseGrenade,
	CBaseParticleEntity,
	CBasePlayer,
	CBasePropDoor,
	CBaseTeamObjectiveResource,
	CBaseTempEntity,
	CBaseToggle,
	CBaseTrigger,
	CBaseViewModel,
	CBaseVPhysicsTrigger,
	CBaseWeaponWorldModel,
	CBeam,
	CBeamSpotlight,
	CBoneFollower,
	CBreakableProp,
	CBreakableSurface,
	CC4,
	CCascadeLight,
	CChicken,
	CColorCorrection,
	CColorCorrectionVolume,
	CCSGameRulesProxy,
	CCSPlayer,
	CCSPlayerResource,
	CCSRagdoll,
	CCSTeam,
	CDEagle,
	CDecoyGrenade,
	CDecoyProjectile,
	CDynamicLight,
	CDynamicProp,
	CEconEntity,
	CEconWearable,
	CEmbers,
	CEntityDissolve,
	CEntityFlame,
	CEntityFreezing,
	CEntityParticleTrail,
	CEnvAmbientLight,
	CEnvDetailController,
	CEnvDOFController,
	CEnvParticleScript,
	CEnvProjectedTexture,
	CEnvQuadraticBeam,
	CEnvScreenEffect,
	CEnvScreenOverlay,
	CEnvTonemapController,
	CEnvWind,
	CFEPlayerDecal,
	CFireCrackerBlast,
	CFireSmoke,
	CFireTrail,
	CFish,
	CFlashbang,
	CFogController,
	CFootstepControl,
	CFunc_Dust,
	CFunc_LOD,
	CFuncAreaPortalWindow,
	CFuncBrush,
	CFuncConveyor,
	CFuncLadder,
	CFuncMonitor,
	CFuncMoveLinear,
	CFuncOccluder,
	CFuncReflectiveGlass,
	CFuncRotating,
	CFuncSmokeVolume,
	CFuncTrackTrain,
	CGameRulesProxy,
	CHandleTest,
	CHEGrenade,
	CHostage,
	CHostageCarriableProp,
	CIncendiaryGrenade,
	CInferno,
	CInfoLadderDismount,
	CInfoOverlayAccessor,
	CItem_Healthshot,
	CItemDogtags,
	CKnife,
	CKnifeGG,
	CLightGlow,
	CMaterialModifyControl,
	CMolotovGrenade,
	CMolotovProjectile,
	CMovieDisplay,
	CParticleFire,
	CParticlePerformanceMonitor,
	CParticleSystem,
	CPhysBox,
	CPhysBoxMultiplayer,
	CPhysicsProp,
	CPhysicsPropMultiplayer,
	CPhysMagnet,
	CPlantedC4,
	CPlasma,
	CPlayerResource,
	CPointCamera,
	CPointCommentaryNode,
	CPointWorldText,
	CPoseController,
	CPostProcessController,
	CPrecipitation,
	CPrecipitationBlocker,
	CPredictedViewModel,
	CProp_Hallucination,
	CPropDoorRotating,
	CPropJeep,
	CPropVehicleDriveable,
	CRagdollManager,
	CRagdollProp,
	CRagdollPropAttached,
	CRopeKeyframe,
	CSCAR17,
	CSceneEntity,
	CSensorGrenade,
	CSensorGrenadeProjectile,
	CShadowControl,
	CSlideshowDisplay,
	CSmokeGrenade,
	CSmokeGrenadeProjectile,
	CSmokeStack,
	CSpatialEntity,
	CSpotlightEnd,
	CSprite,
	CSpriteOriented,
	CSpriteTrail,
	CStatueProp,
	CSteamJet,
	CSun,
	CSunlightShadowControl,
	CTeam,
	CTeamplayRoundBasedRulesProxy,
	CTEArmorRicochet,
	CTEBaseBeam,
	CTEBeamEntPoint,
	CTEBeamEnts,
	CTEBeamFollow,
	CTEBeamLaser,
	CTEBeamPoints,
	CTEBeamRing,
	CTEBeamRingPoint,
	CTEBeamSpline,
	CTEBloodSprite,
	CTEBloodStream,
	CTEBreakModel,
	CTEBSPDecal,
	CTEBubbles,
	CTEBubbleTrail,
	CTEClientProjectile,
	CTEDecal,
	CTEDust,
	CTEDynamicLight,
	CTEEffectDispatch,
	CTEEnergySplash,
	CTEExplosion,
	CTEFireBullets,
	CTEFizz,
	CTEFootprintDecal,
	CTEFoundryHelpers,
	CTEGaussExplosion,
	CTEGlowSprite,
	CTEImpact,
	CTEKillPlayerAttachments,
	CTELargeFunnel,
	CTEMetalSparks,
	CTEMuzzleFlash,
	CTEParticleSystem,
	CTEPhysicsProp,
	CTEPlantBomb,
	CTEPlayerAnimEvent,
	CTEPlayerDecal,
	CTEProjectedDecal,
	CTERadioIcon,
	CTEShatterSurface,
	CTEShowLine,
	CTesla,
	CTESmoke,
	CTESparks,
	CTESprite,
	CTESpriteSpray,
	CTest_ProxyToggle_Networkable,
	CTestTraceline,
	CTEWorldDecal,
	CTriggerPlayerMovement,
	CTriggerSoundOperator,
	CVGuiScreen,
	CVoteController,
	CWaterBullet,
	CWaterLODControl,
	CWeaponAug,
	CWeaponAWP,
	CWeaponBaseItem,
	CWeaponBizon,
	CWeaponCSBase,
	CWeaponCSBaseGun,
	CWeaponCycler,
	CWeaponElite,
	CWeaponFamas,
	CWeaponFiveSeven,
	CWeaponG3SG1,
	CWeaponGalil,
	CWeaponGalilAR,
	CWeaponGlock,
	CWeaponHKP2000,
	CWeaponM249,
	CWeaponM3,
	CWeaponM4A1,
	CWeaponMAC10,
	CWeaponMag7,
	CWeaponMP5Navy,
	CWeaponMP7,
	CWeaponMP9,
	CWeaponNegev,
	CWeaponNOVA,
	CWeaponP228,
	CWeaponP250,
	CWeaponP90,
	CWeaponSawedoff,
	CWeaponSCAR20,
	CWeaponScout,
	CWeaponSG550,
	CWeaponSG552,
	CWeaponSG556,
	CWeaponSSG08,
	CWeaponTaser,
	CWeaponTec9,
	CWeaponTMP,
	CWeaponUMP45,
	CWeaponUSP,
	CWeaponXM1014,
	CWorld,
	DustTrail,
	MovieExplosion,
	ParticleSmokeGrenade,
	RocketTrail,
	SmokeTrail,
	SporeExplosion,
	SporeTrail,
};

enum e_item_definition_index {
	WEAPON_NONE,
	WEAPON_DEAGLE = 1,
	WEAPON_ELITE = 2,
	WEAPON_FIVESEVEN = 3,
	WEAPON_GLOCK = 4,
	WEAPON_AK47 = 7,
	WEAPON_AUG = 8,
	WEAPON_AWP = 9,
	WEAPON_FAMAS = 10,
	WEAPON_G3SG1 = 11,
	WEAPON_GALILAR = 13,
	WEAPON_M249 = 14,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10 = 17,
	WEAPON_P90 = 19,
	WEAPON_UMP45 = 24,
	WEAPON_XM1014 = 25,
	WEAPON_BIZON = 26,
	WEAPON_MAG7 = 27,
	WEAPON_NEGEV = 28,
	WEAPON_SAWEDOFF = 29,
	WEAPON_TEC9 = 30,
	WEAPON_TASER = 31,
	WEAPON_HKP2000 = 32,
	WEAPON_MP7 = 33,
	WEAPON_MP9 = 34,
	WEAPON_NOVA = 35,
	WEAPON_P250 = 36,
	WEAPON_SCAR20 = 38,
	WEAPON_SG556 = 39,
	WEAPON_SSG08 = 40,
	WEAPON_KNIFEGG = 41,
	WEAPON_KNIFE = 42,
	WEAPON_FLASHBANG = 43,
	WEAPON_HEGRENADE = 44,
	WEAPON_SMOKEGRENADE = 45,
	WEAPON_MOLOTOV = 46,
	WEAPON_DECOY = 47,
	WEAPON_INCGRENADE = 48,
	WEAPON_C4 = 49,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER = 60,
	WEAPON_USP_SILENCER = 61,
	WEAPON_CZ75A = 63,
	WEAPON_R8REVOLVER = 64,
	WEAPON_KNIFE_BAYONET = 500,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT = 506,
	WEAPON_KNIFE_KARAMBIT = 507,
	WEAPON_KNIFE_M9_BAYONET = 508,
	WEAPON_KNIFE_TACTICAL = 509,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY = 515,
	WEAPON_KNIFE_PUSH = 516
};
