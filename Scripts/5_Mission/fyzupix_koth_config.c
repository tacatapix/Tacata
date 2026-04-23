/*
PackFazupix KOTH by Touchofdeath Version 1.0
*/

static ref FYZUPIXK_Config g_FYZUPIXKConfig;

ref FYZUPIXK_Config GetFYZUPIXKConfig()
{
	if(!g_FYZUPIXKConfig)
	{
		g_FYZUPIXKConfig = g_FYZUPIXKConfig.LoadConfig();
	}
	
	return g_FYZUPIXKConfig;
};

class FYZUPIXK_Config
{
	[NonSerialized()]
	private static const string configRoot = "$profile:/PackFazupix/KOTH/";
	
	[NonSerialized()]
	private static const string configPathSettings = "$profile:/PackFazupix/KOTH/KOTHConfig.json";

	ref array<ref FYZUPIXK_SettingsConfig> Settings;
	ref array<ref FYZUPIXK_KOTHConfig> KOTHs;
	ref array<ref FYZUPIXK_Reward_Containers> Reward_Containers;
	ref array<ref FYZUPIXK_LootConfig> Loot;
	ref array<ref FYZUPIXK_LootTier> LootTiers;
	
	void FYZUPIXK_Config()
	{
		Settings = new ref array<ref FYZUPIXK_SettingsConfig>();
		KOTHs = new ref array<ref FYZUPIXK_KOTHConfig>();
		Reward_Containers = new ref array<ref FYZUPIXK_Reward_Containers>();
		Loot = new ref array<ref FYZUPIXK_LootConfig>();
		LootTiers = new ref array<ref FYZUPIXK_LootTier>();
	}
	
	void ~FYZUPIXK_Config()
	{
		if(this.Settings) this.Settings.Clear();
		this.Settings = NULL;
		if(this.KOTHs) this.KOTHs.Clear();
		this.KOTHs = NULL;
		if(this.Reward_Containers) this.Reward_Containers.Clear();
		this.Reward_Containers = NULL;
		if(this.Loot) this.Loot.Clear();
		this.Loot = NULL;
		if(this.LootTiers) this.LootTiers.Clear();
		this.LootTiers = NULL;
	}
	
	protected void SaveConfigs()
	{
		if(!FileExist(configRoot))
		{
			MakeDirectory(configRoot);
		}
		
		JsonFileLoader<FYZUPIXK_Config>.JsonSaveFile(configPathSettings, this);
	};

