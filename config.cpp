class CfgPatches
{
	class fyzupix_koth
	{
		units[] = {"fyzupix_KOTHflag", "fyzupix_KOTHchest_SupplyBox1", "fyzupix_KOTHchest_SupplyBox2", "fyzupix_KOTHchest_SupplyBox3"};
		weapons[] = {};
		requiredVersion = 0.10000000149011612;
		requiredAddons[] = {"DZ_Data", "DZ_Gear_Camping"};
	};
};
class CfgMods
{
	class fyzupix_koth
	{
		dir = "fyzupix_koth";
		picture = "";
		action = "";
		hideName = 1;
		hidePicture = 1;
		name = "PackFazupix KOTH";
		logoSmall = "";
		logo = "";
		logoOver = "";
		tooltip = "";
		overview = "";
		credits = "";
		author = "Touchofdeath";
		authorID = "0";
		version = "2.00";
		extra = 1;
		type = "mod";
		dependencies[] = {"World", "Mission"};
		class defs
		{
			class worldScriptModule
			{
				value = "";
				files[] = {"fyzupix_koth/Scripts/4_World"};
			};
			class missionScriptModule
			{
				value = "";
				files[] = {"fyzupix_koth/Scripts/5_Mission"};
			};
		};
	};
};
class CfgVehicles
{
	class BaseBuildingBase;
	class Container_Base;
	class fyzupix_KOTHflag : BaseBuildingBase
	{
		scope = 2;
		displayName = "KOTH Flag";
		descriptionShort = "Capture me daddy!";
		model = "\DZ\gear\camping\territory_flag.p3d";
		attachments[] = {"Material_FPole_Flag"};
		bounding = "BSphere";
		overrideDrawArea = "3.0";
		forceFarBubble = "true";
		handheld = "false";
		lootCategory = "Crafted";
		carveNavmesh = 1;
		weight = 10000;
		itemBehaviour = 0;
		itemSize[] = {15, 15};
		canBeDigged = 0;
		physLayer = "item_large";
		createProxyPhysicsOnInit = "true";
		createdProxiesOnInit[] = {"Base", "Support", "Pole"};
		rotationFlags = 2;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 999999;
				};
			};
			class GlobalArmor
			{
				class Projectile
				{
					class Health
					{
						damage = 0;
					};
					class Blood
					{
						damage = 0;
					};
					class Shock
					{
						damage = 0;
					};
				};
				class Melee
				{
					class Health
					{
						damage = 0;
					};
					class Blood
					{
						damage = 0;
					};
					class Shock
					{
						damage = 0;
					};
				};
				class FragGrenade
				{
					class Health
					{
						damage = 0;
					};
					class Blood
					{
						damage = 0;
					};
					class Shock
					{
						damage = 0;
					};
				};
			};
		};
		class AnimationSources
		{
			class AnimSourceShown
			{
				source = "user";
				animPeriod = 0.009999999776482582;
				initPhase = 0;
			};
			class AnimSourceHidden
			{
				source = "user";
				animPeriod = 0.009999999776482582;
				initPhase = 1;
			};
			class flag_mast
			{
				source = "user";
				initPhase = 1;
				animPeriod = 1;
			};
			class Deployed : AnimSourceShown {};
			class Base : AnimSourceShown {};
			class Support : AnimSourceShown {};
			class Pole : AnimSourceShown {};
		};
	};
	class fyzupix_KOTHchest_Base : Container_Base
	{
		scope = 0;
		displayName = "KOTH Reward Crate";
		descriptionShort = "...";
		slopeTolerance = 0.4000000059604645;
		yawPitchRollLimit[] = {45, 45, 45};
		weight = 10000;
		itemBehaviour = 0;
		itemSize[] = {25, 25};
		carveNavmesh = 1;
		canBeDigged = 0;
		rotationFlags = 2;
		class Cargo
		{
			itemsCargoSize[] = {10, 100};
			openable = 0;
			allowOwnedCargoManipulation = 1;
		};
		soundImpactType = "wood";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 999999;
				};
			};
			class GlobalArmor
			{
				class FragGrenade
				{
					class Health
					{
						damage = 0;
					};
					class Blood
					{
						damage = 0;
					};
					class Shock
					{
						damage = 0;
					};
				};
			};
		};
	};
	class fyzupix_KOTHchest_SupplyBox1 : fyzupix_KOTHchest_Base
	{
		scope = 2;
		model = "DZ\structures\Military\Misc\Misc_SupplyBox1.p3d";
	};
	class fyzupix_KOTHchest_SupplyBox2 : fyzupix_KOTHchest_Base
	{
		scope = 2;
		model = "DZ\structures\Military\Misc\Misc_SupplyBox2.p3d";
	};
	class fyzupix_KOTHchest_SupplyBox3 : fyzupix_KOTHchest_Base
	{
		scope = 2;
		model = "DZ\structures\Military\Misc\Misc_SupplyBox3.p3d";
	};
};
