/*
PackFazupix KOTH by Touchofdeath Version 1.0
*/

ref FYZUPIX_KOTH FYZUPIXKOTH = new FYZUPIX_KOTH();

modded class MissionServer
{
    override void OnInit()
	{
        super.OnInit();				// created special event for preventing potential issue with server/mision start.
        FYZUPIXKOTH.OnInit();	// Would be executed when KOTH is ready and all addons was loaded and active
	}
}