	void LoadDefaultSettings()
	{
		Settings.Insert(new FYZUPIXK_SettingsConfig());

		KOTHs.Insert(new FYZUPIXK_KOTHConfig("Zelenogorsk Military", "2562.7 193.175 5117.338", "0.0 0.0 0.0", 40, 15, 6));
		KOTHs.Insert(new FYZUPIXK_KOTHConfig("Elektro Police Station", "10461.145 5.957 2258.447", "0.0 0.0 0.0", 40, 15, 6, {"fyzupix_KOTHchest_SupplyBox1"}));
		KOTHs.Insert(new FYZUPIXK_KOTHConfig("Downed Helicopter", "144.312469 319.558411 8073.837402", "0.0 0.0 0.0", 40, 15, 6, {"fyzupix_KOTHchest_SupplyBox3"}));
		KOTHs.Get(KOTHs.Count() - 1).AddObject("StaticObj_Wreck_Mi8_Crashed", "144.063995 321.147003 8068.359863", "-69.119629 0.000000 -0.000000");
		
		Reward_Containers.Insert(new FYZUPIXK_Reward_Containers("fyzupix_KOTHchest_SupplyBox1", false));
		Reward_Containers.Insert(new FYZUPIXK_Reward_Containers("fyzupix_KOTHchest_SupplyBox2", false));
		Reward_Containers.Insert(new FYZUPIXK_Reward_Containers("fyzupix_KOTHchest_SupplyBox3", false));
		
		//Default tiers: weighted roll picks a tier per reward slot.
		//Example weights (sum doesn't have to be 100 - they're relative):
		//  T1=60 trash, T2=25 common, T3=10 rare, T4=5 legendary.
		LootTiers.Insert(new FYZUPIXK_LootTier(1, 60, "Trash"));
		LootTiers.Insert(new FYZUPIXK_LootTier(2, 25, "Common"));
		LootTiers.Insert(new FYZUPIXK_LootTier(3, 10, "Rare"));
		LootTiers.Insert(new FYZUPIXK_LootTier(4, 5,  "Legendary"));

		//Loot items. Last arg is Tier (1..N). If you leave it out it defaults to 1.
		Loot.Insert(new FYZUPIXK_LootConfig("FAL", 100, 2, -1, "Mag_FAL_20Rnd", {"Fal_OeBttsck","ACOGOptic"},{"Mag_FAL_20Rnd","Mag_FAL_20Rnd"}, NULL, 4));
		Loot.Insert(new FYZUPIXK_LootConfig("SVD", 100, 2, -1, "Mag_SVD_10Rnd", {"PSO1Optic"},{"Mag_SVD_10Rnd","Mag_SVD_10Rnd"}, NULL, 4));
		Loot.Insert(new FYZUPIXK_LootConfig("PlateCarrierVest", 100, 2, -1, "", NULL, NULL, {"PlateCarrierVest_Black","PlateCarrierVest_Camo"}, 3));
		Loot.Insert(new FYZUPIXK_LootConfig("BallisticHelmet_Green", 100, 2, -1, "", NULL, NULL, {"BallisticHelmet_Black","BallisticHelmet_Woodland"}, 3));
		Loot.Insert(new FYZUPIXK_LootConfig("BandageDressing", 100, 2, -1, "", NULL, NULL, NULL, 1));
		Loot.Insert(new FYZUPIXK_LootConfig("PainkillerTablets", 100, 2, -1, "", NULL, NULL, NULL, 1));
		Loot.Insert(new FYZUPIXK_LootConfig("Morphine", 100, 2, -1, "", NULL, NULL, NULL, 2));
		Loot.Insert(new FYZUPIXK_LootConfig("SledgeHammer", 100, 2, -1, "", NULL, NULL, NULL, 2));
		Loot.Insert(new FYZUPIXK_LootConfig("NailBox", 100, 2, -1, "", NULL, NULL, NULL, 1));
		Loot.Insert(new FYZUPIXK_LootConfig("Hatchet", 100, 2, -1, "", NULL, NULL, NULL, 1));
		Loot.Insert(new FYZUPIXK_LootConfig("MetalPlate", 100, 2, -1, "", NULL, NULL, NULL, 2));
		
		SaveConfigs();
	}
	
	static ref FYZUPIXK_Config LoadConfig() 
	{
		ref FYZUPIXK_Config settings = new FYZUPIXK_Config();
		
		if(!FileExist(configRoot))
		{
			MakeDirectory(configRoot);

			settings.LoadDefaultSettings();
		}
		else if(FileExist(configPathSettings))
		{
			JsonFileLoader<FYZUPIXK_Config>.JsonLoadFile(configPathSettings, settings);
			
			/*if(settings.Settings.Get(0).Config_Version == 0)
			{
				//Future config updates will go here
				
				//settings.SaveConfigs();
			}*/
		}
		else
		{
			settings.LoadDefaultSettings();
		}
		
		return settings;
	}
}

class FYZUPIXK_SettingsConfig
{
	float Config_Version;
	int Minimum_Players;
	int Cleanup_Radius;
	int Cleanup_Time_After_KOTH_Finished;
	bool KOTH_Reward_Delete_On_Cleanup;
	int KOTH_Players_In_Radius_Check_Range;
	int KOTH_Max_Simultaneous;
	int KOTH_Simultaneous_Stagger_Time_Min;
	int KOTH_Simultaneous_Stagger_Time_Max;
	int KOTH_Start_Delay;
	int KOTH_Starting_Failed_Retry_Delay;
	int KOTH_Time_Between;
	int KOTH_Time_Limit;
	int KOTH_Capture_Time;
	int KOTH_Spawn_Start_Delay;
	float KOTH_Capture_Speed_Bonus_PerPlayer_PerSec;
	float KOTH_Capture_Speed_Bonus_PerPlayer_PerSec_Max;
	bool KOTH_Flag_Lower_If_Abandoned;
	bool KOTH_Do_Smoke;
	bool KOTH_Do_Fireworks_On_Win;
	bool Map_Marker_Enabled;
	int LB_Marker_Radius;
	bool Perspective_Marker_Enabled;
	bool Notification_Enabled;
	bool Logging_Enabled;
	bool Debug_QuickMode;
	ref TStringArray Flags;
	ref array<ref KOTH_AI> KOTH_Enemies;
	
