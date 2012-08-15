/*-- Intelligentes Respawnsystem --*/

#strict 2

static const SPAWNSYS_Allies = 50;
static const SPAWNSYS_Enemies = -50;
static const SPAWNSYS_Traps = -30;

global func GetBestSpawnpoint(array aSpawnpoints, int iPlr, int &x, int &y)
{
	var team = GetPlayerTeam(iPlr);
	var spawn_grading = [];
	for(var spawn in aSpawnpoints)
	{
		var i = GetLength(spawn_grading);
		spawn_grading[i] = [spawn, 0];
		for(var obj in FindObjects(Find_Distance(100, spawn[0], spawn[1]), Find_Or(Find_OCF(OCF_CrewMember), Find_And(Find_Hostile(iPlr), Find_Func("IsSpawnTrap")))))
		{
			if(GetOCF(obj) & OCF_CrewMember)
			{
				if(Hostile(iPlr, GetOwner(obj)))
					spawn_grading[i][1] += SPAWNSYS_Enemies;
				else
					spawn_grading[i][1] += SPAWNSYS_Allies;
			}
			else
				spawn_grading[i][1] += SPAWNSYS_Traps;
		}
	}

	var highest = 0x80000000;
	var chosen_spawns = [];
	for(var spawn_data in spawn_grading)
	{
		if(spawn_data[1] > highest)
		{
			chosen_spawns = [spawn_data[0]];
			highest = spawn_data[1];
		}
		else if(spawn_data[1] == highest)
			chosen_spawns[GetLength(chosen_spawns)] = spawn_data[0];
	}
	
	var spawnpoint = chosen_spawns[Random(GetLength(chosen_spawns))];
	x = spawnpoint[0];
	y = spawnpoint[1];

	return spawnpoint;
}