	void FYZUPIXK_SettingsConfig()
	{
		Config_Version = 1.0;
		Minimum_Players = 0;
		Cleanup_Radius = 5;
		Cleanup_Time_After_KOTH_Finished = 300;
		KOTH_Reward_Delete_On_Cleanup = true;
		KOTH_Players_In_Radius_Check_Range = 200;
		KOTH_Max_Simultaneous = 1;
		KOTH_Simultaneous_Stagger_Time_Min = 1200;
		KOTH_Simultaneous_Stagger_Time_Max = 1800;
		KOTH_Start_Delay = 600;
		KOTH_Starting_Failed_Retry_Delay = 120;
		KOTH_Time_Between = 1800;
		KOTH_Time_Limit = 1200;
		KOTH_Capture_Time = 900;
		KOTH_Spawn_Start_Delay = 120;
		KOTH_Capture_Speed_Bonus_PerPlayer_PerSec = 0.25;
		KOTH_Capture_Speed_Bonus_PerPlayer_PerSec_Max = 2.0;
		KOTH_Flag_Lower_If_Abandoned = true;
		KOTH_Do_Smoke = true;
		KOTH_Do_Fireworks_On_Win = true;
		Map_Marker_Enabled = true;
		LB_Marker_Radius = 150;
		Perspective_Marker_Enabled = false;
		Notification_Enabled = true;
		Logging_Enabled = true;
		Debug_QuickMode = false;
		Flags = new TStringArray;
		Flags.Insert("Flag_Bohemia");
		Flags.Insert("Flag_Chernarus");
		Flags.Insert("Flag_Pirates");
		KOTH_Enemies = new ref array<ref KOTH_AI>();
		KOTH_Enemies.Insert(new KOTH_AI("ZmbF_CitizenANormal_Beige", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbF_CitizenANormal_Blue", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbF_CitizenANormal_Brown", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbF_CitizenBSkinny", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbF_Clerk_Normal_Blue", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbF_Clerk_Normal_Green", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbF_Clerk_Normal_Red", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbF_Clerk_Normal_White", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbF_DoctorSkinny", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbF_JoggerSkinny_Blue", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbF_JoggerSkinny_Brown", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbF_JoggerSkinny_Green", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbF_JoggerSkinny_Red", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbF_JournalistNormal_Blue", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbF_JournalistNormal_Green", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbF_JournalistNormal_Red", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbF_JournalistNormal_White", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbF_NurseFat", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbF_ParamedicNormal_Blue", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbF_ParamedicNormal_Green", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbF_ParamedicNormal_Red", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbF_PatientOld", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbF_PoliceWomanNormal", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbF_ShortSkirt_beige", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbF_ShortSkirt_black", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbF_ShortSkirt_brown", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbF_ShortSkirt_checks", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbF_ShortSkirt_green", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbF_ShortSkirt_grey", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbF_ShortSkirt_red", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbF_ShortSkirt_stripes", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbF_ShortSkirt_white", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbF_ShortSkirt_yellow", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbF_SkaterYoung_Brown", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbF_SkaterYoung_Striped", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbF_SkaterYoung_Violet", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbM_CitizenASkinny_Blue", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbM_CitizenASkinny_Brown", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbM_CitizenASkinny_Grey", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbM_CitizenASkinny_Red", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbM_CitizenBFat_Blue", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbM_CitizenBFat_Green", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbM_CitizenBFat_Red", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbM_ClerkFat_Brown", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbM_ClerkFat_Grey", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbM_ClerkFat_Khaki", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbM_ClerkFat_White", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbM_CommercialPilotOld_Blue", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbM_CommercialPilotOld_Brown", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbM_CommercialPilotOld_Grey", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbM_CommercialPilotOld_Olive", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbM_DoctorFat", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbM_JacketNormal_greenchecks", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbM_Jacket_beige", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbM_Jacket_black", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbM_Jacket_blue", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbM_Jacket_bluechecks", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbM_Jacket_brown", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbM_Jacket_greenchecks", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbM_Jacket_grey", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbM_Jacket_khaki", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbM_Jacket_magenta", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbM_Jacket_stripes", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbM_JoggerSkinny_Blue", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbM_JoggerSkinny_Green", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbM_JoggerSkinny_Red", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbM_JournalistSkinny", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbM_ParamedicNormal_Black", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbM_ParamedicNormal_Blue", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbM_ParamedicNormal_Green", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbM_ParamedicNormal_Red", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbM_PatientSkinny", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbM_PatrolNormal_Autumn", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbM_PatrolNormal_Flat", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbM_PatrolNormal_PautRev", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbM_PatrolNormal_Summer", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbM_TerritoryNormal", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbM_PolicemanFat", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbM_PolicemanSpecForce", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbM_PrisonerSkinny", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbM_SkaterYoung_Blue", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbM_SkaterYoung_Brown", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbM_SkaterYoung_Green", 0));
		KOTH_Enemies.Insert(new KOTH_AI("ZmbM_SkaterYoung_Grey", 0));
	}

	void ~FYZUPIXK_SettingsConfig()
	{
		this.Config_Version = 0;
		this.Minimum_Players = 0;
		this.Cleanup_Radius = 0;
		this.Cleanup_Time_After_KOTH_Finished = 0;
		this.KOTH_Players_In_Radius_Check_Range = 0;
		this.KOTH_Reward_Delete_On_Cleanup = false;
		this.KOTH_Max_Simultaneous = 0;
		this.KOTH_Simultaneous_Stagger_Time_Min = 0;
		this.KOTH_Simultaneous_Stagger_Time_Max = 0;
		this.KOTH_Start_Delay = 0;
		this.KOTH_Starting_Failed_Retry_Delay = 0;
		this.KOTH_Time_Between = 0;
		this.KOTH_Time_Limit = 0;
		this.KOTH_Capture_Time = 0;
		this.KOTH_Spawn_Start_Delay = 0;
		this.KOTH_Capture_Speed_Bonus_PerPlayer_PerSec = 0;
		this.KOTH_Capture_Speed_Bonus_PerPlayer_PerSec_Max = 0;
		this.KOTH_Flag_Lower_If_Abandoned = false;
		this.KOTH_Do_Smoke = false;
		this.KOTH_Do_Fireworks_On_Win = false;
		this.Map_Marker_Enabled = false;
		this.LB_Marker_Radius = 0;
		this.Perspective_Marker_Enabled = false;
		this.Notification_Enabled = false;
		this.Logging_Enabled = false;
		this.Debug_QuickMode = false;
		if(this.Flags) this.Flags.Clear();
		this.Flags = NULL;
		if(this.KOTH_Enemies) this.KOTH_Enemies.Clear();
		this.KOTH_Enemies = NULL;
	}
	
	int GetMinPlayers()
	{
		return Minimum_Players;
	}

	int GetCleanupRadius()
	{
		return Cleanup_Radius;
	}

	int GetKOTHFinishedCleanupTime()
	{
		return Cleanup_Time_After_KOTH_Finished;
	}
	
	int GetNoSpawnRange()
	{
		return KOTH_Players_In_Radius_Check_Range;
	}
	
	bool CleanupDeleteCrate()
	{
		return KOTH_Reward_Delete_On_Cleanup;
	}
	
	int GetMaxSimultaneous()
	{
		return KOTH_Max_Simultaneous;
	}
	
	int GetRandomStaggerTime()
	{
		return Math.RandomIntInclusive(KOTH_Simultaneous_Stagger_Time_Min, KOTH_Simultaneous_Stagger_Time_Max);
	}

	int GetStartDelay()
	{
		return KOTH_Start_Delay;
	}

	int GetFailStartDelay()
	{
		return KOTH_Starting_Failed_Retry_Delay;
	}

	int GetTimeBetweenKOTHs()
	{
		return KOTH_Time_Between;
	}

	int GetTimeLimit()
	{
		return KOTH_Time_Limit;
	}

	int GetCaptureTime()
	{
		return KOTH_Capture_Time;
	}
	
	int GetSpawnStartDelay()
	{
		return KOTH_Spawn_Start_Delay;
	}
	
	float GetExtraPlayersCaptureBonusPerSec()
	{
		return KOTH_Capture_Speed_Bonus_PerPlayer_PerSec;
	}
	
	float GetExtraPlayersCaptureBonusPerSecMax()
	{
		return KOTH_Capture_Speed_Bonus_PerPlayer_PerSec_Max;
	}
	
	bool GetLowerIfAbandoned()
	{
		return KOTH_Flag_Lower_If_Abandoned;
	}

	bool GetDoSmoke()
	{
		return KOTH_Do_Smoke;
	}

	bool GetDoFireworksOnWin()
	{
		return KOTH_Do_Fireworks_On_Win;
	}

	bool MapMarkerEnabled()
	{
		return Map_Marker_Enabled;
	}
	
	int GetMarkerRadius()
	{
		return LB_Marker_Radius;
	}
	
	bool Is3DMarkerEnabled()
	{
		return Perspective_Marker_Enabled;
	}
	
	bool NotificationsEnabled()
	{
		return Notification_Enabled;
	}

	bool LoggingEnabled()
	{
		return Logging_Enabled;
	}
	
	bool DebugQuickmode()
	{
		return Debug_QuickMode;
	}
	
	string GetRandomFlag()
	{
		if(Flags.Count() > 0) return Flags.GetRandomElement();
		return "Flag_Chernarus";
	}
}

class KOTH_AI
{
	string ClassName;
	int SpawnRarity;
	
	void KOTH_AI(string AIClass, int SpawnRarity)
	{
		ClassName = AIClass;
		SpawnRarity = SpawnRarity;
	}
	
	void ~KOTH_AI()
	{
		ClassName = "";
		SpawnRarity = 0;
	}
	
	string GetClassname()
	{
		return ClassName;
	}
	
	int GetRarity()
	{
		return SpawnRarity;
	}
}

class FYZUPIXK_KOTHConfig
{
	string Name;
    vector Reward_Position;
    vector Reward_Orientation;
	int Capture_Radius;
	int Enemies_Count;
	int Reward_Loot_Count;
	
	ref TStringArray Reward_Container_Classnames;
	ref array<ref FYZUPIXK_KOTHObjects> Static_Objects;
	
	void FYZUPIXK_KOTHConfig(string mName, vector mPos, vector mRot, int CapRadius, int EnemyCount, int LootCount, TStringArray ContainerClasses = NULL)
	{
		Name = mName;
		Reward_Position = mPos;
		Reward_Orientation = mRot;
		Capture_Radius = CapRadius;
		Enemies_Count = EnemyCount;
		Reward_Loot_Count = LootCount;
		
		if(ContainerClasses != NULL)
		{
			Reward_Container_Classnames = ContainerClasses;
		}
		else
		{
			Reward_Container_Classnames = new TStringArray;
		}

		Static_Objects = new ref array<ref FYZUPIXK_KOTHObjects>();
	}
	
	void ~FYZUPIXK_KOTHConfig()
	{
		this.Name = "";
		this.Reward_Position = "0 0 0";
		this.Reward_Orientation = "0 0 0";
		this.Capture_Radius = 0;
		this.Enemies_Count = 0;
		this.Reward_Loot_Count = 0;
		
		if(this.Reward_Container_Classnames) this.Reward_Container_Classnames.Clear();
		this.Reward_Container_Classnames = NULL;
		if(this.Static_Objects) this.Static_Objects.Clear();
		this.Static_Objects = NULL;
	}
	
	void AddObject(string oClass, vector oPos, vector oRot, float oScale = 1.0)
	{
		Static_Objects.Insert(new FYZUPIXK_KOTHObjects(oClass, oPos, oRot, oScale));
	}

	string GetName()
	{
		return Name;
	}
	
	vector GetPosition()
	{
		return Reward_Position;
	}
	
	vector GetOrientation()
	{
		return Reward_Orientation;
	}
	
	int GetCaptureRadius()
	{
		return Capture_Radius;
	}
	
	int GetEnemyCount()
	{
		return Enemies_Count;
	}
	
	int GetRewardLootCount()
	{
		return Reward_Loot_Count;
	}
	
	TStringArray GetContainerClassnames()
	{
		return Reward_Container_Classnames;
	}
}

class FYZUPIXK_KOTHObjects
{
	string ClassName;
	float Scale;
	vector Position;
	vector Rotation;
	
	void FYZUPIXK_KOTHObjects(string oClass, vector oPos, vector oRot, float oScale)
	{
		ClassName = oClass;
		Scale = oScale;
		Position = oPos;
		Rotation = oRot;
	}
	
	void ~FYZUPIXK_KOTHObjects()
	{
		this.ClassName = "";
		this.Scale = 0;
		this.Position = "0 0 0";
		this.Rotation = "0 0 0";
	}
	
	string GetClassname()
	{
		return ClassName;
	}
	
	float GetScale()
	{
		return Scale;
	}
	
	vector GetPosition()
	{
		return Position;
	}
	
	vector GetOrientation()
	{
		return Rotation;
	}
}

class FYZUPIXK_Reward_Containers
{
	string ClassName;
	bool Is_Scripted_Reward;

	void FYZUPIXK_Reward_Containers(string rClass, bool scriptedReward)
	{
		ClassName = rClass;
		Is_Scripted_Reward = scriptedReward;
	}
	
	void ~FYZUPIXK_Reward_Containers()
	{
		this.ClassName = "";
		this.Is_Scripted_Reward = false;
	}
	
	string GetClassname()
	{
		return ClassName;
	}
	
	bool IsScriptedReward()
	{
		return Is_Scripted_Reward;
	}
}

class FYZUPIXK_LootConfig
{
	string ClassName;
	string Magazine_ClassName;
	int Spawn_Chance;
	int Max_Spawnable;
	int Quantity;
	int Tier;
	ref TStringArray Attach;
	ref TStringArray Extra_Items;
	ref TStringArray Alternatives;
	
	void FYZUPIXK_LootConfig(string sClass, int spawnChance, int maxSpawn, int maxQty, string magClass = "", TStringArray sAtt = NULL, TStringArray sLoot = NULL, TStringArray sAlternatives = NULL, int tier = 1)
	{
		ClassName = sClass;
		Magazine_ClassName = magClass;
		Spawn_Chance = spawnChance;
		Max_Spawnable = maxSpawn;
		Quantity = maxQty;
		Tier = tier;
		Attach = sAtt;
		Extra_Items = sLoot;
		Alternatives = sAlternatives;
		
		if(Attach == NULL) Attach = new TStringArray;
		if(Extra_Items == NULL) Extra_Items = new TStringArray;
		if(Alternatives == NULL) Alternatives = new TStringArray;
	}
	
	void ~FYZUPIXK_LootConfig()
	{
		this.ClassName = "";
		this.Magazine_ClassName = "";
		this.Spawn_Chance = 0;
		this.Max_Spawnable = 0;
		this.Quantity = 0;
		this.Tier = 0;

		if(this.Attach) this.Attach.Clear();
		this.Attach = NULL;
		if(this.Extra_Items) this.Extra_Items.Clear();
		this.Extra_Items = NULL;
		if(this.Alternatives) this.Alternatives.Clear();
		this.Alternatives = NULL;
	}

	int GetTier()
	{
		if(Tier <= 0) return 1;
		return Tier;
	}
	
	string GetClassname()
	{
		return ClassName;
	}
	
	string GetMagClassname()
	{
		return Magazine_ClassName;
	}
	
	int GetSpawnChance()
	{
		return Spawn_Chance;
	}
	
	int GetMaxSpawnable()
	{
		return Max_Spawnable;
	}
	
	int GetQuantity()
	{
		return Quantity;
	}
	
	TStringArray GetAttachments()
	{
		return Attach;
	}
	
	TStringArray GetExtraLoot()
	{
		return Extra_Items;
	}
	
	TStringArray GetAlternatives()
	{
		return Alternatives;
	}
}

class FYZUPIXK_LootTier
{
	int Tier;         //tier id (matches FYZUPIXK_LootConfig.Tier). 1..N.
	int Weight;       //relative weight for the tier roll. Higher = more likely.
	string Name;      //display/log name, purely cosmetic.

	void FYZUPIXK_LootTier(int tier = 1, int weight = 1, string name = "")
	{
		Tier = tier;
		Weight = weight;
		Name = name;
	}

	void ~FYZUPIXK_LootTier()
	{
		this.Tier = 0;
		this.Weight = 0;
		this.Name = "";
	}

	int GetTier()
	{
		return Tier;
	}

	int GetWeight()
	{
		if(Weight < 0) return 0;
		return Weight;
	}

	string GetName()
	{
		return Name;
	}
}
